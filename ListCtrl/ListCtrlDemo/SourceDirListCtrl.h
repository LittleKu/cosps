#if !defined(AFX_SOURCEDIRLISTCTRL_H__AE0D2435_526E_4DE1_B31F_6E02455BE591__INCLUDED_)
#define AFX_SOURCEDIRLISTCTRL_H__AE0D2435_526E_4DE1_B31F_6E02455BE591__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SourceDirListCtrl.h : header file
//
#include "ThirdParty/CheckListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CSourceDirListCtrl window

#define IDM_SOURCE_DIR_FIRST				48297

#define IDM_SOURCE_DIR_REMOVE				(IDM_SOURCE_DIR_FIRST + 0)
#define IDM_SOURCE_DIR_REMOVE_ALL			(IDM_SOURCE_DIR_FIRST + 1)
#define IDM_SOURCE_DIR_SELECT_ALL			(IDM_SOURCE_DIR_FIRST + 2)
#define IDM_SOURCE_DIR_CHECK				(IDM_SOURCE_DIR_FIRST + 3)
#define IDM_SOURCE_DIR_UNCHECK				(IDM_SOURCE_DIR_FIRST + 4)
#define IDM_SOURCE_DIR_OPEN_DIR				(IDM_SOURCE_DIR_FIRST + 5)

#define IDM_SOURCE_DIR_LAST					IDM_SOURCE_DIR_OPEN_DIR

class CSourceDirListCtrl : public CCheckListCtrl
{
// Construction
public:
	CSourceDirListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSourceDirListCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveHistory();
	void LoadHistory();
	void AddSrcDir(LPCTSTR lpszDir, int nCheckedState = CL_CHECKED);
	void Init();
	void OpenFolder();
	void CheckSelectedItems(BOOL bChecked);
	void SelectAllItems();
	void RemoveSelectedItems();
	virtual ~CSourceDirListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSourceDirListCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOURCEDIRLISTCTRL_H__AE0D2435_526E_4DE1_B31F_6E02455BE591__INCLUDED_)
