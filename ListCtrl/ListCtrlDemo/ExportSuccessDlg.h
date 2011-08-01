#if !defined(AFX_EXPORTSUCCESSDLG_H__4B3157C4_0BBF_4361_8341_E7E06C1AE095__INCLUDED_)
#define AFX_EXPORTSUCCESSDLG_H__4B3157C4_0BBF_4361_8341_E7E06C1AE095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportSuccessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportSuccessDlg dialog

class CExportSuccessDlg : public CDialog
{
// Construction
public:
	CExportSuccessDlg(CWnd* pParent = NULL);   // standard constructor
	void SetExportedFile(LPCTSTR lpFileName);
// Dialog Data
	//{{AFX_DATA(CExportSuccessDlg)
	enum { IDD = IDD_EXPORT_RESULT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportSuccessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_szExportedFile;
	// Generated message map functions
	//{{AFX_MSG(CExportSuccessDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonOpenDirectory();
	afx_msg void OnButtonOpenFile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTSUCCESSDLG_H__4B3157C4_0BBF_4361_8341_E7E06C1AE095__INCLUDED_)
