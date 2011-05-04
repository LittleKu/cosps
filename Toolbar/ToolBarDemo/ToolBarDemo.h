// ToolBarDemo.h : main header file for the TOOLBARDEMO application
//

#if !defined(AFX_TOOLBARDEMO_H__3F65D907_63A7_49DA_878B_C9C429604D27__INCLUDED_)
#define AFX_TOOLBARDEMO_H__3F65D907_63A7_49DA_878B_C9C429604D27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CToolBarDemoApp:
// See ToolBarDemo.cpp for the implementation of this class
//

class CToolBarDemoApp : public CWinApp
{
public:
	CToolBarDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CToolBarDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDEMO_H__3F65D907_63A7_49DA_878B_C9C429604D27__INCLUDED_)
