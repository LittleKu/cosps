// DownloaderMgr.cpp: implementation of the CDownloaderMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderMgr.h"
#include "HeaderParser.h"
#include "EasyDownloader.h"
#include "SegmentDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT CDownloaderMgr::StartDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	UINT nResult = pThis->StartDownload();

	return nResult;
}

UINT CDownloaderMgr::ResumeDownloadProc(LPVOID lpvData)
{
	CDownloaderMgr* pThis = (CDownloaderMgr*)lpvData;

	UINT nResult = pThis->ResumeDownload();

	return nResult;
}

CDownloaderMgr::CDownloaderMgr(const CDownloadParam& param) : m_dlParam(param), m_pDownloader(NULL)
{
	
}

CDownloaderMgr::~CDownloaderMgr()
{
	if(m_pDownloader != NULL)
	{
		delete m_pDownloader;
		m_pDownloader = NULL;
	}
}

void CDownloaderMgr::Start()
{
	AfxBeginThread(StartDownloadProc, (LPVOID)this);
}

void CDownloaderMgr::Resume()
{
	AfxBeginThread(ResumeDownloadProc, (LPVOID)this);
}

void CDownloaderMgr::Stop()
{
	ASSERT(m_pDownloader != NULL);
	m_pDownloader->Stop();
}
void CDownloaderMgr::Pause()
{
	ASSERT(m_pDownloader != NULL);
	m_pDownloader->Pause();
}

UINT CDownloaderMgr::StartDownload()
{
	CHeaderParser headerParser(m_dlParam.m_szUrl);
	CHeaderInfo* pHeaderInfo = headerParser.GetHeaderInfo();
	
	if(pHeaderInfo->httpcode == 200 && pHeaderInfo->header_size > 0/* && pHeaderInfo->is_range_bytes*/)
	{
		m_dlParam.m_nFileSize = pHeaderInfo->header_size;
		
		m_pDownloader = new CSegmentDownloader();
	}
	else
	{
		m_dlParam.m_nFileSize = 0;
		
		m_pDownloader = new CEasyDownloader();
	}	
	
	m_pDownloader->Init(m_dlParam);
	
	clock_t start = clock();
	
	m_pDownloader->Start();
	
	clock_t finish = clock();
	UINT nCost = finish - start;
	
	CString szLog;
	szLog.Format("Time Cost (%d)", nCost);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
	return 0;
}

UINT CDownloaderMgr::ResumeDownload()
{
	ASSERT(m_pDownloader != NULL);
	
	clock_t start = clock();
	
	m_pDownloader->Resume();
	
	clock_t finish = clock();
	UINT nCost = finish - start;
	
	CString szLog;
	szLog.Format("Time Cost (%d)", nCost);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		
	return 0;
}