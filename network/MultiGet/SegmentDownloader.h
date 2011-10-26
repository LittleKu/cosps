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
#include <curl/curl.h>
#include "SegmentInfoMap.h"

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

typedef CSize CRange;

class CSegmentInfoEx : public CSegmentInfo
{
public:
	CURL*	m_curl;
	FILE*	m_lpFileHeader;
	FILE*	m_lpFileData;
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
public:
	virtual void Start();
	virtual void Stop();
	virtual void Pause();
	virtual void Resume();
	virtual void Init(const CDownloadParam& param);

	BOOL FormatErrorMsg(CResultCode result_code, CString& szErrorMsg);
	void FormatInternalErrorMsg(int nCode, CString& szErrorMsg);
private:
	void Download();

	void SplitFileRange(UINT nFileSize, CArray<CRange, CRange&>& sizeArray);
	CURL* InitEasyHandle(int nStartPos, int nFinishPos, int nIndex);
	DWORD64 GetTotalDownloadNow();
	void StopConnection(int nIndex, int nCleanType);

	CSegmentInfoEx* GetSegmentInfo(int nIndex);
	void AddSegmentInfo(CSegmentInfoEx* pSegmentInfo);
	void RemoveSegmentInfoArray();
private:	
	size_t ProcessHeader(char *ptr, size_t size, size_t nmemb, int index);
	size_t ProcessData(char *ptr, size_t size, size_t nmemb, int index);
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int index);

	static size_t HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static UINT DownloadProc(LPVOID lpvData);
protected:
	CDownloadParam m_dlParam;

	CURLM*	m_curlm;
	CSegmentInfoArray* m_pSegmentInfoArray;

	CControlInfo m_controlInfo;
	CCriticalSection m_ctritialSection;
};

#endif // !defined(AFX_SEGMENTDOWNLOADER_H__08611C16_1255_458C_BA90_0293742718F4__INCLUDED_)
