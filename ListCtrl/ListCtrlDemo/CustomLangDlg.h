// NewLangDlg.h: interface for the CNewLangDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWLANGDLG_H__E30448A5_5CEE_4C25_A14F_CC332C654C78__INCLUDED_)
#define AFX_NEWLANGDLG_H__E30448A5_5CEE_4C25_A14F_CC332C654C78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LangGrammarDlg.h"
#include "LangGrammarMap.h"

class CCustomLangDlg : public CDialog  
{
public:
	CCustomLangDlg();
	virtual ~CCustomLangDlg();
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
protected:
	void InitGUI();
	CDlgTemplate* m_dlgTemplate;
	CLangGrammarDlg* m_pLangGrammarDlg;
	CButton* m_pOkButton;
	CButton* m_pCancelButton;
};

#endif // !defined(AFX_NEWLANGDLG_H__E30448A5_5CEE_4C25_A14F_CC332C654C78__INCLUDED_)
