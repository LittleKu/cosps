// ProgressDlgDemoDlg.h : header file
//

#if !defined(AFX_PROGRESSDLGDEMODLG_H__351F81B1_211F_4AEA_B029_4F464687B2B1__INCLUDED_)
#define AFX_PROGRESSDLGDEMODLG_H__351F81B1_211F_4AEA_B029_4F464687B2B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProgressDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CProgressDlgDemoDlg dialog

class CProgressDlgDemoDlg : public CDialog
{
// Construction
public:
	CProgressDlgDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDlgDemoDlg)
	enum { IDD = IDD_PROGRESSDLGDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlgDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CProgressDlg* m_pProgressDlg;
	// Generated message map functions
	//{{AFX_MSG(CProgressDlgDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLGDEMODLG_H__351F81B1_211F_4AEA_B029_4F464687B2B1__INCLUDED_)
