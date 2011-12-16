// SegmentDownloader.cpp: implementation of the CSegmentDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SegmentDownloader.h"
#include "SegmentInfoMap.h"
#include "CommonUtils.h"
#include "GenericTools.h"
#include "DownloaderContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME("SEGD")

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
	if( IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine(ptr, size * nmemb);
		CCommonUtils::ReplaceCRLF(szLine);
		
		CString szMsg;
		szMsg.Format("Task[%02d]: %d - %s", m_dlParam.m_nTaskID, index, szLine);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szMsg)
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

		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			szLog.Format("%d - rc=%d, local(%d-%d), remote(%d-%d), result(%d-%d)", index, rc, 
				rLocal.cx, rLocal.cy, rRemote.cx, rRemote.cy, rResult.cx, rResult.cy);
			LOG4CPLUS_TRACE_STR(THE_LOGGER, (LPCTSTR)szLog)
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
			if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
			{
				szLog.Format("%d - range reach out, stop connection", index);
				LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
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
				if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
				{
					szLog.Format("%d - move over. m_nRemotePos=%d, m_nDlNow=%I64d, rangeX=%d, rangeY=%d", 
						index, pSegmentInfo->m_nRemotePos, pSegmentInfo->m_nDlNow, pSegmentInfo->m_range.cx, 
						pSegmentInfo->m_range.cy);
					LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
				}

				return -1;
			}
			return nBytes;
		}
	}
	else if(pSegmentInfo->m_headerInfo.m_nHTTPCode == 206)
	{
		fwrite(ptr, size, nmemb, pSegmentInfo->m_lpFileData);
	}
	else
	{
		CString szLog;
		szLog.Format("Task[%02d]: %d - Stop connection because of unexpected HTTP code: %d", m_dlParam.m_nTaskID, 
			index, pSegmentInfo->m_headerInfo.m_nHTTPCode);
		LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)

		return -1;
	}
	return nBytes;
}

int CSegmentDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int nIndex)
{
	DWORD dwState = m_pContext->GetState();
	if(dwState == TSE_DESTROYING)
	{
		return 1;
	}

	ASSERT(nIndex >= 0 && nIndex < m_pSegmentInfoArray->GetSize());
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);

	if(pSegmentInfo->m_headerInfo.m_nHTTPCode != 200)
	{
		pSegmentInfo->m_nDlNow = (DWORD64)dlnow;
	}

	DWORD64 dwTotalNow, dwCurrentNow;
	GetTotalDownloadNow(&dwTotalNow, &dwCurrentNow);

	m_progressMeter.UpdateSample(clock(), dwTotalNow, dwCurrentNow);
	if(m_progressMeter.IsProgressTimeOut() || dwTotalNow == (DWORD64)m_dlParam.m_nFileSize)
	{
		/*
		//Send progress notification
		CProgressInfo progressInfo;
		progressInfo.m_pProgressMetric = &m_progressMeter;
		progressInfo.m_nTaskID = m_dlParam.m_nTaskID;
		progressInfo.dltotal = (DWORD64)m_dlParam.m_nFileSize;
		progressInfo.dlnow = (DWORD64)dwTotalNow;
		progressInfo.ultotal = (DWORD64)ultotal;
		progressInfo.ulnow = (DWORD64)ulnow;
		
		CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, (LPARAM)0);
		*/


		CTaskInfo updateInfo;
		updateInfo.m_nTaskID = m_dlParam.m_nTaskID;
		updateInfo.mask = CTaskInfo::TIF_PROGRESS;
		updateInfo.m_nFileSize = m_dlParam.m_nFileSize;
		updateInfo.m_nDlNow = dwTotalNow;
		updateInfo.m_nCurrSpeed = m_progressMeter.GetCurrentSpeed();
		updateInfo.m_nCostTime = m_progressMeter.GetCostTime();
		updateInfo.m_nLeftTime = m_progressMeter.GetLeftTime();
		
		::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)m_dlParam.m_nTaskID, (LPARAM)&updateInfo);
	}

	//Pause
	if(dwState == TSE_PAUSING)
	{
		return 2;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSegmentDownloader::CSegmentDownloader(CDownloaderContext* pContext)
 : m_curlm(NULL), m_pSegmentInfoArray(NULL), m_pContext(pContext)
{
}

CSegmentDownloader::~CSegmentDownloader()
{
	ASSERT(m_curlm == NULL);
	RemoveSegmentInfoArray();
}

void CSegmentDownloader::Init(const CDownloadParam& param)
{
	m_dlParam = param;
	
	m_pSegmentInfoArray = new CSegmentInfoArray();
	CSegmentInfoMap::GetInstance()->AddSegmentInfoArray(m_dlParam.m_nTaskID, m_pSegmentInfoArray);
}


int CSegmentDownloader::Start()
{
	VerifyTempFolderExist();

	ASSERT(m_pSegmentInfoArray != NULL);
	DWORD64 dwTotalNow = 0;
	if(m_pSegmentInfoArray->GetSize() == 0)
	{
		StartInitMultiHandle();
	}
	else
	{
		RestartInitMultiHandle();
		GetTotalDownloadNow(&dwTotalNow);
	}
	m_progressMeter.Reset(clock(), m_dlParam.m_nFileSize, dwTotalNow);
	
	return DoDownload();
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
	BOOL bResult = FALSE;
	CDownloadState dlState(TSE_COMPLETE);
	CString szLog;

	while (still_running) 
	{
		//perform
		curl_multi_perform(m_curlm, &still_running);
		if(still_running)
		{
			//select
			bResult = SelectFDSet(fdread, fdwrite, fdexcep, maxfd, curl_timeout, dlState);
			if(!bResult)
			{
				szLog.Format("[SelectFDSet] error: result state = %s", dlState.ToString(TRUE));
				LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)szLog)
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
				rc = ProcessTransferDone(msg, still_running, dlState);
			}
			//error
			else 
			{
				szLog.Format("[curl_multi_info_read] error: msg->msg = %d", msg->msg);
				LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)szLog)
			}
		}

		//we should stop to transfer
		if(rc != 0)
		{
			break;
		}
	}
	
	return PostDownload(dlState);
}

