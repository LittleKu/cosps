#include "StdAfx.h"
#include "ListCtrlDemo.h"
#include "StringUtils.h"

CString FilterGroup::ToString()
{
	CString szNameType;
	szNameType.Format("%s:%d", szLangRuleName, nLangRuleType);

	CString szInclude;
	POSITION pos = includeList.GetHeadPosition();
	while(pos != NULL)
	{
		szInclude += includeList.GetNext(pos);
		if(pos != NULL)
		{
			szInclude += ":";
		}
	}

	CString szExclude;
	pos = excludeList.GetHeadPosition();
	while(pos != NULL)
	{
		szExclude += excludeList.GetNext(pos);
		if(pos != NULL)
		{
			szExclude += ":";
		}
	}

	CString szResult;
	szResult.Format("<%s><%s><%s>", szNameType, szInclude, szExclude);
	return szResult;
}
CountThreadParam::~CountThreadParam()
{
	dirList.RemoveAll();

	POSITION pos = filterGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		LPFilterGroup pFilterGroup = filterGroupList.GetNext(pos);
		delete pFilterGroup;
	}
}

CTimeCost::CTimeCost(UINT timeInMs) : m_nDiff(timeInMs), m_clockCurr(clock()), m_clockLast(clock())
{
}

void CTimeCost::UpdateCurrClock()
{
	m_clockCurr = clock();
}

void CTimeCost::UpdateLastClock()
{
	m_clockLast = clock();
}

UINT CTimeCost::GetTimeCost()
{
	long diff = m_clockCurr - m_clockLast;
#if (CLOCKS_PER_SEC != 1000)
	diff = diff * 1000 / CLOCKS_PER_SEC;
#endif
	if(diff < 0)
	{
		diff = 0;
	}
	return (UINT)diff;
}

BOOL CTimeCost::IsTimeOut()
{
	return (GetTimeCost() >= m_nDiff) ? TRUE : FALSE;
}

void CTimeCost::Reset()
{
	m_clockLast = clock();
	m_clockCurr = clock();
}

#ifdef _DEBUG
#define TRACE_LONG_NAME(x) \
	if(x.GetLength() > MAX_PATH) \
	{ AfxTrace("Long name file: %s\n", x); }
#else
#define TRACE_LONG_NAME(x) 
#endif

BEGIN_NAMESPACE(CommonUtils)
void Trim(CString& str, TCHAR chTarget)
{
	str.TrimLeft(chTarget);
	str.TrimRight(chTarget);
}
void LastErrorHandler(LPCTSTR lpszMsg, UINT uFlags)
{
	TCHAR szBuf[1024]; 
    DWORD dw = GetLastError(); 
	
    _stprintf(szBuf, "%s failed: GetLastError returned %u\n", lpszMsg, dw); 
	
	if(uFlags & LEH_AFX_TRACE)
	{
		AfxTrace("%s", szBuf);
	}
	if(uFlags & LEH_MESSAGE_BOX)
	{
		AfxMessageBox(szBuf);
	}  
}
CString ToString(UINT n)
{
	CString str;
	str.Format("%d", n);
	return str;
}

CString GetPercentStr(UINT nCount, UINT nTotal)
{
	CString str;
	if(nCount == 0 || nTotal <= 0)
	{
		str = "0%";
	}
	else if(nCount == nTotal)
	{
		str = "100%";
	}
	else
	{
		str.Format("%2.2f%c", (100.0 * nCount) / nTotal, '%');
	}
	return str;
}
CString GetCurrentTime()
{
	return CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
}

int GetIconIndex(LPCTSTR lpszPath, DWORD dwFileAttributes, UINT uFlags)
{
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	uFlags |= SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON | SHGFI_SYSICONINDEX;
	DWORD dwResult = ::SHGetFileInfo(lpszPath, dwFileAttributes, &sfi, sizeof(SHFILEINFO), uFlags);
	return sfi.iIcon;
}
int GetWindowsDirIconIndex()
{
	static int iWindowsDirIconIndex = -1;

	if(iWindowsDirIconIndex == -1)
	{
		TCHAR szWinDir[MAX_PATH + 1];
		GetWindowsDirectory(szWinDir, MAX_PATH);
		iWindowsDirIconIndex = GetIconIndex(szWinDir, FILE_ATTRIBUTE_DIRECTORY, 0);
	}
	return iWindowsDirIconIndex;
}
int GetWindowsDirOpenIconIndex()
{
	static int iWindowsDirOpenIconIndex = -1;
	
	if(iWindowsDirOpenIconIndex == -1)
	{
		TCHAR szWinDir[MAX_PATH + 1];
		GetWindowsDirectory(szWinDir, MAX_PATH);
		iWindowsDirOpenIconIndex = GetIconIndex(szWinDir, FILE_ATTRIBUTE_DIRECTORY, SHGFI_OPENICON);
	}
	return iWindowsDirOpenIconIndex;
}

