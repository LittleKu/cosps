// MainDlg.h : header file
//

#if !defined(AFX_MAINDLG_H__10D71ED2_DC89_40F2_AAC4_2CA69F1597B6__INCLUDED_)
#define AFX_MAINDLG_H__10D71ED2_DC89_40F2_AAC4_2CA69F1597B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VCDlg.h"
#include "MainReBar.h"
#include "MainToolBar.h"

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CResizableDialog
{
// Construction
public:
	CMainReBar m_MainReBarCtrl;
	CMainToolBar* m_pMainToolBarCtrl;
	CVCDlg* m_pVCDlg;
	CMainDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMainDlg();

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_SKELETONDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CWnd* InitReBar();
	CWnd* InitMainToolBar();
	void InitGUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__10D71ED2_DC89_40F2_AAC4_2CA69F1597B6__INCLUDED_)
