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
class CTaskListComparator;
class CTaskListCtrl : public CSListCtrl
{
// Construction
public:
	CTaskListCtrl();

// Attributes
public:
	CImageList m_ILSortImages;
	CImageList m_ILTaskStatus;
	CImageList m_ILShell;

	CTaskListComparator* m_pComparator;
// Operations
public:
	void Init();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskListCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual CComparator* CreateComparator(CSortCondition* pSortCondtions, int nCount);
// Implementation
public:
	void StopAllTasks();
	void RemoveSelectedItems();
	//Delete Functions
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();

	int  AddRow(CTaskInfo* pTaskInfo);
	void UpdateRow(int nIndex, CTaskInfo* pTaskInfo);
	virtual ~CTaskListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskListCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void GetFileFullPath(int nItem, CString& szFullPath);
	void OpenFolder();
	void OpenFile();
	void InitShellImageList();
	int StatusToILIndex(int nStatus);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKLISTCTRL_H__D881C866_B5E3_4794_A735_C5029EFB4A7E__INCLUDED_)
