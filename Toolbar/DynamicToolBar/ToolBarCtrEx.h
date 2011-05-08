#if !defined(AFX_TOOLBARCTREX_H__29290B4E_FF6A_46AF_A4CD_EEE36BA8FC3D__INCLUDED_)
#define AFX_TOOLBARCTREX_H__29290B4E_FF6A_46AF_A4CD_EEE36BA8FC3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarCtrEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarCtrEx window

class CToolBarCtrEx : public CToolBarCtrl
{
// Construction
public:
	CToolBarCtrEx();

// Attributes
public:
	CImageList m_normalIL;
	CImageList m_hotIL;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarCtrEx)
	//}}AFX_VIRTUAL
	virtual BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, COLORREF crMask, UINT nFirstButtonID, UINT nLastButtonID);
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);
	BOOL GetMaxSize(LPSIZE pSize) const;
// Implementation
public:
	virtual ~CToolBarCtrEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarCtrEx)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnAutoSize(WPARAM wParam, LPARAM lParam);
		afx_msg BOOL OnTbnEndAdjust(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARCTREX_H__29290B4E_FF6A_46AF_A4CD_EEE36BA8FC3D__INCLUDED_)
