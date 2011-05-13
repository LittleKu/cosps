// SPDemoDlg.h : header file
//

#if !defined(AFX_SPDEMODLG_H__DF3D3AA3_8536_469C_B6A6_32FDC5549ABD__INCLUDED_)
#define AFX_SPDEMODLG_H__DF3D3AA3_8536_469C_B6A6_32FDC5549ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSPDemoDlg dialog
#include "SplitterControl.h"
#include "MyListCtrl.h"
class CSPDemoDlg : public CDialog
{
// Construction
public:
	CSPDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSPDemoDlg)
	enum { IDD = IDD_SPDEMO_DIALOG };
	CEdit	m_txtContent;
	CMyListCtrl	m_lstItem;
	CTreeCtrl	m_wndType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitSampleData();
	void DoResize1(int delta);
	void DoResize2(int delta);
	HICON m_hIcon;
	CSplitterControl m_wndSplitter1, m_wndSplitter2;
	// Generated message map functions
	//{{AFX_MSG(CSPDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPDEMODLG_H__DF3D3AA3_8536_469C_B6A6_32FDC5549ABD__INCLUDED_)
