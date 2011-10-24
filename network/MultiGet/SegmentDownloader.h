// SegmentDownloader.h: interface for the CSegmentDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_)
#define AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Downloader.h"
#include <curl/curl.h>

#define MAX_WORKER_SESSION		8
#define MIN_SEGMENT_SIZE		(1024 * 1024)
#define MAX_RETRY_TIMES			3

typedef CSize CRange;

class CSegmentInfo
{
public:
	int		m_nIndex;
	CURL*	m_curl;
	FILE*	m_lpFileHeader;
	FILE*	m_lpFileData;
	DWORD64	m_nDlBefore;
	DWORD64	m_nDlNow;
	CSize	m_range;
	int		m_nRetry;
	CSegmentInfo()
	{
		m_nIndex = -1;
		m_curl = NULL;
		m_lpFileHeader = NULL;
		m_lpFileData = NULL;
		m_nDlBefore = 0;
		m_nDlNow = 0;
		m_range.cx = 0;
		m_range.cy = 0;
		m_nRetry = 0;
	}
};

class CSegmentDownloader;

class CCallbackParam
{
public:
	int		nIndex;
	CSegmentDownloader* pThis;
	CCallbackParam(int index = -1, CSegmentDownloader* ptr = NULL) : nIndex(index), pThis(ptr) {}
};

class CSegmentDownloader : public CDownloader
{
public:
	CSegmentDownloader();
	virtual ~CSegmentDownloader();
public:
	virtual void Start();
	virtual void Stop();
	virtual void Pause();
	virtual void Resume();
	virtual void Init(LPCTSTR lpszUrl, CDownloadParam param);
private:
	void SplitFileRange(UINT nFileSize, CArray<CRange, CRange&>& sizeArray);
	void InitMultiCurl();
	CURL* InitEasyHandle(int nStartPos, int nFinishPos, int nIndex); 
	DWORD64 GetTotalDownloadNow();
private:	
	size_t ProcessHeader(char *ptr, size_t size, size_t nmemb, int index);
	size_t ProcessData(char *ptr, size_t size, size_t nmemb, int index);
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int index);

	static size_t HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
protected:
	CString m_szUrl;
	CDownloadParam m_dlParam;
	CURLM*	m_curlm;

	CArray<CSegmentInfo, CSegmentInfo&> m_segInfos;
};

#endif // !defined(AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_)
