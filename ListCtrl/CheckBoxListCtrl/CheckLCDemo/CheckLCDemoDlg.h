// CheckLCDemoDlg.h : header file
//

#if !defined(AFX_CHECKLCDEMODLG_H__8F9BBED5_8C3E_447A_ADF4_C4E47DCEC10D__INCLUDED_)
#define AFX_CHECKLCDEMODLG_H__8F9BBED5_8C3E_447A_ADF4_C4E47DCEC10D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CheckListCtrl.h"
#include "CMyDC.h"
#include "MyComboBox.h"
#include "MyGroupBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCheckLCDemoDlg dialog

class CCheckLCDemoDlg : public CDialog
{
// Construction
public:
	CCheckLCDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckLCDemoDlg)
	enum { IDD = IDD_CHECKLCDEMO_DIALOG };
	CMyGroupBox	m_gb1;
	CMyComboBox	m_combobox;
	CCheckListCtrl	m_listCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckLCDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	void wxDrawLine(HDC hdc, int x1, int y1, int x2, int y2);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCheckLCDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLCDEMODLG_H__8F9BBED5_8C3E_447A_ADF4_C4E47DCEC10D__INCLUDED_)
