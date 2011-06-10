#if !defined(AFX_FILTERTREEMODIFYDLG_H__9C15A06E_32D2_4A1D_9B1C_7445AEE3B409__INCLUDED_)
#define AFX_FILTERTREEMODIFYDLG_H__9C15A06E_32D2_4A1D_9B1C_7445AEE3B409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterTreeModifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterTreeModifyDlg dialog

class CEditPropDlg : public CDialog
{
// Construction
public:
	CEditPropDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_NewNameStr;
	CString m_LabelText;
	CString m_sTitle;
// Dialog Data
	//{{AFX_DATA(CFilterTreeModifyDlg)
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterTreeModifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterTreeModifyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERTREEMODIFYDLG_H__9C15A06E_32D2_4A1D_9B1C_7445AEE3B409__INCLUDED_)
