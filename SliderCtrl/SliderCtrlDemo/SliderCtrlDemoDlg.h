// SliderCtrlDemoDlg.h : header file
//

#if !defined(AFX_SLIDERCTRLDEMODLG_H__7B27431A_7F71_40CC_A485_3B0C21EA2686__INCLUDED_)
#define AFX_SLIDERCTRLDEMODLG_H__7B27431A_7F71_40CC_A485_3B0C21EA2686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DragSliderCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlDemoDlg dialog

class CSliderCtrlDemoDlg : public CDialog
{
// Construction
public:
	CSliderCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSliderCtrlDemoDlg)
	enum { IDD = IDD_SLIDERCTRLDEMO_DIALOG };
	CDragSliderCtrl	m_mySlider;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderCtrlDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSliderCtrlDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERCTRLDEMODLG_H__7B27431A_7F71_40CC_A485_3B0C21EA2686__INCLUDED_)