int CSegmentDownloader::PostDownload(CDownloadState& dlState)
{
	//1. Clean up
	//Stop all available connections
	CloseAllConnections();
	
	//Stop multi interface handle
	curl_multi_cleanup(m_curlm);
	m_curlm = NULL;
	
	//Result Msg
	CString szLog;
	szLog.Format("Task[%02d]: Download result: %s", m_dlParam.m_nTaskID, dlState.ToString(TRUE));
	LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)

	//2. Post process
	DWORD dwResultState = dlState.GetState();

	BOOL bValidateResult = (dwResultState == TSE_COMPLETE || dwResultState == TSE_PAUSED || dwResultState == TSE_END_WITH_ERROR
		|| dwResultState == TSE_DESTROYED);

	if(!bValidateResult)
	{
		ASSERT(FALSE);
	}

	//(1). Complete successfully
	if(dwResultState == TSE_COMPLETE)
	{
		CString szTempFolder;
		CCommonUtils::GetTempFolder(szTempFolder, m_dlParam);

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
	//(2). Destroyed
	else if(dwResultState == TSE_DESTROYED)
	{
		CString szTempFolder;
		CCommonUtils::GetTempFolder(szTempFolder, m_dlParam);
		
		//delete task related folder
		if(!(SYS_OPTIONS()->m_bKeepTempFiles))
		{
			CCommonUtils::RemoveDirectory(szTempFolder);
		}
		
		//Remove the segment information data
		RemoveSegmentInfoArray();
	}
	//Nothing to do with other cases
	
	int nResult;
	m_pContext->Lock();

	//Special case for destroy case
	if(m_pContext->NoLockGetState() == TSE_DESTROYING && dwResultState != TSE_DESTROYED)
	{
		szLog.Format("Task[%02d]: PostDownload, explicitly change state from %s to %d(Destroyed)", 
			m_dlParam.m_nTaskID, dlState.ToString(TRUE), TSE_DESTROYED);
		LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)

		dlState.SetState(TSE_DESTROYED);
	}

	//update the task state
	m_pContext->NoLockSetState(dlState.GetState(), dlState.m_szDetail);
	nResult = m_pContext->NoLockGetState();

	m_pContext->Unlock();

	return nResult;
}

