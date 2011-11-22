// Downloader.cpp: implementation of the CDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestDownloader.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#  include <unistd.h>
#endif
#include <curl/multi.h>
#include "string_utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const char *urls[] = {
	"http://www.microsoft.com",
		"http://www.opensource.org",
		"http://www.google.com",
		"http://www.yahoo.com",
		"http://www.ibm.com",
		"http://www.mysql.com",
		"http://www.oracle.com",
		"http://www.ripe.net",
		"http://www.iana.org",
		"http://www.amazon.com",
		"http://www.netcraft.com",
		"http://www.heise.de",
		"http://www.chip.de",
		"http://www.ca.com",
		"http://www.cnet.com",
		"http://www.news.com",
		"http://www.cnn.com",
		"http://www.wikipedia.org",
		"http://www.dell.com",
		"http://www.hp.com",
		"http://www.cert.org",
		"http://www.mit.edu",
		"http://www.nist.gov",
		"http://www.ebay.com",
		"http://www.playstation.com",
		"http://www.uefa.com",
		"http://www.ieee.org",
		"http://www.apple.com",
		"http://www.sony.com",
		"http://www.symantec.com",
		"http://www.zdnet.com",
		"http://www.fujitsu.com",
		"http://www.supermicro.com",
		"http://www.hotmail.com",
		"http://www.ecma.com",
		"http://www.bbc.co.uk",
		"http://news.google.com",
		"http://www.foxnews.com",
		"http://www.msn.com",
		"http://www.wired.com",
		"http://www.sky.com",
		"http://www.usatoday.com",
		"http://www.cbs.com",
		"http://www.nbc.com",
		"http://slashdot.org",
		"http://www.bloglines.com",
		"http://www.techweb.com",
		"http://www.newslink.org",
		"http://www.un.org",
};

#define MAX 10 /* number of simultaneous transfers */
#define CNT sizeof(urls)/sizeof(char*) /* total number of transfers to do */

class ProgressData
{
public:
	HWND hwnd;
	int index;
};

class BodyData
{
public:
	HWND hwnd;
	int index;
	DWORD64 total;
	DWORD64 offset;
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
	
	::SendMessage(pBody->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pBody->index);

	return n*l;
}

int MyProgressCB(void *clientp,double dltotal,double dlnow,double ultotal,double ulnow)
{
	if((DWORD64)dltotal == 0)
	{
		return 0;
	}
	curl_off_t idtotal = (curl_off_t)dltotal;
	curl_off_t idnow = (curl_off_t)dlnow;
	
	//	printf("total=%I64d, now=%I64d\n", idtotal, idnow);

	ProgressData* pData = (ProgressData*)clientp;
	
	CProgressInfo progressInfo;
	progressInfo.dltotal = (DWORD64)dltotal;
	progressInfo.dlnow = (DWORD64)dlnow;
	progressInfo.ultotal = (DWORD64)ultotal;
	progressInfo.ulnow = (DWORD64)ulnow;

	if((DWORD64)dltotal == 0 && dlnow > dltotal)
	{
		AfxTrace("%d: Invalid progress. %g of %g\n", pData->index, dlnow, dltotal);
	}

	::SendMessage(pData->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pData->index);

	return 0;
}

static void init(CURLM *cm, int i, HWND hwnd)
{
	CURL *eh = curl_easy_init();
	
	BodyData* pBody = new BodyData();
	pBody->total = 0;
	pBody->offset = 0;
	pBody->hwnd = hwnd;
	pBody->index = i;

	curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(eh, CURLOPT_WRITEDATA, pBody);
	curl_easy_setopt(eh, CURLOPT_HEADERFUNCTION, hfcb);

	HeaderData* pHeader = new HeaderData();
	pHeader->pBody = pBody;
	char buf[10];
	sprintf(buf, "%d.txt", i);
	pHeader->fp = fopen(buf, "wb");
	pHeader->url = urls[i];
	curl_easy_setopt(eh, CURLOPT_HEADERDATA, pHeader);

	curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L);

//	curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
	curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
	curl_easy_setopt(eh, CURLOPT_PRIVATE, pHeader);
	curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(eh, CURLOPT_NOPROGRESS, 1L);
// 	curl_easy_setopt(eh, CURLOPT_PROGRESSFUNCTION, MyProgressCB);
// 
// 	ProgressData* pData = new ProgressData();
// 	pData->hwnd = hwnd;
// 	pData->index = i;
// 	curl_easy_setopt(eh, CURLOPT_PROGRESSDATA, (void*)pData);
	
	curl_multi_add_handle(cm, eh);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestDownloader::CTestDownloader()
{
	
}

CTestDownloader::~CTestDownloader()
{
	
}

int CTestDownloader::download(HWND hwnd)
{
	CURLM *cm;
	CURLMsg *msg;
	long L;
	unsigned int C=0;
	int M, Q, U = -1;
	fd_set R, W, E;
	struct timeval T;
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	cm = curl_multi_init();
	
	/* we can optionally limit the total amount of connections this multi handle
	uses */
	curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX);
	
	for (C = 0; C < MAX; ++C) {
		init(cm, C, hwnd);
	}
	
	while (U) {
		curl_multi_perform(cm, &U);
		
		if (U) {
			FD_ZERO(&R);
			FD_ZERO(&W);
			FD_ZERO(&E);
			
			if (curl_multi_fdset(cm, &R, &W, &E, &M)) {
				fprintf(stderr, "E: curl_multi_fdset\n");
				AfxTrace("E: curl_multi_fdset\n");
				return EXIT_FAILURE;
			}
			
			if (curl_multi_timeout(cm, &L)) {
				fprintf(stderr, "E: curl_multi_timeout\n");
				AfxTrace("E: curl_multi_timeout\n");
				return EXIT_FAILURE;
			}
			if (L == -1)
				L = 100;
			
			if (M == -1) {
#ifdef WIN32
				Sleep(L);
#else
				sleep(L / 1000);
#endif
			} else {
				T.tv_sec = L/1000;
				T.tv_usec = (L%1000)*1000;
				
				if (0 > select(M+1, &R, &W, &E, &T)) {
					fprintf(stderr, "E: select(%i,,,,%li): %i: %s\n",
						M+1, L, errno, strerror(errno));
					AfxTrace("E: select(%i,,,,%li): %i: %s\n",
						M+1, L, errno, strerror(errno));
					return EXIT_FAILURE;
				}
			}
		}
		
		while ((msg = curl_multi_info_read(cm, &Q))) {
			if (msg->msg == CURLMSG_DONE) {
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
				
				::SendMessage(pHeader->pBody->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pHeader->pBody->index);

				curl_multi_remove_handle(cm, e);
				curl_easy_cleanup(e);
			}
			else {
				HeaderData *pHeader;
				CURL *e = msg->easy_handle;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &pHeader);

				CString szReason;
				szReason.Format("E: CURLMsg (%d)", msg->msg);

				AfxTrace("%s\n", szReason);

				CProgressInfo progressInfo;
				
				::SendMessage(pHeader->pBody->hwnd, WM_DOWNLOAD_PROGRESS, (WPARAM)&progressInfo, pHeader->pBody->index);
			}
			if (C < CNT) {
				init(cm, C++, hwnd);
				U++; /* just to prevent it from remaining at 0 if there are more
                URLs to get */
			}
		}
	}
	
	curl_multi_cleanup(cm);
	curl_global_cleanup();
	
	return EXIT_SUCCESS;
}
