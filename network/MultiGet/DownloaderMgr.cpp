// DownloaderMgr.cpp: implementation of the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderMgr.h"
#include "EasyDownloader.h"
#include "SegmentDownloader.h"
#include "CommonUtils.h"
#include "TimeCost.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef enum
{
	PHASE_INIT = 0,
	PHASE_BEFORE_WORKER_THREAD,
	PHASE_PRE_GET_HEADER,
	PHASE_IN_GET_HEADER,
	PHASE_POST_GET_HEADER,
	PHASE_DOWNLOADER_WORKING,
	PHASE_DOWNLOADER_PAUSED,
	PHASE_DOWNLOADER_END,
	PHASE_END,


	PHASE_NEW_BEFORE_GET_HEADER,
	PHASE_NEW_IN_GET_HEADER,
	PHASE_NEW_IN_DOWNLOADING,
	PHASE_NEW_DOWNLOADER_END,
	PHASE_NEW_OTHER_END
} PhaseEnum;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD WINAPI CDownloaderMgr::StartDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	UINT nResult = pThis->StartDownload();

	return nResult;
}

DWORD WINAPI CDownloaderMgr::ResumeDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	UINT nResult = pThis->ResumeDownload();

	return nResult;
}

CDownloaderMgr::CDownloaderMgr() : m_pDownloader(NULL), m_pHeaderParser(NULL),
 m_dlState(TSE_READY), m_nPhase(PHASE_INIT), m_hWorkerThread(NULL)
{
}

CDownloaderMgr::~CDownloaderMgr()
{
	m_criticalSection.Lock();
	//Just remove the memory, WaitUntilStop is used to stop thread!
	if(m_pDownloader != NULL)
	{
		delete m_pDownloader;
		m_pDownloader = NULL;
	}
	m_criticalSection.Unlock();
}

int CDownloaderMgr::DoAction()
{
	CString szLog;
	
	int rc = 0;
	CDownloadState dlState;
	GetState(dlState);

	szLog.Format("[DoAction]: state=%d", dlState.GetState());
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	ASSERT( (dlState.GetState() >= TSE_READY && dlState.GetState() <= TSE_DESTROYED) && 
		dlState.GetState() != TSE_TRANSFERRING );

	//1. check if this task was removed
	if(dlState.GetState() == TSE_DESTROYED)
	{
		rc = DELETE_BY_EXTERNAL;
	}

	//2. close thread handle
	m_criticalSection.Lock();

	ASSERT(m_hWorkerThread);
	::CloseHandle(m_hWorkerThread);
	m_hWorkerThread = NULL;

	m_criticalSection.Unlock();

	//3. send complete message to GUI
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, m_dlParam.m_nTaskID, (LPARAM)&dlState);

	return rc;
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
}

void CDownloaderMgr::GetState(CDownloadState& dlState)
{
	m_criticalSection.Lock();
	if(IsDownloaderExist())
	{
		ASSERT(m_pDownloader != NULL);
		m_pDownloader->GetState(dlState);
	}
	else
	{
		dlState = m_dlState;
	}
	m_criticalSection.Unlock();
}
DWORD CDownloaderMgr::GetState()
{
	DWORD nResult = 0;

	m_criticalSection.Lock();
	if(IsDownloaderExist())
	{
		ASSERT(m_pDownloader != NULL);
		nResult = m_pDownloader->GetState();
	}
	else
	{
		nResult = m_dlState.GetState();
	}
	m_criticalSection.Unlock();

	return nResult;
}

