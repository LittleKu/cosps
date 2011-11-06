// Downloader.cpp: implementation of the CDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Downloader.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloader::CDownloader() : m_statusChecker(TSE_READY), m_hStopEvent(NULL)
{
	//Init no-signal, auto event
	m_hStopEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CDownloader::~CDownloader()
{
	if(m_hStopEvent)
	{
		::CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
}

void CDownloader::Init(const CDownloadParam& param)
{
	m_dlParam = param;
}

UINT CDownloader::GetCurrentStatus()
{
	return m_statusChecker.GetCurrentStatus();
}

void CDownloader::CurrentStatusChanged(UINT nNewStatus, LPCTSTR lpszDetail)
{
//	ASSERT(m_statusChecker.GetCurrentStatus() != nNewStatus);
	m_statusChecker.SetCurrentStatus(nNewStatus);
	::SetEvent(m_hStopEvent);

	CStatusInfo statusInfo;
	statusInfo.m_nStatusCode = nNewStatus;
	statusInfo.m_szDetail = lpszDetail;

	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_STATUS, m_dlParam.m_nTaskID, (LPARAM)(&statusInfo));
}

void CDownloader::TaskFinished(DWORD dwResult)
{
	m_statusChecker.SetCurrentStatus(CCommonUtils::ResultCode2StatusCode(dwResult));
	::SetEvent(m_hStopEvent);

	CCommonUtils::SendMessage(m_dlParam.m_hWnd, WM_DOWNLOAD_COMPLETE, m_dlParam.m_nTaskID, (LPARAM)dwResult);
}

void CDownloader::WaitUtilStop()
{
	CString szLog;
	DWORD dwResult;
	UINT nStatus;
	while( (nStatus = GetCurrentStatus()) == TSE_TRANSFERRING )
	{
		Destroy();
		
		szLog.Format("Task [%d] is transferring, wait until it stopped.", m_dlParam.m_nTaskID);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		dwResult = ::WaitForSingleObject(m_hStopEvent, INFINITE);

		szLog.Format("Task [%d] WaitForSingleObject returned 0x%08X", m_dlParam.m_nTaskID, dwResult);
		LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}

	szLog.Format("Task [%d] Stopped succesfully. Current status=%d", m_dlParam.m_nTaskID, nStatus);
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
}
