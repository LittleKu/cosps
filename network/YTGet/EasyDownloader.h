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
#include "ProgressMeter.h"

class CConnectionInfo
{
public:
	UINT	m_nDlNow;
	UINT	m_nRetry;
	UINT	m_nRemotePos;
	FILE*	lpFileHeader;
	FILE*	lpFileData;
	CHeaderInfo m_headerInfo;

	CConnectionInfo() : m_nDlNow(0), m_nRetry(0), m_nRemotePos(0), lpFileHeader(0), lpFileData(0)
	{
	}
};

class CDownloaderContext;

class CEasyDownloader : public CDownloader
{
public:
	CEasyDownloader(CDownloaderContext* pContext);
	virtual ~CEasyDownloader();
public:
	virtual void Init(const CDownloadParam& param);
	virtual int Start();
	virtual LPCTSTR GetName() { return _T("EasyDownloader"); }

private:	
	size_t ProcessHeader(char *ptr, size_t size, size_t nmemb);
	size_t ProcessData(char *ptr, size_t size, size_t nmemb);
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow);
	static size_t HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

	int DoDownload();
	BOOL ProcessTransferDone(CURLcode res, CDownloadState& dlState);
	int PostDownload(CDownloadState& dlState);
	//Retrieve the temp directory for the download task
	void GetTempFolder(CString& szTempFolder);
	void VerifyTempFolderExist();

	void StartConnection();
	BOOL RestartConnection(int nRetryOperType, int nMaxRetryLimit = -1);
	void CloseConnection();
	void InitEasyHandle();
protected:
	CDownloaderContext* m_pContext;
	CDownloadParam m_dlParam;
	CURL* m_curl;
	CConnectionInfo m_connInfo;
	
	CProgressMeter m_progressMeter;
};

#endif // !defined(AFX_EASYDOWNLOADER_H__FDE4BCE2_0E7E_474E_9B1A_8B515287CAD2__INCLUDED_)
