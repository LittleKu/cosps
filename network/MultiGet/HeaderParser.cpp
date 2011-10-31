// HeaderParser.cpp: implementation of the CHeaderParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeaderParser.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int GetStatusCode(const char* buffer)
{
	int nc = 0;
	int http_version_major = 0, http_version_minor = 0, rsp_code = 0;
	
	do 
	{
		//try to parse rsp_code line
		nc = sscanf(buffer, " HTTP/%d.%d %3d", &http_version_major, &http_version_minor, &rsp_code);
		if(nc <= 0)
		{
			break;
		}
		
		if(nc != 3)
		{
			/* this is the real world, not a Nirvana
			NCSA 1.5.x returns this crap when asked for HTTP/1.1
			*/
			nc = sscanf(buffer, " HTTP %3d", &rsp_code);
			if(nc <= 0)
			{
				break;
			}
		}
	} while ( 0 );
	
	if(nc <= 0)
	{
		rsp_code = 0;
	}
	
	return rsp_code;
}

static int GetContentLength(const char* buffer)
{
	int nLen = -1;
	sscanf(buffer, "Content-Length: %d", &nLen);
	return nLen;
}


static size_t HeaderParserCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	if( IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
	{
		CString szLine((char*)ptr, size * nmemb);
		CCommonUtils::ReplaceCRLF(szLine);
		
		CString szMsg;
		szMsg.Format("HP - %s", szLine);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szMsg)
	}
	CHeaderInfo* pHeaderInfo = (CHeaderInfo*)data;

#define SCRATCHSIZE 127
	char scratch[SCRATCHSIZE+1];
	int nLineLen = min(SCRATCHSIZE, (size * nmemb));
	strncpy(scratch, (char*)ptr, nLineLen);
	scratch[nLineLen] = 0; /* null terminate */

	do 
	{
		//1. check if status line
		int nRspCode = GetStatusCode(scratch);

		//This is a status line
		if(nRspCode > 0)
		{
			pHeaderInfo->Reset();
			pHeaderInfo->m_nHTTPCode = nRspCode;
			pHeaderInfo->m_szStatusLine = scratch;
			CCommonUtils::ReplaceCRLF(pHeaderInfo->m_szStatusLine, NULL, NULL);
			break;
		}

		
		//2. check if Content-Length line
		int nContentLength = -1;
		sscanf(scratch, "Content-Length: %d", &nContentLength);
		if(nContentLength >= 0)
		{
			pHeaderInfo->m_nContentLength = nContentLength;
			break;
		}

		//3. check if Accept-Ranges line
		char buf[128];
		int nc = sscanf(scratch, "Accept-Ranges: %s", buf);
		if(nc > 0)
		{
			if(strcmp("bytes", buf) == 0)
			{
				pHeaderInfo->m_bRangeBytes = TRUE;
			}
			break;
		}

		//4. check if Content-Type
		nc = sscanf(scratch, "Content-Type: %s", buf);
		if(nc > 0)
		{
			pHeaderInfo->m_szContentType = buf;
			break;
		}

		//Do nothing now

	} while ( 0 );


	return (size_t)(size * nmemb);
}

CHeaderParser::CHeaderParser(const char* url) : m_curl(NULL)
{
	/* init the curl session */
	m_curl = curl_easy_init();
	if(m_curl == NULL)
	{
		return;
	}
	
	curl_easy_setopt(m_curl, CURLOPT_URL, url);
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(m_curl, CURLOPT_NOBODY, 1L);
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, USER_AGENT_IE8);

	if(SYS_OPTIONS()->GetInstance()->m_nProxyMode == PME_SYS_PROXY ||SYS_OPTIONS()->GetInstance()->m_nProxyMode == PME_USER_PROXY)
	{
		if(!SYS_OPTIONS()->GetInstance()->m_szProxy.IsEmpty())
		{
			curl_easy_setopt(m_curl, CURLOPT_PROXY, (LPCTSTR)SYS_OPTIONS()->GetInstance()->m_szProxy);
		}
	}

	//Throw away body if it exists
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, ThrowAwayCallback);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, NULL);
	
	curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, &m_headerInfo);	
	curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, HeaderParserCallback);

	CURLcode res = curl_easy_perform(m_curl);

	m_headerInfo.m_nCurlResult = res;

	if(res != CURLE_OK)
	{
		CString szLog;
		szLog.Format("Failed to GET header. curl code: %d - %s, url = %s", res, 
			curl_easy_strerror(res), url);

		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	/* always cleanup */
    curl_easy_cleanup(m_curl);
}

CHeaderParser::~CHeaderParser()
{
}

CHeaderInfo* CHeaderParser::GetHeaderInfo()
{
	return &m_headerInfo;
}
