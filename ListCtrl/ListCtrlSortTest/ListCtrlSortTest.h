// ListCtrlSortTest.h : main header file for the LISTCTRLSORTTEST application
//

#if !defined(AFX_LISTCTRLSORTTEST_H__E7D90604_BAF7_43C9_9A1D_871A7D77A31F__INCLUDED_)
#define AFX_LISTCTRLSORTTEST_H__E7D90604_BAF7_43C9_9A1D_871A7D77A31F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CListCtrlSortTestApp:
// See ListCtrlSortTest.cpp for the implementation of this class
//

class CListCtrlSortTestApp : public CWinApp
{
public:
	CListCtrlSortTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlSortTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CListCtrlSortTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLSORTTEST_H__E7D90604_BAF7_43C9_9A1D_871A7D77A31F__INCLUDED_)
