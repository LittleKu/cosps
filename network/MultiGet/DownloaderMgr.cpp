// DownloaderMgr.cpp: implementation of the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderMgr.h"
#include "HeaderDownloader.h"
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
}

CDownloaderMgr::~CDownloaderMgr()
{
	//Worker thread should be stopped already
	ASSERT(m_hWorkerThread == NULL);

	if(m_pCurDownloader != NULL)
	{
		delete m_pCurDownloader;
		m_pCurDownloader = NULL;
	}
}

int CDownloaderMgr::DoAction()
{
	CString szLog;
	
	int rc = 0;
	CDownloadState dlState;
	GetState(dlState);

	szLog.Format("Task[%02d]: DoAction: state=%d", m_dlParam.m_nTaskID, dlState.GetState());
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	ASSERT( (dlState.GetState() >= TSE_READY && dlState.GetState() <= TSE_DESTROYED) && 
		dlState.GetState() != TSE_TRANSFERRING );

	//1. check if this task was removed
	if(dlState.GetState() == TSE_DESTROYED)
	{
		rc = DELETE_BY_EXTERNAL;
	}

	//3. send complete message to GUI
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, m_dlParam.m_nTaskID, (LPARAM)&dlState);

	//2. close thread handle
	m_lock.Lock();

	//Case: when the task has been paused/completed, try to destroy it but the thread handle still exist,
	//failed to delete the CDownloaderMgr
	if(m_dlCurState.GetState() == TSE_DESTROYED)
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
		szLog.Format("Task[%02d] can't be started, because the thread handle still exist.", m_dlParam.m_nTaskID);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		m_lock.Unlock();
		return 1;
	}

	//Start is not allowed
	if( (m_dlCurState.GetAccess(DL_OPER_FLAG_START) & DL_OPER_FLAG_START) == 0 )
	{
		szLog.Format("Task[%02d] can't be started, because of wrong state = %d", m_dlParam.m_nTaskID, m_dlCurState.GetState());
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		m_lock.Unlock();
		return 2;
	}

	//Change the state to transferring
	m_dlCurState.SetState(TSE_TRANSFERRING);

	m_lock.Unlock();

	//If we can go here, we should send message to GUI to notify it's now in transferring.
	//Don't worry other threads to modify state now. This is still in GUI thread, no other thread can modify it
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&m_dlCurState));

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

int CDownloaderMgr::ReStart()
{
	CString szLog;
	
	m_lock.Lock();
	
	if(m_hWorkerThread != NULL)
	{
		szLog.Format("Task[%02d] can't be redownloaded, because the thread handle still exist.", m_dlParam.m_nTaskID);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		m_lock.Unlock();
		return 1;
	}
	
	//ReStart is not allowed
	if( (m_dlCurState.GetAccess(DL_OPER_FLAG_REDOWNLOAD) & DL_OPER_FLAG_REDOWNLOAD) == 0 )
	{
		szLog.Format("Task[%02d] can't be redownloaded, because of wrong state = %d", m_dlParam.m_nTaskID, m_dlCurState.GetState());
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
		m_lock.Unlock();
		return 2;
	}
	
	//Change the state to transferring
	m_dlCurState.SetState(TSE_TRANSFERRING);
	
	m_lock.Unlock();
	
	//If we can go here, we should send message to GUI to notify it's now in transferring.
	//Don't worry other threads to modify state now. This is still in GUI thread, no other thread can modify it
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&m_dlCurState));
	
	DWORD dwThreadId;
	m_hWorkerThread = ::CreateThread(NULL, 0, ReDownloadProc, this, CREATE_SUSPENDED, &dwThreadId);
	SYS_THREAD_MONITOR()->AddMoniteeWaitForExist(m_hWorkerThread, this);

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d] worker thread restarted, Thread ID=%d", m_dlParam.m_nTaskID, dwThreadId);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	
	::ResumeThread(m_hWorkerThread);
	
	return 0;
}

int CDownloaderMgr::Pause()
{
	int nResult;

	m_lock.Lock();
	//Pause is allowed
	if( (m_dlCurState.GetAccess(DL_OPER_FLAG_PAUSE) & DL_OPER_FLAG_PAUSE) )
	{
		m_dlCurState.SetState(TSE_PAUSING);
	}
	nResult = m_dlCurState.GetState();
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
		nResult = DELETE_BY_EXTERNAL;
	}
	else
	{
		//Remove is allowed
		if( (m_dlCurState.GetAccess(DL_OPER_FLAG_REMOVE) & DL_OPER_FLAG_REMOVE) )
		{
			//Still in transferring
			if(m_dlCurState.GetState() == TSE_TRANSFERRING)
			{
				m_dlCurState.SetState(TSE_DESTROYING);
			}
			//The states maybe: paused, completed, end with error.
			//change the state to let the thread monitor to delete downloader manager.
			else
			{
				//TODO?
				m_dlCurState.SetState(TSE_DESTROYED);
			}
		}
		nResult = m_dlCurState.GetState();
	}
	m_lock.Unlock();
	
	return nResult;
}

DWORD CDownloaderMgr::StartDownload()
{
	int dwResult = 0;
	CString szLog;
	
	//Start logging
	szLog.Format("[StartDownload]: Task[%02d]: ThreadID=%d, URL=%s", m_dlParam.m_nTaskID, GetCurrentThreadId(), 
		m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

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
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("[StartDownload]: Task[%02d]: Before Start. %s", m_dlParam.m_nTaskID, m_pCurDownloader->GetName());
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}

		dwResult = m_pCurDownloader->Start();

		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("[StartDownload]: Task[%02d]: After Start. %d, %s", m_dlParam.m_nTaskID, dwResult,
				m_pCurDownloader->GetName());
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
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

	return dwResult;
}

DWORD CDownloaderMgr::ReDownload()
{
	int dwResult = 0;
	CString szLog;
	
	//Start logging
	szLog.Format("[ReDownload]: Task[%02d]: ThreadID=%d, URL=%s", m_dlParam.m_nTaskID, GetCurrentThreadId(), 
		m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

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
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("[ReDownload]: Task[%02d]: Before Start. %s", m_dlParam.m_nTaskID, m_pCurDownloader->GetName());
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}

		dwResult = m_pCurDownloader->Start();

		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("[ReDownload]: Task[%02d]: After Start. %d, %s", m_dlParam.m_nTaskID, dwResult,
				m_pCurDownloader->GetName());
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
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
	
	return dwResult;
}
