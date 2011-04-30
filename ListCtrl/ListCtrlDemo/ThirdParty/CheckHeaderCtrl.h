#if !defined(AFX_CHECKHEADERCTRL_H__886B1C45_562B_411B_BE8B_E8CE26FF444D__INCLUDED_)
#define AFX_CHECKHEADERCTRL_H__886B1C45_562B_411B_BE8B_E8CE26FF444D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckHeaderCtrl window
#define DT_DEFAULT	((UINT)-1)		//+++

#define FLATHEADER_TEXT_MAX	    80

#define CL_NONE_CHECK_BOX		    0
#define CL_UNCHECKED	            1
#define CL_CHECKED	                2

class CCheckHeaderCtrl : public CHeaderCtrl
{
	friend class CCheckListCtrl;
// Construction
public:
	CCheckHeaderCtrl();

// Attributes
public:
	COLORREF m_rgbText;
	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crBtnText;

	UINT m_nFormat;
	BOOL m_bDividerLines;
	int  m_nSpace;
	SIZE m_sizeImage;
	BOOL m_bTrack;
	int  m_nHeight;
// Operations
public:
	void SetHeight(int nHeight);
	int  GetHeight();
	void CalcCheckBoxRect(int nSubItem, CRect& checkboxRect, BOOL bCenter = FALSE, int h = 13);
	static void CalcCheckBoxRect(const CRect& boundRect, CRect& checkboxRect, BOOL bCenter = FALSE, int h = 13);
	static void DrawCheckBox(CDC* pDC, LPCRECT lpcRect, BOOL bDrawMark, COLORREF crBkg = ::GetSysColor(COLOR_WINDOW), COLORREF crBorder = RGB(51,102,153), COLORREF crMark = RGB(51,153,51));
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckHeaderCtrl)
	//}}AFX_VIRTUAL
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi);
// Implementation
public:
	virtual ~CCheckHeaderCtrl();
	void DrawCtrl  (CDC* pDC);
	int  DrawImage (CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bDrawBoth = TRUE, BOOL bRight = TRUE);
	int  DrawBitmap(CDC* pDC, CRect rect, LPHDITEM lphdi, HBITMAP hb, BOOL bRight = TRUE);
	int  DrawText  (CDC* pDC, CRect rect, LPHDITEM lphdi);
	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckHeaderCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnSetImageList(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnLayout(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKHEADERCTRL_H__886B1C45_562B_411B_BE8B_E8CE26FF444D__INCLUDED_)
