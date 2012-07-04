// WinBitmapTestDlg.h : header file
//

#if !defined(AFX_WINBITMAPTESTDLG_H__64D78B1D_D94C_410E_8D21_E0F31D5A8B41__INCLUDED_)
#define AFX_WINBITMAPTESTDLG_H__64D78B1D_D94C_410E_8D21_E0F31D5A8B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PreviewCtrl.h"
#include "TestStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CWinBitmapTestDlg dialog

class CWinBitmapTestDlg : public CDialog
{
// Construction
public:
	CWinBitmapTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWinBitmapTestDlg)
	enum { IDD = IDD_WINBITMAPTEST_DIALOG };
	CTestStatic	m_testStatic;
	CPreviewCtrl	m_previewCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinBitmapTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinBitmapTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMbitmap();
	afx_msg void OnMbitamp2();
	afx_msg void OnStartPrev();
	afx_msg void OnStopPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBitmap bmpPrev1;
	CBitmap bmpPrev2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINBITMAPTESTDLG_H__64D78B1D_D94C_410E_8D21_E0F31D5A8B41__INCLUDED_)
