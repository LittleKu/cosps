// BCGHelloWorld.h : main header file for the BCGHELLOWORLD application
//

#if !defined(AFX_BCGHELLOWORLD_H__0D44197E_2ACC_4CCC_A37C_A97ACDBFF447__INCLUDED_)
#define AFX_BCGHELLOWORLD_H__0D44197E_2ACC_4CCC_A37C_A97ACDBFF447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBCGHelloWorldApp:
// See BCGHelloWorld.cpp for the implementation of this class
//

class CBCGHelloWorldApp : public CWinApp, public CBCGPWorkspace
{
public:
	CBCGHelloWorldApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGHelloWorldApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBCGHelloWorldApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGHELLOWORLD_H__0D44197E_2ACC_4CCC_A37C_A97ACDBFF447__INCLUDED_)
