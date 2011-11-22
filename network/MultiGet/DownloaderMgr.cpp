// DownloaderMgr.cpp: implementation of the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderMgr.h"
#include "CommonUtils.h"
#include "HeaderDownloader.h"
#include "SegmentDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME("DMGR")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD WINAPI CDownloaderMgr::StartDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	DWORD dwResult = pThis->StartDownload();

	return dwResult;
}

DWORD WINAPI CDownloaderMgr::ReDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	DWORD dwResult = pThis->ReDownload();

	return dwResult;
}

CDownloaderMgr::CDownloaderMgr() : m_hWorkerThread(NULL)
{
	CString szLog;
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: Constructor called. DMGR(0x%08X)", m_dlParam.m_nTaskID, (int)this);
		LOG4CPLUS_TRACE_STR(THE_LOGGER, (LPCTSTR)szLog)
	}
}

CDownloaderMgr::~CDownloaderMgr()
{
	//Worker thread should be stopped already
	ASSERT(m_hWorkerThread == NULL);

	CString szLog;
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: Before ~DMGR(0x%08X), m_pCurDownloader=0x%08X", m_dlParam.m_nTaskID, (int)this, 
			(int)m_pCurDownloader);
		LOG4CPLUS_TRACE_STR(THE_LOGGER, (LPCTSTR)szLog)
	}

	if(m_pCurDownloader != NULL)
	{
		delete m_pCurDownloader;
		m_pCurDownloader = NULL;
	}

	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: After  ~DMGR(0x%08X), m_pCurDownloader=0x%08X", m_dlParam.m_nTaskID, (int)this, 
			(int)m_pCurDownloader);
		LOG4CPLUS_TRACE_STR(THE_LOGGER, (LPCTSTR)szLog)
	}
}

int CDownloaderMgr::Finalize()
{
	CString szLog;
	
	int rc = 0;
	CDownloadState dlState;
	GetState(dlState);

	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: Finalize: state=%d", m_dlParam.m_nTaskID, dlState.GetState());
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}

	ASSERT( (dlState.GetState() >= TSE_READY && dlState.GetState() <= TSE_DESTROYED) && 
		dlState.GetState() != TSE_TRANSFERRING );

	//1. send complete message to GUI
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, m_dlParam.m_nTaskID, (LPARAM)&dlState);

	//2. close thread handle
	m_lock.Lock();

	//Case: when the task has been paused/completed, try to destroy it but the thread handle still exist,
	//failed to delete the CDownloaderMgr
	if(NoLockGetState() == TSE_DESTROYED)
	{
		rc = DELETE_BY_EXTERNAL;
	}
	ASSERT(m_hWorkerThread);
	::CloseHandle(m_hWorkerThread);
	m_hWorkerThread = NULL;

	m_lock.Unlock();

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

int CDownloaderMgr::Start()
{
	CString szLog;

	m_lock.Lock();
	
	if(m_hWorkerThread != NULL)
	{
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Task[%02d] can't be started, because the thread handle still exist.", m_dlParam.m_nTaskID);
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}
			
		m_lock.Unlock();
		return 1;
	}

	//Start is not allowed
	if( !NoLockIsOperEnabled(DL_OPER_FLAG_START) )
	{
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Task[%02d] can't be started, because of wrong state = %d", m_dlParam.m_nTaskID, 
				NoLockGetState());
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}

		m_lock.Unlock();
		return 2;
	}

	//Change the state to transferring
	NoLockSetState(TSE_TRANSFERRING);

	m_lock.Unlock();

	//If we can go here, we should send message to GUI to notify it's now in transferring.
	//Don't worry other threads to modify state now. This is still in GUI thread, no other thread can modify it
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&m_dlCurState));

	DWORD dwThreadId;
	m_hWorkerThread = ::CreateThread(NULL, 0, StartDownloadProc, this, CREATE_SUSPENDED, &dwThreadId);
	SYS_THREAD_MONITOR()->AddMoniteeWaitForExist(m_hWorkerThread, this);
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d] worker thread started, Thread ID=%d", m_dlParam.m_nTaskID, dwThreadId);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
	}

	::ResumeThread(m_hWorkerThread);
	
	return 0;
}

