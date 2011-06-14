#ifndef _LISTCTRL_DEMO_ABOUT_DLG_H_
#define _LISTCTRL_DEMO_ABOUT_DLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDialog.h : header file
//

#include "./ThirdParty/HyperLink.h"

class CBkgEdit : public CEdit
{
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()	
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog
class CAboutDlg : public CDialog
{
	// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CAboutDialog)
	CStatic	m_ProductVersion;
	CBkgEdit	m_RegInfo;
	CHyperLink	m_HomePage;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAboutDialog)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
	CFont m_fontBold;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _LISTCTRL_DEMO_ABOUT_DLG_H_
