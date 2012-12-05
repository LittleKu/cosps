#if !defined(AFX_MAINREBAR_H__923A789F_40F0_4CC0_8BEB_2DFF75239203__INCLUDED_)
#define AFX_MAINREBAR_H__923A789F_40F0_4CC0_8BEB_2DFF75239203__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainReBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainReBar window

class CMainReBar : public CReBarCtrl
{
// Construction
public:
	CMainReBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainReBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainReBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainReBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINREBAR_H__923A789F_40F0_4CC0_8BEB_2DFF75239203__INCLUDED_)
