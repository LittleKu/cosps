// CDListCtrlDlg.h : header file
//

#if !defined(AFX_CDLISTCTRLDLG_H__CA50F707_95DD_4936_9669_27F3E9E0D7AA__INCLUDED_)
#define AFX_CDLISTCTRLDLG_H__CA50F707_95DD_4936_9669_27F3E9E0D7AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CDListCtrl1.h"
/////////////////////////////////////////////////////////////////////////////
// CCDListCtrlDlg dialog

class CCDListCtrlDlg : public CDialog
{
// Construction
public:
	CCDListCtrlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCDListCtrlDlg)
	enum { IDD = IDD_CDLISTCTRL_DIALOG };
	CCDListCtrl	m_listCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDListCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCDListCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitListCtrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLISTCTRLDLG_H__CA50F707_95DD_4936_9669_27F3E9E0D7AA__INCLUDED_)
