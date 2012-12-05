// SkeletonDemo.h : main header file for the SKELETONDEMO application
//

#if !defined(AFX_SKELETONDEMO_H__282B64E1_9E1D_4DB3_8836_F76D8DB194A3__INCLUDED_)
#define AFX_SKELETONDEMO_H__282B64E1_9E1D_4DB3_8836_F76D8DB194A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSkeletonDemoApp:
// See SkeletonDemo.cpp for the implementation of this class
//

class CSkeletonDemoApp : public CWinApp
{
public:
	CSkeletonDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkeletonDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSkeletonDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKELETONDEMO_H__282B64E1_9E1D_4DB3_8836_F76D8DB194A3__INCLUDED_)
