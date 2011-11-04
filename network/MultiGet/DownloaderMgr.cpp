// DownloaderMgr.cpp: implementation of the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderMgr.h"
#include "HeaderParser.h"
#include "EasyDownloader.h"
#include "SegmentDownloader.h"
#include "CommonUtils.h"
#include "TimeCost.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT CDownloaderMgr::StartDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	UINT nResult = pThis->StartDownload();

	return nResult;
}

UINT CDownloaderMgr::ResumeDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	UINT nResult = pThis->ResumeDownload();

	return nResult;
}

CDownloaderMgr::CDownloaderMgr() : m_pDownloader(NULL)
{
}

CDownloaderMgr::~CDownloaderMgr()
{
	if(m_pDownloader != NULL)
	{
		delete m_pDownloader;
		m_pDownloader = NULL;
	}
}

void CDownloaderMgr::Init(const CDownloadParam& param)
{
	CDownloader::Init(param);

	//Empty file name, auto extract
	if(m_dlParam.m_szSaveToFileName.IsEmpty())
	{
		BOOL bResult = CCommonUtils::ExtractFileName(m_dlParam.m_szUrl, m_dlParam.m_szSaveToFileName);
		if(!bResult || m_dlParam.m_szSaveToFileName.GetLength() >= 64 || m_dlParam.m_szSaveToFileName.IsEmpty())
		{
			m_dlParam.m_szSaveToFileName = "data";
		}
	}
}

UINT CDownloaderMgr::GetCurrentStatus()
{
	if(m_pDownloader != NULL)
	{
		return m_pDownloader->GetCurrentStatus();
	}
	return CDownloader::GetCurrentStatus();
}
int CDownloaderMgr::Start()
{
	AfxBeginThread(StartDownloadProc, (LPVOID)this);
	return 0;
}
int CDownloaderMgr::Stop()
{
	ASSERT(m_pDownloader != NULL);
	return m_pDownloader->Stop();
}
int CDownloaderMgr::Pause()
{
	ASSERT(m_pDownloader != NULL);
	return m_pDownloader->Pause();
}
int CDownloaderMgr::Resume()
{
	AfxBeginThread(ResumeDownloadProc, (LPVOID)this);
	return 0;
}
BOOL CDownloaderMgr::IsResumable()
{
	ASSERT(m_pDownloader != NULL);
	return m_pDownloader->IsResumable();
}

int CDownloaderMgr::Destroy()
{
	if(m_pDownloader == NULL)
	{
		CDownloader::CurrentStatusChanged(TSE_INVALID);
		
		::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_DESTROY, (WPARAM)NULL, (LPARAM)NULL);

		return 0;
	}
	return m_pDownloader->Destroy();
}

void CDownloaderMgr::CurrentStatusChanged(UINT nNewStatus, LPCTSTR lpszDetail)
{
	CDownloader::CurrentStatusChanged(nNewStatus, lpszDetail);
}
UINT CDownloaderMgr::StartDownload()
{
	CString szLog;
	szLog.Format("[StartDownload]: ThreadID=%d, URL=%s", GetCurrentThreadId(), m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	CHeaderParser headerParser(m_dlParam.m_szUrl);
	CHeaderInfo* pHeaderInfo = headerParser.GetHeaderInfo();
	
	if(m_pDownloader != NULL)
	{
		delete m_pDownloader;
		m_pDownloader = NULL;
	}

	//Error checking

	CString szStatusMsg;
	//1. CURL return error
	if(pHeaderInfo->m_nCurlResult != CURLE_OK)
	{
		CString szCurlResult;
		szCurlResult.Format("(%d) %s", pHeaderInfo->m_nCurlResult, 
			curl_easy_strerror((CURLcode)pHeaderInfo->m_nCurlResult));

		CurrentStatusChanged(TSE_END_WITH_ERROR, szCurlResult);

		return 1;
	}
	//2. HTTP return error
	if(pHeaderInfo->m_nHTTPCode != 200 && pHeaderInfo->m_nHTTPCode != 206)
	{	
		CurrentStatusChanged(TSE_END_WITH_ERROR, pHeaderInfo->m_szStatusLine);

		return 2;
	}

	if(pHeaderInfo->m_nHTTPCode == 206)
	{
		m_dlParam.m_nFileSize = pHeaderInfo->m_nContentRangeTotal;
		m_pDownloader = new CSegmentDownloader();
		
		szLog.Format("Segment download support: [Y]. HTTPCode=%d, Content-Length=%d", 
			pHeaderInfo->m_nHTTPCode, pHeaderInfo->m_nContentRangeTotal);
	}
	else if(pHeaderInfo->m_nHTTPCode == 200)
	{
		//When file size is bigger than 5M, try to segment download
		if(pHeaderInfo->m_nContentLength > (5 * 1024 * 1024))
		{
			m_dlParam.m_nFileSize = pHeaderInfo->m_nContentLength;
			m_pDownloader = new CSegmentDownloader();

			szLog.Format("Segment download support: [N]. But try to download with segment. "
				"HTTPCode=%d, Content-Length=%d", pHeaderInfo->m_nHTTPCode, pHeaderInfo->m_nContentLength);
		}
		else
		{
			m_dlParam.m_nFileSize = pHeaderInfo->m_nContentLength;
			m_pDownloader = new CEasyDownloader();
			
			szLog.Format("Segment download support: [N]. HTTPCode=%d, Content-Length=%d", 
				pHeaderInfo->m_nHTTPCode, pHeaderInfo->m_nContentLength);
		}
	}
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	m_pDownloader->Init(m_dlParam);
	
	CTimeCost timeCost;

	m_pDownloader->Start();

	timeCost.UpdateCurrClock();
	szLog.Format("Time Cost (%d)", timeCost.GetTimeCost());
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	return 0;
}

UINT CDownloaderMgr::ResumeDownload()
{
	ASSERT(m_pDownloader != NULL);

	CString szLog;
	szLog.Format("[ResumeDownload]: ThreadID=%d, URL=%s", GetCurrentThreadId(), m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	CTimeCost timeCost;
	m_pDownloader->Resume();
	
	timeCost.UpdateCurrClock();
	szLog.Format("Time Cost (%d)", timeCost.GetTimeCost());
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
	return 0;
}