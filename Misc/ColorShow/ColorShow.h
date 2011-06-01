// ColorShow.h : main header file for the COLORSHOW application
//

#if !defined(AFX_COLORSHOW_H__446E42AF_3C37_4DB4_A5D8_F32088F19B90__INCLUDED_)
#define AFX_COLORSHOW_H__446E42AF_3C37_4DB4_A5D8_F32088F19B90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CColorShowApp:
// See ColorShow.cpp for the implementation of this class
//

class CColorShowApp : public CWinApp
{
public:
	CColorShowApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorShowApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CColorShowApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSHOW_H__446E42AF_3C37_4DB4_A5D8_F32088F19B90__INCLUDED_)
