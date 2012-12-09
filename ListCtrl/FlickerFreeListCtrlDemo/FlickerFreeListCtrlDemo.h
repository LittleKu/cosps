// FlickerFreeListCtrlDemo.h : main header file for the FLICKERFREELISTCTRLDEMO application
//

#if !defined(AFX_FLICKERFREELISTCTRLDEMO_H__6FFF014C_D6B0_420C_BD10_8137D6C16AB8__INCLUDED_)
#define AFX_FLICKERFREELISTCTRLDEMO_H__6FFF014C_D6B0_420C_BD10_8137D6C16AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFlickerFreeListCtrlDemoApp:
// See FlickerFreeListCtrlDemo.cpp for the implementation of this class
//

class CFlickerFreeListCtrlDemoApp : public CWinApp
{
public:
	CFlickerFreeListCtrlDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlickerFreeListCtrlDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFlickerFreeListCtrlDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLICKERFREELISTCTRLDEMO_H__6FFF014C_D6B0_420C_BD10_8137D6C16AB8__INCLUDED_)
