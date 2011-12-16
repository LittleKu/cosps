// ParserDownloader.h: interface for the CParserDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSERDOWNLOADER_H__022741A3_395B_4CC8_B266_AACEA4A9FEEC__INCLUDED_)
#define AFX_PARSERDOWNLOADER_H__022741A3_395B_4CC8_B266_AACEA4A9FEEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <curl/curl.h>
#include "Downloader.h"
#include "ProgressMeter.h"

class CDownloaderContext;
class CDownloaderCreator;

class CParserDownloader : public CDownloader
{
public:
	CParserDownloader(CDownloaderContext* pContext);
	virtual ~CParserDownloader();
public:
	virtual void Init(const CDownloadParam& param);
	virtual int Start();
	virtual CDownloader* GetNext() { return m_pNext; }
	virtual LPCTSTR GetName() { return _T("ParserDownloader"); }
	
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
	void VerifyTempFolderExist();
	
	void StartConnection();
	BOOL RestartConnection(int nRetryOperType, int nMaxRetryLimit = -1);
	void CloseConnection();
	void InitEasyHandle();
protected:
	class CConnectionInfo
	{
	public:
		UINT	m_nDlNow;
		UINT	m_nRetry;
		UINT	m_nRemotePos;
		FILE*	lpFileHeader;
		FILE*	lpFileData;
		CString m_szMemData;
		CHeaderInfo m_headerInfo;
		
		CConnectionInfo() : m_nDlNow(0), m_nRetry(0), m_nRemotePos(0), lpFileHeader(0), lpFileData(0), m_szMemData("")
		{
		}
	};

	virtual void GenerateNextDownloader() 
	{
		ASSERT(m_pNext == NULL);
		m_pNext = NULL;
	}

protected:	
	CDownloaderContext* m_pContext;
	CDownloadParam m_dlParam;
	CURL* m_curl;
	CConnectionInfo m_connInfo;

	CDownloaderCreator* m_pDownloaderCreator;

	CDownloader* m_pNext;
	
	CProgressMeter m_progressMeter;
	BOOL m_bPrgsUpdate;
};

#endif // !defined(AFX_PARSERDOWNLOADER_H__022741A3_395B_4CC8_B266_AACEA4A9FEEC__INCLUDED_)
