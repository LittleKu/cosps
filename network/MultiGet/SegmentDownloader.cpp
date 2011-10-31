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

CSegmentDownloader::CSegmentDownloader() : m_curlm(NULL), m_pSegmentInfoArray(NULL), m_bResumable(TRUE),
	m_bHeaderChecked(FALSE), m_nUsed(-1)
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
	VerifyTempFolderExist();
	StartInitMultiHandle();

	CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_TRANSFERRING);

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
	
	PostDownload(dwResult);
}

void CSegmentDownloader::PostDownload(DWORD dwResult)
{
	//1. Clean up
	//Stop all available connections
	StopAllConnections();
	
	//Stop multi interface handle
	curl_multi_cleanup(m_curlm);
	m_curlm = NULL;
	
	//Result Msg
	CString szErrorMsg;
	CCommonUtils::FormatErrorMsg(dwResult, szErrorMsg);

	CString szLog;
	szLog.Format("Download result: %X - %s", dwResult, szErrorMsg);
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
		if(m_nUsed < 0)
		{
			int i, nSize;
			for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
			{
				srcFileNames.Add(GetSegmentInfo(i)->m_szFileData);
			}
		}
		else
		{
			srcFileNames.Add(GetSegmentInfo(m_nUsed)->m_szFileData);
		}
		//destination file
		CString szDstFile;
		szDstFile.Format("%s\\%s", SYS_OPTIONS()->m_szSaveToFolder, m_dlParam.m_szSaveToFileName);
		
		//merge source files to destination file
		CCommonUtils::MergeFiles(srcFileNames, szDstFile);


		//2. delete task related folder
		CCommonUtils::RemoveDirectory(szTempFolder);


		//3. Remove the segment information data
		RemoveSegmentInfoArray();


		//Final status: complete
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_COMPLETE);
	}
	else if(nMajor == RC_MAJOR_PAUSED)
	{
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_PAUSED);
	}
	else if(nMajor == RC_MAJOR_STOPPED)
	{
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_STOPPED);
	}
	else if(nMajor == RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR)
	{
		//Remove the segment information data, except for Paused or Stopped status
		RemoveSegmentInfoArray();

		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_END_WITH_ERROR, szErrorMsg);
	}
	else if(nMajor == RC_MAJOR_TERMINATED_BY_CURL_CODE)
	{
		//Remove the segment information data, except for Paused or Stopped status
		RemoveSegmentInfoArray();

		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_END_WITH_ERROR, szErrorMsg);
	}
	else
	{
		//TODO
		szErrorMsg.Format("Unknown error: %d", dwResult);
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_END_WITH_ERROR, szErrorMsg);
	}
		
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
			//(0). segment download unsupported, stop other connections except m_nUsed
			if(m_nUsed >= 0)
			{
				ASSERT(m_nUsed != nIndex);

				szLog.Format("(%d) - Connection dropped", nIndex);
				LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

				StopConnection(nIndex, DLE_STOP);
			}
			//(1). Stop
			else if(m_controlInfo.isStopped)
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
			StopConnection(nIndex, DLE_OTHER);
			
			if(pSegmentInfo->m_nRetry < MAX_RETRY_TIMES)
			{							 
				CURL* retry_handle = RestartConnection(nIndex);
				
				if(retry_handle != NULL)
				{
					curl_multi_add_handle(m_curlm, retry_handle);
					still_running++; //just to prevent it from remaining at 0 if there are more URLs to get
				}
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

	VerifyTempFolderExist();

	CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, TSE_TRANSFERRING);

	RestartInitMultiHandle();
	Download();
}

BOOL CSegmentDownloader::IsResumable()
{
	return m_bResumable;
}
void CSegmentDownloader::StartInitMultiHandle()
{
	ASSERT(m_curlm == NULL);
	
	//Split the file size into several segments
	CArray<CRange, CRange&> sizeArray;
	CCommonUtils::Split(sizeArray, m_dlParam.m_nFileSize, MIN_SEGMENT_SIZE, MAX_WORKER_SESSION);
	
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
	

	CString szTempFolder;
	GetTempFolder(szTempFolder);

	pSegmentInfo->m_szFileHeader.Format("%s\\%s_header_%d.txt", szTempFolder, m_dlParam.m_szSaveToFileName, nIndex);
	pSegmentInfo->m_lpFileHeader = fopen(pSegmentInfo->m_szFileHeader, "wb");
	
	pSegmentInfo->m_szFileData.Format("%s\\%s_%d", szTempFolder, m_dlParam.m_szSaveToFileName, nIndex);
	pSegmentInfo->m_lpFileData = fopen(pSegmentInfo->m_szFileData, "wb");
	
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
	

	CString szTempFolder;
	GetTempFolder(szTempFolder);
	
	pSegmentInfo->m_szFileHeader.Format("%s\\%s_header_%d(%d).txt", szTempFolder,
		m_dlParam.m_szSaveToFileName, nIndex, pSegmentInfo->m_nRetry);
	pSegmentInfo->m_lpFileHeader = fopen(pSegmentInfo->m_szFileHeader, "wb");
	
	pSegmentInfo->m_szFileData.Format("%s\\%s_%d", szTempFolder, m_dlParam.m_szSaveToFileName, nIndex);
	pSegmentInfo->m_lpFileData = fopen(pSegmentInfo->m_szFileData, "ab");
	
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

	if(SYS_OPTIONS()->GetInstance()->m_nProxyMode == PME_SYS_PROXY ||SYS_OPTIONS()->GetInstance()->m_nProxyMode == PME_USER_PROXY)
	{
		if(!SYS_OPTIONS()->GetInstance()->m_szProxy.IsEmpty())
		{
			curl_easy_setopt(easy_handle, CURLOPT_PROXY, (LPCTSTR)SYS_OPTIONS()->GetInstance()->m_szProxy);
		}
	}
	
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
	fwrite(ptr, size, nmemb, pSegmentInfo->m_lpFileData);

	return size * nmemb;
}

int CSegmentDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow, int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_pSegmentInfoArray->GetSize());
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);

	if(!m_bHeaderChecked && pSegmentInfo->m_headerInfo.m_nHTTPCode > 0)
	{	
		int nVC = CheckSegmentHeader(nIndex);
		if(nVC == VCE_200)
		{
			m_bHeaderChecked = TRUE;
			m_nUsed = nIndex;
		}
		else if(nVC == VCE_OK)
		{
			int i, nSize, nResult;

			//there is one 200
			for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
			{
				if(i == nIndex)
				{
					continue;
				}
				nResult = CheckSegmentHeader(i);
				if(nResult == VCE_200)
				{
					m_bHeaderChecked = TRUE;
					m_nUsed = nIndex;

					CString szLog;
					szLog.Format("Select connection: (%d), http_code=%d, content_length=%d, range_x=%d, range_y=%d, range_total=%d",
						nIndex, pSegmentInfo->m_headerInfo.m_nHTTPCode, pSegmentInfo->m_headerInfo.m_nContentLength,
						pSegmentInfo->m_headerInfo.m_nContentRangeX, pSegmentInfo->m_headerInfo.m_nContentRangeY,
						pSegmentInfo->m_headerInfo.m_nContentRangeTotal);
					LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

					break;
				}
			}

			if(!m_bHeaderChecked)
			{
				//they are all 206
				BOOL bAllOK = TRUE;
				for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
				{
					if(i == nIndex)
					{
						continue;
					}
					nResult = CheckSegmentHeader(i);
					if(nResult != VCE_OK)
					{
						bAllOK = FALSE;
						break;
					}
				}

				if(bAllOK)
				{
					m_bHeaderChecked = TRUE;
					m_nUsed = -1;
				}
				else
				{
					BOOL bHasNoInvalid = TRUE;
					for(i = 0, nSize = m_pSegmentInfoArray->GetSize(); i < nSize; i++)
					{
						if(i == nIndex)
						{
							continue;
						}
						nResult = CheckSegmentHeader(i);
						if(nResult == VCE_INVALID)
						{
							bHasNoInvalid = FALSE;
							break;
						}
					}

					if(bHasNoInvalid)
					{
						m_bHeaderChecked = TRUE;
						m_nUsed = -1;
					}
				}
			}
		}
	}

	//when the server doesn's support segment download
	if(m_nUsed >= 0)
	{
		if(m_nUsed != nIndex)
		{
			CString szLog;
			szLog.Format("Stop connection: (%d) because of segment download not supported", nIndex);
			return 10;
		}
	}

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
	progressInfo.index = m_dlParam.m_nTaskID;
	
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

int CSegmentDownloader::CheckSegmentHeader(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_pSegmentInfoArray->GetSize());
	CSegmentInfoEx* pSegmentInfo = GetSegmentInfo(nIndex);

	if(pSegmentInfo->m_headerInfo.m_nHTTPCode <= 0)
	{
		return VCE_INVALID;
	}
	else if(pSegmentInfo->m_headerInfo.m_nHTTPCode == 206)
	{
		if( (m_dlParam.m_nFileSize == pSegmentInfo->m_headerInfo.m_nContentRangeTotal)
			&& (pSegmentInfo->m_range.cx == pSegmentInfo->m_headerInfo.m_nContentRangeX)
			&& (pSegmentInfo->m_range.cy == pSegmentInfo->m_headerInfo.m_nContentRangeY) )
		{
			return VCE_OK;
		}
	}
	else if(pSegmentInfo->m_headerInfo.m_nHTTPCode == 200)
	{
		return VCE_200;
	}
	
	return VCE_OTHER;
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
	szLog.Format("(%d) - StopConnection: m_curl = 0x%08X", nIndex, pSegmentInfo->m_curl);
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

void CSegmentDownloader::GetTempFolder(CString& szTempFolder)
{
	szTempFolder.Format("%s\\%s_%d", SYS_OPTIONS()->m_szTempFolder, m_dlParam.m_szSaveToFileName, m_dlParam.m_nTaskID);
}
void CSegmentDownloader::VerifyTempFolderExist()
{
	CString szTempFolder;
	GetTempFolder(szTempFolder);

	BOOL bResult = ::PathFileExists(szTempFolder);
	if(!bResult)
	{
		bResult = ::CreateDirectory(szTempFolder, NULL);

		if(!bResult)
		{
			CString szLog = CGenericTools::LastErrorStr("CreateDirectory", szTempFolder);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
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