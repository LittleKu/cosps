/////////////////////////////////////////////////////////////////////////////
// AdvComboBoxDemo.h : main header file for the ADVCOMBOBOXDEMO application
//
// CAdvComboBox Control
// Version: 2,0
// Date: August 2002
// Author: Mathias Tunared
// Email: Mathias@inorbit.com
// Copyright (c) 2002. All Rights Reserved.
//
// This code, in compiled form or as source code, may be redistributed 
// unmodified PROVIDING it is not sold for profit without the authors 
// written consent, and providing that this notice and the authors name 
// and all copyright notices remains intact.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADVCOMBOBOXDEMO_H__D3488651_1658_4B01_957A_7C90F656E8DA__INCLUDED_)
#define AFX_ADVCOMBOBOXDEMO_H__D3488651_1658_4B01_957A_7C90F656E8DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ResMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CAdvComboBoxDemoApp:
// See AdvComboBoxDemo.cpp for the implementation of this class
//

class CAdvComboBoxDemoApp : public CWinApp
{
public:
	CAdvComboBoxDemoApp();

	CResMgr* GetResMgr();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvComboBoxDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAdvComboBoxDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CResMgr* m_pResMgr;
};

CResMgr* GetSysResMgr();


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVCOMBOBOXDEMO_H__D3488651_1658_4B01_957A_7C90F656E8DA__INCLUDED_)
