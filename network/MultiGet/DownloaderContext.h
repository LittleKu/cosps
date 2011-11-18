// DownloaderContext.h: interface for the CDownloaderContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADERCONTEXT_H__50F92354_27D0_48EB_AAAD_6BCA00B02364__INCLUDED_)
#define AFX_DOWNLOADERCONTEXT_H__50F92354_27D0_48EB_AAAD_6BCA00B02364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDownloader;
class CHeaderDownloader;
class CSegmentDownloader;

class CDownloaderContext  
{
public:
	CDownloaderContext();
	virtual ~CDownloaderContext();
public:
	virtual void SetState(DWORD nState, LPCTSTR lpszDetail = NULL);
	virtual void GetState(CDownloadState& dlState);
	virtual DWORD GetState();
protected:
	void Lock();
	void Unlock();
	
	BOOL NoLockIsOperEnabled(DWORD nOperFlags);
	void NoLockSetState(DWORD nState, LPCTSTR lpszDetail = NULL);
	void NoLockGetState(CDownloadState& dlState);
	DWORD NoLockGetState();
protected:
	friend class CDownloader;
	friend class CHeaderDownloader;
	friend class CSegmentDownloader;

	CDownloadParam	m_dlParam;
	CDownloadState	m_dlCurState;
	CDownloader*	m_pCurDownloader;

	CCriticalSection m_lock;
};

#endif // !defined(AFX_DOWNLOADERCONTEXT_H__50F92354_27D0_48EB_AAAD_6BCA00B02364__INCLUDED_)
