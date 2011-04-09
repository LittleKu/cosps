#if !defined(AFX_TOOLBARDLG_H__C1DA6A83_7003_4035_9D42_304471B28783__INCLUDED_)
#define AFX_TOOLBARDLG_H__C1DA6A83_7003_4035_9D42_304471B28783__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolbarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMakeToolbar dialog

class CToolbarDlg : public CDialog
{
// Construction
public:
	CToolbarDlg(CWnd *pParent, class CResDoc *pDoc);
   BOOL Create(CWnd *pParent);

// Dialog Data
	//{{AFX_DATA(CToolbarDlg)
	enum { IDD = IDD_TOOLBAR_DLG };
	CString	m_bmpDim;
	int		m_width;
	int		m_images;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   class CResDoc *m_pDoc;
   CSize m_dim;
   BOOL m_init;
   int m_imagesOld;

	// Generated message map functions
	//{{AFX_MSG(CToolbarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnCancel();
	afx_msg void OnChangeEdImages();
	afx_msg void OnUpdateEdWidth();
	virtual void OnOK();
	//}}AFX_MSG
   afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDLG_H__C1DA6A83_7003_4035_9D42_304471B28783__INCLUDED_)
