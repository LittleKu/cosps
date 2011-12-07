// MainReBar.h: interface for the CMainReBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINREBAR_H__208583E7_C5F4_4D81_ABA6_B922C87982F3__INCLUDED_)
#define AFX_MAINREBAR_H__208583E7_C5F4_4D81_ABA6_B922C87982F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(AFX_MAINREBAR_H__208583E7_C5F4_4D81_ABA6_B922C87982F3__INCLUDED_)
