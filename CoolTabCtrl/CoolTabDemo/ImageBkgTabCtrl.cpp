#include "stdafx.h"
#include "ImageBkgTabCtrl.h"

IMPLEMENT_DYNCREATE(CImageBkgTabCtrl, CCoolTabCtrl)

BEGIN_MESSAGE_MAP(CImageBkgTabCtrl, CCoolTabCtrl)
END_MESSAGE_MAP()


CImageBkgTabCtrl::CImageBkgTabCtrl(CImgTabBtn* pImgTabBtn) : m_pImgTabBtn(pImgTabBtn)
{
}
CImageBkgTabCtrl::~CImageBkgTabCtrl()
{
}

void CImageBkgTabCtrl::SetImgTabBtn(CImgTabBtn* pImgTabBtn, BOOL bAutoAdjustHeight)
{ 
	m_pImgTabBtn = pImgTabBtn; 
	if(bAutoAdjustHeight && m_pImgTabBtn->m_pTabPart[TB_PT_BACKGROUND] != NULL)
	{
		int height = GetBitmapSize(m_pImgTabBtn->m_pTabPart[TB_PT_BACKGROUND]).cy;
		SetTabItemHeight(height);
	}
}

void CImageBkgTabCtrl::DrawBk(CDC* pDC)
{
	if(m_pImgTabBtn->m_pTabPart[TB_PT_BACKGROUND] != NULL)
	{
		CRect rect;
		CCoolTabCtrl::GetTabItemsRect(&rect);

		PaintBmp(pDC, &rect, m_pImgTabBtn->m_pTabPart[TB_PT_BACKGROUND], STRETCH);
	}
}

void CImageBkgTabCtrl::Draw(CDC* pDC)
{
	CPen	*pOldPen   = pDC->GetCurrentPen();
	CFont	*pOldFont  = pDC->SelectObject(&m_font);
	int		nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	
	
	CCoolTabItem *pItem;
	POSITION pos;
	int nItemIndex = 0;

	CCoolTabItem *pActiveItem = NULL;
	
	for(pos = m_tabItemList.GetHeadPosition(); pos != NULL; nItemIndex++)
	{
		pItem=(CCoolTabItem*)m_tabItemList.GetNext(pos);
		if(pItem == NULL)
		{
			TRACE1("The No.%d tab item is NULL\n", nItemIndex);
			continue;
		}
		if(m_nActivePage == nItemIndex)
		{
			pActiveItem = pItem;
		}
		else
		{
			Draw(pItem, pDC, m_nStyle, FALSE, m_nFocusedPage == nItemIndex, nItemIndex);
		}
	}
	if(pActiveItem != NULL)
	{
		Draw(pActiveItem, pDC, m_nStyle, TRUE, m_nActivePage == m_nFocusedPage, m_nActivePage);
	}
	
	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}
UINT CImageBkgTabCtrl::CalcWidth(CCoolTabItem* pItem, CDC *pDC)
{
	UINT nWidth = CCoolTabCtrl::CalcWidth(pItem, pDC);

	int status = BS_CLICKED;
	BITMAP bmp;
	if(m_pImgTabBtn->m_pBitmaps[status][PS_LEFT] != NULL)
	{
		m_pImgTabBtn->m_pBitmaps[status][PS_LEFT]->GetBitmap(&bmp);
		nWidth += bmp.bmWidth;
	}
	if(m_pImgTabBtn->m_pBitmaps[status][PS_RIGHT] != NULL)
	{
		m_pImgTabBtn->m_pBitmaps[status][PS_RIGHT]->GetBitmap(&bmp);
		nWidth += bmp.bmWidth;
	}
	return nWidth;
}
void CImageBkgTabCtrl::Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered, UINT nIndex)
{
	CRect rect = pItem->m_rect;
	int status = BS_NORMAL;
	if(bActive)
	{
		status = BS_CLICKED;		
	}
	else if(bHovered)
	{
		status = BS_HIGHLIGHT;
	}

	if(m_pImgTabBtn->m_pBitmaps[status][PS_LEFT] != NULL)
	{
		//Left
		PaintBmp(pDC, &rect, m_pImgTabBtn->m_pBitmaps[status][PS_LEFT], COPY);
		
		//Right
		if(m_pImgTabBtn->m_pBitmaps[status][PS_RIGHT] != NULL)
		{
			rect.left = rect.right - GetBitmapSize(m_pImgTabBtn->m_pBitmaps[status][PS_RIGHT]).cx;
			PaintBmp(pDC, &rect, m_pImgTabBtn->m_pBitmaps[status][PS_RIGHT], COPY);

			rect.right = rect.left;
		}
		
		//Mid
		rect.left = pItem->m_rect.left + GetBitmapSize(m_pImgTabBtn->m_pBitmaps[status][PS_LEFT]).cx;
		PaintBmp(pDC, &rect, m_pImgTabBtn->m_pBitmaps[status][PS_MID], STRETCH);
		
		rect = pItem->m_rect;
	}
	if(m_pImgTabBtn->m_pTabPart[TB_PT_SEPERATOR] != NULL && !bActive && nIndex > 0 && nIndex != (UINT)m_nActivePage + 1)
	{
		int height = GetBitmapSize(m_pImgTabBtn->m_pTabPart[TB_PT_SEPERATOR]).cy;
		if(rect.Height() > height)
		{
			rect.top += (rect.Height() - height) / 2;
		}
		PaintBmp(pDC, &rect, m_pImgTabBtn->m_pTabPart[TB_PT_SEPERATOR], COPY);

		rect = pItem->m_rect;
	}
	
	rect.top += 8;
	rect.left += 5;
	rect.right -= 2;
	//Icon
	if(pItem->m_hIcon != NULL && rect.Width() > 16)
	{
		::DrawIconEx(pDC->m_hDC, rect.left, rect.top, pItem->m_hIcon, 16, 16, 0, NULL, DI_NORMAL);
		rect.left += 18;
	}
	
	//Text
	if (!pItem->m_sText.IsEmpty())
	{
		rect.top += 2;
		
		CString sText = pItem->m_sText;
		
		BOOL bTruncated = FALSE;
		int len;
		
		for(len = sText.GetLength(); len > 4 && pDC->GetTextExtent(sText).cx > rect.Width(); len -= 2)
		{
			sText = sText.Left(len - 2);
			bTruncated = TRUE;
		}
		if(bTruncated)
		{
			if(len > 3)
			{
				sText = sText.Left(len - 3);
			}
			else
			{
				sText.Empty();
			}
			sText += "...";
		}
		
		pDC->DrawText(sText, &rect, DT_CENTER | DT_SINGLELINE);
	}
}
