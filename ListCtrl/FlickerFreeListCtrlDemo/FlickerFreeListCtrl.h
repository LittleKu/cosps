#if !defined(AFX_FLICKERFREELISTCTRL_H__CA2DFDC8_71E3_4397_AFA3_517573A2E593__INCLUDED_)
#define AFX_FLICKERFREELISTCTRL_H__CA2DFDC8_71E3_4397_AFA3_517573A2E593__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlickerFreeListCtrl.h : header file
//

class CMemDC;

#define MLC_DT_TEXT (DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS)

/////////////////////////////////////////////////////////////////////////////
// CFlickerFreeListCtrl window

class CTaskInfo
{
public:
	UINT		mask;
	CString		m_szFileName;
	CString		m_szFormat;
	DWORD		m_nDuration;
	DWORD		m_nState;
	DWORD		m_nProgress;
	
	//internal data
	int			m_nTaskID;
	
	CTaskInfo()
	{
		Reset();
	}
	void Reset()
	{
		mask = 0;
		m_szFileName.Empty();
		m_szFormat.Empty();
		m_nDuration = 0;
		m_nState = 0;
		m_nTaskID = -1;
		m_nProgress = 0;
	}
};

class CFlickerFreeListCtrl : public CListCtrl
{
// Construction
public:
	CFlickerFreeListCtrl();
	void SendMeasureItemMsg();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlickerFreeListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlickerFreeListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlickerFreeListCtrl)
	//afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
 	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
 	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()

protected:
	void SetColors();
	void InitItemMemDC(CMemDC *dc, LPDRAWITEMSTRUCT lpDrawItemStruct, BOOL &bCtrlFocused);
	void DrawFocusRect(CDC *pDC, const CRect &rcItem, BOOL bItemFocused, BOOL bCtrlFocused, BOOL bItemSelected);
	static __inline bool HaveIntersection(const CRect &rc1, const CRect &rc2) {
        return (rc1.left   < rc2.right  &&
			rc1.right  > rc2.left   &&
			rc1.top    < rc2.bottom &&
			rc1.bottom > rc2.top);
	}

	COLORREF        m_crWindow;
	COLORREF        m_crWindowText;
	COLORREF        m_crWindowTextBk;
	COLORREF        m_crHighlight;
	COLORREF		m_crHighlightText;
	COLORREF		m_crGlow;
	COLORREF        m_crFocusLine;
	COLORREF        m_crNoHighlight;
	COLORREF        m_crNoFocusLine;
	NMLVCUSTOMDRAW  m_lvcd;
	BOOL            m_bCustomDraw;

	static const int sm_iIconOffset;
	static const int sm_iLabelOffset;
	static const int sm_iSubItemInset;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLICKERFREELISTCTRL_H__CA2DFDC8_71E3_4397_AFA3_517573A2E593__INCLUDED_)