BOOL IsFileExist(LPCTSTR lpFileName)
{
	HANDLE hFile;
	hFile = CreateFile(lpFileName, // open lpFileName
		GENERIC_READ,              // open for reading 
		FILE_SHARE_READ,           // share for reading 
		NULL,                      // no security 
		OPEN_EXISTING,             // existing file only 
		FILE_ATTRIBUTE_NORMAL,     // normal file 
		NULL);                     // no attr. template 
	
	BOOL bResult = TRUE;
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		CString szErrorText;
		szErrorText.Format("IsFileExist(%s) = INVALID_HANDLE_VALUE", lpFileName);
		LastErrorHandler(szErrorText, LEH_AFX_TRACE);
		bResult = FALSE;
	}
	CloseHandle(hFile);
	return bResult;
}

CString GetConfFilePath(LPCTSTR lpFileName, UINT uFlags, LPCTSTR lpBaseDir)
{
	if(lpBaseDir == NULL)
	{
		lpBaseDir = SYS_APP()->m_szWorkDir;
	}

	CString szFilePath;
	if(uFlags == GCFP_AUTO)
	{
		szFilePath.Format("%s\\dat\\user\\%s", lpBaseDir, lpFileName);
		if(!::PathFileExists(szFilePath))
		{
			szFilePath.Format("%s\\dat\\default\\%s", lpBaseDir, lpFileName);
		}
	}
	else if(uFlags == GCFP_USER)
	{
		szFilePath.Format("%s\\dat\\user\\%s", lpBaseDir, lpFileName);
	}
	else if(uFlags == GCFP_DEFAULT)
	{
		szFilePath.Format("%s\\dat\\default\\%s", lpBaseDir, lpFileName);
	}
	return szFilePath;
}

// the following function based on a function by Jack Handy - you may find 
// his original article at: http://www.codeproject.com/useritems/wildcmp.asp
int wildcmp(const char *wild, const char *string) {
	const char *cp, *mp;
	
	while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}
	
	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}
	
	while (*wild == '*'  ||  *wild == '?') {
		wild++;
	}
	return !*wild;
}

