// EasyDownloader.h: interface for the CEasyDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EASYDOWNLOADER_H__FDE4BCE2_0E7E_474E_9B1A_8B515287CAD2__INCLUDED_)
#define AFX_EASYDOWNLOADER_H__FDE4BCE2_0E7E_474E_9B1A_8B515287CAD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <curl/curl.h>
#include "Downloader.h"

class CDownloadInfo
{
public:
	FILE*	lpFileHeader;
	FILE*	lpFileData;
	CHeaderInfo m_headerInfo;
	CController	m_controller;
	CDownloadInfo() : lpFileHeader(0), lpFileData(0)
	{
	}
};

class CEasyDownloader : public CDownloader
{
public:
	CEasyDownloader();
	virtual ~CEasyDownloader();
public:
	virtual int Start();
	virtual void Stop();
	virtual void Pause();
	virtual int Resume();
	virtual BOOL IsResumable();
	virtual void Init(const CDownloadParam& param);
private:	
	size_t ProcessHeader(char *ptr, size_t size, size_t nmemb);
	size_t ProcessData(char *ptr, size_t size, size_t nmemb);
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow);
	static size_t HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

	void PostDownload(DWORD dwResult);
	//Retrieve the temp directory for the download task
	void GetTempFolder(CString& szTempFolder);
	void VerifyTempFolderExist();
protected:
	CDownloadParam m_dlParam;
	CURL* m_curl;
	CDownloadInfo m_dlInfo;
};

#endif // !defined(AFX_EASYDOWNLOADER_H__FDE4BCE2_0E7E_474E_9B1A_8B515287CAD2__INCLUDED_)
