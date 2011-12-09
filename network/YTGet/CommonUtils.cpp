// CommonUtils.cpp: implementation of the CCommonUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonUtils.h"
#include <wininet.h>
#include <curl/curl.h>
#include "GenericTools.h"
#include "StringUtils.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
long CCommonUtils::m_nUniqueID = 0;

CCommonUtils::CCommonUtils()
{

}

CCommonUtils::~CCommonUtils()
{

}
void CCommonUtils::State2Str(CString& szMsg, DWORD dwState, LPCTSTR lpszDetail, BOOL bWithValue)
{
	CString szState, szStateDetail;

	//1. State code to string
	switch(dwState)
	{
	case TSE_READY:
		{
			szState = "Ready";
		}
		break;
	case TSE_COMPLETE:
		{
			szState = "Complete";
		}
		break;
	case TSE_END_WITH_ERROR:
		{
			szState = "End with error";
		}
		break;
	case TSE_PAUSED:
		{
			szState = "Paused";
		}
		break;
	case TSE_DESTROYED:
		{
			szState = "Destroyed";
		}
		break;
	case TSE_TRANSFERRING:
		{
			szState = "Transferring";
		}
		break;
	case TSE_PAUSING:
		{
			szState = "Pausing";
		}
		break;
	case TSE_DESTROYING:
		{
			szState = "Destroying";
		}
		break;
	default:
		{
			szState.Format("Unknown status code: %d", dwState);
		}
		break;
	}

	//2. State with details
	//Not empty
	if(lpszDetail != NULL && (*lpszDetail) != _T('\0'))
	{
		szStateDetail.Format("%s: %s", szState, lpszDetail);
	}
	else
	{
		szStateDetail = szState;
	}

	//3. Prepends state code
	if(bWithValue)
	{
		szMsg.Format("%d - %s", dwState, szStateDetail);
	}
	else
	{
		szMsg = szStateDetail;
	}
}

CString CCommonUtils::State2Str(DWORD dwState)
{
	CString szResult;
	State2Str(szResult, dwState, NULL, TRUE);

	return szResult;
}

CString CCommonUtils::CurlCode2Str(DWORD dwCurlCode)
{
	CString szStr;
	szStr.Format("(%d)%s", dwCurlCode, curl_easy_strerror((CURLcode)dwCurlCode));

	return szStr;
}

void CCommonUtils::ReplaceCRLF(CString& szStr, LPCTSTR lpCR, LPCTSTR lpLF)
{
	szStr.Replace(_T("\r"), lpCR);
	szStr.Replace(_T("\n"), lpLF);
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

		if(Option[1].Value.pszValue != NULL)
		{
			//Free the space
			::GlobalFree(Option[1].Value.pszValue);
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

int CCommonUtils::GetHTTPStatusCode(const char* buffer)
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

BOOL CCommonUtils::ExtractFileName(LPCTSTR lpszUrl, CString& szFileName)
{
	TCHAR* lpLastSlash = _tcsrchr(lpszUrl, _T('/'));
	if(lpLastSlash == NULL)
	{
		return FALSE;
	}

	//"/" is the last char, in case of the address like: http://www.yahoo.com/
	if(*(lpLastSlash + 1) == _T('\0'))
	{
		return FALSE;
	}

	//check if the previous char is '/', in case of the address like: http://www.yahoo.com
	if(*(lpLastSlash - 1) == _T('/'))
	{
		return FALSE;
	}
	
	szFileName = (lpLastSlash + 1);

	//check if the length of candidate file is longer than MAX_PATH
	if(szFileName.GetLength() >= MAX_PATH)
	{
		return FALSE;
	}

	szFileName = StripInvalidFilenameChars(szFileName);

	return TRUE;
}

BOOL CCommonUtils::RemoveDirectory(LPCTSTR lpPathName)
{
	if(!::PathFileExists(lpPathName))
	{
		return TRUE;
	}
	CString sCurFile, sCurFindFileFilter;
	sCurFindFileFilter.Format("%s\\*", lpPathName);

	HANDLE hFind = NULL;
	WIN32_FIND_DATA FindFileData;
	BOOL hasMore;

	DWORD dwError = 0;
	CString szLog;

	hFind = FindFirstFile(sCurFindFileFilter, &FindFileData);
	for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
	{
		//Ignore current, and parent directory
		if(_tcscmp(FindFileData.cFileName, ".") == 0 || _tcscmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}
		//1. File
		if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			sCurFile.Format("%s\\%s", lpPathName, FindFileData.cFileName);
			if(!DeleteFile(sCurFile))
			{
				szLog = CGenericTools::LastErrorStr("DeleteFile", sCurFile);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				FindClose(hFind);
				
				return FALSE;
			}
		}
		//2. Dir ignored
		else if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
		}
	}
	dwError = GetLastError();
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
	}
	if (dwError != ERROR_NO_MORE_FILES)
	{
		szLog.Format("Find File Error: %d, %s, %s, %s", dwError, sCurFile, sCurFindFileFilter, lpPathName);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		return FALSE;
	}

	if(!::RemoveDirectory(lpPathName))
	{
		szLog = CGenericTools::LastErrorStr("RemoveDirectory", lpPathName);
		LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		return FALSE;
	}
	return TRUE;
}

