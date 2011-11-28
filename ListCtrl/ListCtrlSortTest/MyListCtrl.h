#if !defined(AFX_MYLISTCTRL_H__DC84EB3F_9D8C_4430_B931_F654402614F7__INCLUDED_)
#define AFX_MYLISTCTRL_H__DC84EB3F_9D8C_4430_B931_F654402614F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file
//

#include "SListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window

class CRowData
{
public:
	int m_nC1;
	CString m_szDir;
	int m_nC3;
	CString m_szReserved;
	CRowData()
	{
		m_nC1 = 0;
		m_nC3 = 0;
		m_szDir = "";
		m_szReserved = "";
	}
};

class CMyListCtrl : public CSListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:
	CImageList m_ILSortImages;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddRow(CRowData* pRowData);
	void Init();
	virtual ~CMyListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__DC84EB3F_9D8C_4430_B931_F654402614F7__INCLUDED_)
