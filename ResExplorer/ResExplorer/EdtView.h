// EdtView.h : header file
#if !defined(AFX_EDTVIEW_H__D3396065_4085_4AFD_845D_2CAE6ED6ED44__INCLUDED_)
#define AFX_EDTVIEW_H__D3396065_4085_4AFD_845D_2CAE6ED6ED44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CEdtView view
class CResDoc;

class CEdtView : public CEditView
{
protected:
	CEdtView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEdtView)

// Attributes
public:
	CResDoc* GetDocument();

// Operations
public:
	void Begindrag();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtView)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
   CFont m_font;
   class CParams *m_pParam;
   UINT m_nFlags;
   DWORD m_sel;

	virtual ~CEdtView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEdtView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CEdtView.cpp
inline CResDoc* CEdtView::GetDocument() { return (CResDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDTVIEW_H__D3396065_4085_4AFD_845D_2CAE6ED6ED44__INCLUDED_)
