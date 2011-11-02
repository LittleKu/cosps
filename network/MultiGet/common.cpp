#include "stdafx.h"
#include "common.h"


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

void CController::Clear()
{
	m_ctritialSection.Lock();

	m_dwStatus = TSE_READY;
	m_bModified = TRUE;

	m_ctritialSection.Unlock();
}