BOOL IsMatched(CStringArray& sFilterList, LPCTSTR sStr)
{
	int nCount = sFilterList.GetSize();
	if(nCount <= 0)
	{
		return FALSE;
	}
	
	CString filter;
	int i;
	for(i = 0; i < nCount; i++)
	{
		filter = sFilterList.GetAt(i);
		if(CommonUtils::wildcmp(filter, sStr))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsMatched(CStringList& sFilterList, LPCTSTR sStr)
{
	POSITION pos = sFilterList.GetHeadPosition();
	while(pos != NULL)
	{
		CString& sFilter = sFilterList.GetNext(pos);
		//If the last character is '.', the string that contains '.' is not included
		if(sFilter.GetAt(sFilter.GetLength() - 1) == '.')
		{
			if(CStringUtils::IndexOf(sStr, _T('.')) == -1)
			{
				CString sFixedFilter = sFilter;
				sFixedFilter.GetBufferSetLength(sFilter.GetLength() - 1);
				if(CStringUtils::wildicmp(sFixedFilter, sStr))
				{
					return TRUE;
				}
			}
		}
		else
		{
			if(CStringUtils::wildicmp(sFilter, sStr))
			{
				return TRUE;
			}
		}
	}
	return FALSE;	
}

BOOL IsMatched(LPFilterGroup lpFilterGroup, LPCTSTR sStr)
{
	if(IsMatched(lpFilterGroup->excludeList, sStr))
	{
		return FALSE;
	}
	if(IsMatched(lpFilterGroup->includeList, sStr))
	{
		return TRUE;
	}
	return FALSE;
}

POSITION IsMatched(LPFilterGroupList& filterGourpList, const char* sStr)
{
	if(filterGourpList.IsEmpty())
	{
		return NULL;
	}
	POSITION pos = filterGourpList.GetHeadPosition();
	LPFilterGroup lpFilterGroup;
	while(pos != NULL)
	{
		lpFilterGroup = filterGourpList.GetAt(pos);
		if(IsMatched(lpFilterGroup, sStr))
		{
			return pos;
		}
		//Move to next pos
		filterGourpList.GetNext(pos);
	}
	return NULL;
}

void LoadBitmapFromFile(LPCTSTR lpszBmpFilePath, CBitmap* pBitmap)
{
	ASSERT(pBitmap && pBitmap->GetSafeHandle() == NULL);
	HBITMAP hbmp = (HBITMAP)::LoadImage(NULL, lpszBmpFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(hbmp)
	{
		pBitmap->Attach(hbmp);
	}
}

int EnumDirectory(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker)
{
	ASSERT(lpszDirName);
	ASSERT(pVisitor);
	
	//Find Dir
	CString sCurDir;
	sCurDir.Format("%s\\*", lpszDirName);
	
	//Init
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(sCurDir, &FindFileData);
	
	DWORD dwError;
	if (hFind == INVALID_HANDLE_VALUE)
    {
		dwError = GetLastError();
		AfxTrace("FindFirstFile Error: %d, %s\n", dwError, sCurDir);
        return dwError;
    }
	
	CString sCurFile;
	BOOL hasMore;
	for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
	{
		//Ignore hidden files, current, and parent directory
		if( ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
			|| _tcscmp(FindFileData.cFileName, ".") == 0
			|| _tcscmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}
		//1. File
		if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
			if(!IsMatched(sFilterArray, FindFileData.cFileName))
			{
				continue;
			}
			sCurFile.Format("%s\\%s", lpszDirName, FindFileData.cFileName);
			TRACE_LONG_NAME(sCurFile);
            if(pVisitor->VisitFile(sCurFile) == -1)
			{
				AfxTrace ("VisitFile request to quit\n");
				FindClose(hFind);
				return -1;
			}
        }
		//2. Directory
		else if( bRecursive && ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) )
		{
			sCurFile.Format("%s\\%s", lpszDirName, FindFileData.cFileName);
			TRACE_LONG_NAME(sCurFile);
            if(EnumDirectory(sCurFile, sFilterArray, bRecursive, pVisitor, pCancelledChecker) == -1)
			{
				AfxTrace ("VisitFile request to quit\n");
				FindClose(hFind);
				return -1;
			}
		}
	}
	dwError = GetLastError();
	FindClose(hFind);
    if (dwError != ERROR_NO_MORE_FILES)
    {
		AfxTrace ("FindNextFile Error: %d, %s\n", dwError, sCurFile);
        return dwError;
    }
	dwError = 0;
	return dwError;
}

int EnumDirectoryFileFirst(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker)
{
	ASSERT(lpszDirName);
	ASSERT(pVisitor);

	//Find Dir
	CString sCurDir;
	sCurDir.Format("%s\\*", lpszDirName);
	
	//Init
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(sCurDir, &FindFileData);
	
	DWORD dwError;
	if (hFind == INVALID_HANDLE_VALUE)
    {
		dwError = GetLastError();
		AfxTrace("FindFirstFile Error: %d\n", dwError);
        return dwError;
    }
	
	CString sCurFile;
	BOOL hasMore;
	for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
	{
		//Ignore hidden files, current, and parent directory
		if( ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
			|| _tcscmp(FindFileData.cFileName, ".") == 0
			|| _tcscmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}
		//File
		if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
        {
			if(!IsMatched(sFilterArray, FindFileData.cFileName))
			{
				continue;
			}
			sCurFile.Format("%s\\%s", lpszDirName, FindFileData.cFileName);
			TRACE_LONG_NAME(sCurFile);
            if(pVisitor->VisitFile(sCurFile) == -1)
			{
				AfxTrace ("VisitFile request to quit\n");
				FindClose(hFind);
				return -1;
			}
        }
	}
	dwError = GetLastError();
	FindClose(hFind);
    if (dwError != ERROR_NO_MORE_FILES)
    {
		AfxTrace ("FindNextFile Error: %d\n", dwError);
        return dwError;
    }

	//No need to recursive find more
	if(!bRecursive)
	{
		dwError = 0;
		return dwError;
	}
	
	
	sCurDir.Format("%s\\*", lpszDirName);
	//Find directories
	hFind = FindFirstFile(sCurDir, &FindFileData);	
	if (hFind == INVALID_HANDLE_VALUE)
    {
		dwError = GetLastError();
		AfxTrace("Directory FindFirstFile Error: %d\n", dwError);
        return dwError;
    }
	
	for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
	{
		//Ignore hidden files, current, and parent directory
		if( ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
			|| _tcscmp(FindFileData.cFileName, ".") == 0
			|| _tcscmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{	
			sCurFile.Format("%s\\%s", lpszDirName, FindFileData.cFileName);
			TRACE_LONG_NAME(sCurFile);
            if(EnumDirectoryFileFirst(sCurFile, sFilterArray, bRecursive, pVisitor, pCancelledChecker) == -1)
			{
				AfxTrace ("VisitFile request to quit\n");
				FindClose(hFind);
				return -1;
			}
		}
	}
	dwError = GetLastError();
	FindClose(hFind);
	if (dwError != ERROR_NO_MORE_FILES)
    {
		AfxTrace ("Directory FindNextFile Error: %d\n", dwError);
        return dwError;
    }
    dwError = 0;
	return dwError;
}


int EnumDirectoryIt(LPCTSTR lpszDirName, LPFilterGroupList& lpFilterGroupList, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker)
{
	ASSERT(lpszDirName);
	ASSERT(pVisitor);

	CStringList sDirList;
	sDirList.AddTail(lpszDirName);

	CString sCurDir, sCurFile, sCurFindFileFilter;
	HANDLE hFind = NULL;
	WIN32_FIND_DATA FindFileData;
	BOOL hasMore;
	DWORD dwError = 0;
	int nResult = 0;

	while(!sDirList.IsEmpty())
	{
		sCurDir = sDirList.RemoveHead();
		sCurFindFileFilter.Format("%s\\*", sCurDir);

		hFind = FindFirstFile(sCurFindFileFilter, &FindFileData);
		for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
		{
			if(pCancelledChecker->IsCancelled())
			{
				AfxTrace("The EnumDirectoryIt process is cancelled\n");
				FindClose(hFind);
				return COUNTER_RET_CODE_USER_CANCELLED;
			}
			//Ignore hidden files, current, and parent directory
			if( ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
				|| _tcscmp(FindFileData.cFileName, ".") == 0
				|| _tcscmp(FindFileData.cFileName, "..") == 0)
			{
				continue;
			}
			//1. File
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				POSITION pos = IsMatched(lpFilterGroupList, FindFileData.cFileName);
				if(pos == NULL)
				{
					continue;
				}
				sCurFile.Format("%s\\%s", sCurDir, FindFileData.cFileName);
				TRACE_LONG_NAME(sCurFile);

				LPFilterGroup lpFilterGroup = lpFilterGroupList.GetAt(pos);
				ASSERT(lpFilterGroup != NULL);
				nResult = pVisitor->VisitFile(sCurFile, (LPVOID)lpFilterGroup);
				if(nResult < 0)
				{
					AfxTrace ("FileVisitor request to quit\n");
					FindClose(hFind);
					return nResult;
				}
			}
			//2. Dir
			else if( bRecursive && ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) )
			{
				sCurFile.Format("%s\\%s", sCurDir, FindFileData.cFileName);
				TRACE_LONG_NAME(sCurFile);
				sDirList.AddTail(sCurFile);
			}
		}
		dwError = GetLastError();
		if(hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
		}
		if (dwError != ERROR_NO_MORE_FILES)
		{
			AfxTrace ("Find File Error: %d, %s, %s, %s\n", dwError, sCurFile, sCurFindFileFilter, sCurDir);
			return dwError;
		}
	}

	return 0;
}

