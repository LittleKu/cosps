// CommonUtils.cpp: implementation of the CCommonUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonUtils.h"
#include <curl/curl.h>
#include "GenericTools.h"
#include "wininet.h"
#include "StringUtils.h"

#pragma comment(lib, "wininet.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommonUtils::CCommonUtils()
{

}

CCommonUtils::~CCommonUtils()
{

}

void CCommonUtils::FormatErrorMsg(DWORD dwCode, CString& szErrorMsg)
{
	WORD nMajor = LOWORD(dwCode);
	switch(nMajor)
	{
	case RC_MAJOR_OK:
		{
			szErrorMsg = "OK";
		}
		break;
	case RC_MAJOR_PAUSED:
		{
			szErrorMsg = "Paused";
		}
		break;
	case RC_MAJOR_STOPPED:
		{
			szErrorMsg = "Stopped";
		}
		break;
	case RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR:
		{
			FormatInternalErrorMsg(HIWORD(dwCode), szErrorMsg);
		}
		break;
	case RC_MAJOR_TERMINATED_BY_CURL_CODE:
		{
			szErrorMsg.Format("[Transfer Error]: %d - %s", HIWORD(dwCode), curl_easy_strerror((CURLcode)HIWORD(dwCode)));
		}
		break;
	default:
		{
			szErrorMsg.Format("Unkonwn error - %d", dwCode);
		}
		break;
	}
}

void CCommonUtils::FormatInternalErrorMsg(int nCode, CString& szErrorMsg)
{
	switch(nCode)
	{
	case RC_MINOR_OK:
		{
			szErrorMsg = "[Internal Error]: no error";
		}
		break;
	case RC_MINOR_MULTI_FDSET_ERROR:
		{
			szErrorMsg = "[Internal Error]: multi_fdset error";
		}
		break;
	case RC_MINOR_MULTI_TIMEOUT_ERROR:
		{
			szErrorMsg = "[Internal Error]: multi_timout error";
		}
		break;
	case RC_MINOR_SELECT_ERROR:
		{
			szErrorMsg = "[Internal Error]: select error";
		}
		break;
	case RC_MINOR_RETRY_OVER_ERROR:
		{
			szErrorMsg = "[Internal Error]: retry over error";
		}
		break;
	default:
		{
			szErrorMsg.Format("[Internal Error]: Unkonwn error - %d", nCode);
		}
		break;
	}
}

void CCommonUtils::Split(CArray<CRange, CRange&>& sizeArray, UINT nFileSize, int nMinSegmentSize, int nMaxSegmentCount)
{
	int i;
	CRange segment;
	int nSegmentSize = nFileSize / nMaxSegmentCount;

	//Big segment
	if(nSegmentSize >= nMinSegmentSize)
	{
		for(i = 0; i < nMaxSegmentCount - 1; i++)
		{
			segment.cx = i * nSegmentSize;
			segment.cy = segment.cx + nSegmentSize - 1;
			sizeArray.Add(segment);
		}
		segment.cx = i * nSegmentSize;
		segment.cy = nFileSize - 1;
		sizeArray.Add(segment);
	}
	else
	{
		int nSegmentCount = nFileSize / nMinSegmentSize;
		if(nSegmentCount > 0)
		{
			for(i = 0; i < nSegmentCount; i++)
			{
				segment.cx = i * nMinSegmentSize;
				segment.cy = segment.cx + nMinSegmentSize - 1;
				sizeArray.Add(segment);
			}
			if(nFileSize % nMinSegmentSize != 0)
			{
				segment.cx = nSegmentCount * nMinSegmentSize;
				segment.cy = nFileSize - 1;
				sizeArray.Add(segment);
			}			
		}
		//total size is less than the min segment size, only 1 segment
		else
		{
			segment.cx = 0;
			segment.cy = nFileSize - 1;
			sizeArray.Add(segment);
		}		
	}
}

BOOL CCommonUtils::MergeFiles(const CStringArray& srcFileNames, LPCTSTR lpszDstFileName)
{
	ASSERT(srcFileNames.GetSize() > 0);
	CString szLog;

	FILE* lpDstFile = fopen(lpszDstFileName, "wb");
	if(lpDstFile == NULL)
	{
		szLog.Format("Failed to open dst file: %s", lpszDstFileName);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		return FALSE;
	}

	int i, nSize;
	FILE* lpSrcFile = NULL;

	size_t nRead, nWrite;

	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];

	CString srcFileName;
	for(i = 0, nSize = srcFileNames.GetSize(); i < nSize; i++)
	{
		srcFileName = srcFileNames.GetAt(i);

		lpSrcFile = fopen((LPCTSTR)srcFileName, "rb");
		if(lpSrcFile == NULL)
		{
			szLog.Format("Failed to open src file: %s", srcFileName);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
				
			fclose(lpDstFile);

			return FALSE;
		}

		while( (nRead = fread(buffer, 1, BUFFER_SIZE, lpSrcFile)) > 0 )
		{
			nWrite = fwrite(buffer, 1, nRead, lpDstFile);

			if(nWrite != nRead)
			{
				szLog.Format("Failed to write dst file: %s, %d, %d", lpszDstFileName, nWrite, nRead);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

				fclose(lpDstFile);
				fclose(lpSrcFile);

				return FALSE;
			}
		}
		fclose(lpSrcFile);
	}

	fclose(lpDstFile);

	return TRUE;
}

