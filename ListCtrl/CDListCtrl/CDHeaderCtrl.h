#if !defined(AFX_CDHEADERCTRL_H__3B4E6887_51D7_41C7_8DF4_AFA193CD0A3E__INCLUDED_)
#define AFX_CDHEADERCTRL_H__3B4E6887_51D7_41C7_8DF4_AFA193CD0A3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCDHeaderCtrl window
#define DT_DEFAULT	((UINT)-1)		//+++

#define FLATHEADER_TEXT_MAX	80

#define XHEADERCTRL_NO_IMAGE		0
#define XHEADERCTRL_UNCHECKED_IMAGE	1
#define XHEADERCTRL_CHECKED_IMAGE	2

///////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl window

#define FH_PROPERTY_SPACING			1
#define FH_PROPERTY_ARROW			2
#define FH_PROPERTY_STATICBORDER	3
#define FH_PROPERTY_DONTDROPCURSOR	4
#define FH_PROPERTY_DROPTARGET		5

class CCDHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CCDHeaderCtrl();

// Attributes
public:
	CListCtrl * m_pListCtrl;	//+++
	UINT m_nFormat;				//+++
	COLORREF m_rgbText;			//+++
	BOOL m_bDividerLines;		//+++
	BOOL m_bDoubleBuffer;
	BOOL m_bStaticBorder;
	BOOL m_bResizing;
	int m_iSpacing;
	SIZE m_sizeImage;
	SIZE m_sizeArrow;
	UINT m_nDontDropCursor;
	UINT m_nClickFlags;
	CPoint m_ptClickPoint;
	
	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crBtnText;
// Operations
public:
	void SetListCtrl(CListCtrl *pListCtrl) { m_pListCtrl = pListCtrl; }
//	BOOL ModifyProperty(WPARAM wParam, LPARAM lParam);
	int GetSpacing() { return m_iSpacing; }
	void SetSpacing(int nSpacing) { m_iSpacing = nSpacing; }
	UINT GetAlignment() { return m_nFormat; }						//+++
	void SetAlignment(UINT nFormat) { m_nFormat = nFormat; }		//+++
	COLORREF GetTextColor() { return m_rgbText; }					//+++
	void SetTextColor(COLORREF rgbText) { m_rgbText = rgbText; }	//+++
	BOOL GetDividerLines() { return m_bDividerLines; }				//+++
	void EnableDividerLines(BOOL bEnable) { m_bDividerLines = bEnable; }	//+++
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDHeaderCtrl)
	//}}AFX_VIRTUAL
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi);
// Implementation
public:
	virtual ~CCDHeaderCtrl();
	void DrawCtrl(CDC* pDC);
	int DrawImage(CDC* pDC, CRect rect, LPHDITEM hdi, BOOL bRight);
	int DrawBitmap(CDC* pDC, CRect rect, LPHDITEM hdi, CBitmap* pBitmap, 
		BITMAP* pBitmapInfo, BOOL bRight);
	int DrawText (CDC* pDC, CRect rect, LPHDITEM lphdi);
	// Generated message map functions
protected:
	//{{AFX_MSG(CCDHeaderCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDHEADERCTRL_H__3B4E6887_51D7_41C7_8DF4_AFA193CD0A3E__INCLUDED_)
