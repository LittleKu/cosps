#include "StdAfx.h"
#include "Counter.h"

UINT CCounter::CountThreadProc(LPVOID lpvData)
{
	LPCountThreadParam lpThreadParam = (LPCountThreadParam)lpvData;
	ASSERT(lpThreadParam && lpThreadParam->hwndMain);

	CFileVisitor* pVisitor = new CFileCountVisitor();

	UINT cAllFiles = 0;
	int i, count = lpThreadParam->dirList.GetSize();
	int prev = 0;
	for(i = 0; i < count; i++)
	{
		EnumDirectory(lpThreadParam->dirList.GetAt(i), TRUE, pVisitor);
		AfxTrace("dir=%s, count=%d\n", lpThreadParam->dirList.GetAt(i), pVisitor->GetResult() - prev);
		prev = pVisitor->GetResult();
	}
	cAllFiles = pVisitor->GetResult();
	delete pVisitor;
	pVisitor = NULL;
	
	//Start to work
	LRESULT lresult = ::SendMessage(lpThreadParam->hwndMain, WM_START_COUNT, 0, (LPARAM)cAllFiles);
	HWND hProgWnd = (HWND)lresult;
	ASSERT(hProgWnd);
	
	pVisitor = new CFilePrintVisitor(hProgWnd);
	prev = 0;
	for(i = 0; i < count; i++)
	{
		if(EnumDirectory(lpThreadParam->dirList.GetAt(i), TRUE, pVisitor) == -1)
		{
			break;
		}
		AfxTrace("print dir=%s, count=%d\n", lpThreadParam->dirList.GetAt(i), pVisitor->GetResult() - prev);
		prev = pVisitor->GetResult();
	}
// 	for(int f = 0; f < cAllFiles; ++f)
// 	{
// 		lresult = ::SendMessage(hProgWnd, WM_UPDATE_PROGRESS, (WPARAM)(f + 1), 0);
// 		if(lresult == (LRESULT)0)
// 		{
// 			break;
// 		}
// 	}
	
	Sleep(5000);
	::SendMessage(lpThreadParam->hwndMain, WM_END_COUNT, 0, 0);

	return 0;
}

int CCounter::EnumDirectory(LPCTSTR lpszDirName, BOOL bRecursive, CFileVisitor* pVisitor)
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
		//1. File
		if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
        {
			sCurFile.Format("%s\\%s", lpszDirName, FindFileData.cFileName);
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
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
            && _tcscmp(FindFileData.cFileName, ".") != 0
            && _tcscmp(FindFileData.cFileName, "..") != 0)
		{
			sCurFile.Format("%s\\%s", lpszDirName, FindFileData.cFileName);		
            EnumDirectory(sCurFile, bRecursive, pVisitor);
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

CFileCountVisitor::CFileCountVisitor() : m_nCount(0)
{
}

int CFileCountVisitor::VisitFile(LPCTSTR lpszFileName)
{
	m_nCount++;
	return m_nCount;
}

CFilePrintVisitor::CFilePrintVisitor(HWND hWnd) : m_hProgWnd(hWnd), m_nCount(0)
{
}
int CFilePrintVisitor::VisitFile(LPCTSTR lpszFileName)
{
	m_nCount++;

	UpdateProgressParam param;
	param.nPos = m_nCount;
	_stprintf(param.sFile, "%s", lpszFileName);

	LRESULT lr = ::SendMessage(m_hProgWnd, WM_UPDATE_PROGRESS, (WPARAM)(&param), 0);
	if(lr == 0)
	{
		return -1;
	}

	return m_nCount;
}