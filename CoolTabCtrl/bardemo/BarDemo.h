// BarDemo.h : main header file for the BARDEMO application
//

#if !defined(AFX_BARDEMO_H__2559D326_9FAD_11D5_AF7C_0000E8A3A8FB__INCLUDED_)
#define AFX_BARDEMO_H__2559D326_9FAD_11D5_AF7C_0000E8A3A8FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBarDemoApp:
// See BarDemo.cpp for the implementation of this class
//

class CBarDemoApp : public CWinApp
{
public:
	CBarDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CBarDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARDEMO_H__2559D326_9FAD_11D5_AF7C_0000E8A3A8FB__INCLUDED_)