BOOL CCommonUtils::VerifyDirectoryExist(LPCTSTR lpPathName)
{	
	BOOL bResult = ::PathFileExists(lpPathName);
	if(!bResult)
	{
		bResult = ::CreateDirectory(lpPathName, NULL);
		
		if(!bResult)
		{
			CString szLog = CGenericTools::LastErrorStr("CreateDirectory", lpPathName);
			LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
	}
	return bResult;
}

CString CCommonUtils::StripInvalidFilenameChars(const CString& strText)
{
	LPCTSTR pszSource = strText;
	CString strDest;
	
	while (*pszSource != _T('\0'))
	{
		if (!(((_TUCHAR)*pszSource >= 0 && (_TUCHAR)*pszSource <= 31) ||
			// lots of invalid chars for filenames in windows :=)
			*pszSource == _T('\"') || *pszSource == _T('*') || *pszSource == _T('<')  || *pszSource == _T('>') ||
			*pszSource == _T('?')  || *pszSource == _T('|') || *pszSource == _T('\\') || *pszSource == _T('/') || 
			*pszSource == _T(':')) )
		{
			strDest += *pszSource;
		}
		pszSource++;
	}
	
	static const LPCTSTR apszReservedFilenames[] = {
		_T("NUL"), _T("CON"), _T("PRN"), _T("AUX"), _T("CLOCK$"),
		_T("COM1"),_T("COM2"),_T("COM3"),_T("COM4"),_T("COM5"),_T("COM6"),_T("COM7"),_T("COM8"),_T("COM9"),
		_T("LPT1"),_T("LPT2"),_T("LPT3"),_T("LPT4"),_T("LPT5"),_T("LPT6"),_T("LPT7"),_T("LPT8"),_T("LPT9")
	};
	int nCount = sizeof(apszReservedFilenames)/sizeof(apszReservedFilenames[0]);
	
	for (int i = 0; i < nCount; i++)
	{
		int nPrefixLen = _tcslen(apszReservedFilenames[i]);
		if (_tcsnicmp(strDest, apszReservedFilenames[i], nPrefixLen) == 0)
		{
			if (strDest.GetLength() == nPrefixLen) {
				// Filename is a reserved file name:
				// Append an underscore character
				strDest += _T("_");
				break;
			}
			else if (strDest[nPrefixLen] == _T('.')) {
				// Filename starts with a reserved file name followed by a '.' character:
				// Replace that ',' character with an '_' character.
				LPTSTR pszDest = strDest.GetBuffer(strDest.GetLength());
				pszDest[nPrefixLen] = _T('_');
				strDest.ReleaseBuffer(strDest.GetLength());
				break;
			}
		}
	}
	
	return strDest;
}

int CCommonUtils::Intersection(const CRange& r1, const CRange& r2, CRange& rResult)
{
	//r2 is at the left side of r1 entirely, no intersection
	if(r1.cx > r2.cy)
	{
		return 1;
	}
	if(r1.cy < r2.cx)
	{
		return -1;
	}

	rResult.cx = max(r1.cx, r2.cx);
	rResult.cy = min(r1.cy, r2.cy);

	return 0;
}

BOOL CCommonUtils::IntegerOper(UINT& nData, int nOperType)
{
	BOOL bResult = TRUE;
	switch(nOperType)
	{
	case INT_OPER_RESET:
		{
			nData = 0;
		}
		break;
	case INT_OPER_KEEP:
		{
			//do nothing
		}
		break;
	case INT_OPER_INCREASE:
		{
			nData++;
		}
		break;
	default:
		{
			bResult = FALSE;
			LOG4CPLUS_ERROR(ROOT_LOGGER, "Unkown nOperType: "<<nOperType)
		}
		break;
	}
	return bResult;
}

int CCommonUtils::GetUniqueID()
{
	return InterlockedIncrement(&m_nUniqueID);
}

LRESULT CCommonUtils::SendMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CString szLog;

	if(::IsWindow(hWnd) == FALSE)
	{
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("IsWindow(hWnd) = FALSE. hWnd=0x%08X, msg=%d, wParam=%d, lParam=0x%08X", 
				hWnd, msg, wParam, lParam);
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
		return (LRESULT)0;
	}
	if(::IsWindowVisible(hWnd) == FALSE)
	{
		if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			szLog.Format("IsWindowVisible(hWnd) = FALSE. hWnd=0x%08X, msg=%d, wParam=%d, lParam=0x%08X", 
				hWnd, msg, wParam, lParam);
			LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
		return (LRESULT)0;
	}	

	LRESULT lr = ::SendMessage(hWnd, msg, wParam, lParam);

	return lr;
}

