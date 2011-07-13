// NewLangDlg.cpp: implementation of the CNewLangDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewLangDlg.h"
#include "LangGrammar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static const char* lpszLangName = "Name:";
static const char* lpszLineComment = "Line Comment:";
static const char* lpszEscapeString = "Escape String:";
static const char* lpszBlockCommentOn = "Block Comment On:";
static const char* lpszBlockCommentOff = "Block Comment Off:";
static const char* lpszStringOn = "String On:";
static const char* lpszStringOff = "String Off:";
static const char* lpszCharacterOn = "Character On:";
static const char* lpszCharacterOff = "Character Off:";

CNewLangDlg::CNewLangDlg()
{
	SetModal(TRUE);
	SetTitle("Custom Language");

	AddProperty(lpszLangName, 2);

	AddSeparator();

	AddProperty(lpszLineComment, 2);
	AddProperty(lpszEscapeString, 2);
	AddProperty(lpszBlockCommentOn, 1);
	AddProperty(lpszBlockCommentOff, 1);
	AddProperty(lpszStringOn, 1);
	AddProperty(lpszStringOff, 1);
	AddProperty(lpszCharacterOn, 1);
	AddProperty(lpszCharacterOff, 1);

	AddSeparator();
}

CNewLangDlg::~CNewLangDlg()
{

}

BEGIN_MESSAGE_MAP(CNewLangDlg, CLangTemplateDlg)
END_MESSAGE_MAP()

BOOL CNewLangDlg::OnInitDialog()
{
	if(!CLangTemplateDlg::OnInitDialog())
	{
		return FALSE;
	}

	return TRUE;
}

void CNewLangDlg::OnOK()
{
	UpdatePropValue();
	CLangGrammarInfo* pLangGrammarInfo = NULL;
	if(!GetLangGrammarInfo(pLangGrammarInfo))
	{
		return;
	}
	//Add Lang Grammar into map
	CLangGrammarMap::GetInstance()->AddLangGrammarInfo(pLangGrammarInfo);
	
	CLangTemplateDlg::OnOK();
}

BOOL CNewLangDlg::GetLangGrammarInfo(CLangGrammarInfo*& pLangGrammarInfo, BOOL bShowError)
{
	//1. validation
	pLangGrammarInfo = new CLangGrammarInfo();
	BOOL bResult = FALSE;

	CString szTemp1, szTemp2;
	do 
	{
		//Lang Name
		m_mapPropOut.Lookup(lpszLangName, pLangGrammarInfo->m_szLangName);
		pLangGrammarInfo->m_szLangName.TrimLeft();
		if(pLangGrammarInfo->m_szLangName.IsEmpty())
		{
			if(bShowError)
			{
				AfxMessageBox(_T("Language name can't be empty."));
				break;
			}
		}

		CLangGrammarBuilder builder;

		//Line Comment
		m_mapPropOut.Lookup(lpszLineComment, szTemp1);
		if(!szTemp1.IsEmpty())
		{
			builder.AddSingleComment(szTemp1);
		}

		//Escape String
		m_mapPropOut.Lookup(lpszEscapeString, szTemp1);
		if(!szTemp1.IsEmpty())
		{
			builder.AddEscapeStr(szTemp1);
		}

		//Block Comment
		m_mapPropOut.Lookup(lpszBlockCommentOn, szTemp1);
		m_mapPropOut.Lookup(lpszBlockCommentOff, szTemp2);
		if(!szTemp1.IsEmpty() && !szTemp2.IsEmpty())
		{
			builder.AddMultiComment(szTemp1, szTemp2);
		}

		//String
		m_mapPropOut.Lookup(lpszStringOn, szTemp1);
		m_mapPropOut.Lookup(lpszStringOff, szTemp2);
		if(!szTemp1.IsEmpty() && !szTemp2.IsEmpty())
		{
			builder.AddStringMark(szTemp1, szTemp2);
		}

		//Character
		m_mapPropOut.Lookup(lpszCharacterOn, szTemp1);
		m_mapPropOut.Lookup(lpszCharacterOff, szTemp2);
		if(!szTemp1.IsEmpty() && !szTemp2.IsEmpty())
		{
			builder.AddCharMark(szTemp1, szTemp2);
		}

		pLangGrammarInfo->m_pLangGrammar = builder.GetResult();
		bResult = TRUE;
	} while (0);
	
	if(!bResult)
	{
		delete pLangGrammarInfo;
		pLangGrammarInfo = NULL;
		return FALSE;
	}

	return TRUE;
}
