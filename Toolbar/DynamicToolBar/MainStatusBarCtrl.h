#if !defined(AFX_MAINSTATUSBARCTRL_H__1D745BFF_F047_4B4D_AABA_A82F60C060A7__INCLUDED_)
#define AFX_MAINSTATUSBARCTRL_H__1D745BFF_F047_4B4D_AABA_A82F60C060A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainStatusBarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainStatusBarCtrl window

class CMainStatusBarCtrl : public CStatusBarCtrl
{
// Construction
public:
	CMainStatusBarCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainStatusBarCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainStatusBarCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainStatusBarCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINSTATUSBARCTRL_H__1D745BFF_F047_4B4D_AABA_A82F60C060A7__INCLUDED_)