int CDownloaderMgr::ReStart()
{
	CString szLog;
	
	m_lock.Lock();
	
	if(m_hWorkerThread != NULL)
	{
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Task[%02d] can't be redownloaded, because the thread handle still exist.", m_dlParam.m_nTaskID);
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}
			
		m_lock.Unlock();
		return 1;
	}
	
	//ReStart is not allowed
	if( !NoLockIsOperEnabled(DL_OPER_FLAG_REDOWNLOAD) )
	{
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Task[%02d] can't be redownloaded, because of wrong state = %d", m_dlParam.m_nTaskID, 
				NoLockGetState());
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}
		
		m_lock.Unlock();
		return 2;
	}
	
	//Change the state to transferring
	NoLockSetState(TSE_TRANSFERRING);
	m_lock.Unlock();
	
	//If we can go here, we should send message to GUI to notify it's now in transferring.
	//Don't worry other threads to modify state now. This is still in GUI thread, no other thread can modify it
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&m_dlCurState));
	
	DWORD dwThreadId;
	m_hWorkerThread = ::CreateThread(NULL, 0, ReDownloadProc, this, CREATE_SUSPENDED, &dwThreadId);
	SYS_THREAD_MONITOR()->AddMoniteeWaitForExist(m_hWorkerThread, this);

	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d] worker thread restarted, Thread ID=%d", m_dlParam.m_nTaskID, dwThreadId);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
	}
	
	::ResumeThread(m_hWorkerThread);
	
	return 0;
}

int CDownloaderMgr::Pause()
{
	int nResult;

	m_lock.Lock();
	//Pause is allowed
	if( NoLockIsOperEnabled(DL_OPER_FLAG_PAUSE) )
	{
		NoLockSetState(TSE_PAUSING);
	}
	nResult = NoLockGetState();
	m_lock.Unlock();
	
	return nResult;
}
int CDownloaderMgr::Destroy()
{
	int nResult;
	
	m_lock.Lock();
	//Worker thread already exit
	if(m_hWorkerThread == NULL)
	{
		//If this task is already paused/completed/end-with-error
		if(NoLockGetState() != TSE_DESTROYED)
		{
			nResult = DELETE_BY_EXTERNAL;
		}
		else
		{
			nResult = NoLockGetState();
		}
	}
	else
	{
		//Remove is allowed
		if( NoLockIsOperEnabled(DL_OPER_FLAG_REMOVE) )
		{
			//Still in transferring
			if(NoLockGetState() == TSE_TRANSFERRING || NoLockGetState() == TSE_PAUSING)
			{
				NoLockSetState(TSE_DESTROYING);
			}
			//The states maybe: paused, completed, end with error.
			//change the state to let the thread monitor to delete downloader manager.
			else
			{
				//TODO?
				NoLockSetState(TSE_DESTROYED);
			}
		}
		nResult = NoLockGetState();
	}
	m_lock.Unlock();
	
	return nResult;
}

