// SegmentDownloader.h: interface for the CSegmentDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_)
#define AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxmt.h"
#include "Downloader.h"
#include "SegmentInfoMap.h"
#include <curl/curl.h>

#define MAX_WORKER_SESSION		8
#define MIN_SEGMENT_SIZE		(1024 * 1024)
#define MAX_RETRY_TIMES			3

typedef enum 
{
	DLE_OK = 0,
	DLE_PAUSE = 1,
	DLE_STOP = 2,
	DLE_OTHER
} DLCode;

typedef enum
{
	VCE_OK = 0,		//check ok, the response header is same to the request, 206 and the range is also same
	VCE_200,		//response header is 200 other than 206
	VCE_INVALID,	//doesn't get a valid response code yet
	VCE_OTHER		//other cases
} ValidationCheckCode;

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

class CSegmentDownloader;

class CCallbackParam
{
public:
	int		nIndex;
	CSegmentDownloader* pDownloader;
	CCallbackParam(int index = -1, CSegmentDownloader* ptr = NULL) : nIndex(index), pDownloader(ptr) {}
};

class CSegmentDownloader : public CDownloader
{
public:
	CSegmentDownloader();
	virtual ~CSegmentDownloader();
private:
	CSegmentInfoEx* GetSegmentInfo(int nIndex);
	void AddSegmentInfo(CSegmentInfoEx* pSegmentInfo);
	void RemoveSegmentInfoArray();
public:
	virtual void Init(const CDownloadParam& param);
	virtual int Start();
	virtual void Stop();
	virtual void Pause();
	virtual int Resume();
	virtual BOOL IsResumable();
private:
	int Download();
	void PostDownload(DWORD dwResult);

	CURL* InitEasyHandle(int nIndex, int nStartPos, int nFinishPos);
	CURL* StartConnection(int nIndex, int nStartPos, int nFinishPos);
	CURL* RestartConnection(int nIndex);
	void StopConnection(int nIndex, int nCleanType);
	void StopAllConnections();

	void GetTempFolder(CString& szTempFolder);
	void VerifyTempFolderExist();
	void StartInitMultiHandle();
	void RestartInitMultiHandle();

	DWORD64 GetTotalDownloadNow();

	DWORD SelectFDSet(fd_set& fdread, fd_set& fdwrite, fd_set& fdexcep, int& maxfd, long curl_timeout);
	int ProcessTransferDone(CURLMsg *msg, int& still_running, DWORD& dwResult);

	int CheckSegmentHeader(int nIndex);
	void CheckSegmentHeaders(int nIndex);
private:	
	size_t ProcessHeader(char *ptr, size_t size, size_t nmemb, int index);
	size_t ProcessData(char *ptr, size_t size, size_t nmemb, int index);
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int index);

	static size_t HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
protected:
	CDownloadParam m_dlParam;

	CURLM*	m_curlm;
	CSegmentInfoArray* m_pSegmentInfoArray;

	CControlInfo m_controlInfo;
	CCriticalSection m_ctritialSection;

	BOOL m_bHeaderChecked;
	int m_nUsed;

	BOOL m_bResumable;
};

#endif // !defined(AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_)
