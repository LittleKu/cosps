// SegmentDownloader.cpp: implementation of the CSegmentDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SegmentDownloader.h"
#include "SegmentInfoMap.h"
#include "CommonUtils.h"
#include "GenericTools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CCallbackParam
{
public:
	int	nIndex;
	CSegmentDownloader* pDownloader;
	CCallbackParam(int index = -1, CSegmentDownloader* ptr = NULL) : nIndex(index), pDownloader(ptr) {}
};

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

size_t CSegmentDownloader::ProcessHeader(char* ptr, size_t size, size_t nmemb, int index)
{
	if( IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine(ptr, size * nmemb);
		CCommonUtils::ReplaceCRLF(szLine);
		
		CString szMsg;
		szMsg.Format("%d - %s", index, szLine);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szMsg)
	}
	
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(index);
	fwrite(ptr, size, nmemb, pSegmentInfo->m_lpFileHeader);
	
	//Parse
	CString szScratch(ptr, size * nmemb);
	int nc;
	do 
	{
		//1. check if status line
		int nRspCode = CCommonUtils::GetHTTPStatusCode(szScratch);
		
		//This is a status line
		if(nRspCode > 0)
		{
			pSegmentInfo->m_headerInfo.Reset();
			pSegmentInfo->m_headerInfo.m_nHTTPCode = nRspCode;
			pSegmentInfo->m_headerInfo.m_szStatusLine = szScratch;
			CCommonUtils::ReplaceCRLF(pSegmentInfo->m_headerInfo.m_szStatusLine, NULL, NULL);
			break;
		}
		
		
		//2. check if Content-Length line
		int nContentLength = -1;
		nc = sscanf((LPCTSTR)szScratch, "Content-Length: %d", &nContentLength);
		if(nContentLength >= 0)
		{
			pSegmentInfo->m_headerInfo.m_nContentLength = nContentLength;
			break;
		}
		
		//3. check if Content-Range line
		int x, y, total;
		nc = sscanf((LPCTSTR)szScratch, "Content-Range: bytes %d-%d/%d", &x, &y, &total);
		if(nc == 3)
		{
			pSegmentInfo->m_headerInfo.m_nContentRangeX = x;
			pSegmentInfo->m_headerInfo.m_nContentRangeY = y;
			pSegmentInfo->m_headerInfo.m_nContentRangeTotal = total;
			break;
		}
		
		//we don't care other things
		
	} while ( 0 );
	
	return (size_t)(size * nmemb);
}

size_t CSegmentDownloader::ProcessData(char *ptr, size_t size, size_t nmemb, int index)
{
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(index);

	size_t nBytes = size * nmemb;
	//Only process the case of 200
	if(pSegmentInfo->m_headerInfo.m_nHTTPCode == 200)
	{
		CRange rLocal(pSegmentInfo->m_range.cx + pSegmentInfo->m_nDlNow, pSegmentInfo->m_range.cy);
		CRange rRemote(pSegmentInfo->m_nRemotePos, pSegmentInfo->m_nRemotePos + nBytes - 1);
		CRange rResult;
		int rc = CCommonUtils::Intersection(rLocal, rRemote, rResult);
		CString szLog;

		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			szLog.Format("%d - rc=%d, local(%d-%d), remote(%d-%d), result(%d-%d)", index, rc, 
				rLocal.cx, rLocal.cy, rRemote.cx, rRemote.cy, rResult.cx, rResult.cy);
			LOG4CPLUS_TRACE_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}

		pSegmentInfo->m_nRemotePos += nBytes;

		//The data range doesn't reach yet
		if(rc > 0)
		{
			return nBytes;
		}
		//reach out, stop this connection
		else if(rc < 0)
		{
			if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
			{
				szLog.Format("%d - range reach out, stop connection", index);
				LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			
			return -1;
		}
		//There are data we cared
		else
		{			
			int nDataLength = rResult.cy - rResult.cx + 1;
			fwrite(ptr + rResult.cx - rRemote.cx,  1, nDataLength, pSegmentInfo->m_lpFileData);

			pSegmentInfo->m_nDlNow += nDataLength;

			//The data has move over the required block range
			if(pSegmentInfo->m_nRemotePos > pSegmentInfo->m_range.cy)
			{
				szLog.Format("%d - move over. m_nRemotePos=%d, m_nDlNow=%I64d, rangeX=%d, rangeY=%d", 
					index, pSegmentInfo->m_nRemotePos, pSegmentInfo->m_nDlNow, pSegmentInfo->m_range.cx, 
					pSegmentInfo->m_range.cy);
				LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

				return -1;
			}
			return nBytes;
		}
	}
	else
	{
		fwrite(ptr, size, nmemb, pSegmentInfo->m_lpFileData);
	}
	return nBytes;
}

int CSegmentDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int nIndex)
{
	if(m_dlState.GetState() == TSE_DESTROYING)
	{
		return 1;
	}
	ASSERT(nIndex >= 0 && nIndex < m_pSegmentInfoArray->GetSize());
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);
	
	if(pSegmentInfo->m_headerInfo.m_nHTTPCode != 200)
	{
		pSegmentInfo->m_nDlNow = (DWORD64)dlnow;
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
	progressInfo.m_nTaskID = m_dlParam.m_nTaskID;
	
	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, (LPARAM)0);

	//Pause
	if(m_dlState.GetState() == TSE_PAUSING)
	{
		return 2;
	}
	//Stop
	if(m_dlState.GetState() == TSE_STOPPING)
	{
		return 3;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSegmentDownloader::CSegmentDownloader(TaskStatusEnum eInitStatus)
 : m_curlm(NULL), m_pSegmentInfoArray(NULL), m_bResumable(TRUE), m_dlState(eInitStatus)
{
}

CSegmentDownloader::~CSegmentDownloader()
{
	RemoveSegmentInfoArray();
}

void CSegmentDownloader::Init(const CDownloadParam& param)
{
	CDownloader::Init(param);
	
	m_pSegmentInfoArray = new CSegmentInfoArray();
	CSegmentInfoMap::GetInstance()->AddSegmentInfoArray(m_dlParam.m_nTaskID, m_pSegmentInfoArray);
}


int CSegmentDownloader::Start()
{
	m_criticalSection.Lock();
	ASSERT(m_dlState.GetState() == TSE_TRANSFERRING);
	m_criticalSection.Unlock();

	VerifyTempFolderExist();
	StartInitMultiHandle();

	return DoDownload();
}

int CSegmentDownloader::Resume()
{
	m_criticalSection.Lock();
	m_dlState.SetState(TSE_TRANSFERRING);
	m_criticalSection.Unlock();

	VerifyTempFolderExist();
	RestartInitMultiHandle();	
	
	return DoDownload();
}

int CSegmentDownloader::Stop()
{
	int nResult;
	
	m_criticalSection.Lock();
	//Pause is allowed
	if( (m_dlState.GetOperAccess(DL_OPER_FLAG_PAUSE) & DL_OPER_FLAG_PAUSE) )
	{
		m_dlState.SetState(TSE_PAUSING);
	}
	nResult = m_dlState.GetState();
	m_criticalSection.Unlock();
	
	return nResult;
}

int CSegmentDownloader::Pause()
{
	int nResult;
	
	m_criticalSection.Lock();
	//Pause is allowed
	if( (m_dlState.GetOperAccess(DL_OPER_FLAG_PAUSE) & DL_OPER_FLAG_PAUSE) )
	{
		m_dlState.SetState(TSE_PAUSING);
	}
	nResult = m_dlState.GetState();
	m_criticalSection.Unlock();
	
	return nResult;
}

int CSegmentDownloader::Destroy()
{
	int nResult;
	CString szLog;

	m_criticalSection.Lock();
	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: Destroy - current state = %d", m_dlParam.m_nTaskID, m_dlState.GetState());
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	//Remove is allowed
	if( (m_dlState.GetOperAccess(DL_OPER_FLAG_REMOVE) & DL_OPER_FLAG_REMOVE) )
	{
		//Still in transferring
		if(m_dlState.GetState() == TSE_TRANSFERRING)
		{
			m_dlState.SetState(TSE_DESTROYING);
		}
		//Already paused or in other states
		else
		{
			CString szTempFolder;
			GetTempFolder(szTempFolder);
			
			//delete task related folder
			if(!(SYS_OPTIONS()->m_bKeepTempFiles))
			{
				CCommonUtils::RemoveDirectory(szTempFolder);
			}
			
			//Remove the segment information data
			RemoveSegmentInfoArray();

			//No need to notify GUI
		}
	}
	nResult = m_dlState.GetState();
	m_criticalSection.Unlock();
	
	return nResult;
}

UINT CSegmentDownloader::GetCurrentStatus()
{
	UINT nResult;

	m_criticalSection.Lock();
	nResult = m_dlState.GetState();
	m_criticalSection.Unlock();

	return nResult;
}
void CSegmentDownloader::SetState(DWORD nState, LPCTSTR lpszDetail)
{
	m_criticalSection.Lock();
	m_dlState.SetState(nState, lpszDetail);
	m_criticalSection.Unlock();
}

BOOL CSegmentDownloader::IsResumable()
{
	return m_bResumable;
}

int CSegmentDownloader::DoDownload()
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
	
	PostDownload(dwResult);

	return dwResult;
}

