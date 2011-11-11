#include "stdafx.h"
#include "common.h"

/*
BOOL CDownloadParam::IsMsgEnable(UINT nMsg)
{
	if(m_nMsgFilter == MSG_FILTER_ALL)
	{
		return TRUE;
	}
	if(m_nMsgFilter == MSG_FILTER_NONE)
	{
		return FALSE;
	}

	ASSERT(nMsg >= WM_DOWNLOAD_PROGRESS && nMsg <= WM_DOWNLOAD_STATUS);

	UINT nFilter = (UINT)((UINT)1 << (nMsg - WM_DOWNLOAD_PROGRESS));

	return (m_nMsgFilter & nFilter);
}
*/
CController::CController() : m_bModified(FALSE), m_dwStatus(TSE_READY)
{
}

CController::~CController()
{
}

BOOL CController::IsModified()
{
	return m_bModified;
}

void CController::SetModified(BOOL bModified)
{
	m_ctritialSection.Lock();
	
	m_bModified = bModified;
	
	m_ctritialSection.Unlock();
}

BOOL CController::IsPaused()
{
	m_ctritialSection.Lock();

	BOOL bResult = (m_dwStatus == TSE_PAUSED);

	m_ctritialSection.Unlock();
	return bResult;
}
void CController::Pause()
{
	m_ctritialSection.Lock();
	
	m_dwStatus = TSE_PAUSED;
	m_bModified = TRUE;
	
	m_ctritialSection.Unlock();
}

BOOL CController::IsStopped()
{
	m_ctritialSection.Lock();
	
	BOOL bResult = (m_dwStatus == TSE_STOPPED);
	
	m_ctritialSection.Unlock();
	return bResult;
}
void CController::Stop()
{
	m_ctritialSection.Lock();
	
	m_dwStatus = TSE_STOPPED;
	m_bModified = TRUE;
	
	m_ctritialSection.Unlock();
}

BOOL CController::IsDestroyed()
{
	m_ctritialSection.Lock();
	
	BOOL bResult = (m_dwStatus == TSE_DESTROYED);
	
	m_ctritialSection.Unlock();
	return bResult;
}
void CController::Destroy()
{
	m_ctritialSection.Lock();
	
	m_dwStatus = TSE_DESTROYED;
	m_bModified = TRUE;
	
	m_ctritialSection.Unlock();
}

void CController::Clear()
{
	m_ctritialSection.Lock();

	m_dwStatus = TSE_READY;
	m_bModified = TRUE;

	m_ctritialSection.Unlock();
}


void  CDownloadState::SetState(DWORD nState, LPCTSTR lpszDetail)
{
	m_nState = nState;
	m_szDetail = lpszDetail;
}
DWORD CDownloadState::GetState()
{
	return m_nState;
}
DWORD CDownloadState::GetOperAccess(DWORD nOperFlags)
{
	return GetAccess(m_nState, nOperFlags);
}
DWORD CDownloadState::GetAccess(DWORD nState, DWORD nOperFlags)
{
	ASSERT(nState >= TSE_READY && nState <= TSE_DESTROYING);

	DWORD dwResult = DL_OPER_FLAG_NONE;
	
	//Start
	if(nState == TSE_READY || nState == TSE_COMPLETE || nState == TSE_END_WITH_ERROR)
	{
		dwResult |= DL_OPER_FLAG_START;
	}
	
	//Pause
	if(nState == TSE_TRANSFERRING)
	{
		dwResult |= DL_OPER_FLAG_PAUSE;
	}
	
	//Remove
	if(nState != TSE_DESTROYED && nState != TSE_DESTROYING)
	{
		dwResult |= DL_OPER_FLAG_REMOVE;
	}
	
	//Re-download
	if(nState == TSE_READY || nState == TSE_COMPLETE || nState == TSE_END_WITH_ERROR)
	{
		dwResult |= DL_OPER_FLAG_REDOWNLOAD;
	}
	
	//Resume
	if(nState == TSE_PAUSED)
	{
		dwResult |= DL_OPER_FLAG_RESUME;
	}
	
	return dwResult;
}