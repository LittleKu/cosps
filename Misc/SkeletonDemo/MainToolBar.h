#if !defined(AFX_MAINTOOLBAR_H__1D91EACC_1DA7_4505_BA91_B77480C8EAF1__INCLUDED_)
#define AFX_MAINTOOLBAR_H__1D91EACC_1DA7_4505_BA91_B77480C8EAF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainToolBar.h : header file
//

#include "ToolBarCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CMainToolBar window

class CMainToolBar : public CToolBarCtrlEx
{
// Construction
public:
	CMainToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CImageList m_normalIL;
	void Init();
	virtual ~CMainToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainToolBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINTOOLBAR_H__1D91EACC_1DA7_4505_BA91_B77480C8EAF1__INCLUDED_)
