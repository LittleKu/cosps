// ColorShowDlg.h : header file
//

#if !defined(AFX_COLORSHOWDLG_H__CD623E3A_3710_43AA_AE08_831CFD1BBD83__INCLUDED_)
#define AFX_COLORSHOWDLG_H__CD623E3A_3710_43AA_AE08_831CFD1BBD83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorShowDlg dialog

class CColorShowDlg : public CDialog
{
// Construction
public:
	CColorShowDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CColorShowDlg)
	enum { IDD = IDD_COLORSHOW_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorShowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CColorShowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_color;
	int HexToDec(const CString& str);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSHOWDLG_H__CD623E3A_3710_43AA_AE08_831CFD1BBD83__INCLUDED_)
