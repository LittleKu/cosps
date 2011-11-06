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
	BOOL bResult;
	m_criticalSection.Lock();
	
	bResult = (m_pDownloader == NULL);

	m_criticalSection.Unlock();

	if(bResult)
	{
		WaitUtilStop();
	}
	else
	//if(m_pDownloader != NULL)
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

	m_controller.Clear();
}

UINT CDownloaderMgr::GetCurrentStatus()
{
	UINT nResult = 0;

	m_criticalSection.Lock();
	if(m_pDownloader != NULL)
	{
		nResult = m_pDownloader->GetCurrentStatus();
	}
	else
	{
		nResult = CDownloader::GetCurrentStatus();
	}
	m_criticalSection.Unlock();

	return nResult;
}

int CDownloaderMgr::Start()
{
	AfxBeginThread(StartDownloadProc, (LPVOID)this);

	return 0;
}
int CDownloaderMgr::Stop()
{
	m_criticalSection.Lock();
	if(m_pDownloader == NULL)
	{
		m_controller.Stop();
		m_headerParser.Stop();
		m_criticalSection.Unlock();
		return 0;
	}
	m_criticalSection.Unlock();
	return m_pDownloader->Stop();
}
int CDownloaderMgr::Pause()
{
	m_criticalSection.Lock();
	if(m_pDownloader == NULL)
	{
		m_controller.Pause();
		m_headerParser.Stop();
		m_criticalSection.Unlock();
		return 0;
	}
	m_criticalSection.Unlock();
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
	m_criticalSection.Lock();
	if(m_pDownloader == NULL)
	{
		m_controller.Destroy();
		m_headerParser.Stop();
		m_criticalSection.Unlock();
		return 0;
	}
	m_criticalSection.Unlock();
	return m_pDownloader->Destroy();
}

void CDownloaderMgr::CurrentStatusChanged(UINT nNewStatus, LPCTSTR lpszDetail)
{
	CDownloader::CurrentStatusChanged(nNewStatus, lpszDetail);
}

UINT CDownloaderMgr::PreDownload()
{
	UINT nResult = 0;
	CString szLog;

	m_criticalSection.Lock();
	do 
	{
		if(m_pDownloader != NULL)
		{
			delete m_pDownloader;
			m_pDownloader = NULL;
		}
		if(m_controller.IsDestroyed())
		{
			CurrentStatusChanged(TSE_DESTROYED);
			nResult = 1;
			break;
		}
		if(m_controller.IsPaused())
		{
			CurrentStatusChanged(TSE_PAUSED);
			nResult = 2;
			break;
		}
		if(m_controller.IsStopped())
		{
			CurrentStatusChanged(TSE_STOPPED);
			nResult = 3;
			break;
		}
		m_criticalSection.Unlock();

		//2. phase: GET header
		//CHeaderParser headerParser(m_dlParam.m_szUrl);
		m_headerParser.Start(m_dlParam.m_szUrl);
		CHeaderInfo* pHeaderInfo = m_headerParser.GetHeaderInfo();

		m_criticalSection.Lock();
		if(m_controller.IsDestroyed())
		{
			CurrentStatusChanged(TSE_DESTROYED);
			nResult = 1;
			break;
		}
		if(m_controller.IsPaused())
		{
			CurrentStatusChanged(TSE_PAUSED);
			nResult = 2;
			break;
		}
		if(m_controller.IsStopped())
		{
			CurrentStatusChanged(TSE_STOPPED);
			nResult = 3;
			break;
		}

		CString szStatusMsg;
		//1. CURL return error
		if(pHeaderInfo->m_nCurlResult != CURLE_OK)
		{
			CString szCurlResult;
			szCurlResult.Format("(%d) %s", pHeaderInfo->m_nCurlResult, 
				curl_easy_strerror((CURLcode)pHeaderInfo->m_nCurlResult));
			
			CurrentStatusChanged(TSE_END_WITH_ERROR, szCurlResult);
			
			nResult = 4;
			break;
		}
		//2. HTTP return error
		if(pHeaderInfo->m_nHTTPCode != 200 && pHeaderInfo->m_nHTTPCode != 206)
		{	
			CurrentStatusChanged(TSE_END_WITH_ERROR, pHeaderInfo->m_szStatusLine);
			
			nResult = 5;
			break;
		}
		
		if(pHeaderInfo->m_nHTTPCode == 206)
		{
			m_dlParam.m_nFileSize = pHeaderInfo->m_nContentRangeTotal;
			m_pDownloader = new CSegmentDownloader(TSE_TRANSFERRING);
			
			szLog.Format("Segment download support: [Y]. HTTPCode=%d, Content-Length=%d", 
				pHeaderInfo->m_nHTTPCode, pHeaderInfo->m_nContentRangeTotal);
		}
		else if(pHeaderInfo->m_nHTTPCode == 200)
		{
			//When file size is bigger than 5M, try to segment download
			if(pHeaderInfo->m_nContentLength > (5 * 1024 * 1024))
			{
				m_dlParam.m_nFileSize = pHeaderInfo->m_nContentLength;
				m_pDownloader = new CSegmentDownloader(TSE_TRANSFERRING);
				
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

	} while (FALSE);

	m_criticalSection.Unlock();

	return nResult;
}
UINT CDownloaderMgr::StartDownload()
{
	//transfer
	CurrentStatusChanged(TSE_TRANSFERRING);

	UINT nResult = 0;

	CString szLog;
	szLog.Format("[StartDownload]: ThreadID=%d, URL=%s", GetCurrentThreadId(), m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	nResult = PreDownload();
	if(nResult)
	{
		return nResult;
	}
	
	ASSERT(m_pDownloader != NULL);
	
	m_pDownloader->Init(m_dlParam);
	
	CTimeCost timeCost;
	nResult = m_pDownloader->Start();
	timeCost.UpdateCurrClock();

	szLog.Format("Time Cost (%d)", timeCost.GetTimeCost());
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	
	return nResult;
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