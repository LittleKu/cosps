// DrawView.h : interface of the CDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWVIEW_H__4673FBA4_58A6_43DA_B46E_4D55D06EED6A__INCLUDED_)
#define AFX_DRAWVIEW_H__4673FBA4_58A6_43DA_B46E_4D55D06EED6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawView : public CScrollView
{
protected: // create from serialization only
   CSize DrawText(CDC* pDC, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
	CDrawView();
	DECLARE_DYNCREATE(CDrawView)

// Attributes
public:
	CResDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CSize DrawAniIcon(CDC*pDC, ANI_ICON*pAni, DWORD item);
	virtual ~CDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   UINT    m_nFlags;
   CParams *m_pParam;
   CFont m_font;
   CPosObj m_objects;
   BOOL    m_enCopy;
   BOOL    m_diag;
   BOOL    m_hand;
   int     m_rDraw;
   BOOL    m_invalid;
   int     m_select;
   int     m_nbImage;
   DWORD   m_type;

   LPCTSTR m_cbItem; // resource name selected for copy/past or drag/drop

// Generated message map functions
protected:
	void CacheBitmapCopy(BOOL cb);
	int m_timer;
	//{{AFX_MSG(CDrawView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnIconBkcolor(UINT id);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DrawView.cpp
inline CResDoc* CDrawView::GetDocument() { return (CResDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWVIEW_H__4673FBA4_58A6_43DA_B46E_4D55D06EED6A__INCLUDED_)
