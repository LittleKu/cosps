// CNETDownloaderCreator.cpp: implementation of the CCNETDownloaderCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CNETDownloaderCreator.h"
#include "CommonUtils.h"
#include "HeaderDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME("CNTC")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCNETDownloaderCreator::CCNETDownloaderCreator()
{

}

CCNETDownloaderCreator::~CCNETDownloaderCreator()
{

}

CDownloader* CCNETDownloaderCreator::CreateDownloader(const CString& szContent, CDownloaderContext* pContext, 
													  CDownloadParam dlParam)
{
	int nStartIndex = szContent.Find("src:'");
	if(nStartIndex < 0)
	{
		return NULL;
	}
	
	int nEndIndex = szContent.Find('\'', nStartIndex + 5);
	if(nEndIndex < 0)
	{
		return NULL;
	}
	
	int nCount = (nEndIndex - (nStartIndex + 5));
	if(nCount <= 0)
	{
		return NULL;
	}
	
	CString szURL = szContent.Mid(nStartIndex + 5, nCount);
	
	CString szLog;
	szLog.Format("Task[%02d]: Real URL=%s", dlParam.m_nTaskID, szURL);
	LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLog)
		
	dlParam.m_szUrl = szURL;
	//File name
	BOOL bResult = CCommonUtils::ExtractFileName(dlParam.m_szUrl, dlParam.m_szFileName);
	if(!bResult || dlParam.m_szFileName.GetLength() >= 64 || dlParam.m_szFileName.IsEmpty())
	{
		dlParam.m_szFileName = "index.html";
	}
	
	CTaskInfo updateInfo;
	updateInfo.m_nTaskID = dlParam.m_nTaskID;
	updateInfo.mask = CTaskInfo::TIF_FILE_NAME;
	updateInfo.m_szFileName = dlParam.m_szFileName;
	::SendMessage(dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)dlParam.m_nTaskID, (LPARAM)&updateInfo);
	
	CHeaderDownloader* pDownloader = new CHeaderDownloader(pContext);
	pDownloader->Init(dlParam);
	
	return pDownloader;
}