void CSegmentDownloader::PostDownload(DWORD dwResult)
{
	//1. Clean up
	//Stop all available connections
	CloseAllConnections();
	
	//Stop multi interface handle
	curl_multi_cleanup(m_curlm);
	m_curlm = NULL;
	
	//Result Msg
	CString szLog, szErrorMsg;
	CCommonUtils::FormatErrorMsg(dwResult, szErrorMsg);
	szLog.Format("Download result: 0x%08X - %s", dwResult, szErrorMsg);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	//2. Post process
	WORD nMajor = LOWORD(dwResult);

	//Merge files when successfully
	if(nMajor == RC_MAJOR_OK)
	{
		CString szTempFolder;
		GetTempFolder(szTempFolder);

		//1. merge files
		//source files
		CStringArray srcFileNames;
		int i, nSize;
		for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
		{
			srcFileNames.Add(GetSegmentInfo(i)->m_szFileData);
		}
		//destination file
		CString szDstFile;
		szDstFile.Format("%s\\%s", SYS_OPTIONS()->m_szSaveToFolder, m_dlParam.m_szSaveToFileName);
		
		//merge source files to destination file
		CCommonUtils::MergeFiles(srcFileNames, szDstFile);

		//2. delete task related folder
		if(!(SYS_OPTIONS()->m_bKeepTempFiles))
		{
			CCommonUtils::RemoveDirectory(szTempFolder);
		}

		//3. Remove the segment information data
		RemoveSegmentInfoArray();
	}
	else if(nMajor == RC_MAJOR_PAUSED)
	{
//		CurrentStatusChanged(TSE_PAUSED);
	}
	else if(nMajor == RC_MAJOR_STOPPED)
	{
//		CurrentStatusChanged(TSE_STOPPED);
	}
	else if(nMajor == RC_MAJOR_DESTROYED)
	{
		CString szTempFolder;
		GetTempFolder(szTempFolder);
				
		//delete task related folder
		if(!(SYS_OPTIONS()->m_bKeepTempFiles))
		{
			CCommonUtils::RemoveDirectory(szTempFolder);
		}
		
		//Remove the segment information data
		RemoveSegmentInfoArray();

// 		CurrentStatusChanged(TSE_DESTROYED);
// 
// 		::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_DESTROY, (WPARAM)((LPCSTR)szLog), dwResult);
// 
// 		return;
	}
	else if(nMajor == RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR)
	{
		//Remove the segment information data, except for Paused or Stopped status
		RemoveSegmentInfoArray();

//		CurrentStatusChanged(TSE_END_WITH_ERROR, szErrorMsg);
	}
	else if(nMajor == RC_MAJOR_TERMINATED_BY_CURL_CODE)
	{
		//Remove the segment information data, except for Paused or Stopped status
		RemoveSegmentInfoArray();

//		CurrentStatusChanged(TSE_END_WITH_ERROR, szErrorMsg);
	}
	else
	{
		//TODO
// 		szErrorMsg.Format("Unknown error: %d", dwResult);
// 		CurrentStatusChanged(TSE_END_WITH_ERROR, szErrorMsg);
	}

	//TODO: Set State detail information
	m_criticalSection.Lock();
	m_dlState.SetState(CCommonUtils::ResultCode2StatusCode(dwResult));
	m_criticalSection.Unlock();

	::SetEvent(m_hStopEvent);

//	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)&m_dlState);

//	TaskFinished(dwResult);
}

