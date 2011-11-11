// DownloaderMgr.h: interface for the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_)
#define AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Downloader.h"
#include "GetHeader.h"

class CDownloaderMgr;
typedef CArray<CDownloaderMgr*, CDownloaderMgr*> CDownloaderMgrArray;

class CDownloaderMgr
{
public:
	CDownloaderMgr();
	virtual ~CDownloaderMgr();
	virtual void Init(const CDownloadParam& param);
	virtual DWORD GetState();
	virtual int Start();
	virtual int Pause();
	virtual int Stop();
	virtual int Resume();
	virtual int Destroy();
	virtual BOOL IsResumable();

	DWORD GetAccess();

	static UINT DeleteProc(LPVOID lpvData);
	static int Delete(CDownloaderMgrArray* pDownloaderArray);
protected:
	virtual void WaitUntilStop();
private:
	static UINT StartDownloadProc(LPVOID lpvData);
	static UINT ResumeDownloadProc(LPVOID lpvData);

	UINT StartDownload();
	UINT ResumeDownload();

	UINT PreGetHeader();
	UINT PostGetHeader();
	UINT PreDownload();
	UINT CheckStatus();

	BOOL IsDownloaderExist();
	BOOL IsRunning();
protected:
	CDownloadParam m_dlParam;

	int m_nPhase;
	CDownloadState m_dlState;

	CCriticalSection m_criticalSection;

	HANDLE m_hStopEvent;

	CDownloader* m_pDownloader;
	CGetHeader* m_pHeaderParser;
};

#endif // !defined(AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_)
