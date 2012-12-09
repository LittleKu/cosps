// FlickerFreeListCtrlDemoDlg.h : header file
//

#if !defined(AFX_FLICKERFREELISTCTRLDEMODLG_H__CA7DCE88_6417_4B91_A39C_625359C013F3__INCLUDED_)
#define AFX_FLICKERFREELISTCTRLDEMODLG_H__CA7DCE88_6417_4B91_A39C_625359C013F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFlickerFreeListCtrlDemoDlg dialog
#include "MyListCtrl.h"

class CFlickerFreeListCtrlDemoDlg : public CDialog
{
// Construction
public:
	CFlickerFreeListCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlickerFreeListCtrlDemoDlg)
	enum { IDD = IDD_FLICKERFREELISTCTRLDEMO_DIALOG };
	MyListCtrl	m_dataListCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlickerFreeListCtrlDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFlickerFreeListCtrlDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg LRESULT OnMyTestMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void InitListCtrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLICKERFREELISTCTRLDEMODLG_H__CA7DCE88_6417_4B91_A39C_625359C013F3__INCLUDED_)
