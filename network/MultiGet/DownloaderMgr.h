// DownloaderMgr.h: interface for the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_)
#define AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Downloader.h"

class CDownloaderMgr : public CDownloader
{
public:
	CDownloaderMgr();
	virtual ~CDownloaderMgr();
	virtual void Init(const CDownloadParam& param);
	virtual UINT GetCurrentStatus();
	virtual int Start();
	virtual int Stop();
	virtual int Pause();
	virtual int Resume();
	virtual int Destroy();
	virtual BOOL IsResumable();
protected:
	virtual void CurrentStatusChanged(UINT nNewStatus, LPCTSTR lpszDetail = NULL);
private:
	static UINT StartDownloadProc(LPVOID lpvData);
	static UINT ResumeDownloadProc(LPVOID lpvData);

	UINT StartDownload();
	UINT ResumeDownload();
protected:
	CDownloader* m_pDownloader;
};

#endif // !defined(AFX_DOWNLOADERMGR_H__4B2D1E2E_DEC4_4320_9E07_2E0E79B7BC1B__INCLUDED_)
