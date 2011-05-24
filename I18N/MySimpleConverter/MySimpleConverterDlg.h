// MySimpleConverterDlg.h : header file
//

#if !defined(AFX_MYSIMPLECONVERTERDLG_H__F4CD9D45_B117_4B51_B0AC_799507E703FB__INCLUDED_)
#define AFX_MYSIMPLECONVERTERDLG_H__F4CD9D45_B117_4B51_B0AC_799507E703FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMySimpleConverterDlg dialog

class CMySimpleConverterDlg : public CDialog
{
// Construction
public:
	CMySimpleConverterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMySimpleConverterDlg)
	enum { IDD = IDD_MYSIMPLECONVERTER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySimpleConverterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMySimpleConverterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnConvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSIMPLECONVERTERDLG_H__F4CD9D45_B117_4B51_B0AC_799507E703FB__INCLUDED_)
