#ifndef _BITMAP_TAB_CTRL_H_
#define _BITMAP_TAB_CTRL_H_

#pragma once

#include "CoolTabCtrl.h"

class CImagePureTabCtrl : public CCoolTabCtrl
{
	DECLARE_DYNCREATE(CImagePureTabCtrl)
public:
	CImagePureTabCtrl();
	virtual ~CImagePureTabCtrl();
	void SetBackgroundBitmap(CBitmap* pBitmap);
	void SetImageButtonGroup(CImgBtnGroup* pImgBtnGroup);
	void AutoSize();
	void Draw(CDC* pDC);
	UINT CalcWidth(CCoolTabItem* pItem, CDC *pDC);
	void Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered = FALSE, UINT nIndex = 0);
	int CImagePureTabCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

	virtual void DrawBk(CDC* pDC);
public:
 	CBitmap* m_pBkgBitmap;
	CImgBtnGroup* m_pImgBtnGroup;
};

#endif