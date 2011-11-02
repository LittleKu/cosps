// EasyDownloader.cpp: implementation of the CEasyDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EasyDownloader.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

size_t CEasyDownloader::HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	CEasyDownloader* pThis = (CEasyDownloader*)userdata;
	return pThis->ProcessHeader(ptr, size, nmemb);
}

size_t CEasyDownloader::DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	CEasyDownloader* pThis = (CEasyDownloader*)userdata;
	return pThis->ProcessData(ptr, size, nmemb);
}

int CEasyDownloader::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CEasyDownloader* pThis = (CEasyDownloader*)clientp;
	return pThis->ProcessProgress(dltotal, dlnow, ultotal, ulnow);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEasyDownloader::CEasyDownloader()
{
}

CEasyDownloader::~CEasyDownloader()
{

}

int CEasyDownloader::Start()
{
	VerifyTempFolderExist();

	CURLcode res = curl_easy_perform(m_curl);

	PostDownload((DWORD)res);

	return 0;
}

void CEasyDownloader::PostDownload(DWORD dwResult)
{
	CString szErrorMsg, szEndMsg;
	
	BOOL bResult = FALSE;
	do 
	{
		//check if curl fail?
		if(dwResult != CURLE_OK)
		{
			szErrorMsg.Format("[Transfer Error]: %d - %s", dwResult, curl_easy_strerror((CURLcode)dwResult));
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szErrorMsg)	
			break;
		}
		
		//check if HTTP fail?
		if(m_dlInfo.m_headerInfo.m_nHTTPCode != 200)
		{
			szErrorMsg.Format("[Server Error]: %s", m_dlInfo.m_headerInfo.m_szStatusLine);
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szErrorMsg)	
			break;
		}
		
		bResult = TRUE;	
	} while (FALSE);
	
	
	/* always cleanup */
	curl_easy_cleanup(m_curl);
	
	//self cleanup
	fclose(m_dlInfo.lpFileHeader);
	fclose(m_dlInfo.lpFileData);
	
	if(bResult)
	{
		//1. copy file
		CString szTempFolder;
		GetTempFolder(szTempFolder);
		
		CString szSrcFile, szDstFile;
		szSrcFile.Format("%s\\%s", szTempFolder, m_dlParam.m_szSaveToFileName);
		szDstFile.Format("%s\\%s", SYS_OPTIONS()->m_szSaveToFolder, m_dlParam.m_szSaveToFileName);
		
		::CopyFile(szSrcFile, szDstFile, FALSE);
		
		//2. delete task related folder
		if(!(SYS_OPTIONS()->m_bKeepTempFiles))
		{
			CCommonUtils::RemoveDirectory(szTempFolder);
		}
		
		//Final status: complete
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_COMPLETE);

		CCommonUtils::StatusCodeToStr(TSE_COMPLETE, NULL, szEndMsg);
	}
	else
	{
		//error
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_END_WITH_ERROR, szErrorMsg);

		CCommonUtils::StatusCodeToStr(TSE_END_WITH_ERROR, szErrorMsg, szEndMsg);
	}

	::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, (WPARAM)((LPCSTR)szEndMsg), dwResult);
}

void CEasyDownloader::Stop()
{
	m_dlInfo.m_controller.Stop();
}

void CEasyDownloader::Pause()
{
	m_dlInfo.m_controller.Pause();
}

int CEasyDownloader::Resume()
{
	VerifyTempFolderExist();

	m_dlInfo.m_controller.Clear();

	return 0;
}

BOOL CEasyDownloader::IsResumable()
{
	return FALSE;
}