void Split(const CString& szStr, TCHAR delim, CStringList& outStringList)
{
	if(szStr.IsEmpty())
	{
		return;
	}
	
	char cSep = ';';
	int nStart = 0, nIndex;
	CString str;
	while( (nIndex = szStr.Find(delim, nStart)) != -1)
	{
		if(nIndex > nStart)
		{
			str = szStr.Mid(nStart, nIndex - nStart);
			outStringList.AddTail(str);
		}
		nStart = nIndex + 1;
	}
	//Not the last char
	if(nStart < szStr.GetLength())
	{
		str = szStr.Mid(nStart);
		outStringList.AddTail(str);
	}
// #ifdef _DEBUG
// 	for(int i = 0; i < m_sFilterArray.GetSize(); i++)
// 	{
// 		AfxTrace("Filer(%d): [%s]\n", i + 1, m_sFilterArray.GetAt(i));
// 	}
// #endif
}

CString ToString(LPFilterGroupList& filterGroupList)
{
	CString szResult;
	LPFilterGroup lpFilterGroup = NULL;
	POSITION pos = filterGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		lpFilterGroup = filterGroupList.GetNext(pos);
		ASSERT(lpFilterGroup != NULL);
		szResult += lpFilterGroup->ToString();
		if(pos != NULL)
		{
			szResult += " | ";
		}
	}
	return szResult;
}

END_NAMESPACE()