int CDownloaderMgr::Start()
{
	CString szLog;

	m_criticalSection.Lock();
	
	if(m_hWorkerThread != NULL)
	{
		szLog.Format("Task[%02d] can't be started, because the thread handle still exist. phase=%d", m_dlParam.m_nTaskID, m_nPhase);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		m_criticalSection.Unlock();
		return m_nPhase;
	}
	//Only that init or finished phase can be started
	if(m_nPhase != PHASE_INIT && m_nPhase != PHASE_NEW_OTHER_END && m_nPhase != PHASE_NEW_DOWNLOADER_END)
	{
		szLog.Format("Task[%02d] can't be started, because of wrong phase. phase=%d", m_dlParam.m_nTaskID, m_nPhase);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		m_criticalSection.Unlock();
		return m_nPhase;
	}
	if(m_pDownloader != NULL)
	{
		szLog.Format("Task[%02d] has been finished, but downloader not deleted yet. phase=%d", m_dlParam.m_nTaskID, m_nPhase);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		ASSERT(m_nPhase == PHASE_NEW_DOWNLOADER_END);

		if(m_pDownloader->GetState() == TSE_PAUSED)
		{
			szLog.Format("Task[%02d] can't be started, because it is paused. phase = %d", m_dlParam.m_nTaskID, m_nPhase);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

			m_criticalSection.Unlock();
			return m_nPhase;
		}

		delete m_pDownloader;
		m_pDownloader = NULL;
	}
	//In case of the task has been finished
	m_nPhase = PHASE_NEW_BEFORE_GET_HEADER;
	//Change the state to transferring
	m_dlState.SetState(TSE_TRANSFERRING);

	m_criticalSection.Unlock();

	//If we can go here, we should send message to GUI to notify it's now in transferring.
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&m_dlState));

	DWORD dwThreadId;
	m_hWorkerThread = ::CreateThread(NULL, 0, StartDownloadProc, this, CREATE_SUSPENDED, &dwThreadId);
	SYS_THREAD_MONITOR()->AddMoniteeWaitForExist(m_hWorkerThread, this);

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d] worker thread started, Thread ID=%d", m_dlParam.m_nTaskID, dwThreadId);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}

	::ResumeThread(m_hWorkerThread);
	
	return 0;
}

int CDownloaderMgr::Resume()
{
	CString szLog;
	
	m_criticalSection.Lock();
	
	//The download process is paused before real downloading started
	if(m_nPhase != PHASE_DOWNLOADER_PAUSED)
	{
		if(m_nPhase != PHASE_END && m_nPhase != PHASE_DOWNLOADER_END)
		{
			szLog.Format("Task[%02d] can't be resumed, because of wrong phase. phase=%d", 
				m_dlParam.m_nTaskID, m_nPhase);
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

			m_criticalSection.Unlock();
			return m_nPhase;
		}
		else
		{
			szLog.Format("Task[%02d] - Resume: downloader is not paused. Start again. phase=%d", 
				m_dlParam.m_nTaskID, m_nPhase);
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

			m_criticalSection.Unlock();
			return Start();
		}
	}

	ASSERT(m_pDownloader != NULL);
	m_pDownloader->SetState(TSE_TRANSFERRING);
	//Change the state to transferring, this is useless because the current state is determined by m_pDownloader
	m_dlState.SetState(TSE_TRANSFERRING);
	
	m_criticalSection.Unlock();
	
	//If we can go here, we should send message to GUI to notify it's now in transferring.
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&m_dlState));
	
	//AfxBeginThread(ResumeDownloadProc, (LPVOID)this);
	m_hWorkerThread = ::CreateThread(NULL, 0, ResumeDownloadProc, this, 0, NULL);
	SYS_THREAD_MONITOR()->AddMoniteeWaitForExist(m_hWorkerThread, this);
	
	return 0;
}

int CDownloaderMgr::Stop()
{
	int nResult;

	m_criticalSection.Lock();

	if(IsDownloaderExist())
	{
		ASSERT(m_pDownloader != NULL);
		nResult = m_pDownloader->Pause();
	}
	else
	{
		//Pause is allowed
		if( (m_dlState.GetAccess(DL_OPER_FLAG_PAUSE) & DL_OPER_FLAG_PAUSE) )
		{
			m_dlState.SetState(TSE_PAUSING);
		}
		nResult = m_dlState.GetState();
	}
	m_criticalSection.Unlock();
	
	return nResult;
}
int CDownloaderMgr::Pause()
{
	int nResult;

	m_criticalSection.Lock();

	if(IsDownloaderExist())
	{
		ASSERT(m_pDownloader != NULL);
		nResult = m_pDownloader->Pause();
	}
	else
	{
		//Pause is allowed
		if( (m_dlState.GetAccess(DL_OPER_FLAG_PAUSE) & DL_OPER_FLAG_PAUSE) )
		{
			m_dlState.SetState(TSE_PAUSING);
		}
		nResult = m_dlState.GetState();
	}
	m_criticalSection.Unlock();
	
	return nResult;
}
int CDownloaderMgr::Destroy()
{
	int nResult;
	
	m_criticalSection.Lock();
	//Worker thread already exit
	if(m_hWorkerThread == NULL)
	{
		nResult = DELETE_BY_EXTERNAL;
	}
	else
	{
		if(IsDownloaderExist())
		{
			ASSERT(m_pDownloader != NULL);
			nResult = m_pDownloader->Destroy();
		}
		else
		{
			//Remove is allowed
			if( (m_dlState.GetAccess(DL_OPER_FLAG_REMOVE) & DL_OPER_FLAG_REMOVE) )
			{
				//Still in transferring
				if(m_dlState.GetState() == TSE_TRANSFERRING)
				{
					m_dlState.SetState(TSE_DESTROYING);
				}
				//Already paused or in other states
				else
				{
					//...
				}
			}
			nResult = m_dlState.GetState();
		}
	}
	m_criticalSection.Unlock();
	
	return nResult;
}

