// CoolTabDemoDlg.h : header file
//

#if !defined(AFX_COOLTABDEMODLG_H__D570E571_1D4C_4AD4_9355_82E5556D8AB6__INCLUDED_)
#define AFX_COOLTABDEMODLG_H__D570E571_1D4C_4AD4_9355_82E5556D8AB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResMgr.h"
#include "CoolTabCtrl.h"
#include "ImageTabCtrl.h"
#include "BitmapTabCtrl.h"
#include "ResizableDialog.h"
#include "TempDlg.h"
#include "TempDlg2.h"

/////////////////////////////////////////////////////////////////////////////
// CCoolTabDemoDlg dialog

class CCoolTabDemoDlg : public CResizableDialog
{
// Construction
public:
	CCoolTabDemoDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CCoolTabDemoDlg();

// Dialog Data
	//{{AFX_DATA(CCoolTabDemoDlg)
	enum { IDD = IDD_COOLTABDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolTabDemoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
//	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
// Implementation
protected:
	HICON m_hIcon;
	CResMgr m_resMgr;
	TabButtonGroup* m_tabBtnGroup;
	CBitmapTabCtrl m_mainTabCtrl;
	CTempDlg  dlg1;
	CTempDlg2 dlg2;
	// Generated message map functions
	//{{AFX_MSG(CCoolTabDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAppExit();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLTABDEMODLG_H__D570E571_1D4C_4AD4_9355_82E5556D8AB6__INCLUDED_)
