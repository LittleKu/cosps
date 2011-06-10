#include "StdAfx.h"
#include "Counter.h"
#include "FileParser.h"
#include "CFileParser.h"

UINT CCounter::CountThreadProc(LPVOID lpvData)
{
	LPCountThreadParam lpThreadParam = (LPCountThreadParam)lpvData;
	ASSERT(lpThreadParam && lpThreadParam->hwndMain);

	//Start to work
	LRESULT lresult = ::SendMessage(lpThreadParam->hwndMain, WM_START_COUNT, 0, 0);
	HWND hProgWnd = (HWND)lresult;
	ASSERT(hProgWnd);

	//Set current progress status
	LPUpdateProgressParam lpProgressParam = new UpdateProgressParam;
	lpProgressParam->nPos = 0;
	lpProgressParam->sStatus.Format(_T("Found %d files"), 0);
	lpProgressParam->sFile.Empty();
	::SendMessage(hProgWnd, WM_PROGRESS_UPDATE, (WPARAM)lpProgressParam, 0);
	delete lpProgressParam;
	lpProgressParam = NULL;

	//Calculate the files count
	CFileVisitor* pVisitor = new CFileCountVisitor(hProgWnd);

	UINT cAllFiles = 0;
	int i, count = lpThreadParam->dirList.GetSize();
	int result = 0, prev = 0;
	CString sCurDir;
	CProgressChecker progChecker(hProgWnd);
	CTimeCost timeCost;
	for(i = 0; i < count; i++)
	{
		sCurDir = lpThreadParam->dirList.GetAt(i);
		if(sCurDir.GetAt(sCurDir.GetLength() - 1) == '\\')
		{
			sCurDir.SetAt(sCurDir.GetLength() - 1, '\0');
		}
		result = CommonUtils::EnumDirectoryIt(sCurDir, lpThreadParam->filterList, lpThreadParam->bRecursive, pVisitor, &progChecker);
		if(result == -1)
		{
			break;
		}
		AfxTrace("[COUNT]: dir=%s, count=%d\n", lpThreadParam->dirList.GetAt(i), pVisitor->GetResult() - prev);
		prev = pVisitor->GetResult();
	}
	timeCost.UpdateCurrClock();
	AfxTrace("Time Cost %d\n", timeCost.GetTimeCost());
	cAllFiles = pVisitor->GetResult();
	delete pVisitor;
	pVisitor = NULL;

	if(result == -1)
	{
		//End the works
		::SendMessage(lpThreadParam->hwndMain, WM_END_COUNT, 0, 0);

		delete lpThreadParam;
		lpThreadParam = NULL;

		return -1;
	}

	//Update the progress range
	::SendMessage(hProgWnd, WM_PROGRESS_SET_RANGE, 0, cAllFiles);
	
	//Do the works
	pVisitor = new CFileParserVisitor(lpThreadParam->hwndMain, hProgWnd);
	progChecker.Reset();
	prev = 0;
	timeCost.Reset();
	for(i = 0; i < count; i++)
	{
		sCurDir = lpThreadParam->dirList.GetAt(i);
		if(sCurDir.GetAt(sCurDir.GetLength() - 1) == '\\')
		{
			sCurDir.SetAt(sCurDir.GetLength() - 1, '\0');
		}
		if(CommonUtils::EnumDirectoryIt(sCurDir, lpThreadParam->filterList, lpThreadParam->bRecursive, pVisitor, &progChecker) == -1)
		{
			break;
		}
		AfxTrace("[PRINT]: dir=%s, count=%d\n", lpThreadParam->dirList.GetAt(i), pVisitor->GetResult() - prev);
		prev = pVisitor->GetResult();
	}
	timeCost.UpdateCurrClock();
	AfxTrace("Time Cost %d\n", timeCost.GetTimeCost());
	delete pVisitor;
	pVisitor = NULL;

	::SendMessage(lpThreadParam->hwndMain, WM_SUMMARY_UPDATE, 0, 0);

	//End the works
	::SendMessage(lpThreadParam->hwndMain, WM_END_COUNT, 0, 0);

	delete lpThreadParam;
	lpThreadParam = NULL;

	return 0;
}


CProgressChecker::CProgressChecker(HWND hProgWnd) : m_hProgWnd(hProgWnd), m_timeCost(30)
{
}

BOOL CProgressChecker::IsCancelled()
{
	m_timeCost.UpdateCurrClock();
	if(m_timeCost.IsTimeOut())
	{
		//Do the check works
		BOOL isCancelled = ::SendMessage(m_hProgWnd, WM_PROGRESS_IS_CANCELLED, 0, 0);
		if(isCancelled)
		{
			return TRUE;
		}
		m_timeCost.UpdateLastClock();
	}
	return FALSE;
}

void CProgressChecker::Reset()
{
	m_timeCost.Reset();
}

CFileCountVisitor::CFileCountVisitor(HWND hWnd) : m_hProgWnd(hWnd), m_nCount(0), m_timeCost(10)
{
}

int CFileCountVisitor::VisitFile(LPCTSTR lpszFileName)
{
	m_nCount++;

	UpdateProgressParam param;
	param.nPos = 0;
	param.sStatus.Format(_T("Found %d files"), m_nCount);
	param.sFile.Format(_T("%s"), lpszFileName);
	LRESULT lr = ::SendMessage(m_hProgWnd, WM_PROGRESS_UPDATE, (WPARAM)(&param), 0);
	if(lr == 0)
	{
		return -1;
	}

	return m_nCount;
}

CFileParserVisitor::CFileParserVisitor(HWND hMainWnd, HWND hProgressWnd) : m_hMainWnd(hMainWnd), m_hProgWnd(hProgressWnd), m_nCount(0)
{
}
int CFileParserVisitor::VisitFile(LPCTSTR lpszFileName)
{
	m_nCount++;

	UpdateProgressParam param;
	param.nPos = m_nCount;
	param.sFile.Format(_T("%s"), lpszFileName);
	LRESULT lr = ::SendMessage(m_hProgWnd, WM_PROGRESS_UPDATE, (WPARAM)(&param), 0);
	if(lr == 0)
	{
		return -1;
	}

	CFileInfo* pFi = new CFileInfo();
	pFi->SetFileName(lpszFileName);

	CCFileParser cppFileParser(pFi);
	cppFileParser.ParseFile();

	::SendMessage(m_hMainWnd, WM_PROGRESS_UPDATE, (WPARAM)(pFi), 0);

	return m_nCount;
}