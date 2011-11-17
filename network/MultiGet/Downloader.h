// Downloader.h: interface for the CDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADER_H__7AFFE40E_1B65_460E_B5BA_3F1B0265C530__INCLUDED_)
#define AFX_DOWNLOADER_H__7AFFE40E_1B65_460E_B5BA_3F1B0265C530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDownloader  
{
public:
	CDownloader();
	virtual ~CDownloader();
public:
	virtual void Init(const CDownloadParam& param) = 0;
	virtual int Start() = 0;
	virtual int ReDownload() = 0;
	virtual int Pause() = 0;
	virtual int Destroy() = 0;

	virtual CDownloader* GetNext();

	//For debug use
	virtual LPCTSTR GetName();
};

#endif // !defined(AFX_DOWNLOADER_H__7AFFE40E_1B65_460E_B5BA_3F1B0265C530__INCLUDED_)