BOOL CSegmentDownloader::IsStillTransferring()
{
	UINT nStatus = GetCurrentStatus();
	if(nStatus == TSE_TRANSFERRING || nStatus == TSE_PAUSING || nStatus == TSE_STOPPING || nStatus == TSE_DESTROYING)
	{
		return TRUE;
	}
	return FALSE;
}
void CSegmentDownloader::WaitUntilStop()
{
	CString szLog;
	DWORD dwResult;
	while( IsStillTransferring() )
	{
		Destroy();
		
		szLog.Format("Task [%d] is transferring, wait until it stopped.", m_dlParam.m_nTaskID);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		dwResult = ::WaitForSingleObject(m_hStopEvent, INFINITE);
		
		szLog.Format("Task [%d] WaitForSingleObject returned 0x%08X", m_dlParam.m_nTaskID, dwResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	
	szLog.Format("Task [%d] Stopped succesfully. Current status=%d", m_dlParam.m_nTaskID, GetCurrentStatus());
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
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
	
	//log
	CString szLog;
	szLog.Format("(%d) - Transfer result: %d - %s", nIndex, resCode, curl_easy_strerror(resCode));
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	//Firstly close the current connection
	CloseConnection(nIndex);
		
	switch(resCode)
	{
	case CURLE_OK:
		{
			//Do nothing
		}
		break;
	case CURLE_ABORTED_BY_CALLBACK:
		{
			DWORD nState;
			m_criticalSection.Lock();
			nState = m_dlState.GetState();
			m_criticalSection.Unlock();

			//(0). This task been destroyed
			if(nState == TSE_DESTROYING)
			{
				dwResult = MAKELONG(RC_MAJOR_DESTROYED, RC_MINOR_OK);

				szLog.Format("(%d) - Connection destroyed", nIndex);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			//(1). Stop
			else if(nState == TSE_STOPPING)
			{
				dwResult = MAKELONG(RC_MAJOR_STOPPED, RC_MINOR_OK);

				szLog.Format("(%d) - Connection stopped", nIndex);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			//(2). Pause
			else if(nState == TSE_PAUSING)
			{
				dwResult = MAKELONG(RC_MAJOR_PAUSED, RC_MINOR_OK);

				szLog.Format("(%d) - Connection paused", nIndex);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
			//other, auto stop. This should not happen
			else
			{
				szLog.Format("(%d) - Connection stopped. Error: this should not happen", nIndex);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			}
		}
		break;
	case CURLE_WRITE_ERROR:
		{
			szLog.Format("(%d) - Connection stopped by write data function", nIndex);
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
		break;
	case CURLE_OPERATION_TIMEDOUT:
		{
			//Always try to retry
			CURL* retry_handle = RestartConnection(nIndex, CCommonUtils::INT_OPER_KEEP);
			
			if(retry_handle != NULL)
			{
				curl_multi_add_handle(m_curlm, retry_handle);
				still_running++; //just to prevent it from remaining at 0 if there are more URLs to get
			}
		}
		break;
		//other cases: should be actual error, retry
	default:
		{			
			if(pSegmentInfo->m_nRetry < SYS_OPTIONS()->m_nMaxRetryTimes)
			{							 
				CURL* retry_handle = RestartConnection(nIndex, CCommonUtils::INT_OPER_INCREASE);
				
				if(retry_handle != NULL)
				{
					curl_multi_add_handle(m_curlm, retry_handle);
					still_running++; //just to prevent it from remaining at 0 if there are more URLs to get
				}
			}
			else
			{
				rc = resCode;

				szLog.Format("(%d) - Retry times more than %d, abort download", nIndex, SYS_OPTIONS()->m_nMaxRetryTimes);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				dwResult = MAKELONG(RC_MAJOR_TERMINATED_BY_CURL_CODE, resCode);
			}
		}
		break;
	}
	
	return rc;
}


void CSegmentDownloader::StartInitMultiHandle()
{
	ASSERT(m_curlm == NULL);
	
	//Split the file size into several segments
	CArray<CRange, CRange&> sizeArray;
	CCommonUtils::Split(sizeArray, m_dlParam.m_nFileSize, SYS_OPTIONS()->m_nMinSegmentSize, 
		SYS_OPTIONS()->m_nMaxConnectionCount);
	
	//Init multi handle
	m_curlm = curl_multi_init();
	curl_multi_setopt(m_curlm, CURLMOPT_MAXCONNECTS, (long)SYS_OPTIONS()->m_nMaxConnectionCount);
	
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
	curl_multi_setopt(m_curlm, CURLMOPT_MAXCONNECTS, (long)SYS_OPTIONS()->m_nMaxConnectionCount);
	
	//Add connections to the multi handle
	int i, nSize;
	for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
	{
		CURL* easy_handle = RestartConnection(i, CCommonUtils::INT_OPER_RESET);
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
	pSegmentInfo->m_nRemotePos = 0;
	
	pSegmentInfo->m_range.cx = nStartPos;
	pSegmentInfo->m_range.cy = nFinishPos;
	
	pSegmentInfo->m_headerInfo.Reset();

	CString szTempFolder;
	GetTempFolder(szTempFolder);

	pSegmentInfo->m_szFileHeader.Format("%s\\header_%s_%d.txt", szTempFolder, m_dlParam.m_szSaveToFileName, nIndex);
	pSegmentInfo->m_lpFileHeader = fopen(pSegmentInfo->m_szFileHeader, "wb");
	
	pSegmentInfo->m_szFileData.Format("%s\\%s_%d", szTempFolder, m_dlParam.m_szSaveToFileName, nIndex);
	pSegmentInfo->m_lpFileData = fopen(pSegmentInfo->m_szFileData, "wb");
	
	pSegmentInfo->m_curl = easy_handle;

	//3. Return
	return easy_handle;
}
CURL* CSegmentDownloader::RestartConnection(int nIndex, int nRetryOperType)
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
	CCommonUtils::IntegerOper(pSegmentInfo->m_nRetry, nRetryOperType);
	pSegmentInfo->m_nRemotePos = 0;
	
	pSegmentInfo->m_range.cx = nStartPos;
	pSegmentInfo->m_range.cy = nFinishPos;

	pSegmentInfo->m_headerInfo.Reset();

	CString szTempFolder;
	GetTempFolder(szTempFolder);
	
	pSegmentInfo->m_szFileHeader.Format("%s\\header_%s_%d(%d).txt", szTempFolder,
		m_dlParam.m_szSaveToFileName, nIndex, pSegmentInfo->m_nRetry);
	pSegmentInfo->m_lpFileHeader = fopen(pSegmentInfo->m_szFileHeader, "wb");
	
	pSegmentInfo->m_szFileData.Format("%s\\%s_%d", szTempFolder, m_dlParam.m_szSaveToFileName, nIndex);
	pSegmentInfo->m_lpFileData = fopen(pSegmentInfo->m_szFileData, "ab");
	
	pSegmentInfo->m_curl = easy_handle;

	//5. Return
	return easy_handle;
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

void CSegmentDownloader::CloseAllConnections()
{
	int i, nSize;
	for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
	{
		CloseConnection(i);
	}
}
void CSegmentDownloader::CloseConnection(int nIndex)
{
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		CString szLog;
		szLog.Format("(%d) - StopConnection: m_curl = 0x%08X", nIndex, pSegmentInfo->m_curl);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}

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
	
	fclose(pSegmentInfo->m_lpFileHeader);
	fclose(pSegmentInfo->m_lpFileData);
	pSegmentInfo->m_lpFileHeader = NULL;
	pSegmentInfo->m_lpFileData = NULL;

	pSegmentInfo->m_curl = NULL;
	
	if(pCallbackParam != NULL)
	{
		delete pCallbackParam;
		pCallbackParam = NULL;
	}
}

void CSegmentDownloader::GetTempFolder(CString& szTempFolder)
{
	szTempFolder.Format("%s\\%02d_%s", SYS_OPTIONS()->m_szTempFolder, m_dlParam.m_nTaskID, m_dlParam.m_szSaveToFileName);
}
void CSegmentDownloader::VerifyTempFolderExist()
{
	CString szTempFolder;
	GetTempFolder(szTempFolder);

	CCommonUtils::VerifyDirectoryExist(szTempFolder);
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
	if(m_pSegmentInfoArray == NULL)
	{
		return;
	}
	CSegmentInfoArray* pSegInfoArray = CSegmentInfoMap::GetInstance()->GetSegmentInfoArray(m_dlParam.m_nTaskID);
	ASSERT(pSegInfoArray != NULL && m_pSegmentInfoArray == pSegInfoArray);
	
	CSegmentInfoMap::GetInstance()->RemoveSegmentInfoArray(m_dlParam.m_nTaskID);
	
	m_pSegmentInfoArray = NULL;
}

CURL* CSegmentDownloader::InitEasyHandle(int nIndex, int nStartPos, int nFinishPos)
{		
	CURL* easy_handle = curl_easy_init();
	CString szRange;
	CCallbackParam* pCallbackParam = NULL;
	
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
	
	//low speed limit
	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_TIME, 15L);
	
	//Proxy setting
	if(SYS_OPTIONS()->GetProxy() != NULL)
	{
		curl_easy_setopt(easy_handle, CURLOPT_PROXY, SYS_OPTIONS()->GetProxy());
	}
	
	//segment download, range setting
	szRange.Format("%d-%d", nStartPos, nFinishPos);
	curl_easy_setopt(easy_handle, CURLOPT_RANGE, (LPCTSTR)szRange);
	
	pCallbackParam = new CCallbackParam(nIndex, this);
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
