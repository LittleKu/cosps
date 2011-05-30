#if !defined(AFX_RESULTLISTCTRL_H__BCA37418_4BB2_48C5_B594_6D67DF19BFC7__INCLUDED_)
#define AFX_RESULTLISTCTRL_H__BCA37418_4BB2_48C5_B594_6D67DF19BFC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultListCtrl.h : header file
//

#include "ThirdParty/MultiColumnSortListView.h"

class CFileInfo;

/////////////////////////////////////////////////////////////////////////////
// CResultListCtrl window

class CResultListCtrl : public CMultiColumnSortListCtrl
{
// Construction
public:
	CResultListCtrl();

// Attributes
public:
	int AddRow(const CFileInfo& fi);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ExportAsExcel(CDatabase& database);
	void Init();
	void OpenFile();
	void OpenFolder();
	virtual ~CResultListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CResultListCtrl)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	CString QMColumnName(LPCTSTR lpStr);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTLISTCTRL_H__BCA37418_4BB2_48C5_B594_6D67DF19BFC7__INCLUDED_)
