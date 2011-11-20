#if !defined(AFX_TASKLISTCTRL_H__5643868F_4A1F_40BB_B612_8FAE39FDF3FF__INCLUDED_)
#define AFX_TASKLISTCTRL_H__5643868F_4A1F_40BB_B612_8FAE39FDF3FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskListCtrl.h : header file
//

#include "DownloaderMgr.h"

class CTaskInfo
{
public:
	int		m_nTaskID;
	CString m_url;
	CString m_fileName;
	CString m_fileSize;
	CString m_progress;
	CDownloadState m_dlState;
	CDownloaderMgr* m_lpDownloaderMgr;
	CTaskInfo() : m_nTaskID(-1), m_lpDownloaderMgr(0) {}
	~CTaskInfo()
	{
	}
};

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
	void RemoveSelectedItems();
	int AddRow(const CTaskInfo& taskInfo);
	void Init();
	void InvalidateSubItem(int nItem, int nSubItem);
	BOOL GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect);
	virtual ~CTaskListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskListCtrl)
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKLISTCTRL_H__5643868F_4A1F_40BB_B612_8FAE39FDF3FF__INCLUDED_)
