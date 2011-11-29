#if !defined(AFX_SHEADERCTRL_H__4A4CBE1E_13C6_4D16_BA5C_67AE5AA027A3__INCLUDED_)
#define AFX_SHEADERCTRL_H__4A4CBE1E_13C6_4D16_BA5C_67AE5AA027A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl window
#define DT_DEFAULT					((UINT)-1)

#define FLATHEADER_TEXT_MAX			80

#define SHC_NONE_CHECK_BOX		    0
#define SHC_UNCHECKED	            1
#define SHC_CHECKED	                2

#define SHC_NONE_SORT				0
#define SHC_SORT_ASC				1
#define SHC_SORT_DES				2

#define SHCF_IMAGE_STATE_CHECK		0x00000003
#define SHCF_IMAGE_STATE_SORT		0x0000000C
#define SHCF_IAMGE_STATE_ALL		0x0000000F

#define SHC_INDEX_TO_STATE_CHECK(i)	((i) & SHCF_IMAGE_STATE_CHECK)
#define SHC_STATE_TO_INDEX_CHECK(i)	((i) & SHCF_IMAGE_STATE_CHECK)

#define SHC_INDEX_TO_STATE_SORT(i)	(((i) << 2) & SHCF_IMAGE_STATE_SORT)
#define SHC_STATE_TO_INDEX_SORT(i)	(((i) >> 2) & 3)

#define SHC_INDEX_TO_STATE(iCheck, iSort)	(SHC_INDEX_TO_STATE_CHECK(iCheck) | SHC_INDEX_TO_STATE_SORT(iSort))

class CSHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CSHeaderCtrl();
	virtual ~CSHeaderCtrl();
// Attributes
public:
	COLORREF m_rgbText;
	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crBtnText;

	COLORREF m_crBorder;
	
	UINT m_nFormat;
	BOOL m_bDividerLines;
	int  m_nSpace;
	SIZE m_sizeImage;
	int  m_nHeight;
// Operations
public:
	void SetHeight(int nHeight);
	int  GetHeight();
	BOOL GetCheckBoxRect(int nSubItem, CRect& chkboxRect);
	BOOL IsClickedCheckBox(int nSubItem);

	UINT GetCheckBoxState(int nSubItem);
	void SetCheckBoxState(int nSubItem, UINT nCheckIndex);
	UINT GetSortState(int nSubItem);
	void SetSortState(int nSubItem, UINT nSortIndex);
	void RemoveAllSortImages();

	UINT GetImageState(int nSubItem, UINT nStateMask);
	void SetImageState(int nSubItem, UINT nState, UINT nStateMask);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHeaderCtrl)
	//}}AFX_VIRTUAL
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi);
	virtual int  GetIndexOfImageList(int nSortState);
	// Implementation
private:
	void DrawCtrl  (CDC* pDC);
	int  DrawCheckBox(CDC* pDC, const CRect& rect, LPHDITEM lphdi);
	int  DrawSortImage(CDC* pDC, const CRect& rect, LPHDITEM lphdi);
	int  DrawText  (CDC* pDC, CRect rect, LPHDITEM lphdi);
	BOOL GetCheckBoxRect(const CRect& rectItem, LPHDITEM lphdi, CRect& chkboxRect);
	BOOL HasSortImage(int iImageState);
	// Generated message map functions
protected:
	//{{AFX_MSG(CSHeaderCtrl)
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

#endif // !defined(AFX_SHEADERCTRL_H__4A4CBE1E_13C6_4D16_BA5C_67AE5AA027A3__INCLUDED_)
