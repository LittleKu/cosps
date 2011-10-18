// MultiGetDlg.h : header file
//

#if !defined(AFX_MULTIGETDLG_H__C5E60D81_8643_4061_A988_19D6E12FB92F__INCLUDED_)
#define AFX_MULTIGETDLG_H__C5E60D81_8643_4061_A988_19D6E12FB92F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TaskListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMultiGetDlg dialog

class CMultiGetDlg : public CDialog
{
// Construction
public:
	CMultiGetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiGetDlg)
	enum { IDD = IDD_MULTIGET_DIALOG };
	CEdit	m_editUrl;
	CTaskListCtrl	m_taskListCtrl;
	CControlInfo m_controlInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiGetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMultiGetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonAdd();
	virtual void OnOK();
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAdd1();
	afx_msg void OnButtonStart1();
	afx_msg void OnButtonPause();
	afx_msg void OnButton4();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonResume();
	afx_msg void OnButtonHeader();
	afx_msg void OnButtonRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIGETDLG_H__C5E60D81_8643_4061_A988_19D6E12FB92F__INCLUDED_)
