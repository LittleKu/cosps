// ListCtrlDemo.h : main header file for the LISTCTRLDEMO application
//

#if !defined(AFX_LISTCTRLDEMO_H__8AD279F6_DEE8_461E_81C2_49319D058BF3__INCLUDED_)
#define AFX_LISTCTRLDEMO_H__8AD279F6_DEE8_461E_81C2_49319D058BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoApp:
// See ListCtrlDemo.cpp for the implementation of this class
//

class CListCtrlDemoApp : public CWinApp
{
public:
	CListCtrlDemoApp();
	virtual ~CListCtrlDemoApp();
	CBrush*  m_pSysBkBrush;
	CString  m_szPrefIniFile;
	CString  m_szWorkDir;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CListCtrlDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnHelp();          // F1 (uses current context)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void InitWorkDir();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

CString& SYS_PREF_INI_FILE();
CListCtrlDemoApp* SYS_APP();

#endif // !defined(AFX_LISTCTRLDEMO_H__8AD279F6_DEE8_461E_81C2_49319D058BF3__INCLUDED_)