DWORD CDownloaderMgr::StartDownload()
{
	int dwResult = 0;
	CString szLog;
	
	//Start logging
	szLog.Format("Task[%02d]: [StartDownload] - ThreadID=%d, URL=%s", m_dlParam.m_nTaskID, GetCurrentThreadId(), 
		m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)

	CTimeCost timeCost;

	//really new start
	if(m_pCurDownloader == NULL)
	{
		m_pCurDownloader = new CHeaderDownloader(this);
		m_pCurDownloader->Init(m_dlParam);
	}
	//if m_pDownloader != NULL, that means the previous download was paused or ended with other reasons

	CDownloader* pDownloader = NULL;
	do 
	{
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::INFO_LOG_LEVEL))
		{
			szLog.Format("Task[%02d]: [StartDownload] - Before [%s] Start", m_dlParam.m_nTaskID, m_pCurDownloader->GetName());
			LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)
		}

		dwResult = m_pCurDownloader->Start();

		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::INFO_LOG_LEVEL))
		{
			szLog.Format("Task[%02d]: [StartDownload] - After  [%s] Start. Result = %s", m_dlParam.m_nTaskID, 
				m_pCurDownloader->GetName(), CCommonUtils::State2Str(dwResult));
			LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)
		}

		if(dwResult != TSE_TRANSFERRING)
		{
			ASSERT(m_pCurDownloader->GetNext() == NULL);

			//NOTE: MUST NOT delete the last downloader
			break;
		}

		ASSERT(m_pCurDownloader->GetNext() != NULL);
		pDownloader = m_pCurDownloader;

		m_pCurDownloader = m_pCurDownloader->GetNext();
		//let the previous downloader to init 
		//m_pCurDownloader->Init(m_dlParam);

		delete pDownloader;
		pDownloader = NULL;

	} while (TRUE);

	if( IS_LOG_ENABLED(GET_LOGGER(TIME_COST_NAME), log4cplus::INFO_LOG_LEVEL) )
	{
		timeCost.UpdateCurrClock();
		
		szLog.Format("Task[%02d]: [StartDownload] - TimeCost %u(ms)", m_dlParam.m_nTaskID, timeCost.GetTimeCost());
		LOG4CPLUS_INFO_STR(GET_LOGGER(TIME_COST_NAME), (LPCTSTR)szLog)
	}
	return dwResult;
}

DWORD CDownloaderMgr::ReDownload()
{
	int dwResult = 0;
	CString szLog;
	
	//Start logging
	szLog.Format("Task[%02d]: [ReDownload] - ThreadID=%d, URL=%s", m_dlParam.m_nTaskID, GetCurrentThreadId(), 
		m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)

	CTimeCost timeCost;

	//if m_pDownloader != NULL, that means the previous download was paused or ended with other reasons
	//The previous downloader exist, delete it. Start from the beginning
	if(m_pCurDownloader != NULL)
	{
		delete m_pCurDownloader;
		m_pCurDownloader = NULL;
	}
	//really new start
	m_pCurDownloader = new CHeaderDownloader(this);
	m_pCurDownloader->Init(m_dlParam);

	CDownloader* pDownloader = NULL;
	do 
	{
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::INFO_LOG_LEVEL))
		{
			szLog.Format("Task[%02d]: [ReDownload] - Before [%s] Start", m_dlParam.m_nTaskID, m_pCurDownloader->GetName());
			LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)
		}

		dwResult = m_pCurDownloader->Start();

		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::INFO_LOG_LEVEL))
		{
			szLog.Format("Task[%02d]: [ReDownload] - After  [%s] Start. Result = %s", m_dlParam.m_nTaskID, 
				m_pCurDownloader->GetName(), CCommonUtils::State2Str(dwResult));
			LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)
		}

		if(dwResult != TSE_TRANSFERRING)
		{
			ASSERT(m_pCurDownloader->GetNext() == NULL);

			//NOTE: MUST NOT delete the last downloader
			break;
		}

		ASSERT(m_pCurDownloader->GetNext() != NULL);
		pDownloader = m_pCurDownloader;

		m_pCurDownloader = m_pCurDownloader->GetNext();
		//let the previous downloader to init 
		//m_pCurDownloader->Init(m_dlParam);

		delete pDownloader;
		pDownloader = NULL;
		
	} while (TRUE);

	if( IS_LOG_ENABLED(GET_LOGGER(TIME_COST_NAME), log4cplus::INFO_LOG_LEVEL) )
	{
		timeCost.UpdateCurrClock();
		
		szLog.Format("Task[%02d]: [StartDownload] - TimeCost %u(ms)", m_dlParam.m_nTaskID, timeCost.GetTimeCost());
		LOG4CPLUS_INFO_STR(GET_LOGGER(TIME_COST_NAME), (LPCTSTR)szLog)
	}
	
	return dwResult;
}
