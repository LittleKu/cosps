// MultiLangDllDemo.h : main header file for the MULTILANGDLLDEMO application
//

#if !defined(AFX_MULTILANGDLLDEMO_H__BD8CDDF2_DE96_46F9_BBF1_BC775B94B4BA__INCLUDED_)
#define AFX_MULTILANGDLLDEMO_H__BD8CDDF2_DE96_46F9_BBF1_BC775B94B4BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMultiLangDllDemoApp:
// See MultiLangDllDemo.cpp for the implementation of this class
//

class CMultiLangDllDemoApp : public CWinApp
{
public:
	CMultiLangDllDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiLangDllDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMultiLangDllDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTILANGDLLDEMO_H__BD8CDDF2_DE96_46F9_BBF1_BC775B94B4BA__INCLUDED_)
