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

void CEasyDownloader::Init(const CDownloadParam& param)
{
	CDownloader::Init(param);
}

int CEasyDownloader::Start()
{
	VerifyTempFolderExist();
	StartConnection();

	CurrentStatusChanged(TSE_TRANSFERRING);

	DoDownload();

	return 0;
}

int CEasyDownloader::Stop()
{
	m_controller.Stop();

	return 0;
}

int CEasyDownloader::Pause()
{
	m_controller.Pause();

	return 0;
}

int CEasyDownloader::Resume()
{
	m_controller.Clear();

	VerifyTempFolderExist();
	//Restart because of pause or stop. Don't change retry times, and alway retry
	RestartConnection(CCommonUtils::INT_OPER_RESET, CCommonUtils::MAX_INTEGER);
	
	CurrentStatusChanged(TSE_TRANSFERRING);

	DoDownload();

	return 0;
}

BOOL CEasyDownloader::IsResumable()
{
	return FALSE;
}

int CEasyDownloader::Destroy()
{
	if(GetCurrentStatus() != TSE_TRANSFERRING)
	{
		CString szTempFolder;
		GetTempFolder(szTempFolder);
		
		//delete task related folder
		if(!(SYS_OPTIONS()->m_bKeepTempFiles))
		{
			CCommonUtils::RemoveDirectory(szTempFolder);
		}
		
		CurrentStatusChanged(TSE_INVALID);
		
		::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		
		return 0;
	}
	
	m_controller.Destroy();
	
	return 0;
}

int CEasyDownloader::DoDownload()
{
	CURLcode res;
	DWORD dwResult;
	BOOL bRetry;

	do 
	{
		//transfer
		res = curl_easy_perform(m_curl);

		//process when the transfer done, check if we should retry to download?
		bRetry = ProcessTransferDone(res, dwResult);

	} while (bRetry);
	
	PostDownload(dwResult);

	return 0;
}

