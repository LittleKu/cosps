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
	PHASE_END
} PhaseEnum;

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

CDownloaderMgr::CDownloaderMgr() : m_pDownloader(NULL), m_pHeaderParser(NULL),
 m_dlState(TSE_READY), m_nPhase(PHASE_INIT)
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
	bResult = IsDownloaderExist();
	m_criticalSection.Unlock();

	if(bResult)
	{
		ASSERT(m_pDownloader != NULL);
		m_pDownloader->WaitUntilStop();
		return;
	}

	CString szLog;
	DWORD dwResult;
	UINT nStatus = GetCurrentStatus();

	BOOL bTransferring = (nStatus == TSE_TRANSFERRING || nStatus == TSE_PAUSING
		|| nStatus == TSE_STOPPING || nStatus == TSE_DESTROYING);
	while( bTransferring )
	{
		Destroy();
		
		szLog.Format("Task [%02d] is transferring, wait until it stopped.", m_dlParam.m_nTaskID);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		dwResult = ::WaitForSingleObject(m_hStopEvent, INFINITE);
		
		szLog.Format("Task [%02d] WaitForSingleObject returned 0x%08X", m_dlParam.m_nTaskID, dwResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		nStatus = GetCurrentStatus();
		bTransferring = (nStatus == TSE_TRANSFERRING || nStatus == TSE_PAUSING 
			|| nStatus == TSE_STOPPING || nStatus == TSE_DESTROYING);
	}
	
	szLog.Format("Task [%02d] Stopped succesfully. Current status=%d", m_dlParam.m_nTaskID, nStatus);
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
}

DWORD CDownloaderMgr::GetCurrentStatus()
{
	DWORD nResult = 0;

	m_criticalSection.Lock();
	if(IsDownloaderExist())
	{
		ASSERT(m_pDownloader != NULL);
		nResult = m_pDownloader->GetCurrentStatus();
	}
	else
	{
		nResult = m_dlState.GetState();
	}
	m_criticalSection.Unlock();

	return nResult;
}

DWORD CDownloaderMgr::GetAccess()
{
	return CDownloadState::GetAccess(GetCurrentStatus(), DL_OPER_FLAG_ALL);
}


