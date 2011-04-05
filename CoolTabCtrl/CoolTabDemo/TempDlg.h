#if !defined(AFX_TEMPDLG_H__F97FE3F5_4123_44EF_AFD0_1E2511B20061__INCLUDED_)
#define AFX_TEMPDLG_H__F97FE3F5_4123_44EF_AFD0_1E2511B20061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TempDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTempDlg dialog

class CTempDlg : public CDialog
{
// Construction
public:
	CTempDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CTempDlg)
// Dialog Data
	//{{AFX_DATA(CTempDlg)
	enum { IDD = IDD_DIALOG_TEMP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTempDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTempDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPDLG_H__F97FE3F5_4123_44EF_AFD0_1E2511B20061__INCLUDED_)
