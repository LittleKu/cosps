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

CDownloaderMgr::CDownloaderMgr(const CDownloadParam& param) : m_dlParam(param), m_pDownloader(NULL)
{
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

CDownloaderMgr::~CDownloaderMgr()
{
	if(m_pDownloader != NULL)
	{
		delete m_pDownloader;
		m_pDownloader = NULL;
	}
}

void CDownloaderMgr::Start()
{
	AfxBeginThread(StartDownloadProc, (LPVOID)this);
}

void CDownloaderMgr::Resume()
{
	AfxBeginThread(ResumeDownloadProc, (LPVOID)this);
}

void CDownloaderMgr::Stop()
{
	ASSERT(m_pDownloader != NULL);
	m_pDownloader->Stop();
}
void CDownloaderMgr::Pause()
{
	ASSERT(m_pDownloader != NULL);
	m_pDownloader->Pause();
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

		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_END_WITH_ERROR, szCurlResult);

		return 1;
	}
	//2. HTTP return error
	if(pHeaderInfo->m_nHTTPCode != 200 && pHeaderInfo->m_nHTTPCode != 206)
	{	
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_END_WITH_ERROR, pHeaderInfo->m_szStatusLine);

		return 2;
	}

	m_dlParam.m_nFileSize = pHeaderInfo->m_nContentLength;
	if(pHeaderInfo->m_nContentLength > 0 && pHeaderInfo->m_nHTTPCode == 206/* && pHeaderInfo->m_bRangeBytes*/)
	{
		szLog.Format("Segment download support: [Y]. HTTPCode=%d, Content-Length=%d", 
			pHeaderInfo->m_nHTTPCode, pHeaderInfo->m_nContentLength);	
		
		m_pDownloader = new CSegmentDownloader();
	}
	else
	{
		szLog.Format("Segment download support: [N]. HTTPCode=%d, Content-Length=%d", 
			pHeaderInfo->m_nHTTPCode, pHeaderInfo->m_nContentLength);

		m_pDownloader = new CEasyDownloader();
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