BOOL CCommonUtils::MergeFiles(LPCTSTR lpSrcFileName, int nSize)
{
	CStringArray srcFileNames;

	CString szFileName;
	for(int i = 0; i < nSize; i++)
	{
		szFileName.Format("%s_%d", lpSrcFileName, i);
		srcFileNames.Add(szFileName);
	}
	return MergeFiles(srcFileNames, lpSrcFileName);
}

BOOL CCommonUtils::DeleteFiles(const CStringArray& srcFileNames)
{
	int i, nSize;
	CString szFileName;
	for(i = 0, nSize = srcFileNames.GetSize(); i < nSize; i++)
	{
		szFileName = srcFileNames.GetAt(i);

		if(!DeleteFile(szFileName))
		{
			CString szLog;
			szLog.Format("Failed to delete file: %s", szFileName);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCommonUtils::DeleteFiles(LPCTSTR lpSrcFileName, int nSize)
{
	CStringArray srcFileNames;
	
	CString szFileName;
	for(int i = 0; i < nSize; i++)
	{
		szFileName.Format("%s_%d", lpSrcFileName, i);
		srcFileNames.Add(szFileName);
	}
	return DeleteFiles(srcFileNames);
}

BOOL CCommonUtils::GetProxyInfo(CString& szProxyInfo, int nServerType)
{
	CMapUInt2String proxyInfoMap;
	if(!GetProxyInfo(proxyInfoMap))
	{
		return FALSE;
	}

	//Is all the proxy using the same server?
	if(proxyInfoMap.Lookup(PROXY_SERVER_ALL, szProxyInfo))
	{
		return TRUE;
	}

	//No common proxy server, try to find the correct type server
	if(proxyInfoMap.Lookup(nServerType, szProxyInfo))
	{
		return TRUE;
	}
	szProxyInfo.Empty();
	return FALSE;
}

BOOL CCommonUtils::GetProxyInfo(CMapUInt2String& proxyInfoMap)
{
	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[2];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 2;
	List.dwOptionError = 0;
	List.pOptions = Option;
	
	CString szLog;
	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
	{
		szLog = CGenericTools::LastErrorStr("InternetQueryOption", "INTERNET_OPTION_PER_CONNECTION_OPTION");
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		return FALSE;
	}

	//check if proxy used
	if( (Option[0].Value.dwValue & PROXY_TYPE_PROXY) != PROXY_TYPE_PROXY )
	{
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("No Proxy used");
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}

		return FALSE;
	}

	if(Option[1].Value.pszValue == NULL)
	{
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("Null Proxy Info");
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
		
		return FALSE;
	}
	//Copy to local 
	CString szProxyServers = Option[1].Value.pszValue;
	//Free the space
	::GlobalFree(Option[1].Value.pszValue);

	szLog.Format("Raw proxy servers: %s", szProxyServers);
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

	if(szProxyServers.IsEmpty())
	{
		return FALSE;
	}
	
	//there is no "="
	if(szProxyServers.Find('=') < 0)
	{
		szLog.Format("Use same proxy servers for all protocols. Proxy=%s", szProxyServers);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		proxyInfoMap.SetAt(PROXY_SERVER_ALL, szProxyServers);
		return TRUE;
	}

	CStringList szServerList;
	CStringUtils::Split(szProxyServers, _T(';'), szServerList);

	CString szServerType, szServerAddress;
	int nIndex;

	POSITION pos = szServerList.GetHeadPosition();
	while(pos != NULL)
	{
		CString& szServer = szServerList.GetNext(pos);

		nIndex = szServer.Find('=');
		if(nIndex < 0)
		{
			//This should not happen
			szLog.Format("Invalid server: %s", szServer);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

			continue;
		}
		
		szServerType = szServer.Left(nIndex);
		szServerAddress = szServer.Mid(nIndex + 1);

		if(szServerType.Compare("http") == 0)
		{
			proxyInfoMap.SetAt(PROXY_SERVER_HTTP, szServerAddress);
		}
		else if(szServerType.Compare("https") == 0)
		{
			proxyInfoMap.SetAt(PROXY_SERVER_HTTPS, szServerAddress);
		}
		else if(szServerType.Compare("ftp") == 0)
		{
			proxyInfoMap.SetAt(PROXY_SERVER_FTP, szServerAddress);
		}
		else if(szServerType.Compare("socks") == 0)
		{
			proxyInfoMap.SetAt(PROXY_SERVER_SOCKS, szServerAddress);
		}
		else
		{
			szLog.Format("Unknown proxy server type: %s", szServer);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
	}

	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		POSITION pos = proxyInfoMap.GetStartPosition();
		UINT nServerType;
		CString szServerAddress;

		while(pos != NULL)
		{
			proxyInfoMap.GetNextAssoc( pos, nServerType, szServerAddress );

			szLog.Format("Proxy Server: %d=%s", nServerType, szServerAddress);
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
	}

	return TRUE;
}