// BitmapDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BITMAPDLG_H__F0098613_0573_4FAF_96B5_605CCEC5AC0C__INCLUDED_)
#define AFX_BITMAPDLG_H__F0098613_0573_4FAF_96B5_605CCEC5AC0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
// CBitmapDlg dialog

class CBitmapDlg : public CDialog
{
// Construction
public:
	CBitmapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBitmapDlg)
	enum { IDD = IDD_BITMAP_DLG };
	int		m_bkColor;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBitmapDlg)
	virtual void OnOK();
	//}}AFX_MSG
   afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPDLG_H__F0098613_0573_4FAF_96B5_605CCEC5AC0C__INCLUDED_)
