// DownloaderContext.cpp: implementation of the CDownloaderContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderContext.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


DECLARE_THE_LOGGER_NAME("CTXT")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloaderContext::CDownloaderContext() : m_pCurDownloader(NULL), m_nMaxCount(-1)
{
	m_nMaxCount = SYS_OPTIONS()->m_nMaxConnectionCount;
}

CDownloaderContext::~CDownloaderContext()
{

}
void CDownloaderContext::Lock()
{
	m_lock.Lock();
}
void CDownloaderContext::Unlock()
{
	m_lock.Unlock();
}
void CDownloaderContext::SetState(DWORD nState, LPCTSTR lpszDetail)
{
	m_lock.Lock();
	NoLockSetState(nState, lpszDetail);
	m_lock.Unlock();
}
void CDownloaderContext::GetState(CDownloadState& dlState)
{
	m_lock.Lock();
	NoLockGetState(dlState);
	m_lock.Unlock();
}
DWORD CDownloaderContext::GetState()
{
	DWORD dwResult;

	m_lock.Lock();
	dwResult = NoLockGetState();
	m_lock.Unlock();

	return dwResult;
}

void CDownloaderContext::NoLockSetState(DWORD nState, LPCTSTR lpszDetail)
{
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		if(m_dlCurState.GetState() != nState)
		{
			CString szLog, szOldState, szNewState;
			szOldState = CCommonUtils::State2Str(m_dlCurState.GetState());
			szNewState = CCommonUtils::State2Str(nState);
			szLog.Format("Task[%02d]: StateChanged from [%s] to [%s]", m_dlParam.m_nTaskID, 
				(LPCTSTR)szOldState, (LPCTSTR)szNewState);

			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}
	}
	if(m_dlCurState.GetState() == TSE_DESTROYING && nState != TSE_DESTROYED)
	{
		CString szLog, szOldState, szNewState;
		szOldState = CCommonUtils::State2Str(TSE_DESTROYING);
		szNewState = CCommonUtils::State2Str(nState);

		nState = TSE_DESTROYED;

		szLog.Format("Task[%02d]: [%s]->[%s] changed to [%s]-[%s]", m_dlParam.m_nTaskID, 
			(LPCTSTR)szOldState, (LPCTSTR)szNewState, (LPCTSTR)szOldState, 
			(LPCTSTR)(CCommonUtils::State2Str(TSE_DESTROYED)));
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
	}
	m_dlCurState.SetState(nState, lpszDetail);
}
void CDownloaderContext::NoLockGetState(CDownloadState& dlState)
{
	dlState = m_dlCurState;
}
DWORD CDownloaderContext::NoLockGetState()
{
	return m_dlCurState.GetState();
}
BOOL CDownloaderContext::NoLockIsOperEnabled(DWORD nOperFlags)
{
	return (BOOL)(m_dlCurState.GetAccess(nOperFlags) & nOperFlags);
}
