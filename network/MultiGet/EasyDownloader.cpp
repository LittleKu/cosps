// EasyDownloader.cpp: implementation of the CEasyDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EasyDownloader.h"

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

void CEasyDownloader::Init(LPCTSTR lpszUrl, CDownloadParam param)
{
	m_szUrl = lpszUrl;
	m_dlParam = param;

	m_curl = curl_easy_init();

	//URL address
	curl_easy_setopt(m_curl, CURLOPT_URL, (LPCTSTR)m_szUrl);


	//agent: IE8
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, USER_AGENT_IE8);
	//redirect
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
	//no verbose
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0L);	

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
	m_controlData.lpFileHeader = fopen("header.txt", "wb");
	m_controlData.lpFileData = fopen("data", "wb");
}

void CEasyDownloader::Start()
{
	CURLcode res = curl_easy_perform(m_curl);
	//failed?
	if(res != CURLE_OK)
	{
		CString szLogInfo;
		szLogInfo.Format("curl_easy_perform failed. res = (%d) - %s, url=%s", res, curl_easy_strerror(res), m_szUrl);
		
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLogInfo)
	}

	long rspCode = 0;
	curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &rspCode);
	//check if http success?
	if(rspCode != 200)
	{
		CString szLogInfo;
		szLogInfo.Format("response code failed. rspCode = %d, url=%s", rspCode, m_szUrl);
		
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLogInfo)
	}

	/* always cleanup */
    curl_easy_cleanup(m_curl);

	//self cleanup
	fclose(m_controlData.lpFileHeader);
	fclose(m_controlData.lpFileData);
}

void CEasyDownloader::Stop()
{
	m_controlData.control.isStopped = TRUE;
}

void CEasyDownloader::Pause()
{
	m_controlData.control.isModified = TRUE;
	m_controlData.control.isPaused = TRUE;	
}

void CEasyDownloader::Resume()
{
	m_controlData.control.isModified = TRUE;
	m_controlData.control.isPaused = FALSE;
}

size_t CEasyDownloader::ProcessHeader(char *ptr, size_t size, size_t nmemb)
{
	if( IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine(ptr, size * nmemb);
		szLine.Replace(_T("\r"), _T("[0x0D]"));
		szLine.Replace(_T("\n"), _T("[0x0A]"));
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLine)
	}

	fwrite(ptr, size, nmemb, m_controlData.lpFileHeader);

	return (size_t)(size * nmemb);
}

size_t CEasyDownloader::ProcessData(char *ptr, size_t size, size_t nmemb)
{
	fwrite(ptr, size, nmemb, m_controlData.lpFileData);

	return (size_t)(size * nmemb);
}

int CEasyDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow)
{
	//Pause
	if(m_controlData.control.isModified && m_controlData.control.isPaused == TRUE)
	{
		m_controlData.control.isModified = FALSE;
		curl_easy_pause(m_curl, CURLPAUSE_ALL);
	}
	//Resume
	if(m_controlData.control.isModified && m_controlData.control.isPaused == FALSE)
	{
		m_controlData.control.isModified = FALSE;
		curl_easy_pause(m_curl, CURLPAUSE_CONT);
	}
	//Stop
	if(m_controlData.control.isStopped)
	{
		return 5;
	}

	//Send progress notification
	CProgressInfo progressInfo;
	progressInfo.dltotal = (DWORD64)dltotal;
	progressInfo.dlnow = (DWORD64)dlnow;
	progressInfo.ultotal = (DWORD64)ultotal;
	progressInfo.ulnow = (DWORD64)ulnow;
	progressInfo.retCode = -1;
	progressInfo.szReason = "";
	progressInfo.index = m_dlParam.m_nIndex;
	
	::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, (LPARAM)0);

	return 0;
}
