// ToolBarDemoDlg.h : header file
//

#if !defined(AFX_TOOLBARDEMODLG_H__B01087F1_1766_4689_934D_3958828F835A__INCLUDED_)
#define AFX_TOOLBARDEMODLG_H__B01087F1_1766_4689_934D_3958828F835A__INCLUDED_

#include "ToolBarCtrEx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CToolBarDemoDlg dialog

class CToolBarDemoDlg : public CDialog
{
// Construction
public:
	CToolBarDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CToolBarDemoDlg)
	enum { IDD = IDD_TOOLBARDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CToolBarCtrEx m_toolbar;
	// Generated message map functions
	//{{AFX_MSG(CToolBarDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDEMODLG_H__B01087F1_1766_4689_934D_3958828F835A__INCLUDED_)
