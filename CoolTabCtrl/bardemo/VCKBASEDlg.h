#if !defined(AFX_VCKBASEDLG_H__2559D334_9FAD_11D5_AF7C_0000E8A3A8FB__INCLUDED_)
#define AFX_VCKBASEDLG_H__2559D334_9FAD_11D5_AF7C_0000E8A3A8FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VCKBASEDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVCKBASEDlg dialog

class CVCKBASEDlg : public CDialog
{
// Construction
public:
	CVCKBASEDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CVCKBASEDlg)

// Dialog Data
	//{{AFX_DATA(CVCKBASEDlg)
	enum { IDD = IDD_DIALOGBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCKBASEDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVCKBASEDlg)
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCKBASEDLG_H__2559D334_9FAD_11D5_AF7C_0000E8A3A8FB__INCLUDED_)
