// SliderCtrlDemo.h : main header file for the SLIDERCTRLDEMO application
//

#if !defined(AFX_SLIDERCTRLDEMO_H__F1484526_5D38_406A_B36A_30DB91712083__INCLUDED_)
#define AFX_SLIDERCTRLDEMO_H__F1484526_5D38_406A_B36A_30DB91712083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlDemoApp:
// See SliderCtrlDemo.cpp for the implementation of this class
//

class CSliderCtrlDemoApp : public CWinApp
{
public:
	CSliderCtrlDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderCtrlDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSliderCtrlDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERCTRLDEMO_H__F1484526_5D38_406A_B36A_30DB91712083__INCLUDED_)