BOOL CDownloaderMgr::IsResumable()
{
	ASSERT(m_pDownloader != NULL);
	return m_pDownloader->IsResumable();
}

BOOL CDownloaderMgr::IsDownloaderExist()
{
	if(m_nPhase == PHASE_NEW_IN_DOWNLOADING || m_nPhase == PHASE_NEW_DOWNLOADER_END)
	{
		ASSERT(m_pDownloader != NULL);
		return TRUE;
	}
	return FALSE;
}

UINT CDownloaderMgr::CheckStatus()
{
	UINT nResult = 0;
	do 
	{
		if(m_dlState.GetState() == TSE_DESTROYING)
		{
			m_dlState.SetState(TSE_DESTROYED);
			break;
		}
		if(m_dlState.GetState() == TSE_PAUSING)
		{
			m_dlState.SetState(TSE_PAUSED);
			break;
		}
		if(m_dlState.GetState() == TSE_STOPPING)
		{
			m_dlState.SetState(TSE_STOPPED);
			break;
		}
	} while (FALSE);

	nResult = m_dlState.GetState();
	return nResult;
}

UINT CDownloaderMgr::PreGetHeader()
{
	UINT nResult = 0;

	//Phase checking and setting	
	ASSERT(m_nPhase == PHASE_NEW_BEFORE_GET_HEADER);
	ASSERT(m_pDownloader == NULL);
	ASSERT(m_pHeaderParser == NULL);
	
	//Get the current state
	nResult = CheckStatus();

	CString szLog;
	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: PreGetHeader - current state = %d", m_dlParam.m_nTaskID, nResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}

	return nResult;
}
UINT CDownloaderMgr::PostGetHeader()
{
	UINT nResult = 0;
	CString szLog;

	//1. Phase checking and setting	
	ASSERT(m_nPhase == PHASE_NEW_IN_GET_HEADER);
	ASSERT(m_pDownloader == NULL);
	ASSERT(m_pHeaderParser != NULL);
	
	//Get the current state
	nResult = CheckStatus();

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: PostGetHeader after CheckStatus - current state = %d", m_dlParam.m_nTaskID, nResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	
	if(nResult != TSE_TRANSFERRING)
	{	
		//Don't care about the content of header
		ASSERT(m_pHeaderParser != NULL);
		delete m_pHeaderParser;
		m_pHeaderParser = NULL;

		return nResult;
	}


	//2. Parse Header content
	CString szDetail;
	CHeaderInfo* pHeaderInfo = m_pHeaderParser->GetHeaderInfo();
	do 
	{
		//1. CURL return error
		if(pHeaderInfo->m_nCurlResult != CURLE_OK)
		{
			szDetail.Format("(%d) %s", pHeaderInfo->m_nCurlResult, curl_easy_strerror((CURLcode)pHeaderInfo->m_nCurlResult));
			m_dlState.SetState(TSE_END_WITH_ERROR, szDetail);
			break;
		}
		//2. HTTP return error
		if(pHeaderInfo->m_nHTTPCode != 200 && pHeaderInfo->m_nHTTPCode != 206)
		{
			m_dlState.SetState(TSE_END_WITH_ERROR, pHeaderInfo->m_szStatusLine);
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
				//TODO
				m_pDownloader = new CSegmentDownloader(TSE_TRANSFERRING);
				
				szLog.Format("Segment download support: [N]. HTTPCode=%d, Content-Length=%d", 
					pHeaderInfo->m_nHTTPCode, pHeaderInfo->m_nContentLength);
			}
		}
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	} while (FALSE);

	ASSERT(m_pHeaderParser != NULL);
	delete m_pHeaderParser;
	m_pHeaderParser = NULL;

	nResult = m_dlState.GetState();

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: PostGetHeader after parsing header - current state = %d", m_dlParam.m_nTaskID, nResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}

	return nResult;
}
UINT CDownloaderMgr::PreDownload()
{
	DWORD nResult = 0;
	CString szLog;

	//1. PreGetHeader
	m_criticalSection.Lock();
	nResult = PreGetHeader();
	if(nResult != TSE_TRANSFERRING)
	{
		m_criticalSection.Unlock();
		return nResult;
	}

	//2. InGetHeader
	ASSERT(m_nPhase == PHASE_NEW_BEFORE_GET_HEADER);
	ASSERT(m_pDownloader == NULL);
	ASSERT(m_pHeaderParser == NULL);

	m_pHeaderParser = new CGetHeader();
	m_nPhase = PHASE_NEW_IN_GET_HEADER;
	m_criticalSection.Unlock();

	//Time-costing operation, start to retrieve HTTP header
	m_pHeaderParser->Start(m_dlParam.m_szUrl);

	//3. PostGetHeader
	m_criticalSection.Lock();
	nResult = PostGetHeader();
	if(nResult != TSE_TRANSFERRING)
	{
		ASSERT(m_pDownloader == NULL);
		ASSERT(m_pHeaderParser == NULL);
		m_criticalSection.Unlock();
		return nResult;
	}

	ASSERT(m_nPhase == PHASE_NEW_IN_GET_HEADER);
	ASSERT(m_pDownloader != NULL);
	ASSERT(m_pHeaderParser == NULL);
	
	m_nPhase = PHASE_NEW_IN_DOWNLOADING;
	m_criticalSection.Unlock();

	return nResult;
}
DWORD CDownloaderMgr::StartDownload()
{
	DWORD dwResult = 0;
	CString szLog;

	//Start logging
	szLog.Format("[StartDownload]: Task[%02d]: ThreadID=%d, URL=%s", m_dlParam.m_nTaskID, GetCurrentThreadId(), 
		m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	//PreDownload
	dwResult = PreDownload();
	if(dwResult != TSE_TRANSFERRING)
	{
		m_criticalSection.Lock();
		m_nPhase = PHASE_NEW_OTHER_END;
		m_criticalSection.Unlock();

		/*
		let the thread monitor to do the works
		::SetEvent(m_hStopEvent);
		CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)&m_dlState);
		*/

		return dwResult;
	}
	
	//Actually start downloading
	ASSERT(m_pDownloader != NULL);
	m_pDownloader->Init(m_dlParam);
	
	CTimeCost timeCost;

	dwResult = m_pDownloader->Start();
	timeCost.UpdateCurrClock();

	szLog.Format("[StartDownload]: Task[%02d]: Time Cost (%d)", m_dlParam.m_nTaskID, timeCost.GetTimeCost());
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	m_criticalSection.Lock();
	m_nPhase = PHASE_NEW_DOWNLOADER_END;
	m_criticalSection.Unlock();

	//Send Msg
	//let the thread monitor to do the works
	//CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, m_dlParam.m_nTaskID, (LPARAM)dwResult);
	
	return dwResult;
}

DWORD CDownloaderMgr::ResumeDownload()
{
	UINT nResult = 0;
	CString szLog;

	szLog.Format("[ResumeDownload]: Task[%02d]: ThreadID=%d, URL=%s", m_dlParam.m_nTaskID, GetCurrentThreadId(), 
		m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	ASSERT(m_pDownloader != NULL);
	CTimeCost timeCost;

	m_criticalSection.Lock();
	m_nPhase = PHASE_DOWNLOADER_WORKING;
	m_criticalSection.Unlock();

	nResult = m_pDownloader->Resume();
	timeCost.UpdateCurrClock();

	szLog.Format("[ResumeDownload]: Task[%02d]: Time Cost (%d)", m_dlParam.m_nTaskID, timeCost.GetTimeCost());
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	DWORD dwState = CCommonUtils::ResultCode2StatusCode(nResult);	
	m_criticalSection.Lock();
	if(dwState == TSE_PAUSED || dwState == TSE_STOPPED)
	{
		m_nPhase = PHASE_DOWNLOADER_PAUSED;
	}
	else
	{
		m_nPhase = PHASE_DOWNLOADER_END;
	}
	m_criticalSection.Unlock();

	//Send Msg
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, m_dlParam.m_nTaskID, (LPARAM)nResult);
		
	return nResult;
}

void CDownloaderMgr::ChangeDownloader(CDownloader* pDownloader)
{
	m_pNewDownloader = pDownloader;
}