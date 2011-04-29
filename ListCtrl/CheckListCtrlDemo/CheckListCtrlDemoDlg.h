// CheckListCtrlDemoDlg.h : header file
//

#if !defined(AFX_CHECKLISTCTRLDEMODLG_H__9903514F_9A93_4AF4_B30E_035742FDE6FC__INCLUDED_)
#define AFX_CHECKLISTCTRLDEMODLG_H__9903514F_9A93_4AF4_B30E_035742FDE6FC__INCLUDED_

#include "CheckListCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrlDemoDlg dialog

class CCheckListCtrlDemoDlg : public CDialog
{
// Construction
public:
	CCheckListCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckListCtrlDemoDlg)
	enum { IDD = IDD_CHECKLISTCTRLDEMO_DIALOG };
	CCheckListCtrl	m_listCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckListCtrlDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCheckListCtrlDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnCheckbox(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private:
	void InitListCtrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLISTCTRLDEMODLG_H__9903514F_9A93_4AF4_B30E_035742FDE6FC__INCLUDED_)