size_t CEasyDownloader::ProcessHeader(char *ptr, size_t size, size_t nmemb)
{
	if( IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine(ptr, size * nmemb);
		CCommonUtils::ReplaceCRLF(szLine);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLine)
	}

	fwrite(ptr, size, nmemb, m_dlInfo.lpFileHeader);

	CString szScratch(ptr, size * nmemb);
	do 
	{
		//1. check if status line
		int nRspCode = CCommonUtils::GetHTTPStatusCode(szScratch);
		
		//This is a status line
		if(nRspCode > 0)
		{
			m_dlInfo.m_headerInfo.Reset();
			m_dlInfo.m_headerInfo.m_nHTTPCode = nRspCode;
			m_dlInfo.m_headerInfo.m_szStatusLine = szScratch;
			CCommonUtils::ReplaceCRLF(m_dlInfo.m_headerInfo.m_szStatusLine, NULL, NULL);
			break;
		}
	}
	while(FALSE);	

	return (size_t)(size * nmemb);
}

size_t CEasyDownloader::ProcessData(char *ptr, size_t size, size_t nmemb)
{
	fwrite(ptr, size, nmemb, m_dlInfo.lpFileData);

	return (size_t)(size * nmemb);
}

int CEasyDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow)
{
	//Pause
	if(m_dlInfo.m_controller.IsModified() && m_dlInfo.m_controller.IsPaused())
	{
		m_dlInfo.m_controller.SetModified(FALSE);
		curl_easy_pause(m_curl, CURLPAUSE_ALL);
	}
	//Resume
	if(m_dlInfo.m_controller.IsModified() && !m_dlInfo.m_controller.IsPaused())
	{
		m_dlInfo.m_controller.SetModified(FALSE);
		curl_easy_pause(m_curl, CURLPAUSE_CONT);
	}
	//Stop
	if(m_dlInfo.m_controller.IsStopped())
	{
		return 2;
	}

	//Send progress notification
	CProgressInfo progressInfo;
	progressInfo.dltotal = (DWORD64)m_dlParam.m_nFileSize;
	progressInfo.dlnow = (DWORD64)dlnow;
	progressInfo.ultotal = (DWORD64)ultotal;
	progressInfo.ulnow = (DWORD64)ulnow;
	progressInfo.retCode = -1;
	progressInfo.szReason = "";
	progressInfo.index = m_dlParam.m_nTaskID;
	
	::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, (LPARAM)0);

	return 0;
}

void CEasyDownloader::GetTempFolder(CString& szTempFolder)
{
	szTempFolder.Format("%s\\%s_%d", SYS_OPTIONS()->m_szTempFolder, m_dlParam.m_szSaveToFileName, m_dlParam.m_nTaskID);
}
void CEasyDownloader::VerifyTempFolderExist()
{
	CString szTempFolder;
	GetTempFolder(szTempFolder);
	
	CCommonUtils::VerifyDirectoryExist(szTempFolder);
}

void CEasyDownloader::Init(const CDownloadParam& param)
{
	CString szTempFolder;
	CString szFileName;

	m_dlParam = param;
	
	m_curl = curl_easy_init();
	
	//URL address
	curl_easy_setopt(m_curl, CURLOPT_URL, (LPCTSTR)m_dlParam.m_szUrl);
	
	
	//agent: IE8
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, USER_AGENT_IE8);
	//redirect
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
	//no verbose
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0L);	
	
	//Proxy setting
	if(SYS_OPTIONS()->GetInstance()->GetProxy() != NULL)
	{
		curl_easy_setopt(m_curl, CURLOPT_PROXY, SYS_OPTIONS()->GetInstance()->GetProxy());
	}
	
	//header
	curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, CEasyDownloader::HeaderCallback);
	curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, this);
	
	//data
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CEasyDownloader::DataCallback);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	
	//progress
	curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, CEasyDownloader::ProgressCallback);
	curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
	
	//init control data
	GetTempFolder(szTempFolder);
	
	szFileName.Format("%s\\header_%s.txt", szTempFolder, m_dlParam.m_szSaveToFileName);
	m_dlInfo.lpFileHeader = fopen(szFileName, "wb");
	
	szFileName.Format("%s\\%s", szTempFolder, m_dlParam.m_szSaveToFileName);
	m_dlInfo.lpFileData = fopen(szFileName, "wb");
}

