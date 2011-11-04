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

CDownloader::CDownloader() : m_statusChecker(TSE_READY)
{
}

CDownloader::~CDownloader()
{
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

	if(m_statusChecker.GetCurrentStatus() != TSE_INVALID)
	{
		CCommonUtils::SendStatusMsg(m_dlParam.m_hWnd, m_statusChecker.GetCurrentStatus(), lpszDetail);
	}
}
