// MainDlg.h : header file
//

#if !defined(AFX_MAINDLG_H__68E84B2D_98FD_4FE5_B53C_CD0DB7C0F70B__INCLUDED_)
#define AFX_MAINDLG_H__68E84B2D_98FD_4FE5_B53C_CD0DB7C0F70B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

#include "YTGetDlg.h"
#include "MainToolBarCtrl.h"
#include "MainReBar.h"

#define IDC_TOOLBAR			18880

class CMainDlg : public CResizableDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMainDlg();
	CYTGetDlg* m_pYTGetDlg;
	CMainToolBarCtrl* m_pMainToolBarCtrl;
	CMainReBarCtrl m_MainReBarCtrl;
// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
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
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetMenuBarBkg();
	CWnd* InitReBar();
	void InitGUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__68E84B2D_98FD_4FE5_B53C_CD0DB7C0F70B__INCLUDED_)
