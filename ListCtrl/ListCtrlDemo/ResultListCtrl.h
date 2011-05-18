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
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResultListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CResultListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTLISTCTRL_H__BCA37418_4BB2_48C5_B594_6D67DF19BFC7__INCLUDED_)
