// CDListCtrl.h : main header file for the CDLISTCTRL application
//

#if !defined(AFX_CDLISTCTRL_H__5922153D_6E28_40A3_ABC7_39A8F2AE6FAE__INCLUDED_)
#define AFX_CDLISTCTRL_H__5922153D_6E28_40A3_ABC7_39A8F2AE6FAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCDListCtrlApp:
// See CDListCtrl.cpp for the implementation of this class
//

class CCDListCtrlApp : public CWinApp
{
public:
	CCDListCtrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDListCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCDListCtrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLISTCTRL_H__5922153D_6E28_40A3_ABC7_39A8F2AE6FAE__INCLUDED_)
