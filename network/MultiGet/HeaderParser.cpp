// HeaderParser.cpp: implementation of the CHeaderParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeaderParser.h"

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
			pHeaderInfo->httpcode = nRspCode;
			break;
		}

		
		//2. check if Content-Length line
		int nContentLength = -1;
		sscanf(scratch, "Content-Length: %d", &nContentLength);
		if(nContentLength >= 0)
		{
			pHeaderInfo->header_size = nContentLength;
			break;
		}

		//3. check if Accept-Ranges line
		char buf[32];
		int nc = sscanf(scratch, "Accept-Ranges: %s", buf);
		if(nc > 0)
		{
			if(strcmp("bytes", buf) == 0)
			{
				pHeaderInfo->is_range_bytes = true;
			}
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

	if(CURLE_OK == res)
	{
	}
	else
	{
		AfxTrace("Failed to GET header of %s: curl error code = %d\n", url, res);
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
