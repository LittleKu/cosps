// NewFilterGroupDlg.h: interface for the CNewFilterGroupDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWFILTERGROUPDLG_H__98B80DB9_FC75_4701_9BB0_684DD2AFD4A8__INCLUDED_)
#define AFX_NEWFILTERGROUPDLG_H__98B80DB9_FC75_4701_9BB0_684DD2AFD4A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgTemplate;
class CLangRuleDlg;

class CNewFilterGroupDlg  : public CDialog
{
public:
	CNewFilterGroupDlg();
	virtual ~CNewFilterGroupDlg();
protected:
	virtual void InitGUI();
	void InitComboBox();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboBox();
	afx_msg LRESULT OnLangListBoxSelChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void CalcLabelSize(CSize& szMax);
	void ComboBoxSelChanged(int nIndex, BOOL bSendMsg = FALSE);
protected:
	//The dialog template
	CDlgTemplate* m_dlgTemplate;
	CLangRuleDlg* m_pLangRuleDlg;

	CStatic* m_pFilterNameLabel;
	CStatic* m_pLangRuleTypeLabel;
	CEdit* m_pFilterName;
	CComboBox* m_pLangRuleType;

	CButton* m_pOkButton;
	CButton* m_pCancelButton;

	//The current selected index
	int m_nCurSelectedIndex;
};

#endif // !defined(AFX_NEWFILTERGROUPDLG_H__98B80DB9_FC75_4701_9BB0_684DD2AFD4A8__INCLUDED_)
