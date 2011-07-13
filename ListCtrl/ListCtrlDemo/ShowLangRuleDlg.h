// ShowLangRuleDlg.h: interface for the CShowLangRuleDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWLANGRULEDLG_H__BEE6F4B9_E806_4285_B055_C19A11A44296__INCLUDED_)
#define AFX_SHOWLANGRULEDLG_H__BEE6F4B9_E806_4285_B055_C19A11A44296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgTemplate;
class CLangGrammarDlg;

class CShowLangRuleDlg : public CDialog
{
public:
	CShowLangRuleDlg();
	virtual ~CShowLangRuleDlg();
protected:
	void InitGUI();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
private:
	CDlgTemplate* m_dlgTemplate;
	CLangGrammarDlg* m_pLangGrammarDlg;
	CListBox*	m_pLangListBox;

	CButton* m_pOkButton;
	CButton* m_pCancelButton;
};

#endif // !defined(AFX_SHOWLANGRULEDLG_H__BEE6F4B9_E806_4285_B055_C19A11A44296__INCLUDED_)
