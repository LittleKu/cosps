#include "stdafx.h"
#include "BitmapTabCtrl.h"


TabButtonGroup::TabButtonGroup(CBitmap* pNormal, CBitmap* pHighlight, CBitmap* pClicked, CBitmap* pDisabled)
{
	m_pNormal = pNormal;
	m_pHightlight = pHighlight;
	m_pClicked = pClicked;
	m_pDisabled = pDisabled;
}
TabButtonGroup::~TabButtonGroup()
{
	CRect* pRect = NULL;
	int i, size = m_tabBtnArray.GetSize();
	for(i = 0; i < size; i++)
	{
		pRect = (CRect*)m_tabBtnArray.GetAt(i);
		if(pRect != NULL)
		{
			delete pRect;
		}
	}
	m_tabBtnArray.RemoveAll();
}
void TabButtonGroup::AddTabButton(LPCRECT lpcRect)
{
	CRect* pRect = new CRect(lpcRect);
	m_tabBtnArray.Add(pRect);
}

IMPLEMENT_DYNCREATE(CBitmapTabCtrl, CCoolTabCtrl)

BEGIN_MESSAGE_MAP(CBitmapTabCtrl, CCoolTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


CBitmapTabCtrl::CBitmapTabCtrl() : m_pBkgBitmap(NULL), m_pTabBtnGroup(NULL)
{
}
CBitmapTabCtrl::~CBitmapTabCtrl()
{
}

void CBitmapTabCtrl::SetBackgroundBitmap(CBitmap* pBitmap) 
{ 
	m_pBkgBitmap = pBitmap;

	SetTabItemHeight(GetBitmapSize(m_pBkgBitmap).cy);

}
BOOL CBitmapTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(m_pBkgBitmap != NULL)
	{
		CRect rect;
		CCoolTabCtrl::GetTabItemsRect(&rect);

		PaintBmp(pDC, &rect, m_pBkgBitmap, COPY);

		return TRUE;
	}
	return CCoolTabCtrl::OnEraseBkgnd(pDC);
}

void CBitmapTabCtrl::AutoSize()
{
	CRect viewClientRect;
	GetViewClientRect(&viewClientRect);

	POSITION	 pos = NULL;
	CCoolTabItem *pItem = NULL;
	UINT nIndex = 0;
	CRect* pRect = NULL;
	//1. Set the width of each tab item
	//2. Move all the client window to the client area by the way
	for(pos = m_tabItemList.GetHeadPosition(); pos != NULL; nIndex++)
	{
		pItem=(CCoolTabItem*)m_tabItemList.GetNext(pos);
		ASSERT(pItem);

		pRect = (CRect*)m_pTabBtnGroup->m_tabBtnArray.GetAt(nIndex);
		ASSERT(pRect);

// 		pItem->m_rect.left = pRect->left;
// 		pItem->m_rect.right = pRect->right;
// 		pItem->m_rect.top = 0;
// 		pItem->m_rect.bottom = m_nTabItemHeight;

		pItem->m_rect = *pRect;
		
		//Move all the client window to the client area by the way
		if(pItem->m_pWnd != NULL)
		{
			pItem->m_pWnd->MoveWindow(viewClientRect);
		}		
	}
}

void CBitmapTabCtrl::Draw(CDC* pDC)
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
UINT CBitmapTabCtrl::CalcWidth(CCoolTabItem* pItem, CDC *pDC)
{
	int nTabItemIndex = GetPageIndex(pItem);
	ASSERT(nTabItemIndex >= 0);
	ASSERT(nTabItemIndex < m_pTabBtnGroup->m_tabBtnArray.GetSize());

	CRect* pRect = (CRect*)m_pTabBtnGroup->m_tabBtnArray.GetAt(nTabItemIndex);
	ASSERT(pRect);

	return pRect->Width();
}
void CBitmapTabCtrl::Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered, UINT nIndex)
{
	CBitmap* pBitmap = NULL;
	if(bActive)
	{
		pBitmap = m_pTabBtnGroup->m_pClicked;
	}
	else if(bHovered)
	{
		pBitmap = m_pTabBtnGroup->m_pHightlight;
	}
	//Disabled state now is not available
	else
	{
		pBitmap = m_pTabBtnGroup->m_pNormal;
	}

	//The bitmap rect
	CRect* pRect = (CRect*)m_pTabBtnGroup->m_tabBtnArray.GetAt(nIndex);
	ASSERT(pRect);
	
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = dc.SelectObject(pBitmap);	

	ASSERT(pRect->Width() == pItem->m_rect.Width());
	ASSERT(pRect->Height() <= pItem->m_rect.Height());

	pDC->BitBlt(pItem->m_rect.left, pItem->m_rect.top, pItem->m_rect.Width(), pItem->m_rect.Height(), 
		&dc, pRect->left, pRect->top, SRCCOPY);

	dc.SelectObject(pOldBitmap);
	dc.DeleteDC();
}