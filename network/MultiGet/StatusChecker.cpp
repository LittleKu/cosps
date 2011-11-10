// StatusChecker.cpp: implementation of the CStatusChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatusChecker::CStatusChecker(UINT nInitStatus) : m_nCurStatus(nInitStatus)
{
}

CStatusChecker::~CStatusChecker()
{

}

UINT CStatusChecker::GetCurrentStatus()
{
	return m_nCurStatus;
}
void CStatusChecker::SetCurrentStatus(UINT nCurStatus)
{
	m_nCurStatus = nCurStatus;
}

BOOL CStatusChecker::IsStartable()
{
	if(m_nCurStatus != TSE_READY && m_nCurStatus != TSE_COMPLETE && m_nCurStatus != TSE_END_WITH_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CStatusChecker::IsResumable()
{
	if(m_nCurStatus != TSE_PAUSED && m_nCurStatus != TSE_STOPPED)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CStatusChecker::IsPausable()
{
	return (m_nCurStatus == TSE_TRANSFERRING);
}
BOOL CStatusChecker::IsStopable()
{
	return (m_nCurStatus == TSE_TRANSFERRING);
}

DWORD CStatusChecker::GetOperState(UINT nCurStatus)
{
	DWORD dwResult = DL_OPER_FLAG_NONE;

	//Start
	if(nCurStatus == TSE_READY || nCurStatus == TSE_COMPLETE || nCurStatus == TSE_END_WITH_ERROR)
	{
		dwResult |= DL_OPER_FLAG_START;
	}

	//Pause
	if(nCurStatus == TSE_TRANSFERRING)
	{
		dwResult |= DL_OPER_FLAG_PAUSE;
	}

	//Remove
	dwResult |= DL_OPER_FLAG_REMOVE;

	//Re-download
	if(nCurStatus == TSE_READY || nCurStatus == TSE_COMPLETE || nCurStatus == TSE_END_WITH_ERROR)
	{
		dwResult |= DL_OPER_FLAG_REDOWNLOAD;
	}

	//Resume
	if(nCurStatus == TSE_PAUSED)
	{
		dwResult |= DL_OPER_FLAG_RESUME;
	}

	return dwResult;
}
