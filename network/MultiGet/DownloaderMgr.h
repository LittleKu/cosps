// DownloaderMgr.h: interface for the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_)
#define AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadMonitor.h"
#include "DownloaderContext.h"

class CDownloaderMgr : public CPostAction, public CDownloaderContext
{
public:
	CDownloaderMgr();
	virtual ~CDownloaderMgr();
public:
	virtual void Init(const CDownloadParam& param);
	virtual int Start();
	virtual int Pause();
	virtual int ReStart();
	virtual int Destroy();

	virtual int DoAction();
private:
	static DWORD WINAPI StartDownloadProc(LPVOID lpParameter);
	static DWORD WINAPI ReDownloadProc(LPVOID lpParameter);

	DWORD StartDownload();
	DWORD ReDownload();
protected:
	CDownloadParam m_dlParam;
	HANDLE m_hWorkerThread;
};

#endif // !defined(AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_)
