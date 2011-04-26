// CheckLCDemo.h : main header file for the CHECKLCDEMO application
//

#if !defined(AFX_CHECKLCDEMO_H__94784997_3C7A_4022_8DB1_A5441C7E5BAD__INCLUDED_)
#define AFX_CHECKLCDEMO_H__94784997_3C7A_4022_8DB1_A5441C7E5BAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCheckLCDemoApp:
// See CheckLCDemo.cpp for the implementation of this class
//

class CCheckLCDemoApp : public CWinApp
{
public:
	CCheckLCDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckLCDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCheckLCDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLCDEMO_H__94784997_3C7A_4022_8DB1_A5441C7E5BAD__INCLUDED_)
