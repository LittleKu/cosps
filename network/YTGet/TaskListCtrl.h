#if !defined(AFX_TASKLISTCTRL_H__D881C866_B5E3_4794_A735_C5029EFB4A7E__INCLUDED_)
#define AFX_TASKLISTCTRL_H__D881C866_B5E3_4794_A735_C5029EFB4A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskListCtrl.h : header file
//
#include "./ThirdParty/SListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl window

class CTaskListCtrl : public CSListCtrl
{
// Construction
public:
	CTaskListCtrl();

// Attributes
public:
	CImageList m_ILSortImages;
	CImageList m_ILTaskStatus;
// Operations
public:
	void Init();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTaskListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKLISTCTRL_H__D881C866_B5E3_4794_A735_C5029EFB4A7E__INCLUDED_)
