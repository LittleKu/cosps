// SegmentDownloader.cpp: implementation of the CSegmentDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SegmentDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

size_t CSegmentDownloader::HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	CCallbackParam* lpParam = (CCallbackParam*)userdata;
	return lpParam->pThis->ProcessHeader(ptr, size, nmemb, lpParam->nIndex);
}

size_t CSegmentDownloader::DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	CCallbackParam* lpParam = (CCallbackParam*)userdata;
	return lpParam->pThis->ProcessData(ptr, size, nmemb, lpParam->nIndex);
}

int CSegmentDownloader::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CCallbackParam* lpParam = (CCallbackParam*)clientp;
	return lpParam->pThis->ProcessProgress(dltotal, dlnow, ultotal, ulnow, lpParam->nIndex);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSegmentDownloader::CSegmentDownloader()
{
}

CSegmentDownloader::~CSegmentDownloader()
{
	m_segInfos.RemoveAll();
}

void CSegmentDownloader::Init(LPCTSTR lpszUrl, CDownloadParam param)
{
	m_segInfos.RemoveAll();

	m_szUrl = lpszUrl;
	m_dlParam = param;

	m_curlm = curl_multi_init();
	
	curl_multi_setopt(m_curlm, CURLMOPT_MAXCONNECTS, (long)MAX_WORKER_SESSION);

	InitMultiCurl();
}

CURL* CSegmentDownloader::InitEasyHandle(int nStartPos, int nFinishPos, int nIndex)
{
	CURL* easy_handle = curl_easy_init();

	//URL address
	curl_easy_setopt(easy_handle, CURLOPT_URL, (LPCTSTR)m_szUrl);

	//agent: IE8
	curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, USER_AGENT_IE8);
	//redirect
	curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
	//no verbose
	curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
	//connect timeout: 10s
	curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 10L);

	//segment download, range setting
	CString szRange;
	szRange.Format("%d-%d", nStartPos, nFinishPos);
	curl_easy_setopt(easy_handle, CURLOPT_RANGE, (LPCTSTR)szRange);
	
	CCallbackParam* pCallbackParam = new CCallbackParam(nIndex, this);
	//header
	curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, CSegmentDownloader::HeaderCallback);
	curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, pCallbackParam);
	
	//data
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, CSegmentDownloader::DataCallback);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, pCallbackParam);
	
	//progress
	curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, CSegmentDownloader::ProgressCallback);
	curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, pCallbackParam);
	
	//private
	curl_easy_setopt(easy_handle, CURLOPT_PRIVATE, pCallbackParam);
	
	//Init parameters
	if(nIndex < m_segInfos.GetSize())
	{

	}
	else
	{

	}
	CSegmentInfo segmentInfo;
	segmentInfo.m_nIndex = i;
	segmentInfo.m_curl = easy_handle;
	
	char buf[32];
	sprintf(buf, "header_%d.txt", i);
	segmentInfo.m_lpFileHeader = fopen(buf, "wb");
	
	sprintf(buf, "data_%d", i);
	segmentInfo.m_lpFileData = fopen(buf, "wb");
	
	m_segInfos.Add(segmentInfo);

	return;
}
void CSegmentDownloader::InitMultiCurl()
{
	CArray<CRange, CRange&> sizeArray;
	SplitFileRange(m_dlParam.m_nFileSize, sizeArray);
	
	int i, nSize;
	CString szRange;

	CCallbackParam* pCallbackParam;
	
	for(i = 0, nSize = sizeArray.GetSize(); i < nSize; i++)
	{
		CRange& segment = sizeArray.GetAt(i);
		szRange.Format("%d-%d", segment.cx, segment.cy);

		CURL* easy_handle = curl_easy_init();
		
		//URL address
		curl_easy_setopt(easy_handle, CURLOPT_URL, (LPCTSTR)m_szUrl);
		
		
		//agent: IE8
		curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, USER_AGENT_IE8);
		//redirect
		curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
		//no verbose
		curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
		//connect timeout: 10s
		curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 10L);
		//segment download, range setting
		curl_easy_setopt(easy_handle, CURLOPT_RANGE, (LPCTSTR)szRange);

		pCallbackParam = new CCallbackParam(i, this);
		//header
		curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, CSegmentDownloader::HeaderCallback);
		curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, pCallbackParam);
		
		//data
		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, CSegmentDownloader::DataCallback);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, pCallbackParam);
		
		//progress
		curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, CSegmentDownloader::ProgressCallback);
		curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, pCallbackParam);

		//private
		curl_easy_setopt(easy_handle, CURLOPT_PRIVATE, pCallbackParam);

		//Init parameters
		CSegmentInfo segmentInfo;
		segmentInfo.m_nIndex = i;
		segmentInfo.m_curl = easy_handle;

		char buf[32];
		sprintf(buf, "header_%d.txt", i);
		segmentInfo.m_lpFileHeader = fopen(buf, "wb");

		sprintf(buf, "data_%d", i);
		segmentInfo.m_lpFileData = fopen(buf, "wb");
		
		m_segInfos.Add(segmentInfo);

		curl_multi_add_handle(m_curlm, easy_handle); 
	}
}

