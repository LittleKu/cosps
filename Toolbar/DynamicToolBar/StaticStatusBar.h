#if !defined(AFX_STATICSTATUSBAR_H__C7B18C6B_6CD4_4653_8515_39E7F5D97615__INCLUDED_)
#define AFX_STATICSTATUSBAR_H__C7B18C6B_6CD4_4653_8515_39E7F5D97615__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticStatusBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticStatusBar window

class CStaticStatusBar : public CStatic
{
public:
	struct PartData
	{
		int nIndex;
		int nRightEdge;
		CString szText;
		int nDrawType;
	};
	typedef CMap<int, int, PartData*, PartData*> CMapIndex2PartData;
// Construction
public:
	CStaticStatusBar();
	virtual ~CStaticStatusBar();
// Attributes
public:
	COLORREF m_crBk;
	COLORREF m_crTopLeft;
	COLORREF m_crBottomRight;
	COLORREF m_crSep;
	BOOL	 m_bDrawBorder;
// Operations
public:
	BOOL SetParts( int nParts, int* pWidths );
	BOOL SetText( LPCTSTR lpszText, int nPane, int nType = 0);
	BOOL GetRect( int nPane, LPRECT lpRect ) const;
	COLORREF SetBkColor( COLORREF cr );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticStatusBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawSep(CDC* pDC, LPCRECT lpRect);
	virtual void DrawBk(CDC* pDC);
	// Generated message map functions
protected:
	virtual void RemoveAllPartData();
	CMapIndex2PartData m_mapParts;
	//{{AFX_MSG(CStaticStatusBar)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICSTATUSBAR_H__C7B18C6B_6CD4_4653_8515_39E7F5D97615__INCLUDED_)
