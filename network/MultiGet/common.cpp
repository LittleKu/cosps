#include "stdafx.h"
#include "common.h"

void  CDownloadState::SetState(DWORD nState, LPCTSTR lpszDetail)
{
	m_nState = nState;
	m_szDetail = lpszDetail;
}
DWORD CDownloadState::GetState()
{
	return m_nState;
}
DWORD CDownloadState::GetAccess(DWORD nOperFlags)
{
	return GetAccessPermission(m_nState, nOperFlags);
}
DWORD CDownloadState::GetAccessPermission(DWORD nState, DWORD nOperFlags)
{
	ASSERT(nState >= TSE_READY && nState <= TSE_DESTROYING);

	DWORD dwResult = DL_OPER_FLAG_NONE;
	
	//Start
	if(nState == TSE_READY || nState == TSE_END_WITH_ERROR || nState == TSE_PAUSED)
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
	if(nState == TSE_COMPLETE || nState == TSE_END_WITH_ERROR || nState == TSE_PAUSED)
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