#ifndef _IMAGE_TAB_CTRL_H_
#define _IMAGE_TAB_CTRL_H_

#pragma once

#include "CoolTabCtrl.h"

class CImageBkgTabCtrl : public CCoolTabCtrl
{
	DECLARE_DYNCREATE(CImageBkgTabCtrl)
public:
	CImageBkgTabCtrl(CImgTabBtn* pImgTabBtn = NULL);
	virtual ~CImageBkgTabCtrl();
	void SetImgTabBtn(CImgTabBtn* pImgTabBtn) { m_pImgTabBtn = pImgTabBtn; }
	void Draw(CDC* pDC);
	UINT CalcWidth(CCoolTabItem* pItem, CDC *pDC);
	void Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered = FALSE, UINT nIndex = 0);
	virtual void DrawBk(CDC* pDC);
protected:
	DECLARE_MESSAGE_MAP()
private:
	CImgTabBtn* m_pImgTabBtn;
};

#endif
