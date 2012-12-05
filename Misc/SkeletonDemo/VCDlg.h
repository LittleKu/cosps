#if !defined(AFX_VCDLG_H__42E8ED94_BE1F_4D7E_B9B2_CAD0618EA58E__INCLUDED_)
#define AFX_VCDLG_H__42E8ED94_BE1F_4D7E_B9B2_CAD0618EA58E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VCDlg.h : header file
//

#include "cflmfc/SplitterControl.h"

/////////////////////////////////////////////////////////////////////////////
// CVCDlg dialog

class CVCDlg : public CResizableDialog
{
// Construction
public:
	CVCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVCDlg)
	enum { IDD = IDD_VC_DLG };
	CStatic	m_deviceComboBox;
	CStatic	m_profileComboBox;
	CStatic	m_wndOutputPropListPos;
	cfl::CSplitterControl	m_splitterTableOutput;
	cfl::CSplitterControl	m_splitterTreeTable;
	CListCtrl	m_taskListCtrl;
	CTreeCtrl	m_taskTreeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitResizableDlgAnchor();
	void InitTaskListCtrl();
	void InitTaskTree();
	void InitSplitters();
	void SetSplitterTreeTableRange();
	void SetSplitterTableOutputRange();
	void DoSizeTreeTable(int delta);
	void DoSizeTableOutput(int delta);

private:
	CBitmap m_splitterVBkBitmap;
	CBitmap m_splitterHBkBitmap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCDLG_H__42E8ED94_BE1F_4D7E_B9B2_CAD0618EA58E__INCLUDED_)
