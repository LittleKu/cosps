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
	void UpdateButtonStatus();
	void InitLangGrammarData();
	void InitGUI();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeListLangGrammar();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonModify();
	afx_msg LRESULT OnLangGrammarDlgEditChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void SetModified(BOOL bModified = TRUE);
	BOOL GetModified() { return m_bModified; }
private:
	CDlgTemplate* m_dlgTemplate;
	CLangGrammarDlg* m_pLangGrammarDlg;
	CListBox*	m_pLangListBox;

	CButton* m_pOkButton;
	CButton* m_pCancelButton;
	CButton* m_pApplyButton;
	CButton* m_pDeleteButton;
	CButton* m_pNewButton;
	CButton* m_pModifyButton;

	int m_nCurSelectedIndex;
	int m_nCurEditStatus;
	BOOL m_bModified;
};

#endif // !defined(AFX_SHOWLANGRULEDLG_H__BEE6F4B9_E806_4285_B055_C19A11A44296__INCLUDED_)
