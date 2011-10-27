// SegmentDownloader.cpp: implementation of the CSegmentDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SegmentDownloader.h"
#include "SegmentInfoMap.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

size_t CSegmentDownloader::HeaderCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	CCallbackParam* lpParam = (CCallbackParam*)userdata;
	return lpParam->pDownloader->ProcessHeader(ptr, size, nmemb, lpParam->nIndex);
}

size_t CSegmentDownloader::DataCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	CCallbackParam* lpParam = (CCallbackParam*)userdata;
	return lpParam->pDownloader->ProcessData(ptr, size, nmemb, lpParam->nIndex);
}

int CSegmentDownloader::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CCallbackParam* lpParam = (CCallbackParam*)clientp;
	return lpParam->pDownloader->ProcessProgress(dltotal, dlnow, ultotal, ulnow, lpParam->nIndex);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSegmentDownloader::CSegmentDownloader() : m_curlm(NULL), m_pSegmentInfoArray(NULL)
{
}

CSegmentDownloader::~CSegmentDownloader()
{
	m_curlm = NULL;
	m_pSegmentInfoArray = NULL;
}

void CSegmentDownloader::Init(const CDownloadParam& param)
{
	m_dlParam = param;

	m_pSegmentInfoArray = new CSegmentInfoArray();
	CSegmentInfoMap::GetInstance()->AddSegmentInfoArray(m_dlParam.m_szUrl, m_pSegmentInfoArray);
}


void CSegmentDownloader::Start()
{
	StartInitMultiHandle();
	Download();
}
void CSegmentDownloader::Download()
{
	fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
	int maxfd = -1;
	long curl_timeout = -1;

	int still_running = -1; /* keep number of running handles */
	CURLMsg *msg; /* for picking up messages with the transfer status */
	int msgs_left; /* how many messages are left */

	int rc = 0;	//return value
	DWORD dwResult = 0;
	CString szLog;

	while (still_running) 
	{
		//perform
		curl_multi_perform(m_curlm, &still_running);
		if(still_running)
		{
			//select
			dwResult = SelectFDSet(fdread, fdwrite, fdexcep, maxfd, curl_timeout);
			if(dwResult != 0)
			{
				szLog.Format("[SelectFDSet] error: dwResult = %d", dwResult);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
				break;
			}	
		}
		//If we go here, everything should be ok.
		
		/* See how the transfers went */
		while( (msg = curl_multi_info_read(m_curlm, &msgs_left)) )
		{
			//Done
			if (msg->msg == CURLMSG_DONE) 
			{
				rc = ProcessTransferDone(msg, still_running, dwResult);
			}
			//error
			else 
			{
				szLog.Format("[curl_multi_info_read] error: msg->msg = %d", msg->msg);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
		}

		//we should stop to transfer
		if(rc != 0)
		{
			break;
		}
	}

	//Stop all available connections
 	StopAllConnections();

	//Stop multi interface handle
	curl_multi_cleanup(m_curlm);
	m_curlm = NULL;

	WORD nMajor = LOWORD(dwResult);
	//Remove the segment information data, except for Paused or Stopped status
	if(nMajor != RC_MAJOR_PAUSED && nMajor != RC_MAJOR_STOPPED)
	{
		RemoveSegmentInfoArray();
	}

	CString szErrorMsg;
	CCommonUtils::FormatErrorMsg(dwResult, szErrorMsg);

	szLog.Format("Download result: %X - %s", dwResult, szErrorMsg);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, (WPARAM)((LPCSTR)szLog), dwResult);
}