BOOL CEasyDownloader::ProcessTransferDone(CURLcode res, DWORD& dwResult)
{
	BOOL bRetry = FALSE;
	dwResult = 0;

	//First close connection, this function doesn't change "m_dlInfo" data
	CloseConnection();

	//log
	CString szLog;
	szLog.Format("Transfer result: %d - %s", res, curl_easy_strerror(res));
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)	
	
	switch(res)
	{
	case CURLE_OK:
		{
			//This case should rarely happen
			if(m_connInfo.m_headerInfo.m_nHTTPCode != 200)
			{
				bRetry = RestartConnection(CCommonUtils::INT_OPER_INCREASE);

				szLog.Format("[Server Error]: %s. Try to retry", m_connInfo.m_headerInfo.m_szStatusLine);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
		}
		break;
	case CURLE_ABORTED_BY_CALLBACK:
		{
			//(1). Stop
			if(m_controller.IsStopped())
			{
				dwResult = MAKELONG(RC_MAJOR_STOPPED, RC_MINOR_OK);

				szLog.Format("Connection stopped");
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)				
			}
			//(2). Pause
			else if(m_controller.IsPaused())
			{
				dwResult = MAKELONG(RC_MAJOR_PAUSED, RC_MINOR_OK);

				szLog.Format("Connection paused");
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			//(3). This should not happen.
			else
			{
				szLog.Format("Connection aborted by other reason. This should be an error.");
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
		}
		break;
	case CURLE_WRITE_ERROR:
		{
			//This should not happen
			szLog.Format("Connection stopped by write data function");
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
		break;
	case CURLE_OPERATION_TIMEDOUT:
		{
			//Always retry
			bRetry = RestartConnection(CCommonUtils::INT_OPER_KEEP, CCommonUtils::MAX_INTEGER);

			szLog.Format("Timeout, retry [%d]", (int)bRetry);
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
		break;
	default:
		{
			bRetry = RestartConnection(CCommonUtils::INT_OPER_INCREASE);

			if(bRetry)
			{
				szLog.Format("Retry [Y], times = %d", m_connInfo.m_nRetry);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			else
			{
				dwResult = MAKELONG(RC_MAJOR_TERMINATED_BY_CURL_CODE, res);

				szLog.Format("Retry [N], times = %d", m_connInfo.m_nRetry);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
		}
		break;
	}

	return bRetry;
}

void CEasyDownloader::PostDownload(DWORD dwResult)
{
	//1. Clean up
	m_curl = NULL;
	
	//Result Msg
	CString szErrorMsg;
	CCommonUtils::FormatErrorMsg(dwResult, szErrorMsg);
	
	CString szLog;
	szLog.Format("Download result: %X - %s", dwResult, szErrorMsg);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
	//2. Post process
	WORD nMajor = LOWORD(dwResult);
	
	//Merge files when successfully
	if(nMajor == RC_MAJOR_OK)
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
		CurrentStatusChanged(TSE_COMPLETE);
	}
	else if(nMajor == RC_MAJOR_PAUSED)
	{
		CurrentStatusChanged(TSE_PAUSED);
	}
	else if(nMajor == RC_MAJOR_STOPPED)
	{
		CurrentStatusChanged(TSE_STOPPED);
	}
	else if(nMajor == RC_MAJOR_DESTROYED)
	{
		//delete task related folder
		if(!(SYS_OPTIONS()->m_bKeepTempFiles))
		{
			CString szTempFolder;
			GetTempFolder(szTempFolder);
			CCommonUtils::RemoveDirectory(szTempFolder);
		}
		
		CurrentStatusChanged(TSE_INVALID);
		
		::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_DESTROY, (WPARAM)((LPCSTR)szLog), dwResult);

		return;
	}
	else if(nMajor == RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR)
	{		
		CurrentStatusChanged(TSE_END_WITH_ERROR, szErrorMsg);
	}
	else if(nMajor == RC_MAJOR_TERMINATED_BY_CURL_CODE)
	{
		CurrentStatusChanged(TSE_END_WITH_ERROR, szErrorMsg);
	}
	else
	{
		szErrorMsg.Format("Unknown error: %d", dwResult);
		CurrentStatusChanged(TSE_END_WITH_ERROR, szErrorMsg);
	}
	
	::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, (WPARAM)((LPCSTR)szLog), dwResult);
}

size_t CEasyDownloader::ProcessHeader(char *ptr, size_t size, size_t nmemb)
{
	if( IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine(ptr, size * nmemb);
		CCommonUtils::ReplaceCRLF(szLine);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLine)
	}

	fwrite(ptr, size, nmemb, m_connInfo.lpFileHeader);

	CString szScratch(ptr, size * nmemb);
	do 
	{
		//1. check if status line
		int nRspCode = CCommonUtils::GetHTTPStatusCode(szScratch);
		
		//This is a status line
		if(nRspCode > 0)
		{
			m_connInfo.m_headerInfo.Reset();
			m_connInfo.m_headerInfo.m_nHTTPCode = nRspCode;
			m_connInfo.m_headerInfo.m_szStatusLine = szScratch;
			CCommonUtils::ReplaceCRLF(m_connInfo.m_headerInfo.m_szStatusLine, NULL, NULL);
			break;
		}
	}
	while(FALSE);	

	return (size_t)(size * nmemb);
}

size_t CEasyDownloader::ProcessData(char *ptr, size_t size, size_t nmemb)
{
	if(m_connInfo.m_headerInfo.m_nHTTPCode != 200)
	{
		fwrite(ptr, size, nmemb, m_connInfo.lpFileData);
		return (size_t)(size * nmemb);
	}

	//When the HTTP response code is 200
	size_t nBytes = size * nmemb;

	CRange rLocal(m_connInfo.m_nDlNow, m_dlParam.m_nFileSize);
	if(m_dlParam.m_nFileSize <= 0)
	{
		rLocal.cy = 0x7FFFFFFF;
	}
	CRange rRemote(m_connInfo.m_nRemotePos, m_connInfo.m_nRemotePos + nBytes - 1);
	CRange rResult;

	int rc = CCommonUtils::Intersection(rLocal, rRemote, rResult);

	CString szLog;
	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		szLog.Format("rc=%d, local(%d-%d), remote(%d-%d), result(%d-%d)", rc, 
			rLocal.cx, rLocal.cy, rRemote.cx, rRemote.cy, rResult.cx, rResult.cy);
		LOG4CPLUS_TRACE_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	
	m_connInfo.m_nRemotePos += nBytes;

	//The data range doesn't reach yet
	if(rc > 0)
	{
		return nBytes;
	}
	//reach out, stop this connection, this should not happen in easy handle
	else if(rc < 0)
	{
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("range reach out, stop connection");
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
		
		return -1;
	}
	//There are data we cared
	else
	{			
		int nDataLength = rResult.cy - rResult.cx + 1;
		fwrite(ptr + rResult.cx - rRemote.cx,  1, nDataLength, m_connInfo.lpFileData);
		
		m_connInfo.m_nDlNow += nDataLength;
		
		return nBytes;
	}
}

int CEasyDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow)
{
	//Send progress notification
	CProgressInfo progressInfo;
	progressInfo.dltotal = (DWORD64)m_dlParam.m_nFileSize;

	//The current downloaded data length is stored in m_dlInfo.m_nDlNow
	progressInfo.dlnow = (DWORD64)m_connInfo.m_nDlNow; 
	progressInfo.ultotal = (DWORD64)ultotal;
	progressInfo.ulnow = (DWORD64)ulnow;
	progressInfo.retCode = -1;
	progressInfo.szReason = "";
	progressInfo.m_nTaskID = m_dlParam.m_nTaskID;
	
	::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, (LPARAM)0);

	//Pause
	if(m_controller.IsPaused())
	{
		return 1;
	}
	//Stop
	if(m_controller.IsStopped())
	{
		return 2;
	}

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

