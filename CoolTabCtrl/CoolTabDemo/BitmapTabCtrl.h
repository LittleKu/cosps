#ifndef _BITMAP_TAB_CTRL_H_
#define _BITMAP_TAB_CTRL_H_

#pragma once

#include "CoolTabCtrl.h"

class CBitmapTabCtrl : public CCoolTabCtrl
{
	DECLARE_DYNCREATE(CBitmapTabCtrl)
public:
	CBitmapTabCtrl();
	virtual ~CBitmapTabCtrl();
	void SetBackgroundBitmap(CBitmap* pBitmap);
	void SetImageButtonGroup(CImgBtnGroup* pImgBtnGroup);
	void AutoSize();
	void Draw(CDC* pDC);
	UINT CalcWidth(CCoolTabItem* pItem, CDC *pDC);
	void Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered = FALSE, UINT nIndex = 0);
	int CBitmapTabCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
public:
 	CBitmap* m_pBkgBitmap;
	CImgBtnGroup* m_pImgBtnGroup;
};

#endif