// DlgRectangleTest.h : main header file for the DLGRECTANGLETEST application
//

#if !defined(AFX_DLGRECTANGLETEST_H__690B0FF7_29B7_4024_B269_A7740A3386A5__INCLUDED_)
#define AFX_DLGRECTANGLETEST_H__690B0FF7_29B7_4024_B269_A7740A3386A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDlgRectangleTestApp:
// See DlgRectangleTest.cpp for the implementation of this class
//

class CDlgRectangleTestApp : public CWinApp
{
public:
	CDlgRectangleTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRectangleTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDlgRectangleTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECTANGLETEST_H__690B0FF7_29B7_4024_B269_A7740A3386A5__INCLUDED_)