void CCommonUtils::FormatSpeed(DWORD dwBytesPerSeconds, CString& szOut)
{
	//less than 100 bytes
	if(dwBytesPerSeconds < 100)
	{
		szOut.Format("%u Bytes/s", dwBytesPerSeconds);
	}
	//less than 1M
	else if(dwBytesPerSeconds < ONE_MB)
	{
		double dSpeed = ((double)dwBytesPerSeconds / ONE_KB);
		szOut.Format("%.3f KB/s", dSpeed);
	}
	//less than 1G
	else if(dwBytesPerSeconds < ONE_GB)
	{
		double dSpeed = ((double)dwBytesPerSeconds / ONE_MB);
		szOut.Format("%.3f MB/s", dSpeed);
	}
	//Greater than 1G
	else
	{
		double dSpeed = ((double)dwBytesPerSeconds / ONE_GB);
		szOut.Format("%.3f GB/s", dSpeed);
	}
}

void CCommonUtils::FormatFileSize(DWORD dwBytes, CString& szOut)
{
	double dSize;
	if(dwBytes < ONE_KB)
	{
		szOut.Format("%u", dwBytes);
	}
	else if(dwBytes < ONE_MB)
	{
		dSize = ((double)dwBytes / ONE_KB);
		szOut.Format("%.3f KB", dSize);
	}
	else if(dwBytes < ONE_GB)
	{
		dSize = ((double)dwBytes / ONE_MB);
		szOut.Format("%.3f MB", dSize);
	}
	else
	{
		dSize = ((double)dwBytes / ONE_GB);
		szOut.Format("%.3f GB", dSize);
	}
}

void CCommonUtils::FormatTime(DWORD dwTimeInMs, CString& szOut)
{
	szOut.Format("%d", dwTimeInMs / 1000);
}

void CCommonUtils::FormatPercent(int nCurrent, int nMax, CString& szOut)
{
	if(nCurrent > 0 && nMax > 0)
	{
		if(nCurrent == nMax)
		{
			szOut = "100 %";
		}
		else
		{
			double d = ((double)nCurrent) * 100 / nMax;
			szOut.Format("%.2f %c", d, '%');
		}
		
	}
	else
	{
		szOut = "0 %";
	}
}

int CCommonUtils::GetIconIndex(LPCTSTR lpszPath, DWORD dwFileAttributes, UINT uFlags)
{
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	uFlags |= SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON | SHGFI_SYSICONINDEX;
	DWORD dwResult = ::SHGetFileInfo(lpszPath, dwFileAttributes, &sfi, sizeof(SHFILEINFO), uFlags);
	return sfi.iIcon;
}