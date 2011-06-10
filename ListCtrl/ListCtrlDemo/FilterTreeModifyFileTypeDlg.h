#if !defined(AFX_FILTERTREEMODIFYFILETYPEDLG_H__F62265AF_662F_4B62_B918_DDFB56A1932C__INCLUDED_)
#define AFX_FILTERTREEMODIFYFILETYPEDLG_H__F62265AF_662F_4B62_B918_DDFB56A1932C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterTreeModifyFileTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterTreeModifyFileTypeDlg dialog

class CFilterTreeModifyFileTypeDlg : public CDialog
{
// Construction
public:
	CFilterTreeModifyFileTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterTreeModifyFileTypeDlg)
	CString m_sTitle;
	CString	m_NewNameStr;
	CString	m_FilterFileTypeStr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterTreeModifyFileTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterTreeModifyFileTypeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERTREEMODIFYFILETYPEDLG_H__F62265AF_662F_4B62_B918_DDFB56A1932C__INCLUDED_)
