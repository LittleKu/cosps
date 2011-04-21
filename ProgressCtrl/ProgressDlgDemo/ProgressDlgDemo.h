// ProgressDlgDemo.h : main header file for the PROGRESSDLGDEMO application
//

#if !defined(AFX_PROGRESSDLGDEMO_H__CE96CE34_EF19_487C_9A22_4970C97539ED__INCLUDED_)
#define AFX_PROGRESSDLGDEMO_H__CE96CE34_EF19_487C_9A22_4970C97539ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CProgressDlgDemoApp:
// See ProgressDlgDemo.cpp for the implementation of this class
//

class CProgressDlgDemoApp : public CWinApp
{
public:
	CProgressDlgDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlgDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProgressDlgDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLGDEMO_H__CE96CE34_EF19_487C_9A22_4970C97539ED__INCLUDED_)
