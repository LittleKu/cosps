// MultiLangDllDemoDlg.h : header file
//

#if !defined(AFX_MULTILANGDLLDEMODLG_H__9FA2E92C_7F38_4870_97E0_BA42F2FBC429__INCLUDED_)
#define AFX_MULTILANGDLLDEMODLG_H__9FA2E92C_7F38_4870_97E0_BA42F2FBC429__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMultiLangDllDemoDlg dialog

class CMultiLangDllDemoDlg : public CDialog
{
// Construction
public:
	void Localize();
	CMultiLangDllDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiLangDllDemoDlg)
	enum { IDD = IDD_MULTILANGDLLDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiLangDllDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMultiLangDllDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTILANGDLLDEMODLG_H__9FA2E92C_7F38_4870_97E0_BA42F2FBC429__INCLUDED_)
