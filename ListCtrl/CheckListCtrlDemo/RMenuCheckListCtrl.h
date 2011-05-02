#if !defined(AFX_RMENUCHECKLISTCTRL_H__7B8D911A_8A0C_42FC_A17A_0506A9BB6323__INCLUDED_)
#define AFX_RMENUCHECKLISTCTRL_H__7B8D911A_8A0C_42FC_A17A_0506A9BB6323__INCLUDED_

#include "CheckListCtrl.h"
#include "BCMenu.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RMenuCheckListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRMenuCheckListCtrl window

class CRMenuCheckListCtrl : public CCheckListCtrl
{
// Construction
public:
	CRMenuCheckListCtrl();

// Attributes
public:
	BCMenu m_popupMenu;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRMenuCheckListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRMenuCheckListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRMenuCheckListCtrl)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RMENUCHECKLISTCTRL_H__7B8D911A_8A0C_42FC_A17A_0506A9BB6323__INCLUDED_)
