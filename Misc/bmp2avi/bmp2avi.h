// bmp2avi.h : main header file for the BMP2AVI application
//

#if !defined(AFX_BMP2AVI_H__A12C132B_65CF_453E_AB6E_C0914200CE4C__INCLUDED_)
#define AFX_BMP2AVI_H__A12C132B_65CF_453E_AB6E_C0914200CE4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBmp2aviApp:
// See bmp2avi.cpp for the implementation of this class
//

class CBmp2aviApp : public CWinApp
{
public:
	CBmp2aviApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmp2aviApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBmp2aviApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMP2AVI_H__A12C132B_65CF_453E_AB6E_C0914200CE4C__INCLUDED_)
