#if !defined(AFX_MYREBAR_H__8B6DB37B_68C5_4E06_A996_F242E1A28A28__INCLUDED_)
#define AFX_MYREBAR_H__8B6DB37B_68C5_4E06_A996_F242E1A28A28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyReBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyReBar window

class CMyReBar : public CReBarCtrl
{
// Construction
public:
	CMyReBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyReBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	HBITMAP m_bmpBack;
	void UpdateBackground();
	virtual ~CMyReBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyReBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYREBAR_H__8B6DB37B_68C5_4E06_A996_F242E1A28A28__INCLUDED_)
