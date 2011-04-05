#if !defined(AFX_TEMPDLG2_H__735F5404_8431_4174_8F4C_A00FAA9EB7BF__INCLUDED_)
#define AFX_TEMPDLG2_H__735F5404_8431_4174_8F4C_A00FAA9EB7BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TempDlg2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTempDlg2 dialog
#include "ResizableDialog.h"
class CTempDlg2 : public CResizableDialog
{
// Construction
public:
	CTempDlg2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTempDlg2)
	enum { IDD = IDD_DIALOG_TEMP2 };
	CButton	m_btn2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTempDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTempDlg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPDLG2_H__735F5404_8431_4174_8F4C_A00FAA9EB7BF__INCLUDED_)
