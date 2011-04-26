#include "StdAfx.h"

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

CFileInfo::CFileInfo() : m_nTotalLines(0), m_nCodeLines(0), m_nCommentLines(0), m_nBlankLines(0)
{
}

void CFileInfo::SetFileName(LPCTSTR lpszFullFileName)
{
    TCHAR drive[MAX_PATH];
    TCHAR dir[MAX_PATH];
    TCHAR fname[MAX_PATH];
    TCHAR ext[MAX_PATH];

    m_sFullFileName = lpszFullFileName;

    _tsplitpath(m_sFullFileName, drive, dir, fname, ext);

    m_sFilePath = drive;
    m_sFilePath += dir;

	m_sFileExt = ext;

    m_sFileName = fname;
	m_sFileName += ext;
}

UINT CFileInfo::GetMixedLines() const
{
	return (m_nCodeLines + m_nCommentLines) - (m_nTotalLines - m_nBlankLines);
}

CString CFileInfo::ToString(UINT n) const
{
	CString str;
	str.Format("%d", n);
	return str;
}

#ifdef _DEBUG
#define TRACE_LONG_NAME(x) \
	if(x.GetLength() > MAX_PATH) \
	{ AfxTrace("Long name file: %s\n", x); }
#else
#define TRACE_LONG_NAME(x) 
#endif

BEGIN_NAMESPACE(CommonUtils)
// the following function based on a function by Jack Handy - you may find 
// his original article at: http://www.codeproject.com/useritems/wildcmp.asp
int CommonUtils::wildcmp(const char *wild, const char *string) {
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

BOOL IsMatched(CStringArray& sFilterList, const char* sStr)
{
	int nCount = sFilterList.GetSize();
	if(nCount <= 0)
	{
		return TRUE;
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


int EnumDirectoryIt(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker)
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
				return -1;
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
				if(!IsMatched(sFilterArray, FindFileData.cFileName))
				{
					continue;
				}
				sCurFile.Format("%s\\%s", sCurDir, FindFileData.cFileName);
				TRACE_LONG_NAME(sCurFile);

				if(pVisitor->VisitFile(sCurFile) == -1)
				{
					AfxTrace ("FileVisitor request to quit\n");
					FindClose(hFind);
					return -1;
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

END_NAMESPACE()