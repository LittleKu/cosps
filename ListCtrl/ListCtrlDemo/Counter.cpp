#include "StdAfx.h"
#include "Counter.h"
#include "LangGrammarMap.h"
#include "LicenseMgr.h"

DECLARE_THE_LOGGER_NAME("Counter.cpp")

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
	CString szLogInfo;
	for(i = 0; i < count; i++)
	{
		sCurDir = lpThreadParam->dirList.GetAt(i);
		result = CommonUtils::EnumDirectoryIt(sCurDir, lpThreadParam->filterGroupList, lpThreadParam->bRecursive, 
			pVisitor, &progChecker);
		if(result < 0)
		{
			break;
		}
		//Format Log
		szLogInfo.Format("[SEARCH]: dir=%s, count=%d", lpThreadParam->dirList.GetAt(i), pVisitor->GetResult() - prev);
		LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLogInfo)

		prev = pVisitor->GetResult();
	}
	timeCost.UpdateCurrClock();

	szLogInfo.Format("Cost %d for [SEARCH]", timeCost.GetTimeCost());
	LOG4CPLUS_INFO_STR(GET_LOGGER("Timer"), (LPCTSTR)szLogInfo)
	cAllFiles = pVisitor->GetResult();
	delete pVisitor;
	pVisitor = NULL;

	if(result < 0)
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
	if(CLicenseMgr::GetInstance()->IsRegistered(TRUE))
	{
		pVisitor = new CFileParserVisitor(lpThreadParam->hwndMain, hProgWnd);
	}
	else
	{
		pVisitor = new CEvaluationFileParserVisitor(lpThreadParam->hwndMain, hProgWnd);
	}

	progChecker.Reset();
	prev = 0;
	timeCost.Reset();
	for(i = 0; i < count; i++)
	{
		sCurDir = lpThreadParam->dirList.GetAt(i);
		result = CommonUtils::EnumDirectoryIt(sCurDir, lpThreadParam->filterGroupList, lpThreadParam->bRecursive, 
			pVisitor, &progChecker);
		if(result < 0)
		{
			break;
		}
		//Format Log
		szLogInfo.Format("[COUNT]: dir=%s, count=%d", lpThreadParam->dirList.GetAt(i), pVisitor->GetResult() - prev);
		LOG4CPLUS_INFO_STR(THE_LOGGER, (LPCTSTR)szLogInfo)
		prev = pVisitor->GetResult();
	}
	timeCost.UpdateCurrClock();

	szLogInfo.Format("Cost %d for [COUNT]", timeCost.GetTimeCost());
	LOG4CPLUS_INFO_STR(GET_LOGGER("Timer"), (LPCTSTR)szLogInfo)
	delete pVisitor;
	pVisitor = NULL;

	::SendMessage(lpThreadParam->hwndMain, WM_SUMMARY_UPDATE, 0, 0);

	//End the works
	::SendMessage(lpThreadParam->hwndMain, WM_END_COUNT, result, 0);

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

int CFileCountVisitor::VisitFile(LPCTSTR lpszFileName, LPVOID lpParam)
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
int CFileParserVisitor::VisitFile(LPCTSTR lpszFileName, LPVOID lpParam)
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

	LPFilterGroup lpFilterGroup = (LPFilterGroup)lpParam;
	ASSERT(lpFilterGroup != NULL && lpFilterGroup->nLangRuleType >= 0);

	IFileParser* pFileParser = GetFileParser(lpFilterGroup->nLangRuleType, pFi, SYS_PREF()->m_nStatMode);
	if(pFileParser == NULL)
	{
		AfxTrace("File[%s], Lang[%s:%d]\n", lpszFileName, lpFilterGroup->szLangRuleName, lpFilterGroup->nLangRuleType);
	}
	else
	{
		pFileParser->ParseFile();
	}	

	::SendMessage(m_hMainWnd, WM_PROGRESS_UPDATE, (WPARAM)(pFi), 0);

	delete pFileParser;

	return m_nCount;
}

IFileParser* CFileParserVisitor::GetFileParser(int nLangRuleType, CFileInfo* pFileInfo, DWORD nMode)
{
#ifdef _DEBUG
	nMode |= FP_MODE_LOG_ALL;
#endif
	IFileParser* pFileParser = NULL;
	if(!CLangGrammarInfo::IsUserDefinedLangGrammar(nLangRuleType))
	{
		pFileParser = CFileParserFactory::GetFileParser(static_cast<ELangType>(nLangRuleType), pFileInfo, nMode);
	}
	if(pFileParser != NULL)
	{
		return pFileParser;
	}
	CLangGrammarInfo* pLangGrammarInfo = CLangGrammarMap::GetInstance()->GetLangGrammarInfo(nLangRuleType);
	ASSERT(pLangGrammarInfo != NULL);
	return CFileParserFactory::GetGenericFileParser(pLangGrammarInfo->m_pLangGrammar, pFileInfo, nMode);
}

CEvaluationFileParserVisitor::CEvaluationFileParserVisitor(HWND hMainWnd, HWND hProgressWnd)
 : CFileParserVisitor(hMainWnd, hProgressWnd)
{
}

int CEvaluationFileParserVisitor::VisitFile(LPCTSTR lpszFileName, LPVOID lpParam)
{
	if(m_nCount >= MAX_EVALUATION_FILE_COUNT)
	{
		return COUNTER_RET_CODE_EXCEEDS_LIMITATION;
	}
	return CFileParserVisitor::VisitFile(lpszFileName, lpParam);
}