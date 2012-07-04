#if !defined(AFX_PREVIEWCTRL_H__26227DEC_3E08_4FDD_9C4C_CE988C338E61__INCLUDED_)
#define AFX_PREVIEWCTRL_H__26227DEC_3E08_4FDD_9C4C_CE988C338E61__INCLUDED_

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
	void StopPreview();
	void StartPreview();
	void SetBitmaps(CBitmap* pBmp1, CBitmap* pBmp2);
	virtual ~CPreviewCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CBitmap* m_pBmp2;
	CBitmap* m_pBmp1;
	UINT m_nTimer;
	int m_nFrame;
	int m_nTotalFrames;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWCTRL_H__26227DEC_3E08_4FDD_9C4C_CE988C338E61__INCLUDED_)
