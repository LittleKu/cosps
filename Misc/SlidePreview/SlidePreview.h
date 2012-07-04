// SlidePreview.h : main header file for the SLIDEPREVIEW application
//

#if !defined(AFX_SLIDEPREVIEW_H__D3CEDA3D_71D1_4152_A735_0625E6271A84__INCLUDED_)
#define AFX_SLIDEPREVIEW_H__D3CEDA3D_71D1_4152_A735_0625E6271A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSlidePreviewApp:
// See SlidePreview.cpp for the implementation of this class
//

class CSlidePreviewApp : public CWinApp
{
public:
	CSlidePreviewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlidePreviewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSlidePreviewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDEPREVIEW_H__D3CEDA3D_71D1_4152_A735_0625E6271A84__INCLUDED_)
