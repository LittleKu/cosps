#if !defined(AFX_EVALUATIONLIMITDLG_H__5E27A7C6_F8A9_4E1D_800F_94B701463405__INCLUDED_)
#define AFX_EVALUATIONLIMITDLG_H__5E27A7C6_F8A9_4E1D_800F_94B701463405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EvaluationLimitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEvaluationLimitDlg dialog

class CEvaluationLimitDlg : public CDialog
{
// Construction
public:
	CEvaluationLimitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEvaluationLimitDlg)
	enum { IDD = IDD_EVALUATION_LIMIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvaluationLimitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_fontBold;
	// Generated message map functions
	//{{AFX_MSG(CEvaluationLimitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonOrder();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVALUATIONLIMITDLG_H__5E27A7C6_F8A9_4E1D_800F_94B701463405__INCLUDED_)
