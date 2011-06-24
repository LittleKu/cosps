#if !defined(AFX_STATICSTATUSBAR_H__C7B18C6B_6CD4_4653_8515_39E7F5D97615__INCLUDED_)
#define AFX_STATICSTATUSBAR_H__C7B18C6B_6CD4_4653_8515_39E7F5D97615__INCLUDED_

#pragma once

// StaticStatusBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticStatusBar window

class AFX_EXT_CLASS CStaticStatusBar : public CStatic
{
public:
	struct PartData
	{
		int nIndex;
		int nRightEdge;
		CString szText;
		int nDrawType;
	};
// Construction
public:
	CStaticStatusBar();
	virtual ~CStaticStatusBar();
// Attributes
public:
	COLORREF m_crBk;
	COLORREF m_crTopSep;
	COLORREF m_crTopLeft;
	COLORREF m_crBottomRight;
	COLORREF m_crBorderSepStart;
	COLORREF m_crBorderSepEnd;
	UINT	 m_nSepWidth;
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
	virtual void DrawSep(CDC* pDC, LPCRECT lpRect);
	virtual void DrawBk(CDC* pDC);
	virtual void RemoveAllPartData();
	// Generated message map functions
protected:
	typedef CMap<int, int, PartData*, PartData*> CMapIndex2PartData;
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
