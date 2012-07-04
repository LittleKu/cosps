#if !defined(AFX_PREVIEWCTRL_H__9E8D4EC1_65C0_43D9_AD47_24ECCCD3F942__INCLUDED_)
#define AFX_PREVIEWCTRL_H__9E8D4EC1_65C0_43D9_AD47_24ECCCD3F942__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewCtrl window

class CPreviewCtrl : public CStatic
{
// Construction
public:
	CPreviewCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveFrames(int nTotalFrames);
	HBITMAP GetFrameBitmap(HDC hdc, int nFrame, int nTotalFrames, int nWidth, int nHeight);
	void StopPreview();
	void StartPreview();
	void SetBitmaps(CBitmap* pBitmap1, CBitmap* pBitmap2);
	virtual ~CPreviewCtrl();
	static BOOL SaveBitmap(HDC hdc, HBITMAP hbmp, DWORD dwWidth, DWORD dwHeight, LPCTSTR lpszFileName);
	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewCtrl)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL ReleaseBitmap(HBITMAP hbmp);
	HBITMAP GetStretchedBitmap(HDC hdc, HBITMAP hSrcBmp, int nWidth, int nHeight);
	void Draw(CDC* pDC);
	static BOOL SaveToFile(BITMAPFILEHEADER* pBFH, BITMAPINFOHEADER* pBIH, LPVOID lpDibData, LPCTSTR lpszFileName);
	int m_nFrame;
	int m_nTotalFrames;

	UINT m_nTimer;
	CBitmap* m_pBitmap2;
	CBitmap* m_pBitmap1;

	HBITMAP m_hBitmap1;
	HBITMAP m_hBitmap2;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWCTRL_H__9E8D4EC1_65C0_43D9_AD47_24ECCCD3F942__INCLUDED_)
