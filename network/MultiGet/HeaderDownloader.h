// HeaderDownloader.h: interface for the CHeaderDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADERDOWNLOADER_H__8B4E3777_EB77_4AE7_8054_CFE6DEEE6FA8__INCLUDED_)
#define AFX_HEADERDOWNLOADER_H__8B4E3777_EB77_4AE7_8054_CFE6DEEE6FA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Downloader.h"
#include <curl/curl.h>
#include "DownloaderContext.h"

class CHeaderDownloader : public CDownloader  
{
public:
	CHeaderDownloader(CDownloaderContext* pContext = NULL);
	virtual ~CHeaderDownloader();
public:
	virtual void Init(const CDownloadParam& param);
	virtual int Start();
// 	virtual int ReDownload();
// 	virtual int Pause();
// 	virtual int Destroy();

	virtual CDownloader* GetNext();

	virtual LPCTSTR GetName() { return _T("HeaderDownloader"); }
private:
	int DoProcess();
	int PostProcess(CURLcode res);
	void GenerateNextDownloader();
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow);
	size_t ProcessHeader(void *ptr, size_t size, size_t nmemb);

	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static size_t HeaderCallback(void *ptr, size_t size, size_t nmemb, void *userdata);
private:
	CDownloaderContext* m_pContext;
	CDownloadParam m_dlParam;

	CDownloader* m_pNext;

	CURL* m_curl;
	CHeaderInfo m_headerInfo;
};

#endif // !defined(AFX_HEADERDOWNLOADER_H__8B4E3777_EB77_4AE7_8054_CFE6DEEE6FA8__INCLUDED_)
