#if !defined(AFX_DRAGSLIDERCTRL_H__D3251B5B_FD1C_4E41_97A6_1B8D992E5B9F__INCLUDED_)
#define AFX_DRAGSLIDERCTRL_H__D3251B5B_FD1C_4E41_97A6_1B8D992E5B9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragSliderCtrl.h : header file
//http://forums.codeguru.com/showthread.php?502637-How-to-implement-thumbnail-dragging-in-CSliderCtrl-derived-class-in-vc
//http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/9b1cdc80-f0e9-4176-9c7d-6ffaa2097262

/////////////////////////////////////////////////////////////////////////////
// CDragSliderCtrl window

class CDragSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CDragSliderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragSliderCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDragSliderCtrl();

protected:
	BOOL	m_bDragging;
	CRect	m_rcClient;
	CRect	m_rcChannel;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDragSliderCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGSLIDERCTRL_H__D3251B5B_FD1C_4E41_97A6_1B8D992E5B9F__INCLUDED_)
