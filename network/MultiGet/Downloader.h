// Downloader.h: interface for the CDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADER_H__7AFFE40E_1B65_460E_B5BA_3F1B0265C530__INCLUDED_)
#define AFX_DOWNLOADER_H__7AFFE40E_1B65_460E_B5BA_3F1B0265C530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusChecker.h"

class CDownloader;
typedef CArray<CDownloader*, CDownloader*> CDownloaderArray;

class CDownloader  
{
public:
	
	CDownloader();
	virtual ~CDownloader();
public:
	virtual void Init(const CDownloadParam& param);
	virtual UINT GetCurrentStatus();
	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual int Pause() = 0;
	virtual int Resume() = 0;
	virtual int Destroy() = 0;
	virtual BOOL IsResumable() = 0;

	static UINT DeleteProc(LPVOID lpvData);
	static int Delete(CDownloaderArray* pDownloaderArray);
protected:
	virtual void CurrentStatusChanged(UINT nNewStatus, LPCTSTR lpszDetail = NULL);
	virtual void TaskFinished(DWORD dwResult);
	virtual void WaitUtilStop();
protected:
	CDownloadParam m_dlParam;
	CStatusChecker m_statusChecker;

	HANDLE m_hStopEvent;
};

#endif // !defined(AFX_DOWNLOADER_H__7AFFE40E_1B65_460E_B5BA_3F1B0265C530__INCLUDED_)
