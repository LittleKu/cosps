#if !defined(AFX_COOLTABCTRL_H__INCLUDED_)
#define AFX_COOLTABCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//TCS -- Tab Control Style
#define TCS_TOP			0x0001		//Tab Items at TOP
#define TCS_BOTTOM		0x0002		//Tab Items at BOTTOM
#define TCS_REPLACEMENT_MASK 0x0003 //Tab Items replacement mask

#define TCS_MONOSPACE	0x0004		//MONOSPACE
#define TCS_ANIMATE		0x0008		//Animate
#define TCS_AUTOADJUST	0x0010		//Auto adjust
#define TCS_MASK		0x00FF		//Internal use

//Tab Item Create Type
#define TI_CT_NORMAL         0      //The Tab Item is created already when added into Tab Control
#define TI_CT_DYNAMIC        1      //The Tab Item is created dynamically by RUNTIME_CLASS MACRO

/////////////////////////////////////////////////////////////////////////////
// CCoolTabCtrl window

class CCoolTabItem
{
public:
	CWnd*		m_pWnd;		//The Wnd of this Tab Item represents
	UINT		m_nCreateType;	//Create Type
	CString		m_sText;
	HICON		m_hIcon;
	CRect		m_rect;	
};

class CCoolTabCtrl : public CWnd
{
	DECLARE_DYNCREATE(CCoolTabCtrl)
// Construction
public:
	CCoolTabCtrl();
	virtual ~CCoolTabCtrl();
// Implementation
public:
	BOOL Create(UINT wStyle, const CRect& rect, CWnd * pParentWnd, UINT nID);
	CCoolTabItem* AddTab(CWnd *pWnd, LPCTSTR sText, UINT IconID = NULL);
	BOOL AddTab(CRuntimeClass* pClass, UINT nIDTemplate, LPCTSTR sText, UINT IconID = NULL);
	void UpdateWindow();

	CCoolTabItem* GetPageItem(UINT nIndex);
	int GetPageIndex(CCoolTabItem* pItem);
	void SetActivePage(int nIndex);
	int GetActivePage() { return m_nActivePage; }
	void GetTabItemsRect(LPRECT lpRect);

	virtual void AutoSize();
	virtual void GetViewClientRect(LPRECT lpRect);
	virtual void Draw(CDC* pDC);
	virtual UINT CalcWidth(CCoolTabItem* pItem, CDC *pDC);
	virtual void Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered = FALSE, UINT nIndex = 0);
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
public:
	//Attributes
	UINT GetStyle() { return m_nStyle; }
	void SetStyle(UINT style) { m_nStyle = style; }
	void SetMonoSpace(BOOL bEnable = TRUE);
	void SetTabReplacement(int replacement = TCS_TOP);
	void SetStyleAnimate(BOOL bEnable = TRUE);
	void SetAutoAdjustWidth(BOOL bEnable = TRUE);
	BOOL IsAutoAdjustWidth();
	void SetTabItemHeight(UINT nHeight) { m_nTabItemHeight = nHeight; }
	UINT GetTabItemHeight() { return m_nTabItemHeight; }
private:
	int GetPageIndex(CPoint point);
	UINT GetTotalRequiredWidth(CDC* pDC);
	// Generated message map functions
protected:
	//{{AFX_MSG(CCoolTabCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL        m_bEraseBkgnd;
	CFont		m_font;
	int			m_nActivePage;
	int         m_nFocusedPage;
	UINT		m_nStyle;
	CPtrList	m_tabItemList;
	UINT        m_nTabItemHeight;
	BOOL        m_bMouseHovered;

	//static variables
	static CPen PEN_3DFACE;	
	static CPen PEN_WHITE;
	static CPen PEN_3DLIGHT;
	static CPen PEN_3DSHADOW;
	static CPen PEN_3DDKSHADOW;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLTABCTRL_H__83DD41F0_25C9_417D_9353_777A80FAD1CF__INCLUDED_)