void CEasyDownloader::StartConnection()
{
	InitEasyHandle();

	m_connInfo.m_nDlNow = 0;
	m_connInfo.m_nRetry = 0;
	m_connInfo.m_nRemotePos = 0;
	
	//File create
	CString szTempFolder, szFileName;
	GetTempFolder(szTempFolder);
	
	szFileName.Format("%s\\header_%s.txt", szTempFolder, m_dlParam.m_szSaveToFileName);
	m_connInfo.lpFileHeader = fopen(szFileName, "wb");

	szFileName.Format("%s\\%s", szTempFolder, m_dlParam.m_szSaveToFileName);
	m_connInfo.lpFileData = fopen(szFileName, "wb");
}
BOOL CEasyDownloader::RestartConnection(int nRetryOperType, int nMaxRetryLimit)
{
	if(nMaxRetryLimit < 0)
	{
		nMaxRetryLimit = SYS_OPTIONS()->m_nMaxRetryTimes;
	}
	
	if(m_connInfo.m_nRetry >= nMaxRetryLimit)
	{
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			CString szLog;
			szLog.Format("Retry times over, RestartConnection failed. retry=%d, max retry=%d", 
				m_connInfo.m_nRetry, nMaxRetryLimit);
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}

		return FALSE;
	}

	InitEasyHandle();
	
	CCommonUtils::IntegerOper(m_connInfo.m_nRetry, nRetryOperType);
	m_connInfo.m_nRemotePos = 0;
	m_connInfo.m_headerInfo.Reset();

	//File create
	CString szTempFolder, szFileName;
	GetTempFolder(szTempFolder);
	
	szFileName.Format("%s\\header_%s.txt", szTempFolder, m_dlParam.m_szSaveToFileName);
	m_connInfo.lpFileHeader = fopen(szFileName, "wb");
	
	szFileName.Format("%s\\%s", szTempFolder, m_dlParam.m_szSaveToFileName);
	m_connInfo.lpFileData = fopen(szFileName, "ab");

	return TRUE;
}

void CEasyDownloader::CloseConnection()
{
	/* always cleanup */
	curl_easy_cleanup(m_curl);
	m_curl = NULL;
	
	//self cleanup
	fclose(m_connInfo.lpFileHeader);
	fclose(m_connInfo.lpFileData);

	m_connInfo.lpFileHeader = NULL;
	m_connInfo.lpFileData = NULL;
}
void CEasyDownloader::InitEasyHandle()
{
	m_curl = curl_easy_init();
	
	//URL address
	curl_easy_setopt(m_curl, CURLOPT_URL, (LPCTSTR)m_dlParam.m_szUrl);
	
	
	//agent: IE8
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, USER_AGENT_IE8);
	//redirect
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
	//no verbose
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0L);
	//connect timeout: 10s
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 10L);
	
	//low speed limit
	curl_easy_setopt(m_curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(m_curl, CURLOPT_LOW_SPEED_TIME, 15L);
	
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
}