int CDownloaderMgr::Start()
{
	CString szLog;

	m_criticalSection.Lock();

	//Only that init or finished phase can be started
	if(m_nPhase != PHASE_INIT && m_nPhase != PHASE_END && m_nPhase != PHASE_DOWNLOADER_END)
	{
		szLog.Format("Task[%02d] can't be started, because of wrong phase. phase=%d", m_dlParam.m_nTaskID, m_nPhase);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		m_criticalSection.Unlock();
		return m_nPhase;
	}
	if(m_pDownloader != NULL)
	{
		ASSERT(m_nPhase == PHASE_DOWNLOADER_END);

		szLog.Format("Task[%02d] has been finished, but downloader not deleted yet. phase=%d", m_dlParam.m_nTaskID, m_nPhase);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		delete m_pDownloader;
		m_pDownloader = NULL;
	}
	//In case of the task has been finished
	m_nPhase = PHASE_BEFORE_WORKER_THREAD;
	//Change the state to transferring
	m_dlState.SetState(TSE_TRANSFERRING);

	m_criticalSection.Unlock();

	//If we can go here, we should send message to GUI to notify it's now in transferring.
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&m_dlState));

	AfxBeginThread(StartDownloadProc, (LPVOID)this);

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
	
	AfxBeginThread(ResumeDownloadProc, (LPVOID)this);
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
		if( (m_dlState.GetOperAccess(DL_OPER_FLAG_PAUSE) & DL_OPER_FLAG_PAUSE) )
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
		if( (m_dlState.GetOperAccess(DL_OPER_FLAG_PAUSE) & DL_OPER_FLAG_PAUSE) )
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
	
	if(IsDownloaderExist())
	{
		ASSERT(m_pDownloader != NULL);
		nResult = m_pDownloader->Destroy();
	}
	else
	{
		//Remove is allowed
		if( (m_dlState.GetOperAccess(DL_OPER_FLAG_REMOVE) & DL_OPER_FLAG_REMOVE) )
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
	if(m_nPhase == PHASE_DOWNLOADER_WORKING || m_nPhase == PHASE_DOWNLOADER_PAUSED
		|| m_nPhase == PHASE_DOWNLOADER_END)
	{
		ASSERT(m_pDownloader != NULL);
		return TRUE;
	}
	return FALSE;
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
	m_criticalSection.Lock();
	
	ASSERT(m_nPhase == PHASE_BEFORE_WORKER_THREAD);
	ASSERT(m_pDownloader == NULL);
	ASSERT(m_pHeaderParser == NULL);
	
	m_nPhase = PHASE_PRE_GET_HEADER;
	//Get the current state
	nResult = CheckStatus();

	m_criticalSection.Unlock();

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
	m_criticalSection.Lock();
	
	ASSERT(m_nPhase == PHASE_IN_GET_HEADER);
	ASSERT(m_pDownloader == NULL);
	ASSERT(m_pHeaderParser != NULL);
	
	m_nPhase = PHASE_POST_GET_HEADER;
	//Get the current state
	nResult = CheckStatus();
	
	m_criticalSection.Unlock();

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: PostGetHeader after CheckStatus - current state = %d", m_dlParam.m_nTaskID, nResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	
	if(nResult != TSE_TRANSFERRING)
	{	
		//Don't care about the content of header
		m_criticalSection.Lock();

		ASSERT(m_pHeaderParser != NULL);
		delete m_pHeaderParser;
		m_pHeaderParser = NULL;

		m_criticalSection.Unlock();

		return nResult;
	}


	//2. Parse Header content
	CString szDetail;
	m_criticalSection.Lock();

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

	m_criticalSection.Unlock();

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: PostGetHeader after parsing header - current state = %d", m_dlParam.m_nTaskID, nResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}

	return nResult;
}
UINT CDownloaderMgr::PreDownload()
{
	UINT nResult = 0;
	CString szLog;

	//1. PreGetHeader
	nResult = PreGetHeader();
	if(nResult != TSE_TRANSFERRING)
	{
		return nResult;
	}

	//2. InGetHeader
	m_criticalSection.Lock();

	ASSERT(m_nPhase == PHASE_PRE_GET_HEADER);
	ASSERT(m_pDownloader == NULL);
	ASSERT(m_pHeaderParser == NULL);

	m_nPhase = PHASE_IN_GET_HEADER;
	m_pHeaderParser = new CGetHeader();

	m_criticalSection.Unlock();

	//Time-costing operation, start to retrieve HTTP header
	m_pHeaderParser->Start(m_dlParam.m_szUrl);

	//3. PostGetHeader
	nResult = PostGetHeader();
	if(nResult != TSE_TRANSFERRING)
	{
		return nResult;
	}

	return nResult;
}
UINT CDownloaderMgr::StartDownload()
{
	UINT nResult = 0;
	CString szLog;

	//Start logging
	szLog.Format("[StartDownload]: Task[%02d]: ThreadID=%d, URL=%s", m_dlParam.m_nTaskID, GetCurrentThreadId(), 
		m_dlParam.m_szUrl);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	//PreDownload
	nResult = PreDownload();
	if(nResult != TSE_TRANSFERRING)
	{
		m_criticalSection.Lock();
		m_nPhase = PHASE_END;
		m_criticalSection.Unlock();

		::SetEvent(m_hStopEvent);
		CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)&m_dlState);

		return nResult;
	}
	
	//Actually start downloading
	ASSERT(m_pDownloader != NULL);
	m_pDownloader->Init(m_dlParam);
	
	CTimeCost timeCost;

	m_criticalSection.Lock();
	m_nPhase = PHASE_DOWNLOADER_WORKING;
	m_criticalSection.Unlock();

	nResult = m_pDownloader->Start();
	timeCost.UpdateCurrClock();

	szLog.Format("[StartDownload]: Task[%02d]: Time Cost (%d)", m_dlParam.m_nTaskID, timeCost.GetTimeCost());
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

UINT CDownloaderMgr::ResumeDownload()
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