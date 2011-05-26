#if !defined(AFX_MAINREBAR_H__9FF8EFB3_5755_4EC2_A887_0C2BF1925A8F__INCLUDED_)
#define AFX_MAINREBAR_H__9FF8EFB3_5755_4EC2_A887_0C2BF1925A8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainReBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainReBar window

class CMainReBarCtrl : public CReBarCtrl
{
// Construction
public:
	CMainReBarCtrl();

// Attributes
public:
	CBitmap m_bkBitmap;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainReBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainReBarCtrl();

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

#endif // !defined(AFX_MAINREBAR_H__9FF8EFB3_5755_4EC2_A887_0C2BF1925A8F__INCLUDED_)
