// DownloaderContext.cpp: implementation of the CDownloaderContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloaderContext::CDownloaderContext() : m_pCurDownloader(NULL)
{

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
	m_dlCurState.SetState(nState, lpszDetail);
	m_lock.Unlock();
}
void CDownloaderContext::GetState(CDownloadState& dlState)
{
	m_lock.Lock();
	dlState = m_dlCurState;
	m_lock.Unlock();
}
DWORD CDownloaderContext::GetState()
{
	CDownloadState dlState;
	GetState(dlState);

	return dlState.GetState();
}

void CDownloaderContext::SetStateNoLock(DWORD nState, LPCTSTR lpszDetail)
{
	m_dlCurState.SetState(nState, lpszDetail);
}
void CDownloaderContext::GetStateNoLock(CDownloadState& dlState)
{
	dlState = m_dlCurState;
}
DWORD CDownloaderContext::GetStateNoLock()
{
	return m_dlCurState.GetState();
}