void CSegmentDownloader::Start()
{
	int still_running = -1; /* keep number of running handles */
	fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
	int maxfd = -1;
	long curl_timeout = -1;
	struct timeval timeout;

	CURLMsg *msg; /* for picking up messages with the transfer status */
	int msgs_left; /* how many messages are left */

	CString szLog;

	while (still_running) 
	{
		curl_multi_perform(m_curlm, &still_running);
		
		if (still_running) 
		{
			FD_ZERO(&fdread);
			FD_ZERO(&fdwrite);
			FD_ZERO(&fdexcep);
			
			if (curl_multi_fdset(m_curlm, &fdread, &fdwrite, &fdexcep, &maxfd)) 
			{
				szLog.Format("curl_multi_fdset error");
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCSTR)szLog)
				break;
			}
			
			if (curl_multi_timeout(m_curlm, &curl_timeout)) 
			{
				szLog.Format("curl_multi_timeout error");
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCSTR)szLog)				
				break;
			}
			if (curl_timeout == -1)
			{
				curl_timeout = 100;
			}			
			if (maxfd == -1) 
			{
				Sleep(curl_timeout);
			} 
			else 
			{
				timeout.tv_sec = curl_timeout/1000;
				timeout.tv_usec = (curl_timeout%1000)*1000;
				
				if (0 > select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout)) 
				{
					szLog.Format("select(%i, , , ,%li): error", maxfd + 1, timeout);
					LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCSTR)szLog)	
// 					fprintf(stderr, "E: select(%i,,,,%li): %i: %s\n",
// 						M+1, L, errno, strerror(errno));
					break;
				}
			}
		}
		
		/* See how the transfers went */
		while( (msg = curl_multi_info_read(m_curlm, &msgs_left)) )
		{
			if (msg->msg == CURLMSG_DONE) 
			{
				/*
				HeaderData *pHeader;
				CURL *e = msg->easy_handle;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &pHeader);
				fprintf(stderr, "R: %d - %s <%s>\n",
					msg->data.result, curl_easy_strerror(msg->data.result), pHeader->url);
				AfxTrace("R: %d - %s <%s>\n",
					msg->data.result, curl_easy_strerror(msg->data.result), pHeader->url);
				fclose(pHeader->fp);

				CProgressInfo progressInfo;
				progressInfo.dltotal = pHeader->pBody->offset;
				progressInfo.dlnow = pHeader->pBody->offset;
				progressInfo.retCode = msg->data.result;
				progressInfo.szReason = curl_easy_strerror(msg->data.result);
				
				::SendMessage(pHeader->pBody->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pHeader->pBody->index);

				curl_multi_remove_handle(cm, e);
				curl_easy_cleanup(e);
				*/

				CURL* easy_handle = msg->easy_handle;

				CCallbackParam* pCallbackParam = NULL;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &pCallbackParam);

				CURLcode resCode = msg->data.result;
				//Transfer error, re-try
				if(resCode != CURLE_OK)
				{
					szLog.Format("(%d) - Transfer Error: %d - %s", pCallbackParam->nIndex, resCode, curl_easy_strerror(resCode));
					LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCSTR)szLog)
				}		

				CSegmentInfo& segmentInfo = m_segInfos.GetAt(pCallbackParam->nIndex);
				fclose(segmentInfo.m_lpFileHeader);
				fclose(segmentInfo.m_lpFileData);

				if(pCallbackParam != NULL)
				{
					delete pCallbackParam;
					pCallbackParam = NULL;
				}

				curl_multi_remove_handle(m_curlm, easy_handle);
				curl_easy_cleanup(easy_handle);
			}
			else 
			{
				szLog.Format("R: msg->msg != CURLMSG_DONE");
				LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCSTR)szLog)
				/*
				HeaderData *pHeader;
				CURL *e = msg->easy_handle;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &pHeader);

				CString szReason;
				szReason.Format("E: CURLMsg (%d)", msg->msg);

				AfxTrace("%s\n", szReason);

				CProgressInfo progressInfo;
				progressInfo.retCode = 2999;
				progressInfo.szReason = szReason;
				
				::SendMessage(pHeader->pBody->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pHeader->pBody->index);
				*/
			}

			/*
			if (C < CNT) 
			{
				init(cm, C++, hwnd);
				U++; //just to prevent it from remaining at 0 if there are more URLs to get
			}
			*/
		}
	}
	
	curl_multi_cleanup(m_curlm);

	m_segInfos.RemoveAll();
}

