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

CDownloaderMgr::CDownloaderMgr() : m_pDownloader(NULL), m_pHeaderParser(NULL), m_statusChecker(TSE_READY)
{
	//Init no-signal, auto event
	m_hStopEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CDownloaderMgr::~CDownloaderMgr()
{
	BOOL bResult;
	m_criticalSection.Lock();
	
	bResult = (m_pDownloader == NULL);

	//Just remove the memory, WaitUntilStop is used to stop thread!
	if(m_pDownloader != NULL)
	{
		delete m_pDownloader;
		m_pDownloader = NULL;
	}

	m_criticalSection.Unlock();

	if(m_hStopEvent)
	{
		::CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
}

void CDownloaderMgr::WaitUntilStop()
{
	BOOL bResult;
	m_criticalSection.Lock();
	bResult = (m_pDownloader != NULL);
	m_criticalSection.Unlock();

	if(bResult)
	{
		m_pDownloader->WaitUntilStop();
		return;
	}

	CString szLog;
	DWORD dwResult;
	UINT nStatus;
	while( (nStatus = GetCurrentStatus()) == TSE_TRANSFERRING )
	{
		Destroy();
		
		szLog.Format("Task [%d] is transferring, wait until it stopped.", m_dlParam.m_nTaskID);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		dwResult = ::WaitForSingleObject(m_hStopEvent, INFINITE);
		
		szLog.Format("Task [%d] WaitForSingleObject returned 0x%08X", m_dlParam.m_nTaskID, dwResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	
	szLog.Format("Task [%d] Stopped succesfully. Current status=%d", m_dlParam.m_nTaskID, nStatus);
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
}

void CDownloaderMgr::Init(const CDownloadParam& param)
{
	m_dlParam = param;

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
		nResult = m_statusChecker.GetCurrentStatus();
	}
	m_criticalSection.Unlock();

	return nResult;
}

DWORD CDownloaderMgr::GetOperState()
{
	DWORD dwResult = CStatusChecker::GetOperState(GetCurrentStatus());

	return dwResult;
}

void CDownloaderMgr::NotifyStopped(BOOL bStopped)
{
	if(bStopped)
	{
		::SetEvent(m_hStopEvent);
	}
}
void CDownloaderMgr::CurrentStatusChanged(UINT nNewStatus, LPCTSTR lpszDetail, BOOL bWorkThreadStopped, BOOL bSendMessage)
{
	m_statusChecker.SetCurrentStatus(nNewStatus);

	if(bSendMessage)
	{
		CStatusInfo statusInfo;
		statusInfo.m_nStatusCode = nNewStatus;
		statusInfo.m_szDetail = lpszDetail;
		CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&statusInfo));
	}

	NotifyStopped(bWorkThreadStopped);
}

int CDownloaderMgr::Start()
{
	CString szLog;
	CurrentStatusChanged(TSE_TRANSFERRING, NULL, FALSE);
	szLog.Format("Status changed to transferring.");
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	AfxBeginThread(StartDownloadProc, (LPVOID)this);

	return 0;
}
int CDownloaderMgr::Stop()
{
	int nResult;

	m_criticalSection.Lock();
	if(m_pDownloader == NULL)
	{
		m_controller.Stop();
		if(m_pHeaderParser != NULL)
		{
			m_pHeaderParser->Stop();
		}
		nResult = 0;
	}
	else
	{
		nResult = m_pDownloader->Stop();
	}
	m_criticalSection.Unlock();
	
	return nResult;
}
int CDownloaderMgr::Pause()
{
	int nResult;

	m_criticalSection.Lock();
	if(m_pDownloader == NULL)
	{
		m_controller.Pause();
		if(m_pHeaderParser != NULL)
		{
			m_pHeaderParser->Stop();
		}
		nResult = 0;
	}
	else
	{
		nResult = m_pDownloader->Pause();
	}
	m_criticalSection.Unlock();

	return nResult;
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
	int nResult;
	
	m_criticalSection.Lock();
	if(m_pDownloader == NULL)
	{
		m_controller.Destroy();
		if(m_pHeaderParser != NULL)
		{
			m_pHeaderParser->Stop();
		}
		nResult = 0;
	}
	else
	{
		nResult = m_pDownloader->Destroy();
	}
	m_criticalSection.Unlock();
	
	return nResult;
}

UINT CDownloaderMgr::DeleteProc(LPVOID lpvData)
{
	CDownloaderMgrArray* pDownloaderArray = (CDownloaderMgrArray*)lpvData;
	
	CString szLog;
	szLog.Format("Start to destroy all tasks.");
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
	int i, nSize;
	CDownloaderMgr* pDownloaderMgr;
	
	//Send destroy command
	for(i = 0, nSize = pDownloaderArray->GetSize(); i < nSize; i++)
	{
		pDownloaderMgr = (CDownloaderMgr*)pDownloaderArray->GetAt(i);
		ASSERT(pDownloaderMgr != NULL);
		
		pDownloaderMgr->Destroy();
	}
	//Wait all task to finish
	for(i = 0, nSize = pDownloaderArray->GetSize(); i < nSize; i++)
	{
		pDownloaderMgr = (CDownloaderMgr*)pDownloaderArray->GetAt(i);
		ASSERT(pDownloaderMgr != NULL);
		
		pDownloaderMgr->WaitUntilStop();

		ASSERT(pDownloaderMgr->GetCurrentStatus() != TSE_TRANSFERRING);
		delete pDownloaderMgr;
	}
	
	delete pDownloaderArray;
	pDownloaderArray = NULL;
	
	szLog.Format("Finished destroy all tasks.");
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
	return 0;
}
int CDownloaderMgr::Delete(CDownloaderMgrArray* pDownloaderArray)
{
	ASSERT(pDownloaderArray != NULL && pDownloaderArray->GetSize() > 0);
	
	AfxBeginThread(CDownloaderMgr::DeleteProc, pDownloaderArray);
	
	return 0;
}

UINT CDownloaderMgr::CheckStatus()
{
	UINT nResult = 0;
	CString szLog;

	do 
	{
		if(m_controller.IsDestroyed())
		{
			CurrentStatusChanged(TSE_DESTROYED, NULL, TRUE, FALSE);
			nResult = 1;
			
			if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
			{
				szLog.Format("T[%02d]: destroyed.", m_dlParam.m_nTaskID);
				LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			break;
		}
		if(m_controller.IsPaused())
		{
			CurrentStatusChanged(TSE_PAUSED, NULL, TRUE, FALSE);
			nResult = 2;

			if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
			{
				szLog.Format("T[%02d]: paused.", m_dlParam.m_nTaskID);
				LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			break;
		}
		if(m_controller.IsStopped())
		{
			CurrentStatusChanged(TSE_STOPPED, NULL, TRUE, FALSE);
			nResult = 3;

			if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
			{
				szLog.Format("T[%02d]: stopped.", m_dlParam.m_nTaskID);
				LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			break;
		}
	} while (FALSE);

	return nResult;
}

UINT CDownloaderMgr::PreGetHeader()
{
	UINT nResult = 0;
	CString szLog;

	m_criticalSection.Lock();

	if(m_pDownloader != NULL)
	{
		szLog.Format("T[%02d]: Restart from start postion.", m_dlParam.m_nTaskID);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		delete m_pDownloader;
		m_pDownloader = NULL;
	}

	nResult = CheckStatus();
	nResult = m_statusChecker.GetCurrentStatus();

	m_criticalSection.Unlock();

	if(nResult != TSE_TRANSFERRING)
	{
		CStatusInfo statusInfo;
		statusInfo.m_nStatusCode = nResult;
		
		CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)&statusInfo);
	}

	return nResult;
}
UINT CDownloaderMgr::PostGetHeader(CHeaderInfo* pHeaderInfo, CStatusInfo* pStatusInfo)
{
	ASSERT(pStatusInfo != NULL);

	UINT nResult = 0;
	CString szLog;

	//lock 1st
	m_criticalSection.Lock();
	nResult = CheckStatus();
	nResult = m_statusChecker.GetCurrentStatus();
	m_criticalSection.Unlock();

	if(nResult != TSE_TRANSFERRING)
	{
		CStatusInfo statusInfo;
		statusInfo.m_nStatusCode = nResult;
		
		CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)&statusInfo);
		
		m_criticalSection.Lock();
		if(m_pHeaderParser != NULL)
		{
			delete m_pHeaderParser;
			m_pHeaderParser = NULL;
		}
		m_criticalSection.Unlock();

		return nResult;
	}

	//lock 2
	CString szStatusMsg;
	nResult = TSE_TRANSFERRING;
	m_criticalSection.Lock();
	do 
	{
		//1. CURL return error
		if(pHeaderInfo->m_nCurlResult != CURLE_OK)
		{
			CString szCurlResult;
			szCurlResult.Format("(%d) %s", pHeaderInfo->m_nCurlResult, 
				curl_easy_strerror((CURLcode)pHeaderInfo->m_nCurlResult));
			
			CurrentStatusChanged(TSE_END_WITH_ERROR, szCurlResult, TRUE, FALSE);
			
			pStatusInfo->m_nStatusCode = TSE_END_WITH_ERROR;
			pStatusInfo->m_szDetail = szCurlResult;
			
			nResult = TSE_END_WITH_ERROR;
			break;
		}
		//2. HTTP return error
		if(pHeaderInfo->m_nHTTPCode != 200 && pHeaderInfo->m_nHTTPCode != 206)
		{	
			CurrentStatusChanged(TSE_END_WITH_ERROR, pHeaderInfo->m_szStatusLine, TRUE, FALSE);
			
			pStatusInfo->m_nStatusCode = TSE_END_WITH_ERROR;
			pStatusInfo->m_szDetail = pHeaderInfo->m_szStatusLine;
			
			nResult = TSE_END_WITH_ERROR;
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

	if(m_pHeaderParser != NULL)
	{
		delete m_pHeaderParser;
		m_pHeaderParser = NULL;
	}
	m_criticalSection.Unlock();

	if(nResult != TSE_TRANSFERRING)
	{
		ASSERT(pStatusInfo->m_nStatusCode != TSE_TRANSFERRING);

		CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)pStatusInfo);
	}
	return nResult;
}
UINT CDownloaderMgr::PreDownload()
{
	UINT nResult = 0;
	CString szLog;

	//1. pre-GetHeader
	nResult = PreGetHeader();
	if(nResult != TSE_TRANSFERRING)
	{
		return nResult;
	}

	//2. in-GetHeader
	m_criticalSection.Lock();
	if(m_pHeaderParser != NULL)
	{
		ASSERT(m_pHeaderParser == NULL);
	}
	m_pHeaderParser = new CGetHeader();
	m_criticalSection.Unlock();

	m_pHeaderParser->Start(m_dlParam.m_szUrl);
	CHeaderInfo* pHeaderInfo = m_pHeaderParser->GetHeaderInfo();

	//3. post-GetHeader
	CStatusInfo statusInfo;
	nResult = PostGetHeader(pHeaderInfo, &statusInfo);
	if(nResult != TSE_TRANSFERRING)
	{
		return nResult;
	}

	return nResult;
}
UINT CDownloaderMgr::StartDownload()
{
	CurrentStatusChanged(TSE_TRANSFERRING, NULL, FALSE);

	//transfer
	UINT nResult = 0;

	CString szLog;
	szLog.Format("[StartDownload]: ThreadID=%d, URL=%s", GetCurrentThreadId(), m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	nResult = PreDownload();
	if(nResult != TSE_TRANSFERRING)
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