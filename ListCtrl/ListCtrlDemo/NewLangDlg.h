// NewLangDlg.h: interface for the CNewLangDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWLANGDLG_H__E30448A5_5CEE_4C25_A14F_CC332C654C78__INCLUDED_)
#define AFX_NEWLANGDLG_H__E30448A5_5CEE_4C25_A14F_CC332C654C78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LangTemplateDlg.h"
#include "LangGrammarMap.h"

class CNewLangDlg : public CLangTemplateDlg  
{
public:
	BOOL GetLangGrammarInfo(CLangGrammarInfo*& pLangGrammarInfo, BOOL bShowError = TRUE);
	CNewLangDlg();
	virtual ~CNewLangDlg();
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_NEWLANGDLG_H__E30448A5_5CEE_4C25_A14F_CC332C654C78__INCLUDED_)
