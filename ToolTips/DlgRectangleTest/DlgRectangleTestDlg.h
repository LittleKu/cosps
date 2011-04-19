// DlgRectangleTestDlg.h : header file
//

#if !defined(AFX_DLGRECTANGLETESTDLG_H__7A6FF80F_F3EC_4623_9E6C_BF21D26E79B7__INCLUDED_)
#define AFX_DLGRECTANGLETESTDLG_H__7A6FF80F_F3EC_4623_9E6C_BF21D26E79B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgRectangleTestDlg dialog

#include "PPTooltip.h"
#include "hyperlink.h"

class CDlgRectangleTestDlg : public CDialog
{
// Construction
public:
	CDlgRectangleTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRectangleTestDlg)
	enum { IDD = IDD_DLGRECTANGLETEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRectangleTestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
//	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDlgRectangleTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPPToolTip m_tooltip;
	CHyperLink m_link;
	CRect m_rect[2];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECTANGLETESTDLG_H__7A6FF80F_F3EC_4623_9E6C_BF21D26E79B7__INCLUDED_)
