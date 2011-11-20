// HeaderDownloader.cpp: implementation of the CHeaderDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeaderDownloader.h"
#include "CommonUtils.h"
#include "SegmentDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CHeaderDownloader::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CHeaderDownloader* lpDownloader = (CHeaderDownloader*)clientp;
	return lpDownloader->ProcessProgress(dltotal, dlnow, ultotal, ulnow);
}
size_t CHeaderDownloader::HeaderCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	CHeaderDownloader* lpDownloader = (CHeaderDownloader*)userdata;
	return lpDownloader->ProcessHeader(ptr, size, nmemb);
}

CHeaderDownloader::CHeaderDownloader(CDownloaderContext* pContext)
 : m_pContext(pContext), m_curl(NULL), m_pNext(NULL)
{

}

CHeaderDownloader::~CHeaderDownloader()
{

}

void CHeaderDownloader::Init(const CDownloadParam& param)
{
	m_dlParam = param;
}
int CHeaderDownloader::Start()
{
	return DoProcess();
}

CDownloader* CHeaderDownloader::GetNext()
{
	return m_pNext;
}


int CHeaderDownloader::ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow)
{
	int rc = 0;
	CString szLog;

	DWORD dwState = m_pContext->GetState();
	if(dwState == TSE_PAUSING)
	{
		rc = 1;

		szLog.Format("[CHeaderDownloader::ProcessProgress]: Paused");
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	else if(dwState == TSE_DESTROYING)
	{
		rc = 2;

		szLog.Format("[CHeaderDownloader::ProcessProgress]: Destroyed");
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	
	return rc;
}

size_t CHeaderDownloader::ProcessHeader(void *ptr, size_t size, size_t nmemb)
{
	if( IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine((char*)ptr, size * nmemb);
		CCommonUtils::ReplaceCRLF(szLine);
		
		CString szMsg;
		szMsg.Format("Task[%02d]: Header - %s", m_dlParam.m_nTaskID, szLine);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szMsg)
	}
	
	CString szScratch((char*)ptr, size * nmemb);
	int nc;
	do 
	{
		//1. check if status line
		int nRspCode = CCommonUtils::GetHTTPStatusCode(szScratch);
		
		//This is a status line
		if(nRspCode > 0)
		{
			m_headerInfo.Reset();
			m_headerInfo.m_nHTTPCode = nRspCode;
			m_headerInfo.m_szStatusLine = szScratch;
			CCommonUtils::ReplaceCRLF(m_headerInfo.m_szStatusLine, NULL, NULL);
			break;
		}
		
		
		//2. check if Content-Length line
		int nContentLength = -1;
		nc = sscanf(szScratch, "Content-Length: %d", &nContentLength);
		if(nc > 0)
		{
			m_headerInfo.m_nContentLength = nContentLength;
			break;
		}
		
		//3. check if Accept-Ranges line
		char buf[128];
		nc = sscanf(szScratch, "Accept-Ranges: %s", buf);
		if(nc > 0)
		{
			if(strcmp("bytes", buf) == 0)
			{
				m_headerInfo.m_bRangeBytes = TRUE;
			}
			break;
		}
		
		//4. check if Content-Type
		nc = sscanf(szScratch, "Content-Type: %s", buf);
		if(nc > 0)
		{
			m_headerInfo.m_szContentType = buf;
			break;
		}
		
		//5. check if Content-Range
		int x, y, total;
		nc = sscanf(szScratch, "Content-Range: bytes %d-%d/%d", &x, &y, &total);
		if(nc == 3)
		{
			m_headerInfo.m_nContentRangeTotal = total;
			break;
		}		
	} while ( FALSE );
	
	return (size_t)(size * nmemb);
}

int CHeaderDownloader::PostProcess(CURLcode res)
{
	int nResult;
	CString szLog;

	DWORD dwState;
	m_pContext->Lock();

	_ASSERTE(_CrtCheckMemory());

	dwState = m_pContext->NoLockGetState();
	szLog.Format("Task[%02d] [CHeaderDownloader::PostProcess] state = %d, curl result = %d", 
		m_dlParam.m_nTaskID, dwState, (int)res);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	_ASSERTE(_CrtCheckMemory());

	if(dwState == TSE_PAUSING)
	{
		m_pContext->NoLockSetState(TSE_PAUSED);		
	}
	else if(dwState == TSE_DESTROYING)
	{
		m_pContext->NoLockSetState(TSE_DESTROYED);
	}
	else if(dwState == TSE_TRANSFERRING)
	{
		switch(res)
		{
		case CURLE_OK:
			{
				//Generate next downloader
				GenerateNextDownloader();
			}
			break;
		case CURLE_ABORTED_BY_CALLBACK:
			{
				szLog.Format("Task[%02d]: Unexpected state when callback aborted. TSE_TRANSFERRING", 
					m_dlParam.m_nTaskID);
				LOG4CPLUS_FATAL_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				ASSERT(FALSE);
			}
			break;
		default:
			{
				CString szDetail;
				szDetail.Format("(%d) %s", res, curl_easy_strerror(res));
				m_pContext->NoLockSetState(TSE_END_WITH_ERROR, szDetail);
			}
			break;
		}
	}
	else
	{
		szLog.Format("Task[%02d]: Unexpected state = %d", m_dlParam.m_nTaskID, dwState);
		LOG4CPLUS_FATAL_STR(ROOT_LOGGER, (LPCTSTR)szLog)
			
		ASSERT(FALSE);
	}
	
	nResult = m_pContext->NoLockGetState();
	m_pContext->Unlock();

	return nResult;
}

void CHeaderDownloader::GenerateNextDownloader()
{
	ASSERT(m_pNext == NULL);
	m_pNext = NULL;
	CString szLog;
	do 
	{
		//1. HTTP return error
		if(m_headerInfo.m_nHTTPCode != 200 && m_headerInfo.m_nHTTPCode != 206 && m_headerInfo.m_nHTTPCode != 416)
		{
			m_pContext->NoLockSetState(TSE_END_WITH_ERROR, m_headerInfo.m_szStatusLine);
			break;
		}

		//2. Error if there's no valid content length field when HTTP response is 416
		if(m_headerInfo.m_nHTTPCode == 416 && m_headerInfo.m_nContentLength <= 0)
		{
			m_pContext->NoLockSetState(TSE_END_WITH_ERROR, m_headerInfo.m_szStatusLine);
			break;
		}

		szLog.Format("Task[%02d]: HTTPCode=%d, Content-Range(Total)=%d, Content-Length=%d", m_dlParam.m_nTaskID,
			m_headerInfo.m_nHTTPCode, m_headerInfo.m_nContentRangeTotal, m_headerInfo.m_nContentLength);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		if(m_headerInfo.m_nHTTPCode == 206)
		{
			m_dlParam.m_nFileSize = m_headerInfo.m_nContentRangeTotal;

			m_pNext = new CSegmentDownloader(m_pContext);
		}
		else if(m_headerInfo.m_nHTTPCode == 200 || m_headerInfo.m_nHTTPCode == 416)
		{
			m_dlParam.m_nFileSize = m_headerInfo.m_nContentLength;

			//When file size is bigger than 5M, try to segment download
			if(m_headerInfo.m_nContentLength > (5 * 1024 * 1024))
			{
				m_pNext = new CSegmentDownloader(m_pContext);
			}
			else
			{
				m_pNext = new CSegmentDownloader(m_pContext);
			}
		}

		ASSERT(m_pNext != NULL);
		m_pNext->Init(m_dlParam);
	} while (FALSE);
}

int CHeaderDownloader::DoProcess()
{
	CString szLog;
	CString szRange;
	CURLcode res = CURLE_OK;

	/* init the curl session */
	m_curl = curl_easy_init();
	if(m_curl == NULL)
	{
		m_pContext->SetState(TSE_END_WITH_ERROR);
		return TSE_END_WITH_ERROR;
	}
	
	curl_easy_setopt(m_curl, CURLOPT_URL, (LPCTSTR)(m_dlParam.m_szUrl));
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(m_curl, CURLOPT_NOBODY, 1L);
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, USER_AGENT_IE8);
	
	//connect timeout: 10s
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 10L);
	
	//low speed limit
	curl_easy_setopt(m_curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(m_curl, CURLOPT_LOW_SPEED_TIME, 15L);
	
	
	szRange.Format("%d-%d", 0, 0);
	curl_easy_setopt(m_curl, CURLOPT_RANGE, (LPCTSTR)szRange);
	
	//Proxy setting
	if(SYS_OPTIONS()->GetInstance()->GetProxy() != NULL)
	{
		curl_easy_setopt(m_curl, CURLOPT_PROXY, SYS_OPTIONS()->GetInstance()->GetProxy());
	}
	
	//Throw away body if it exists
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, ThrowAwayCallback);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, NULL);
	
	curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
	curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, this);	
	
	
	curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
	
	res = curl_easy_perform(m_curl);

	m_headerInfo.m_nCurlResult = res;
	szLog.Format("Task[%02d]: HeaderDownloader result: %d - %s", m_dlParam.m_nTaskID, res, curl_easy_strerror(res));
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	/* always cleanup */
    curl_easy_cleanup(m_curl);
	m_curl = NULL;

	return PostProcess(res);
}