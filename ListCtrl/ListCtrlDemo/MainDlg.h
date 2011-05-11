#if !defined(AFX_MAINDLG_H__A562221D_B4E5_4673_B7FF_EFE117374C97__INCLUDED_)
#define AFX_MAINDLG_H__A562221D_B4E5_4673_B7FF_EFE117374C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//
#include "ListCtrlDemoDlg.h"
#include "MainToolBarCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog
#define IDC_TOOLBAR			18880

class CMainDlg : public CResizableDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();
	CListCtrlDemoDlg* m_pCounterDlg;
	CMainToolBarCtrl* m_pMainToolBarCtrl;
	CReBarCtrl m_MainReBarCtrl;
// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	void SetMenuBarBkg();
	void InitGUI();
	CWnd* InitReBar();
	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__A562221D_B4E5_4673_B7FF_EFE117374C97__INCLUDED_)
