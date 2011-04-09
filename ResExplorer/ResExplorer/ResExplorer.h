// ResExplorer.h : main header file for the RESEXPLORER application
//

#if !defined(AFX_RESEXPLORER_H__0A97B32C_1D00_4684_B3B2_96143314B135__INCLUDED_)
#define AFX_RESEXPLORER_H__0A97B32C_1D00_4684_B3B2_96143314B135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CResExplorer:
// See ResExplorer.cpp for the implementation of this class
//

class CResExplorer : public CWinApp
{
public:
	CResExplorer();
   BOOL WaitEndOfWork();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResExplorer)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CResExplorer)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESEXPLORER_H__0A97B32C_1D00_4684_B3B2_96143314B135__INCLUDED_)