DWORD CSegmentDownloader::SelectFDSet(fd_set& fdread, fd_set& fdwrite, fd_set& fdexcep, int& maxfd, long curl_timeout)
{
	int rc = 0;
	DWORD dwResult = 0;

	CString szLog;
	struct timeval timeout;
	do 
	{
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcep);
		
		/* get file descriptors from the transfers */
		rc = curl_multi_fdset(m_curlm, &fdread, &fdwrite, &fdexcep, &maxfd);
		if(rc != CURLM_OK)
		{
			szLog.Format("[curl_multi_fdset] error: (%d)", rc);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

			dwResult = MAKELONG(RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR, RC_MINOR_MULTI_FDSET_ERROR);
			break;
		}
		
		rc = curl_multi_timeout(m_curlm, &curl_timeout);
		if(rc != CURLM_OK)
		{
			szLog.Format("[curl_multi_timeout] error: (%d)", rc);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)	
			
			dwResult = MAKELONG(RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR, RC_MINOR_MULTI_TIMEOUT_ERROR);
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
			timeout.tv_sec = curl_timeout / 1000;
			timeout.tv_usec = (curl_timeout % 1000) * 1000;
			
			/* In a real-world program you OF COURSE check the return code of the
			function calls.  On success, the value of maxfd is guaranteed to be
			greater or equal than -1.  We call select(maxfd + 1, ...), specially in
			case of (maxfd == -1), we call select(0, ...), which is basically equal
			to sleep. */

			rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
			if(rc == -1)
			{
				szLog.Format("[select] error: maxfd = %d, curl_timeout = %ld, %d - %s", maxfd, curl_timeout, 
					errno, strerror(errno));
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

				dwResult = MAKELONG(RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR, RC_MINOR_SELECT_ERROR);

				break;
			}
		}
	} while (FALSE);

	return dwResult;
}
int CSegmentDownloader::ProcessTransferDone(CURLMsg *msg, int& still_running, DWORD& dwResult)
{
	int rc = 0;
	//Get the current connection information
	CCallbackParam* pCallbackParam = NULL;
	curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &pCallbackParam);
	
	ASSERT(pCallbackParam != NULL);
	
	//Data backup already
	int nIndex = pCallbackParam->nIndex;
	CURL* easy_handle = msg->easy_handle;
	CURLcode resCode = msg->data.result;
	
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);
	
	CString szLog;
	//log
	szLog.Format("(%d) - Transfer result: %d - %s", nIndex, resCode, curl_easy_strerror(resCode));
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
	switch(resCode)
	{
	case CURLE_OK:
		{
			//Just stop connection
			StopConnection(nIndex, DLE_OK);
		}
		break;
	case CURLE_ABORTED_BY_CALLBACK:
		{
			//(1). Stop
			if(m_controlInfo.isStopped)
			{
				szLog.Format("(%d) - Connection stopped", nIndex);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				dwResult = MAKELONG(RC_MAJOR_STOPPED, RC_MINOR_OK);
				
				StopConnection(nIndex, DLE_STOP);
			}
			//(2). Pause
			else if(m_controlInfo.isPaused)
			{
				szLog.Format("(%d) - Connection paused", nIndex);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				dwResult = MAKELONG(RC_MAJOR_PAUSED, RC_MINOR_OK);
				
				StopConnection(nIndex, DLE_PAUSE);
			}
		}
		break;
	case CURLE_OPERATION_TIMEDOUT:
		{
			StopConnection(nIndex, DLE_OTHER);
			
			CURL* retry_handle = RestartConnection(nIndex);
			
			curl_multi_add_handle(m_curlm, retry_handle);
			still_running++; //just to prevent it from remaining at 0 if there are more URLs to get
		}
		break;
		//other cases: should be actual error, retry
	default:
		{
			StopConnection(nIndex, DLE_OTHER);
			
			if(pSegmentInfo->m_nRetry < MAX_RETRY_TIMES)
			{							 
				CURL* retry_handle = RestartConnection(nIndex);
				
				curl_multi_add_handle(m_curlm, retry_handle);
				still_running++; //just to prevent it from remaining at 0 if there are more URLs to get
			}
			else
			{
				rc = resCode;

				szLog.Format("(%d) - Retry times more than %d, abort download", nIndex, MAX_RETRY_TIMES);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				dwResult = MAKELONG(RC_MAJOR_TERMINATED_BY_CURL_CODE, resCode);
			}
		}
		break;
	}
	
	return rc;
}

void CSegmentDownloader::Stop()
{
	m_ctritialSection.Lock();

	m_controlInfo.isStopped = TRUE;

	m_ctritialSection.Unlock();
}

void CSegmentDownloader::Pause()
{
	m_ctritialSection.Lock();

	m_controlInfo.isModified = TRUE;
	m_controlInfo.isPaused = TRUE;
	
	m_ctritialSection.Unlock();
}

