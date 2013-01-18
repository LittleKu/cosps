#if !defined(AFX_TASKLISTCTRL_H__A466C395_1F8B_40AB_8FC3_49D93B6F523F__INCLUDED_)
#define AFX_TASKLISTCTRL_H__A466C395_1F8B_40AB_8FC3_49D93B6F523F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskListCtrl.h : header file
//

#include "CommonDef.h"
#include "SListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl window

class CTaskListCtrl : public CSListCtrl
{
// Construction
public:
	CTaskListCtrl();

// Attributes
public:
	CImageList m_ILTaskStatus;
	CImageList m_ILShell;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init();

	//Insert Functions
	int  AddRow(CTaskInfo* pTaskInfo);
	void UpdateRow(int nIndex, CTaskInfo* pNewTaskInfo);

	//Delete Functions
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();

	//APIs
	int AddTask(LPCTSTR lpszFileName);

	
	BOOL GetTaskInfo(int nTaskID, int* nIndex, CTaskInfo** ppTaskInfo);
	bool GetMetaInfo(int nTaskID, int nMetaID, std::string& val);

	virtual ~CTaskListCtrl();

private:
	enum TaskInfoColum
	{
		COL_CHECK = 0,
		COL_STATUS,
		COL_FILE_NAME,
		COL_FORMAT,
		COL_DURATION,
		COL_PROGRESS,
		COL_COUNT
	};

	void FormatProgress(double dPercent, CString& rText);
	int StatusToILIndex(int nStatus);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskListCtrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKLISTCTRL_H__A466C395_1F8B_40AB_8FC3_49D93B6F523F__INCLUDED_)
