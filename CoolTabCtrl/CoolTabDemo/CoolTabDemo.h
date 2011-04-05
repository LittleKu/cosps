// CoolTabDemo.h : main header file for the COOLTABDEMO application
//

#if !defined(AFX_COOLTABDEMO_H__C99F4BFE_6C6B_4692_A546_FC575773CAA3__INCLUDED_)
#define AFX_COOLTABDEMO_H__C99F4BFE_6C6B_4692_A546_FC575773CAA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCoolTabDemoApp:
// See CoolTabDemo.cpp for the implementation of this class
//

class CCoolTabDemoApp : public CWinApp
{
public:
	CCoolTabDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolTabDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCoolTabDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLTABDEMO_H__C99F4BFE_6C6B_4692_A546_FC575773CAA3__INCLUDED_)