void CSegmentDownloader::Resume()
{
	m_ctritialSection.Lock();
	m_controlInfo.isModified = TRUE;
	m_controlInfo.isPaused = FALSE;
	m_controlInfo.isStopped = FALSE;
	m_ctritialSection.Unlock();

	RestartInitMultiHandle();
	Download();
}

void CSegmentDownloader::StartInitMultiHandle()
{
	ASSERT(m_curlm == NULL);
	
	//Split the file size into several segments
	CArray<CRange, CRange&> sizeArray;
	SplitFileRange(m_dlParam.m_nFileSize, sizeArray);
	
	//Init multi handle
	m_curlm = curl_multi_init();
	curl_multi_setopt(m_curlm, CURLMOPT_MAXCONNECTS, (long)MAX_WORKER_SESSION);
	
	//Add connections to the multi handle
	int i, nSize;	
	for(i = 0, nSize = sizeArray.GetSize(); i < nSize; i++)
	{
		CRange& segment = sizeArray.ElementAt(i);
		
		CURL* easy_handle = StartConnection(i, segment.cx, segment.cy);
		curl_multi_add_handle(m_curlm, easy_handle); 
	}
}
void CSegmentDownloader::RestartInitMultiHandle()
{
	ASSERT(m_curlm == NULL);
	
	//Init multi handle
	m_curlm = curl_multi_init();
	curl_multi_setopt(m_curlm, CURLMOPT_MAXCONNECTS, (long)MAX_WORKER_SESSION);
	
	//Add connections to the multi handle
	int i, nSize;
	for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
	{
		CURL* easy_handle = RestartConnection(i);
		if(easy_handle == NULL)
		{
			continue;
		}
		curl_multi_add_handle(m_curlm, easy_handle);
	} 
}

CURL* CSegmentDownloader::StartConnection(int nIndex, int nStartPos, int nFinishPos)
{
	//1. Create Easy Handle
	CURL* easy_handle = InitEasyHandle(nIndex, nStartPos, nFinishPos);

	//2. Init Segment Information
	CSegmentInfoEx* pSegmentInfo = new CSegmentInfoEx();
	AddSegmentInfo(pSegmentInfo);
	
	pSegmentInfo->m_nIndex = nIndex;
	pSegmentInfo->m_nDlBefore = 0;
	pSegmentInfo->m_nDlNow = 0;
	pSegmentInfo->m_nRetry = 0;
	
	pSegmentInfo->m_range.cx = nStartPos;
	pSegmentInfo->m_range.cy = nFinishPos;
	
	char buf[32];
	
	sprintf(buf, "header_%d.txt", nIndex);
	pSegmentInfo->m_lpFileHeader = fopen(buf, "wb");
	
	sprintf(buf, "data_%d", nIndex);
	pSegmentInfo->m_lpFileData = fopen(buf, "wb");
	
	pSegmentInfo->m_curl = easy_handle;

	//3. Return
	return easy_handle;
}
CURL* CSegmentDownloader::RestartConnection(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_pSegmentInfoArray->GetSize());

	//1. Get existing segment information
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);
	
	//2. Get the range
	int nStartPos = pSegmentInfo->m_range.cx + pSegmentInfo->m_nDlNow;
	int nFinishPos = pSegmentInfo->m_range.cy;

	//validation check
	if(nStartPos > nFinishPos)
	{
		CString szLog;
		szLog.Format("(%d) - Transfer ended. %d - %d", nIndex, nStartPos, nFinishPos);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		return NULL;
	}

	//3. Create Easy Handle
	CURL* easy_handle = InitEasyHandle(nIndex, nStartPos, nFinishPos);

	//4. Re-config segment information
	ASSERT(pSegmentInfo->m_nIndex == nIndex);
	pSegmentInfo->m_nDlBefore += pSegmentInfo->m_nDlNow;
	pSegmentInfo->m_nDlNow = 0;
	pSegmentInfo->m_nRetry++;
	
	pSegmentInfo->m_range.cx = nStartPos;
	pSegmentInfo->m_range.cy = nFinishPos;
	
	char buf[32];
	
	sprintf(buf, "header_%d(%d).txt", nIndex, pSegmentInfo->m_nRetry);
	pSegmentInfo->m_lpFileHeader = fopen(buf, "wb");
	
	sprintf(buf, "data_%d", nIndex);
	pSegmentInfo->m_lpFileData = fopen(buf, "ab");
	
	pSegmentInfo->m_curl = easy_handle;

	//5. Return
	return easy_handle;
}
CURL* CSegmentDownloader::InitEasyHandle(int nIndex, int nStartPos, int nFinishPos)
{
	CURL* easy_handle = curl_easy_init();
	
	//URL address
	curl_easy_setopt(easy_handle, CURLOPT_URL, (LPCTSTR)m_dlParam.m_szUrl);
	
	//agent: IE8
	curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, USER_AGENT_IE8);
	//redirect
	curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
	//no verbose
	curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
	//connect timeout: 10s
	curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 10L);
	
	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_TIME, 15L);
	
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

	return easy_handle;
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
	
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(index);
	fwrite(ptr, size, nmemb, pSegmentInfo->m_lpFileHeader);
	
	return (size_t)(size * nmemb);

	return size * nmemb;
}

