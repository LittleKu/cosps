// YTGet.h : main header file for the YTGET application
//

#if !defined(AFX_YTGET_H__4127A1CE_E2B9_450F_99A6_0D9F3476F6FF__INCLUDED_)
#define AFX_YTGET_H__4127A1CE_E2B9_450F_99A6_0D9F3476F6FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ThreadMonitor.h"

/////////////////////////////////////////////////////////////////////////////
// CYTGetApp:
// See YTGet.cpp for the implementation of this class
//

class CYTGetApp : public CWinApp
{
public:
	CYTGetApp();
	virtual ~CYTGetApp();
	CThreadMonitor* GetThreadMonitor();
private:
	void Init();
	void DeInit();
// Attributes
public:
	CBrush*  m_pSysBkBrush;
private:
	CThreadMonitor m_threadMonitor;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYTGetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CYTGetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CYTGetApp* SYS_APP();
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YTGET_H__4127A1CE_E2B9_450F_99A6_0D9F3476F6FF__INCLUDED_)
