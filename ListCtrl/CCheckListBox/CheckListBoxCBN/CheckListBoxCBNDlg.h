// CheckListBoxCBNDlg.h : header file
//

#if !defined(AFX_CHECKLISTBOXCBNDLG_H__ADB1995A_6CE0_4686_B6B3_1FE2B12E1B89__INCLUDED_)
#define AFX_CHECKLISTBOXCBNDLG_H__ADB1995A_6CE0_4686_B6B3_1FE2B12E1B89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCheckListBoxCBNDlg dialog

class CCheckListBoxCBNDlg : public CDialog
{
// Construction
public:
	CCheckListBoxCBNDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckListBoxCBNDlg)
	enum { IDD = IDD_CHECKLISTBOXCBN_DIALOG };
	//}}AFX_DATA
	CCheckListBox	m_ctlCheckList;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckListBoxCBNDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCheckListBoxCBNDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeList1();
	afx_msg void OnCheckchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLISTBOXCBNDLG_H__ADB1995A_6CE0_4686_B6B3_1FE2B12E1B89__INCLUDED_)
