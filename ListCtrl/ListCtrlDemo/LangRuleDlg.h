// LangRuleDlg.h: interface for the CLangRuleDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGRULEDLG_H__114D274C_764A_4D0C_BA7F_19878C52E8CA__INCLUDED_)
#define AFX_LANGRULEDLG_H__114D274C_764A_4D0C_BA7F_19878C52E8CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgTemplate;
class CLangGrammarDlg;

class CLangRuleDlg  : public CDialog
{
public:
	CLangRuleDlg();
	virtual ~CLangRuleDlg();
	void CreateModeless(CWnd* pParentWnd = NULL);
	void AddMsgReceiver(HWND hWnd) { m_hWndMsgReceiver = hWnd; }
	static UINT WM_LANG_SEL_CHANGED;
protected:
	void AddStringToListBox(LPCTSTR lpszString, DWORD nItemData, CDC* pDC);
	virtual void InitGUI();
	void InitLangGrammarData();
	void SelChanged(int nIndex);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListLangGrammar();
	afx_msg LRESULT OnLangSelChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
	//The dialog template
	CDlgTemplate* m_dlgTemplate;
	//Lang Grammar Dialog component
	CLangGrammarDlg* m_pLangGrammarDlg;
	//Lang Selection ListBox
	CListBox*	m_pLangListBox;
	//BkWnd for render ListBox
	CBkWnd* m_pListBoxBkWnd;
	//The current selected ListBox index
	int m_nCurSelectedIndex;

	//The wnd which receives the message when selection changed
	HWND m_hWndMsgReceiver;
};

#endif // !defined(AFX_LANGRULEDLG_H__114D274C_764A_4D0C_BA7F_19878C52E8CA__INCLUDED_)
