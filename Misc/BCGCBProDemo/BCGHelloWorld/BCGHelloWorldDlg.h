// BCGHelloWorldDlg.h : header file
//

#if !defined(AFX_BCGHELLOWORLDDLG_H__8A04AE0D_CE5B_442B_A9A1_EEF216C9B3BF__INCLUDED_)
#define AFX_BCGHELLOWORLDDLG_H__8A04AE0D_CE5B_442B_A9A1_EEF216C9B3BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBCGHelloWorldDlg dialog
#include "BCGPPropList.h"

class CBCGHelloWorldDlg : public CDialog
{
// Construction
public:
	CBCGHelloWorldDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBCGHelloWorldDlg)
	enum { IDD = IDD_BCGHELLOWORLD_DIALOG };
	CStatic	m_wndPropListLocation;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGHelloWorldDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CBCGPPropList m_wndPropList;
	// Generated message map functions
	//{{AFX_MSG(CBCGHelloWorldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnExpandAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGHELLOWORLDDLG_H__8A04AE0D_CE5B_442B_A9A1_EEF216C9B3BF__INCLUDED_)
