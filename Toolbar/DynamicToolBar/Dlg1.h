#if !defined(AFX_DLG1_H__821093FF_F6FC_460B_B3FC_E157848B7475__INCLUDED_)
#define AFX_DLG1_H__821093FF_F6FC_460B_B3FC_E157848B7475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg1.h : header file
//
#include "ResizableDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDlg1 dialog

class CDlg1 : public CResizableDialog
{
// Construction
public:
	CDlg1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg1)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG1_H__821093FF_F6FC_460B_B3FC_E157848B7475__INCLUDED_)