BOOL CSegmentDownloader::SelectFDSet(fd_set& fdread, fd_set& fdwrite, fd_set& fdexcep, int& maxfd, long curl_timeout, CDownloadState& dlState)
{
	int rc = 0;
	BOOL bResult = FALSE;

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
			LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)szLog)

			dlState.SetState(TSE_END_WITH_ERROR, _T("[Internal Error]: multi_fdset error"));
			break;
		}
		
		rc = curl_multi_timeout(m_curlm, &curl_timeout);
		if(rc != CURLM_OK)
		{
			szLog.Format("[curl_multi_timeout] error: (%d)", rc);
			LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)szLog)

			dlState.SetState(TSE_END_WITH_ERROR, _T("[Internal Error]: multi_timout error"));
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
				LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)szLog)

				dlState.SetState(TSE_END_WITH_ERROR, _T("[Internal Error]: select error"));

				break;
			}
		}

		bResult = TRUE;

	} while (FALSE);

	return bResult;
}
int CSegmentDownloader::ProcessTransferDone(CURLMsg *msg, int& still_running, CDownloadState& dlResultState)
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
	
	DWORD dwCurrState = m_pContext->GetState();
	//log
	CString szLog;

	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: (%d) - State: %d, Transfer result: %d - %s", m_dlParam.m_nTaskID, nIndex, 
			dwCurrState, resCode, curl_easy_strerror(resCode));
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
	}

	//Firstly close the current connection
	CloseConnection(nIndex);
	
	//Default make the state to complete
	dlResultState.SetState(TSE_COMPLETE);

	if(dwCurrState == TSE_DESTROYING)
	{
		dlResultState.SetState(TSE_DESTROYED);
	}
	else if(dwCurrState == TSE_PAUSING)
	{
		dlResultState.SetState(TSE_PAUSED);
	}
	else if(dwCurrState == TSE_TRANSFERRING)
	{
		switch(resCode)
		{
		case CURLE_OK:
			{
				dlResultState.SetState(TSE_COMPLETE);
			}
			break;
		case CURLE_ABORTED_BY_CALLBACK:
			{
				szLog.Format("Task[%02d]: [ProcessTransferDone] (%d) - Connection stopped by abort callback. "
					"Unexpected State: %d", m_dlParam.m_nTaskID, nIndex, dwCurrState);
				LOG4CPLUS_FATAL_STR(THE_LOGGER, (LPCTSTR)szLog)
					
				ASSERT(FALSE);
			}
			break;
		case CURLE_WRITE_ERROR:
			{
				if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
				{
					szLog.Format("Task[%02d]: [ProcessTransferDone] (%d) - Connection stopped by write data function", 
						m_dlParam.m_nTaskID, nIndex);
					LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
				}
				if(pSegmentInfo->m_headerInfo.m_nHTTPCode != 200 && pSegmentInfo->m_headerInfo.m_nHTTPCode != 206)
				{
					//Always try to retry
					CURL* retry_handle = RestartConnection(nIndex, CCommonUtils::INT_OPER_KEEP);
					
					if(retry_handle != NULL)
					{
						curl_multi_add_handle(m_curlm, retry_handle);
						still_running++; //just to prevent it from remaining at 0 if there are more URLs to get
					}
				}
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
					
					szLog.Format("Task[%02d]: [ProcessTransferDone] (%d) - Retry times more than %d, abort download", 
						m_dlParam.m_nTaskID, nIndex, SYS_OPTIONS()->m_nMaxRetryTimes);
					LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)

					dlResultState.SetState(TSE_END_WITH_ERROR, CCommonUtils::CurlCode2Str(resCode));
				}
			}
			break;
		}
	}
	else
	{
		szLog.Format("Task[%02d]: [ProcessTransferDone] Unexpected state = %d", m_dlParam.m_nTaskID, dwCurrState);
		LOG4CPLUS_FATAL_STR(THE_LOGGER, (LPCTSTR)szLog)
			
		ASSERT(FALSE);
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
	CCommonUtils::GetTempFolder(szTempFolder, m_dlParam);

	pSegmentInfo->m_szFileHeader.Format("%s\\head_%d.txt", szTempFolder, nIndex);
	pSegmentInfo->m_lpFileHeader = fopen(pSegmentInfo->m_szFileHeader, "wb");
	
	pSegmentInfo->m_szFileData.Format("%s\\data_%d", szTempFolder, nIndex);
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
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Task[%02d]: (%d) - Transfer ended. %d - %d", m_dlParam.m_nTaskID, nIndex, nStartPos, nFinishPos);
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
			
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
	CCommonUtils::GetTempFolder(szTempFolder, m_dlParam);
	
	pSegmentInfo->m_szFileHeader.Format("%s\\head_%d(%d).txt", szTempFolder, nIndex, pSegmentInfo->m_nRetry);
	pSegmentInfo->m_lpFileHeader = fopen(pSegmentInfo->m_szFileHeader, "wb");
	
	pSegmentInfo->m_szFileData.Format("%s\\data_%d", szTempFolder, m_dlParam.m_szSaveToFileName, nIndex);
	pSegmentInfo->m_lpFileData = fopen(pSegmentInfo->m_szFileData, "ab");
	
	pSegmentInfo->m_curl = easy_handle;

	//5. Return
	return easy_handle;
}

void CSegmentDownloader::GetTotalDownloadNow(DWORD64* lpTotalNow, DWORD64* lpCurrentNow)
{	
	if(lpTotalNow != NULL)
	{
		*lpTotalNow = 0;
	}
	if(lpCurrentNow != NULL)
	{
		*lpCurrentNow = 0;
	}

	int i, nSize;	
	for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
	{
		CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(i);
		if(lpTotalNow != NULL)
		{
			*lpTotalNow += pSegmentInfo->m_nDlNow + pSegmentInfo->m_nDlBefore;
		}
		if(lpCurrentNow != NULL)
		{
			*lpCurrentNow += pSegmentInfo->m_nDlNow;
		}
	}
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

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		CString szLog;
		szLog.Format("Task[%02d]: (%d) - StopConnection: m_curl = 0x%08X", m_dlParam.m_nTaskID, 
			nIndex, pSegmentInfo->m_curl);
		LOG4CPLUS_TRACE_STR(ROOT_LOGGER, (LPCTSTR)szLog)
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

void CSegmentDownloader::VerifyTempFolderExist()
{
	CString szTempFolder;
	CCommonUtils::GetTempFolder(szTempFolder, m_dlParam);

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

	CSegmentInfoMap::GetInstance()->RemoveSegmentInfoArray(m_dlParam.m_nTaskID, m_pSegmentInfoArray);
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
