#include "stdafx.h"
#include "ImagePureTabCtrl.h"
#include "ResMgr.h"

IMPLEMENT_DYNCREATE(CImagePureTabCtrl, CCoolTabCtrl)

BEGIN_MESSAGE_MAP(CImagePureTabCtrl, CCoolTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


CImagePureTabCtrl::CImagePureTabCtrl() : m_pBkgBitmap(NULL), m_pImgBtnGroup(NULL)
{
}
CImagePureTabCtrl::~CImagePureTabCtrl()
{
}

void CImagePureTabCtrl::SetBackgroundBitmap(CBitmap* pBitmap) 
{
	m_pBkgBitmap = pBitmap;
	if(pBitmap != NULL)
	{
		SetTabItemHeight(GetBitmapSize(m_pBkgBitmap).cy);
	}
}

void CImagePureTabCtrl::SetImageButtonGroup(CImgBtnGroup* pImgBtnGroup)
{
	this->m_pImgBtnGroup = pImgBtnGroup;
	CBitmap* pBitmap = m_pImgBtnGroup->m_pBitmaps[BS_NORMAL];
	SetBackgroundBitmap(pBitmap);
}
BOOL CImagePureTabCtrl::OnEraseBkgnd(CDC* pDC)
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

void CImagePureTabCtrl::AutoSize()
{
	if(m_pImgBtnGroup == NULL)
	{
		return;
	}
	CRect viewClientRect;
	GetViewClientRect(&viewClientRect);

	POSITION	 pos = NULL;
	CCoolTabItem *pItem = NULL;
	UINT nIndex = 0;
	CRect* pRect = NULL;
	CImgBtn* pImgBtn = NULL;
	//1. Set the width of each tab item
	//2. Move all the client window to the client area by the way
	POSITION pos1 = m_tabItemList.GetHeadPosition();
	for( ; pos1 != NULL; nIndex++)
	{
		pItem=(CCoolTabItem*)m_tabItemList.GetNext(pos1);
		ASSERT(pItem);

		pImgBtn = (CImgBtn*)m_pImgBtnGroup->m_imgBtnArray[nIndex];
		ASSERT(pImgBtn);

		pItem->m_rect = pImgBtn->rect;
		
		//Move all the client window to the client area by the way
		if(pItem->m_pWnd != NULL)
		{
			pItem->m_pWnd->MoveWindow(viewClientRect);
		}		
	}
}

void CImagePureTabCtrl::Draw(CDC* pDC)
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
UINT CImagePureTabCtrl::CalcWidth(CCoolTabItem* pItem, CDC *pDC)
{
	int nTabItemIndex = GetPageIndex(pItem);
	ASSERT(nTabItemIndex >= 0);
	ASSERT(nTabItemIndex < m_pImgBtnGroup->m_imgBtnArray.GetSize());

	CImgBtn* pImgBtn = m_pImgBtnGroup->m_imgBtnArray.GetAt(nTabItemIndex);
	ASSERT(pImgBtn);
	return pImgBtn->rect.Width();
}
void CImagePureTabCtrl::Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered, UINT nIndex)
{
	CBitmap* pBitmap = NULL;
	int bs = BS_NORMAL;
	if(bActive)
	{
		bs = BS_CLICKED;
	}
	else if(bHovered)
	{
		bs = BS_HIGHLIGHT;
	}
	pBitmap = m_pImgBtnGroup->m_pBitmaps[bs];

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

int CImagePureTabCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	POSITION pos = m_tabItemList.GetHeadPosition();
	POSITION imgBtnPos = NULL;
	int i = 0;
	CString tip;
	CRect rect;
	CImgBtn* pImgBtn = NULL;
	while(pos)
	{
		CCoolTabItem * pItem=(CCoolTabItem *)m_tabItemList.GetNext(pos);		
		if(pItem->m_rect.PtInRect(point))
		{
			pTI->cbSize = sizeof(TOOLINFO);
			pTI->uFlags = 0;
			pTI->hwnd = GetSafeHwnd();
			pTI->rect = pItem->m_rect;

			pImgBtn = m_pImgBtnGroup->m_imgBtnArray.GetAt(i);
			ASSERT(pImgBtn);
			
			pTI->lpszText = (LPTSTR)malloc(sizeof(TCHAR)*(pImgBtn->sTips.GetLength()+1));
			_tcscpy(pTI->lpszText, pImgBtn->sTips);
			pTI->uId = pImgBtn->id;

			return pImgBtn->id;
		}
		i++;
	}
	return -1;
}