// bmp2aviDlg.h : header file
//

#if !defined(AFX_BMP2AVIDLG_H__3E6E615D_D536_41D3_B6D6_A6B00132082A__INCLUDED_)
#define AFX_BMP2AVIDLG_H__3E6E615D_D536_41D3_B6D6_A6B00132082A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBmp2aviDlg dialog

class CBmp2aviDlg : public CDialog
{
// Construction
public:
	LONG BMPtoAVI(CString szAVIName, CString strBmpDir);
	void AVItoBmp(CString strAVIFileName, CString strBmpDir);
	CBmp2aviDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBmp2aviDlg)
	enum { IDD = IDD_BMP2AVI_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmp2aviDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBmp2aviDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAviToBmp();
	afx_msg void OnBmpToAvi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMP2AVIDLG_H__3E6E615D_D536_41D3_B6D6_A6B00132082A__INCLUDED_)
