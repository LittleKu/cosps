#if !defined(AFX_MAINTOOLBARCTRL_H__7D4FFCCA_24C7_4EFE_8DDB_AAF0E7EFADB5__INCLUDED_)
#define AFX_MAINTOOLBARCTRL_H__7D4FFCCA_24C7_4EFE_8DDB_AAF0E7EFADB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainToolBarCtrl.h : header file
//

#include "ThirdParty/ToolBarCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CMainToolBarCtrl window

class CMainToolBarCtrl : public CToolBarCtrlEx
{
// Construction
public:
	CMainToolBarCtrl();

// Attributes
public:
	CImageList m_normalIL;
// Operations
public:
	void Init();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainToolBarCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainToolBarCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainToolBarCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINTOOLBARCTRL_H__7D4FFCCA_24C7_4EFE_8DDB_AAF0E7EFADB5__INCLUDED_)
