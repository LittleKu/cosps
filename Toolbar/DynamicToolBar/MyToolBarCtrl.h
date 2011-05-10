#if !defined(AFX_MYTOOLBARCTRL_H__DDC6B735_5848_488C_BDBF_16C9529B559B__INCLUDED_)
#define AFX_MYTOOLBARCTRL_H__DDC6B735_5848_488C_BDBF_16C9529B559B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyToolBarCtrl.h : header file
//
#include "ToolBarCtrlEx.h"

#define IDC_TOOLBARBUTTON	16129

#define	TBBTN_DLG1			(IDC_TOOLBARBUTTON + 0)
#define	TBBTN_DLG2			(IDC_TOOLBARBUTTON + 1)
#define	TBBTN_CHANGESTYLE	(IDC_TOOLBARBUTTON + 2)
#define	TBBTN_REFRESH		(IDC_TOOLBARBUTTON + 3)
#define	TBBTN_DUMMY1		(IDC_TOOLBARBUTTON + 4)
#define	TBBTN_DUMMY2		(IDC_TOOLBARBUTTON + 5)
#define	TBBTN_DUMMY3		(IDC_TOOLBARBUTTON + 6)
#define	TBBTN_DUMMY4		(IDC_TOOLBARBUTTON + 7)
#define	TBBTN_DUMMY5		(IDC_TOOLBARBUTTON + 7)

#define FISRT_TOOLBAR_BUTTON_ID	TBBTN_DLG1
#define LAST_TOOLBAR_BUTTON_ID	TBBTN_DUMMY5

/////////////////////////////////////////////////////////////////////////////
// CMyToolBarCtrl window

class CMyToolBarCtrl : public CToolBarCtrlEx
{
// Construction
public:
	CMyToolBarCtrl();
	void Init();
// Attributes
public:
	CImageList m_normalIL;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyToolBarCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyToolBarCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyToolBarCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOOLBARCTRL_H__DDC6B735_5848_488C_BDBF_16C9529B559B__INCLUDED_)
