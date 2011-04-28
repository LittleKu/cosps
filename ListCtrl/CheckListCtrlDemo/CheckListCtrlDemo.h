// CheckListCtrlDemo.h : main header file for the CHECKLISTCTRLDEMO application
//

#if !defined(AFX_CHECKLISTCTRLDEMO_H__8CB5BBE3_5452_4A33_9A11_9DF67DAD4830__INCLUDED_)
#define AFX_CHECKLISTCTRLDEMO_H__8CB5BBE3_5452_4A33_9A11_9DF67DAD4830__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrlDemoApp:
// See CheckListCtrlDemo.cpp for the implementation of this class
//

class CCheckListCtrlDemoApp : public CWinApp
{
public:
	CCheckListCtrlDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckListCtrlDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCheckListCtrlDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLISTCTRLDEMO_H__8CB5BBE3_5452_4A33_9A11_9DF67DAD4830__INCLUDED_)
