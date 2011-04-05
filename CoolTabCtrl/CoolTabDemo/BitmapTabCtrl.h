#ifndef _BITMAP_TAB_CTRL_H_
#define _BITMAP_TAB_CTRL_H_

#pragma once

#include "CoolTabCtrl.h"

class TabButtonGroup
{
public:
	TabButtonGroup(CBitmap* pNormal, CBitmap* pHighlight, CBitmap* pClicked, CBitmap* pDisabled = NULL);
	virtual ~TabButtonGroup();
	void AddTabButton(LPCRECT lpcRect);
public:
	CBitmap* m_pNormal;
	CBitmap* m_pHightlight;
	CBitmap* m_pClicked;
	CBitmap* m_pDisabled;
	CPtrArray m_tabBtnArray;
};

class CBitmapTabCtrl : public CCoolTabCtrl
{
	DECLARE_DYNCREATE(CBitmapTabCtrl)
public:
	CBitmapTabCtrl();
	virtual ~CBitmapTabCtrl();
	void SetBackgroundBitmap(CBitmap* pBitmap);
	
	void AutoSize();
	void Draw(CDC* pDC);
	UINT CalcWidth(CCoolTabItem* pItem, CDC *pDC);
	void Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered = FALSE, UINT nIndex = 0);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
public:
	CBitmap* m_pBkgBitmap;
	TabButtonGroup* m_pTabBtnGroup;
};

#endif