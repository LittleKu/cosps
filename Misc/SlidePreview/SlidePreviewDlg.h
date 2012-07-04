// SlidePreviewDlg.h : header file
//

#if !defined(AFX_SLIDEPREVIEWDLG_H__50370488_C1BD_4B92_A9DB_7766DCE491E7__INCLUDED_)
#define AFX_SLIDEPREVIEWDLG_H__50370488_C1BD_4B92_A9DB_7766DCE491E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PreviewCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSlidePreviewDlg dialog

class CSlidePreviewDlg : public CDialog
{
// Construction
public:
	CSlidePreviewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSlidePreviewDlg)
	enum { IDD = IDD_SLIDEPREVIEW_DIALOG };
	CPreviewCtrl m_prevCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlidePreviewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSlidePreviewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartPrev();
	afx_msg void OnStopPrev();
	afx_msg void OnSaveFrames();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bitmap2;
	CBitmap m_bitmap1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDEPREVIEWDLG_H__50370488_C1BD_4B92_A9DB_7766DCE491E7__INCLUDED_)
