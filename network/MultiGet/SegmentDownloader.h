// SegmentDownloader.h: interface for the CSegmentDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_)
#define AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Downloader.h"
#include "SegmentInfoMap.h"
#include <curl/curl.h>
#include "TimeCost.h"

class CSegmentInfoEx : public CSegmentInfo
{
public:
	CURL*	m_curl;
	FILE*	m_lpFileHeader;
	FILE*	m_lpFileData;
	CHeaderInfo		m_headerInfo;

	CSegmentInfoEx() : CSegmentInfo()
	{
		m_curl = NULL;
		m_lpFileHeader = NULL;
		m_lpFileData = NULL;
	}
};

/************************************************************************/
/* Segment download in 1 thread											*/
/************************************************************************/
class CDownloaderContext;
class CSegmentDownloader : public CDownloader
{
public:
	CSegmentDownloader(CDownloaderContext* pContext);
	virtual ~CSegmentDownloader();
public:
	virtual void Init(const CDownloadParam& param);
	virtual int Start();

	virtual LPCTSTR GetName() { return _T("SegmentDownloader"); }
private:
	//Actual download process
	int DoDownload();

	//Post process after download finished
	int PostDownload(DWORD dwResult);


	//Init an easy handle connection
	CURL* InitEasyHandle(int nIndex, int nStartPos, int nFinishPos);
	//Create a new easy handle connection
	CURL* StartConnection(int nIndex, int nStartPos, int nFinishPos);
	//Re-create a easy handle connection that continue previous connections' work
	CURL* RestartConnection(int nIndex, int nRetryOperType);
	//Create a new multi handle to process all the segment downloading
	void StartInitMultiHandle();
	//Re-create a multi handle to resume the download process that has been stopped before
	void RestartInitMultiHandle();


	//download sub process, a wrapper for function select()
	DWORD SelectFDSet(fd_set& fdread, fd_set& fdwrite, fd_set& fdexcep, int& maxfd, long curl_timeout);
	//process when a easy handle connection finished its transfer
	int ProcessTransferDone(CURLMsg *msg, int& still_running, DWORD& dwResult);

	//Close a connection
	void CloseConnection(int nIndex);
	//Close all the connections
	void CloseAllConnections();
private:
	//Helper functions

	//Calculate the total download size now
	DWORD64 GetTotalDownloadNow();

	CSegmentInfoEx* GetSegmentInfo(int nIndex);
	void AddSegmentInfo(CSegmentInfoEx* pSegmentInfo);
	void RemoveSegmentInfoArray();
	
	//Retrieve the temp directory for the download task
	void GetTempFolder(CString& szTempFolder);
	void VerifyTempFolderExist();
private:
	//CURL related callback functions
	size_t ProcessHeader(char *ptr, size_t size, size_t nmemb, int index);
	size_t ProcessData(char *ptr, size_t size, size_t nmemb, int index);
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int index);

	static size_t HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
protected:
	CDownloaderContext* m_pContext;
	CDownloadParam m_dlParam;
	CURLM*	m_curlm;
	CSegmentInfoArray* m_pSegmentInfoArray;

	CTimeCost m_progTimer;
};

#endif // !defined(AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_)
