// MultiGet.h : main header file for the MULTIGET application
//

#if !defined(AFX_MULTIGET_H__4D44A0F9_7FE8_449B_B30F_C59BD9B30DD3__INCLUDED_)
#define AFX_MULTIGET_H__4D44A0F9_7FE8_449B_B30F_C59BD9B30DD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMultiGetApp:
// See MultiGet.cpp for the implementation of this class
//

class CMultiGetApp : public CWinApp
{
public:
	CMultiGetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiGetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMultiGetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIGET_H__4D44A0F9_7FE8_449B_B30F_C59BD9B30DD3__INCLUDED_)