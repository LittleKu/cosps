#if !defined(AFX_TASKLISTCTRL_H__5643868F_4A1F_40BB_B612_8FAE39FDF3FF__INCLUDED_)
#define AFX_TASKLISTCTRL_H__5643868F_4A1F_40BB_B612_8FAE39FDF3FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl window

class CTaskListCtrl : public CListCtrl
{
// Construction
public:
	CTaskListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	int AddRow(const CTaskInfo& taskInfo);
	void Init();
	void InvalidateSubItem(int nItem, int nSubItem);
	BOOL GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect);
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

#endif // !defined(AFX_TASKLISTCTRL_H__5643868F_4A1F_40BB_B612_8FAE39FDF3FF__INCLUDED_)
