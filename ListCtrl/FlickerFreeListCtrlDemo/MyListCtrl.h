#if !defined(AFX_MYLISTCTRL_H__B93ADAB9_8427_4BB3_9FA7_791DFA309151__INCLUDED_)
#define AFX_MYLISTCTRL_H__B93ADAB9_8427_4BB3_9FA7_791DFA309151__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file
//

#include "FlickerFreeListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// MyListCtrl window

class MyListCtrl : public CFlickerFreeListCtrl
{
// Construction
public:
	MyListCtrl();

// Attributes
public:

// Operations
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//Insert Functions
	int  AddRow(CTaskInfo* pTaskInfo);
	void UpdateRow(int nIndex, CTaskInfo* pTaskInfo);
	
	//Delete Functions
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MyListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__B93ADAB9_8427_4BB3_9FA7_791DFA309151__INCLUDED_)
