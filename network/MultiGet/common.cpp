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
	
	BOOL bResult = (m_dwStatus == TSE_INVALID);
	
	m_ctritialSection.Unlock();
	return bResult;
}
void CController::Destroy()
{
	m_ctritialSection.Lock();
	
	m_dwStatus = TSE_INVALID;
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