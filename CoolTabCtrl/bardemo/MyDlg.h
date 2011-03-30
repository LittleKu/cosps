#if !defined(AFX_MYDLG_H__C2D2DCC5_AE7F_4C35_8729_9C8395D2FA9B__INCLUDED_)
#define AFX_MYDLG_H__C2D2DCC5_AE7F_4C35_8729_9C8395D2FA9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CMyDlg : public CDialog
{
// Construction
public:
	CMyDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CMyDlg)
		
// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_DIALOGBAR2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDLG_H__C2D2DCC5_AE7F_4C35_8729_9C8395D2FA9B__INCLUDED_)