size_t CSegmentDownloader::ProcessData(char *ptr, size_t size, size_t nmemb, int index)
{
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(index);
	fwrite(ptr, size, nmemb, pSegmentInfo->m_lpFileData);

	return size * nmemb;
}

int CSegmentDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_pSegmentInfoArray->GetSize());
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);

	pSegmentInfo->m_nDlNow = (DWORD64)dlnow;

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

	//Pause
	if(m_controlInfo.IsModified() && m_controlInfo.IsPaused())
	{
		return 5;
	}
	//Stop
	if(m_controlInfo.IsStopped())
	{
		return 5;
	}

	return 0;
}

DWORD64 CSegmentDownloader::GetTotalDownloadNow()
{	
	DWORD64 nDlNow = 0;
	int i, nSize;	
	for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
	{
		CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(i);
		nDlNow += pSegmentInfo->m_nDlNow + pSegmentInfo->m_nDlBefore;
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

void CSegmentDownloader::StopAllConnections()
{
	int i, nSize;
	for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
	{
		StopConnection(i, 0);
	}
}
void CSegmentDownloader::StopConnection(int nIndex, int nCleanType)
{
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);

	CString szLog;
	szLog.Format("(%d) - StopConnection: m_curl = %8X", nIndex, pSegmentInfo->m_curl);
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	if(pSegmentInfo->m_curl == NULL)
	{
		return;
	}

	//Get the current connection information
	CCallbackParam* pCallbackParam = NULL;
	curl_easy_getinfo(pSegmentInfo->m_curl, CURLINFO_PRIVATE, &pCallbackParam);
	ASSERT(pCallbackParam != NULL);
	
	//clean up
	curl_multi_remove_handle(m_curlm, pSegmentInfo->m_curl);
	curl_easy_cleanup(pSegmentInfo->m_curl);
	
	if(nCleanType == DLE_STOP)
	{
		pSegmentInfo->m_nRetry = 0;
	}
	pSegmentInfo->m_curl = NULL;
	
	fclose(pSegmentInfo->m_lpFileHeader);
	fclose(pSegmentInfo->m_lpFileData);
	if(pCallbackParam != NULL)
	{
		delete pCallbackParam;
		pCallbackParam = NULL;
	}
}

CSegmentInfoEx* CSegmentDownloader::GetSegmentInfo(int nIndex)
{
	ASSERT(m_pSegmentInfoArray != NULL);
	ASSERT(nIndex >= 0 && nIndex < m_pSegmentInfoArray->GetSize());

	return (CSegmentInfoEx*)m_pSegmentInfoArray->GetAt(nIndex);
}
void CSegmentDownloader::AddSegmentInfo(CSegmentInfoEx* pSegmentInfo)
{
	ASSERT(pSegmentInfo != NULL);
	m_pSegmentInfoArray->Add(pSegmentInfo);
}

void CSegmentDownloader::RemoveSegmentInfoArray()
{
	CSegmentInfoArray* pSegInfoArray = CSegmentInfoMap::GetInstance()->GetSegmentInfoArray(m_dlParam.m_szUrl);
	ASSERT(pSegInfoArray != NULL && m_pSegmentInfoArray == pSegInfoArray);

	CSegmentInfoMap::GetInstance()->RemoveSegmentInfoArray(m_dlParam.m_szUrl);

	m_pSegmentInfoArray = NULL;
}