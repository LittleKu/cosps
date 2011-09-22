#include "stdafx.h"
#include "easy_down.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#  include <unistd.h>
#endif
#include <curl/multi.h>
#include "string_utils.h"
#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


class ProgressData
{
public:
	HWND hwnd;
	int index;
	CEasyDown* ptr;
	CURL *eh;
};

class BodyData
{
public:
	HWND hwnd;
	int index;
	FILE* fp;
	DWORD64 total;
	DWORD64 offset;
	CEasyDown* ptr;
};

class HeaderData
{
public:
	BodyData* pBody;
	FILE* fp;
	const char* url;
};

static size_t hfcb(char *d, size_t n, size_t l, void *p)
{
	/* take care of the data here, ignored in this example */
	(void)d;
	HeaderData* pHeader = (HeaderData*)p;
	
	
	std::string line((char*)d, n*l);
	std::string content_length;

	if(start_with(line, "HTTP/"))
	{
		pHeader->pBody->total = 0;
	}

	do 
	{
		if(find_between(line, "Content-Length:", "\r\n", content_length) == 0)
		{
			break;
		}
		trim(content_length);
		DWORD64 len = (DWORD64)_atoi64(content_length.c_str());
		if(len == 0)
		{
			break;
		}
		
		pHeader->pBody->total = len;
	} while (0);

	if(pHeader->fp)
	{
		fwrite(d, n, l, pHeader->fp);
	}

	if(start_with(line, "\r\n"))
	{
		fclose(pHeader->fp);
	}


	return n*l;
}

static size_t cb(char *d, size_t n, size_t l, void *p)
{
	/* take care of the data here, ignored in this example */
	(void)d;
	BodyData* pBody = (BodyData*)p;

	pBody->offset += n * l;


	CProgressInfo progressInfo;
	progressInfo.dltotal = pBody->total;
	progressInfo.dlnow = pBody->offset;

	if(pBody->fp)
	{
		fwrite(d, n, l, pBody->fp);
	}
	
	::SendMessage(pBody->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pBody->index);

// 	if(pBody->ptr->m_pControlInfo->isModified && pBody->ptr->m_pControlInfo->isPaused)
// 	{
// 		pBody->ptr->m_pControlInfo->isModified = FALSE;
// 		return CURL_WRITEFUNC_PAUSE;
// 	}

	return n*l;
}

int MyEasyProgressCB(void *clientp,double dltotal,double dlnow,double ultotal,double ulnow)
{
// 	if((DWORD64)dltotal == 0)
// 	{
// 		return 0;
// 	}
	curl_off_t idtotal = (curl_off_t)dltotal;
	curl_off_t idnow = (curl_off_t)dlnow;
	
	//	printf("total=%I64d, now=%I64d\n", idtotal, idnow);

	ProgressData* pData = (ProgressData*)clientp;
	
	CProgressInfo progressInfo;
	progressInfo.dltotal = (DWORD64)dltotal;
	progressInfo.dlnow = (DWORD64)dlnow;
	progressInfo.ultotal = (DWORD64)ultotal;
	progressInfo.ulnow = (DWORD64)ulnow;
	progressInfo.retCode = -1;
	progressInfo.szReason = "";

	if((DWORD64)dltotal == 0 && dlnow > dltotal)
	{
		AfxTrace("%d: Invalid progress. %g of %g\n", pData->index, dlnow, dltotal);
	}

//	::SendMessage(pData->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pData->index);

	if(pData->ptr->m_pControlInfo->isModified && pData->ptr->m_pControlInfo->isPaused == TRUE)
	{
		pData->ptr->m_pControlInfo->isModified = FALSE;
		
		curl_easy_pause(pData->eh, CURLPAUSE_ALL);
	}
	if(pData->ptr->m_pControlInfo->isModified && pData->ptr->m_pControlInfo->isPaused == FALSE)
	{
		pData->ptr->m_pControlInfo->isModified = FALSE;

		curl_easy_pause(pData->eh, CURLPAUSE_CONT);
	}

	if(pData->ptr->m_pControlInfo->isStopped)
	{
		return 5;
	}

	return 0;
}

static CURL* init(const char* url, int i, HWND hwnd, CEasyDown* ptr)
{
	CURL *eh = curl_easy_init();

	
	BodyData* pBody = new BodyData();
	pBody->total = 0;
	pBody->offset = 0;
	pBody->hwnd = hwnd;
	pBody->index = i;
	pBody->ptr = ptr;
	pBody->fp = fopen("data", "wb");

	curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(eh, CURLOPT_WRITEDATA, pBody);
	curl_easy_setopt(eh, CURLOPT_HEADERFUNCTION, hfcb);

	HeaderData* pHeader = new HeaderData();
	pHeader->pBody = pBody;
	char buf[10];
	sprintf(buf, "%d.txt", i);
	pHeader->fp = fopen(buf, "wb");
	pHeader->url = url;
	curl_easy_setopt(eh, CURLOPT_HEADERDATA, pHeader);

	curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L);

//	curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
	curl_easy_setopt(eh, CURLOPT_URL, url);
	curl_easy_setopt(eh, CURLOPT_PRIVATE, pBody);
	curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(eh, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(eh, CURLOPT_PROGRESSFUNCTION, MyEasyProgressCB);

	ProgressData* pData = new ProgressData();
	pData->hwnd = hwnd;
	pData->index = i;
	pData->eh = eh;
	pData->ptr = ptr;
	curl_easy_setopt(eh, CURLOPT_PROGRESSDATA, (void*)pData);

	return eh;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEasyDown::CEasyDown()
{
	m_pControlInfo = NULL;
}

CEasyDown::~CEasyDown()
{
	
}

int CEasyDown::download(const char* url, HWND hwnd, CControlInfo* pControlInfo)
{	
	m_pControlInfo = pControlInfo;
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* eh = init(url, 0, hwnd, this);
	
	CURLcode retCode = curl_easy_perform(eh);

	AfxTrace("%d\n", retCode);

	char* pPrivateData = NULL;
	curl_easy_getinfo(eh, CURLINFO_PRIVATE, &pPrivateData);
	BodyData* pBody = (BodyData*)pPrivateData;
	if(pBody->fp)
	{
		fclose(pBody->fp);
	}

	curl_easy_cleanup(eh);

	curl_global_cleanup();
	
	return EXIT_SUCCESS;
}
