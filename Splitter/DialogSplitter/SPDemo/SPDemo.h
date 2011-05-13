// SPDemo.h : main header file for the SPDEMO application
//

#if !defined(AFX_SPDEMO_H__A56AE7D1_6049_40FE_8BF5_04EC931D409D__INCLUDED_)
#define AFX_SPDEMO_H__A56AE7D1_6049_40FE_8BF5_04EC931D409D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSPDemoApp:
// See SPDemo.cpp for the implementation of this class
//

class CSPDemoApp : public CWinApp
{
public:
	CSPDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSPDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPDEMO_H__A56AE7D1_6049_40FE_8BF5_04EC931D409D__INCLUDED_)
