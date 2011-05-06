// DynamicToolBarDlg.h : header file
//

#if !defined(AFX_DYNAMICTOOLBARDLG_H__BCDFA58A_ECD6_4A0F_89C1_F4F91FF2B8B2__INCLUDED_)
#define AFX_DYNAMICTOOLBARDLG_H__BCDFA58A_ECD6_4A0F_89C1_F4F91FF2B8B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableDialog.h"
#include "Dlg1.h"
#include "Dlg2.h"
#include "ToolBarCtrEx.h"
#include "MyReBar.h"

#define IDC_TOOLBAR			16127

/////////////////////////////////////////////////////////////////////////////
// CDynamicToolBarDlg dialog

class CDynamicToolBarDlg : public CResizableDialog
{
// Construction
public:
	CDynamicToolBarDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CDynamicToolBarDlg();
	CToolBarCtrEx* toolbar;
	CStatusBarCtrl* statusbar;
	CDlg1* pDlg1;
	CDlg2* pDlg2;
	CWnd*			activewnd;
	CMyReBar		m_ctlMainTopReBar;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
// Dialog Data
	//{{AFX_DATA(CDynamicToolBarDlg)
	enum { IDD = IDD_DYNAMICTOOLBAR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicToolBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void SetActiveDialog(CWnd* dlg);
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDynamicToolBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICTOOLBARDLG_H__BCDFA58A_ECD6_4A0F_89C1_F4F91FF2B8B2__INCLUDED_)
