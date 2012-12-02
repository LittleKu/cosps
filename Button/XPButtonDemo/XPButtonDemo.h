// XPButtonDemo.h : main header file for the XPBUTTONDEMO application
//

#if !defined(AFX_XPBUTTONDEMO_H__709D23F7_E13A_4D46_A5FD_826C394418B7__INCLUDED_)
#define AFX_XPBUTTONDEMO_H__709D23F7_E13A_4D46_A5FD_826C394418B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXPButtonDemoApp:
// See XPButtonDemo.cpp for the implementation of this class
//

class CXPButtonDemoApp : public CWinApp
{
public:
	CXPButtonDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPButtonDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXPButtonDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPBUTTONDEMO_H__709D23F7_E13A_4D46_A5FD_826C394418B7__INCLUDED_)
