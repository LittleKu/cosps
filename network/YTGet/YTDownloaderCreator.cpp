// YTDownloaderCreator.cpp: implementation of the CYTDownloaderCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YTDownloaderCreator.h"
#include "CommonUtils.h"
#include "HeaderDownloader.h"
#include "StringUtils.h"
#include <vector>
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME("YTDC")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CYTDownloaderCreator::CURLInfo::operator<(const CURLInfo& other)
{
	int nThisIndex, nOtherIndex;

	nThisIndex = SYS_OPTIONS()->GetQualityIndex(quality);
	nOtherIndex = SYS_OPTIONS()->GetQualityIndex(other.quality);
	if(nThisIndex != nOtherIndex)
	{
		return nThisIndex < nOtherIndex;
	}

	nThisIndex = SYS_OPTIONS()->GetFormatIndex(format);
	nOtherIndex = SYS_OPTIONS()->GetFormatIndex(other.format);
	if(nThisIndex != nOtherIndex)
	{
		return nThisIndex < nOtherIndex;
	}

	return true;
}

CYTDownloaderCreator::CYTDownloaderCreator()
{

}

CYTDownloaderCreator::~CYTDownloaderCreator()
{

}

CDownloader* CYTDownloaderCreator::CreateDownloader(const CString& szContent, CDownloaderContext* pContext, 
	CDownloadParam dlParam)
{
	//Parse
	BOOL bResult = ParseHTMLFile(szContent, dlParam);
	if(!bResult)
	{
		return NULL;
	}

	//Update file name and detail information
	CTaskInfo updateInfo;
	updateInfo.m_nTaskID = dlParam.m_nTaskID;
	updateInfo.mask = CTaskInfo::TIF_FILE_NAME | CTaskInfo::TIF_DETAIL;
	updateInfo.m_szFileName = dlParam.m_szFileName;
	updateInfo.m_dlState.m_szDetail = "";
	::SendMessage(dlParam.m_hWnd, WM_DOWNLOAD_PROGRESS, (WPARAM)dlParam.m_nTaskID, (LPARAM)&updateInfo);
	
	//Create next downloader
	CHeaderDownloader* pDownloader = new CHeaderDownloader(pContext);
	pDownloader->Init(dlParam);
	
	return pDownloader;
}

BOOL CYTDownloaderCreator::ParseHTMLFile(const CString& szContent, CDownloadParam& dlParam)
{
	BOOL bResult;

	//Title
	CString szTitle;
	bResult = CStringUtils::FindBetween(szTitle, szContent, _T("<title>"), _T("</title>"));
	if(!bResult)
	{
		return FALSE;
	}
	TrimTitle(szTitle);

	//URLs
	CString szURLMap;
	bResult = CStringUtils::FindBetween(szURLMap, szContent, _T("url_encoded_fmt_stream_map="), _T("&amp;"));
	if(!bResult)
	{
		return FALSE;
	}

	CStringList szUrlList;
	bResult = CStringUtils::Split(szURLMap, "%2C", szUrlList);

	CString szLog;

	std::vector<CURLInfo> urlInfoList;
	POSITION pos = szUrlList.GetHeadPosition();
	while(pos != NULL)
	{
		CString& szUrl = szUrlList.GetNext(pos);

		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Raw ULR=%s", szUrl);
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}

		CCommonUtils::Unescape(szUrl);

		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Unescaped ULR=%s", szUrl);
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}
		
		CURLInfo url_info;
		URLUnescaped2URLInfo(szUrl, url_info);
		
		urlInfoList.push_back(url_info);
	}

	if(urlInfoList.empty())
	{
		return FALSE;
	}

	//log
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		for(int i = 0; i < urlInfoList.size(); i++)
		{
			szLog.Format("(%d) - Title=%s, URL=%s", i, szTitle, urlInfoList[i].url);
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
		}
	}

	std::sort(urlInfoList.begin(), urlInfoList.end());
	CURLInfo& target = urlInfoList[0];

	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		szLog.Format("Task[%02d]: select the url with tag=%s", dlParam.m_nTaskID, target.itag);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
	}

	dlParam.m_szFileName.Format("%s%s%s", szTitle, ".", target.format);
	dlParam.m_szUrl = target.url;

	return TRUE;
}

BOOL CYTDownloaderCreator::URLUnescaped2URLInfo(const CString& szURL, CURLInfo& urlInfo)
{
	if(!CStringUtils::StartWith(szURL, "url="))
	{
		return FALSE;
	}

	int nIndex;
	nIndex = szURL.Find("&quality=");
	if(nIndex < 0)
	{
		return FALSE;
	}

	int nPrefixLen = strlen("url=");
	urlInfo.url = szURL.Mid(nPrefixLen, nIndex - nPrefixLen);

	CString remaining = szURL.Mid(nIndex + 1);
	
	CStringList proper_list;
	if(CStringUtils::Split(remaining, "&", proper_list) == FALSE)
	{
		return FALSE;
	}
	
	CMapStringToString proper_map;

	POSITION pos = proper_list.GetHeadPosition();
	while(pos != NULL)
	{
		CString& szProp = proper_list.GetNext(pos);
		CStringUtils::SplitPair(szProp, "=", proper_map);
	}
	
	urlInfo.quality = proper_map["quality"];
	urlInfo.fallback_host = proper_map["fallback_host"];
	urlInfo.type = proper_map["type"];
	GetFileExt(urlInfo.type, urlInfo.format);

	urlInfo.itag = proper_map["itag"];
	return TRUE;
}

BOOL CYTDownloaderCreator::GetFileExt(const CString& szType, CString& szFileExt)
{
	BOOL bResult = TRUE;

	if(CStringUtils::StartWith(szType, "video/webm"))
	{
		szFileExt = "webm";
	}
	else if(CStringUtils::StartWith(szType, "video/x-flv"))
	{
		szFileExt = "flv";
	}
	else if(CStringUtils::StartWith(szType, "video/mp4"))
	{
		szFileExt = "mp4";
	}
	else
	{
		bResult = FALSE;
		szFileExt = "flv";
	}
	return bResult;
}

void CYTDownloaderCreator::TrimTitle(CString& szTitle)
{
	//Convert UTF-8 to ANSI
	LPSTR lpDst = NULL;
	int cbByteDst = 0;
	BOOL bResult = CStringUtils::MultiByteToMultiByte((LPCTSTR)szTitle, szTitle.GetLength(), CP_UTF8, 
		&lpDst, &cbByteDst, CP_ACP, 0);	
	if(bResult)
	{
		ASSERT(strlen(lpDst) == cbByteDst);
		
		szTitle = lpDst;
		
		delete lpDst;
		lpDst = NULL;
	}

	szTitle.TrimLeft(" \t\r\n");
	szTitle.TrimRight(" \t\r\n");
	
	int nIndex;
	nIndex = szTitle.FindOneOf("\r\n");
	if(nIndex >= 0)
	{
		szTitle = szTitle.Left(nIndex);
	}

	CCommonUtils::DecodeSpecialChars(szTitle);
	szTitle = CCommonUtils::StripInvalidFilenameChars(szTitle);
}