#if !defined(AFX_YTGETDLG_H__551EA730_0691_4EC6_8B7B_CDB165ED8879__INCLUDED_)
#define AFX_YTGETDLG_H__551EA730_0691_4EC6_8B7B_CDB165ED8879__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YTGetDlg.h : header file
//
#include "TaskListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CYTGetDlg dialog

class CYTGetDlg : public CResizableDialog
{
// Construction
public:
	void ListCtrlSelectionChanged();
	void AddTask(LPCTSTR lpszAddress);
	void Restart();
	void Remove();
	void Pause();
	void Start();
	void AddBatchTask();
	void AddTask();
	CYTGetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYTGetDlg)
	enum { IDD = IDD_YTGET_DIALOG };
	CTaskListCtrl	m_taskListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYTGetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL GetTaskInfo(int nTaskID, int* nIndex, CTaskInfo** ppTaskInfo);

	// Generated message map functions
	//{{AFX_MSG(CYTGetDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnItemchangedTaskList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void EnableButtons(DWORD dwStatus);
	void InitResizableDlgAnchor();
	void InitTaskListCtrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YTGETDLG_H__551EA730_0691_4EC6_8B7B_CDB165ED8879__INCLUDED_)
