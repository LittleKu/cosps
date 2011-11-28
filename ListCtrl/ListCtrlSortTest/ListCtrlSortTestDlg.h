// ListCtrlSortTestDlg.h : header file
//

#if !defined(AFX_LISTCTRLSORTTESTDLG_H__B891DFBB_0721_42B0_82BC_B05D14C07623__INCLUDED_)
#define AFX_LISTCTRLSORTTESTDLG_H__B891DFBB_0721_42B0_82BC_B05D14C07623__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CListCtrlSortTestDlg dialog
#include "MyListCtrl.h"

class CListCtrlSortTestDlg : public CDialog
{
// Construction
public:
	CListCtrlSortTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CListCtrlSortTestDlg)
	enum { IDD = IDD_LISTCTRLSORTTEST_DIALOG };
	CMyListCtrl	m_myListCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlSortTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CListCtrlSortTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLSORTTESTDLG_H__B891DFBB_0721_42B0_82BC_B05D14C07623__INCLUDED_)