void CSegmentDownloader::Stop()
{
}

void CSegmentDownloader::Pause()
{	
}

void CSegmentDownloader::Resume()
{
}

size_t CSegmentDownloader::ProcessHeader(char *ptr, size_t size, size_t nmemb, int index)
{
	if( IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine(ptr, size * nmemb);
		szLine.Replace(_T("\r"), _T("[0x0D]"));
		szLine.Replace(_T("\n"), _T("[0x0A]"));

		CString szMsg;
		szMsg.Format("%d - %s", index, szLine);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szMsg)
	}
	
	CSegmentInfo& segmentInfo = m_segInfos.GetAt(index);
	fwrite(ptr, size, nmemb, segmentInfo.m_lpFileHeader);
	
	return (size_t)(size * nmemb);

	return size * nmemb;
}

size_t CSegmentDownloader::ProcessData(char *ptr, size_t size, size_t nmemb, int index)
{
	CSegmentInfo& segmentInfo = m_segInfos.GetAt(index);
	fwrite(ptr, size, nmemb, segmentInfo.m_lpFileData);

	return size * nmemb;
}

int CSegmentDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int index)
{
	CSegmentInfo& segmentInfo = m_segInfos.ElementAt(index);
	segmentInfo.m_nDlNow = (DWORD64)dlnow;

	if(dlnow > 0.0)
	{
		AfxTrace("hello: %I64d\n", segmentInfo.m_nDlNow);
	}

	DWORD64 nDlNow = GetTotalDownloadNow();

	//Send progress notification
	CProgressInfo progressInfo;
	progressInfo.dltotal = (DWORD64)m_dlParam.m_nFileSize;
	progressInfo.dlnow = (DWORD64)nDlNow;
	progressInfo.ultotal = (DWORD64)ultotal;
	progressInfo.ulnow = (DWORD64)ulnow;
	progressInfo.retCode = -1;
	progressInfo.szReason = "";
	progressInfo.index = m_dlParam.m_nIndex;
	
	::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, (LPARAM)0);

	return 0;
}

DWORD64 CSegmentDownloader::GetTotalDownloadNow()
{	
	DWORD64 nDlNow = 0;
	int i, nSize;	
	for(i = 0, nSize = m_segInfos.GetSize(); i < nSize; i++)
	{
		nDlNow += m_segInfos.ElementAt(i).m_nDlNow;
	}

	return nDlNow;
}

void CSegmentDownloader::SplitFileRange(UINT nFileSize, CArray<CRange, CRange&>& sizeArray)
{
	int i;
	CRange segment;
	int nMinSize = nFileSize / MAX_WORKER_SESSION;
	if(nMinSize >= MIN_SEGMENT_SIZE)
	{
		for(i = 0; i < MAX_WORKER_SESSION - 1; i++)
		{
			segment.cx = i * nMinSize;
			segment.cy = segment.cx + nMinSize - 1;
			sizeArray.Add(segment);
		}
		segment.cx = i * nMinSize;
		segment.cy = nFileSize - 1;
		sizeArray.Add(segment);
	}
	else
	{
		int nSessionNumber = nFileSize / MIN_SEGMENT_SIZE;
		if(nSessionNumber > 0)
		{
			for(i = 0; i < nSessionNumber; i++)
			{
				segment.cx = i * MIN_SEGMENT_SIZE;
				segment.cy = segment.cx + MIN_SEGMENT_SIZE - 1;
				sizeArray.Add(segment);
			}
			if(nFileSize % MIN_SEGMENT_SIZE != 0)
			{
				segment.cx = nSessionNumber * MIN_SEGMENT_SIZE;
				segment.cy = nFileSize - 1;
				sizeArray.Add(segment);
			}			
		}
		//total size is less than MIN_SEGMENT_SIZE
		else
		{
			segment.cx = 0;
			segment.cy = nFileSize - 1;
			sizeArray.Add(segment);
		}		
	}
}
