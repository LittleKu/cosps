#include "stdafx.h"
#include "ImageTabCtrl.h"

IMPLEMENT_DYNCREATE(CImageTabCtrl, CCoolTabCtrl)

BEGIN_MESSAGE_MAP(CImageTabCtrl, CCoolTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


CImageTabCtrl::CImageTabCtrl() : m_pBkgBitmap(NULL)
{
}
CImageTabCtrl::~CImageTabCtrl()
{
}

BOOL CImageTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(m_pBkgBitmap != NULL)
	{
		CRect rect;
		CCoolTabCtrl::GetTabItemsRect(&rect);

		PaintBmp(pDC, &rect, m_pBkgBitmap, STRETCH);

		return TRUE;
	}
	return CCoolTabCtrl::OnEraseBkgnd(pDC);
}

void CImageTabCtrl::Draw(CDC* pDC)
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
UINT CImageTabCtrl::CalcWidth(CCoolTabItem* pItem, CDC *pDC)
{
	UINT nWidth = CCoolTabCtrl::CalcWidth(pItem, pDC);

	BITMAP bmp;
	if(m_pLeft != NULL)
	{
		m_pLeft->GetBitmap(&bmp);
		nWidth += bmp.bmWidth;
	}
	if(m_pRight != NULL)
	{
		m_pRight->GetBitmap(&bmp);
		nWidth += bmp.bmWidth;
	}
	return nWidth;
}
void CImageTabCtrl::Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered, UINT nIndex)
{
	CRect rect = pItem->m_rect;
	if(bActive)
	{
		//Left
		PaintBmp(pDC, &rect, m_pLeft, COPY);
		
		//Right
		rect.left = rect.right - GetBitmapSize(m_pRight).cx;
		PaintBmp(pDC, &rect, m_pRight, COPY);

		//Mid
		rect.right = rect.left;
		rect.left = pItem->m_rect.left + GetBitmapSize(m_pLeft).cx;
		PaintBmp(pDC, &rect, m_pMid, STRETCH);

		rect = pItem->m_rect;
	}
	else if(nIndex > 0 && nIndex != (UINT)m_nActivePage + 1)
	{
		PaintBmp(pDC, &rect, m_pSep, COPY);
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
