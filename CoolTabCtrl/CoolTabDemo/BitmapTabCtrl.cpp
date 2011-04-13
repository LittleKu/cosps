#include "stdafx.h"
#include "BitmapTabCtrl.h"
#include "ResMgr.h"

IMPLEMENT_DYNCREATE(CBitmapTabCtrl, CCoolTabCtrl)

BEGIN_MESSAGE_MAP(CBitmapTabCtrl, CCoolTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


CBitmapTabCtrl::CBitmapTabCtrl() : m_pBkgBitmap(NULL), m_pImgBtnGroup(NULL)
{
}
CBitmapTabCtrl::~CBitmapTabCtrl()
{
}

void CBitmapTabCtrl::SetBackgroundBitmap(CBitmap* pBitmap) 
{
	m_pBkgBitmap = pBitmap;
	if(pBitmap != NULL)
	{
		SetTabItemHeight(GetBitmapSize(m_pBkgBitmap).cy);
	}
}

void CBitmapTabCtrl::SetImageButtonGroup(ImageButtonGroup* pImgBtnGroup)
{
	this->m_pImgBtnGroup = pImgBtnGroup;
	CBitmap* pBitmap = CResMgr::GetInstance()->GetBitmap(m_pImgBtnGroup->btnBitmapPath[BS_NORMAL]);
	SetBackgroundBitmap(pBitmap);
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
	if(m_pImgBtnGroup == NULL)
	{
		return;
	}
	CRect viewClientRect;
	GetViewClientRect(&viewClientRect);

//	POSITION	 pos = NULL;
	CCoolTabItem *pItem = NULL;
	UINT nIndex = 0;
	CRect* pRect = NULL;
	IMAGE_BUTTON* pImgBtn = NULL;
	//1. Set the width of each tab item
	//2. Move all the client window to the client area by the way
	POSITION pos1 = m_tabItemList.GetHeadPosition();
	POSITION pos2 = m_pImgBtnGroup->imgBtnPtrList.GetHeadPosition();
	for( ; pos1 != NULL && pos2 != NULL; nIndex++)
	{
		pItem=(CCoolTabItem*)m_tabItemList.GetNext(pos1);
		ASSERT(pItem);

		pImgBtn = (IMAGE_BUTTON*)m_pImgBtnGroup->imgBtnPtrList.GetNext(pos2);
		ASSERT(pImgBtn);

		pItem->m_rect = pImgBtn->rect;
		
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
	ASSERT(nTabItemIndex < m_pImgBtnGroup->imgBtnPtrList.GetCount());

	return pItem->m_rect.Width();
// 	CRect* pRect = (CRect*)m_pTabBtnGroup->m_tabBtnArray.GetAt(nTabItemIndex);
// 	ASSERT(pRect);
// 
// 	return pRect->Width();
}
void CBitmapTabCtrl::Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered, UINT nIndex)
{
	CBitmap* pBitmap = NULL;
	CString bmpPath;
	if(bActive)
	{
		bmpPath = m_pImgBtnGroup->btnBitmapPath[BS_CLICKED];
	}
	else if(bHovered)
	{
		bmpPath = m_pImgBtnGroup->btnBitmapPath[BS_HIGHLIGHT];
	}
	//Disabled state now is not available
	else
	{
		bmpPath = m_pImgBtnGroup->btnBitmapPath[BS_NORMAL];
	}

	pBitmap = CResMgr::GetInstance()->GetBitmap(bmpPath);

	//The bitmap rect
	CRect* pRect = (CRect*)(&(pItem->m_rect));
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

int CBitmapTabCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	POSITION pos = m_tabItemList.GetHeadPosition();
	POSITION imgBtnPos = NULL;
	int i=0;
	CString tip;
	CRect rect;
	while(pos)
	{
		CCoolTabItem * pItem=(CCoolTabItem *)m_tabItemList.GetNext(pos);
		i++;
		if(pItem->m_rect.PtInRect(point))
		{
			pTI->cbSize = sizeof(TOOLINFO);
			pTI->uFlags = 0;
			pTI->hwnd = GetSafeHwnd();
			pTI->uId = i;
			pTI->rect = pItem->m_rect;

			imgBtnPos = m_pImgBtnGroup->imgBtnPtrList.FindIndex(i - 1);
			if(imgBtnPos == NULL)
			{
				tip.Format("tips for %d", i);
				pTI->lpszText = (LPTSTR)malloc(sizeof(TCHAR)*(tip.GetLength()+1));
				_tcscpy(pTI->lpszText, tip);
			}
			else
			{
				IMAGE_BUTTON* pImgBtn = m_pImgBtnGroup->imgBtnPtrList.GetAt(imgBtnPos);
				ASSERT(pImgBtn);
				pTI->lpszText = (LPTSTR)malloc(sizeof(TCHAR)*(pImgBtn->sTips.GetLength()+1));
				_tcscpy(pTI->lpszText, pImgBtn->sTips);				
			}
			return i;
		}
	}
	return -1;
}