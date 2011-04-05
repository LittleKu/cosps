#ifndef _IMAGE_TAB_CTRL_H_
#define _IMAGE_TAB_CTRL_H_

#pragma once

#include "CoolTabCtrl.h"

class CImageTabCtrl : public CCoolTabCtrl
{
	DECLARE_DYNCREATE(CImageTabCtrl)
public:
	CImageTabCtrl();
	virtual ~CImageTabCtrl();
	void SetBackgroundBitmap(CBitmap* pBitmap) { m_pBkgBitmap = pBitmap; }

	void Draw(CDC* pDC);
	UINT CalcWidth(CCoolTabItem* pItem, CDC *pDC);
	void Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered = FALSE, UINT nIndex = 0);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
public:
	CBitmap* m_pBkgBitmap;
	CBitmap* m_pLeft;
	CBitmap* m_pMid;
	CBitmap* m_pRight;
	CBitmap* m_pSep;
};

#endif
