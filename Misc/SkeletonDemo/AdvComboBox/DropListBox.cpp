/////////////////////////////////////////////////////////////////////////////
// DropListBox.cpp : implementation file
// 
// CAdvComboBox Control
// Version: 2.1
// Date: September 2002
// Author: Mathias Tunared
// Email: Mathias@inorbit.com
// Copyright (c) 2002. All Rights Reserved.
//
// This code, in compiled form or as source code, may be redistributed 
// unmodified PROVIDING it is not sold for profit without the authors 
// written consent, and providing that this notice and the authors name 
// and all copyright notices remains intact.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DropListBox.h"
#include "AdvComboBox.h"
#include "DropWnd.h"
#include "DropScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDropListBox

CDropListBox::CDropListBox( CAdvComboBox* pComboParent, CDropWnd* pDropWnd )
 : m_pComboParent( pComboParent ), m_pDropWnd( pDropWnd )
{
	m_nLastTopIdx = 0;
	m_nSpaceX = 4;
}

CDropListBox::~CDropListBox()
{
}


BEGIN_MESSAGE_MAP(CDropListBox, CListBox)
	//{{AFX_MSG_MAP(CDropListBox)
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_COMPAREITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_VRC_SETCAPTURE, OnSetCapture )
	ON_MESSAGE( WM_VRC_RELEASECAPTURE, OnReleaseCapture )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropListBox message handlers


int CDropListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	//
	// Because this window doesn't have an owner, there will appear 
	// a 'blank' button on the taskbar. The following are to hide 
	// that 'blank' button on the taskbar
	ShowWindow( SW_HIDE );
	ModifyStyleEx( 0, WS_EX_TOOLWINDOW );// |WS_VSCROLL );//| WS_EX_NOACTIVATE ); // WS_EX_CONTROLPARENT
	ShowWindow( SW_SHOW );
	SetWindowPos( &wndTopMost, lpCreateStruct->x, lpCreateStruct->y, lpCreateStruct->cx, lpCreateStruct->cy, SWP_SHOWWINDOW );

	SetFont( m_pComboParent->GetFont() ); 
	return 0;
}

void CDropListBox::OnNcDestroy()
{	
	CListBox::OnNcDestroy();
}


LONG CDropListBox::OnSetCapture( WPARAM wParam, LPARAM lParam )
{
	SetCapture();
	return FALSE;
}

LONG CDropListBox::OnReleaseCapture( WPARAM wParam, LPARAM lParam )
{
	ReleaseCapture();
	return FALSE;
}

void CDropListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	//
	// Is mouse within listbox
	CRect rcClient;
	GetClientRect( rcClient );
	if( !rcClient.PtInRect( point ) )
	{
		ReleaseCapture();
		GetParent()->SendMessage( WM_VRC_SETCAPTURE );
	}

	//
	// Set selection item under mouse
	int nPos = PointTest(point);
	PLIST_ITEM pItem = (PLIST_ITEM)GetItemDataPtr(nPos);
	if( nPos != LB_ERR && (DWORD)pItem != -1 && pItem != NULL)
	{
		if( GetCurSel() != nPos && !(pItem->state & ACBIS_DISABLED) )
		{
			SetCurSel( nPos );
		}
	}

	//
	// Check if we have auto scrolled
	if( m_nLastTopIdx != GetTopIndex() )
	{
		int nDiff = m_nLastTopIdx - GetTopIndex();
		m_nLastTopIdx = GetTopIndex();

		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		if( m_pDropWnd->GetScrollBarPtr()->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPos = m_nLastTopIdx;
			m_pDropWnd->GetScrollBarPtr()->SetScrollInfo( &info );
		}
	}

	CListBox::OnMouseMove(nFlags, point);
}

void CDropListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
//	CListBox::OnLButtonUp(nFlags, point);
}

void CDropListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//Click in the client area?
	CRect rcClient;
	GetClientRect( rcClient );
	if( !rcClient.PtInRect( point ) )
	{
		ReleaseCapture();
		GetParent()->SendMessage( WM_VRC_SETCAPTURE );
	}

	//Click on the disabled item is disallowed
	int nPos = PointTest(point);

	if(nPos != LB_ERR)
	{
		PLIST_ITEM pItem = (PLIST_ITEM)GetItemDataPtr(nPos);
		if((DWORD)pItem != -1 && pItem != NULL && (pItem->state & ACBIS_DISABLED))
		{
			return;
		}
	}

	//Get current selected item
	nPos = GetCurSel();

	if(nPos != LB_ERR)
	{
		PLIST_ITEM pItem = (PLIST_ITEM)GetItemDataPtr(nPos);
		if((DWORD)pItem != -1 && pItem != NULL && (pItem->state & ACBIS_DISABLED))
		{
			return;
		}

		//parent node can not be selected
		if(pItem->GetChildCount() > 0)
		{
			CRect rcItem;
			GetItemRect(nPos, &rcItem);
			if(PointTestState(point, rcItem))
			{
				Expand(pItem, LBE_TOGGLE);
			}
			return;
		}

		//send current selection to combobox
		m_pComboParent->PostMessage( WM_SELECTED_ITEM, (WPARAM)nPos, 0 );
	}

	//Destroy the dropdown list
	ReleaseCapture();
	m_pComboParent->PostMessage( WM_DESTROY_DROPLIST );

	//
	// Return so that the listbox can be destroyed
//	CListBox::OnLButtonDown(nFlags, point);
}

int CDropListBox::GetBottomIndex()
{
	int nTop = GetTopIndex();
	if(nTop == LB_ERR)
	{
		return LB_ERR;
	}
	CRect rc;
	GetClientRect( &rc );

	int nBottomIndex = LB_ERR;
	//variable height
	if((GetStyle() & LBS_OWNERDRAWVARIABLE) == LBS_OWNERDRAWVARIABLE)
	{
		int i, nHeight, nCount = GetCount(), nTotalHeight = 0;
		for(i = nTop; i < nCount; i++)
		{
			if((nHeight = GetItemHeight(i)) == LB_ERR)
			{
				ASSERT(FALSE);
				break;
			}
			nTotalHeight += nHeight;
			if(nTotalHeight > rc.Height())
			{
				break;
			}
		}
		nBottomIndex = i;
	}
	else
	{
		int nVisCount = rc.Height() / GetItemHeight(0);
		nBottomIndex = nTop + nVisCount;
		if(nBottomIndex > GetCount())
		{
			nBottomIndex = GetCount();
		}
	}
	//AfxTrace(_T("GetBottomIndex=%d\n"), nBottomIndex);
	return nBottomIndex;
}

void CDropListBox::SetTopIdx(int nPos, BOOL bUpdateScrollbar)
{
	m_nLastTopIdx = nPos;
	SetTopIndex( nPos );
	if( bUpdateScrollbar )
	{
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		if( m_pDropWnd->GetScrollBarPtr()->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPos = m_nLastTopIdx;
			m_pDropWnd->GetScrollBarPtr()->SetScrollInfo( &info );
		}
	}
}

void CDropListBox::GetTextSize(LPCTSTR lpszText, int nCount, CSize &size)
{
	CClientDC dc(this);
	int nSave = dc.SaveDC();
	dc.SelectObject( m_pComboParent->GetFont() );
	size = dc.GetTextExtent( lpszText, nCount );
	if( m_pComboParent->GetACBStyle() & ACBS_CHECKED )
	{
		size.cx += 14;
	}
	dc.RestoreDC(nSave);
}

int CDropListBox::AtomicAddListItem( PLIST_ITEM pItem )
{
	int nOldState = pItem->state;

	pItem->state &= ~ACBIS_INVISIBLE;
	int nPos = AddString(pItem->strText);

	if(nPos >= 0)
	{
		SetItemDataPtr( nPos, (void*)pItem );
	}
	else
	{
		pItem->state = nOldState;
	}
	return nPos;
}
int CDropListBox::AtomicInsertListItem(int nIndex, PLIST_ITEM pItem)
{
	int nOldState = pItem->state;

	pItem->state &= ~ACBIS_INVISIBLE;
	int nPos = InsertString(nIndex, pItem->strText);
	if(nPos >= 0)
	{
		SetItemDataPtr( nPos, (void*)pItem );
	}
	else
	{
		pItem->state = nOldState;
	}
	return nPos;
}
int CDropListBox::AtomicDeleteListItem( PLIST_ITEM pItem )
{
	int nPos = GetItemIndex(pItem);
	//not in the listbox
	if(nPos < 0)
	{
		return nPos;
	}
	
	pItem->state |= ACBIS_INVISIBLE;
	DeleteString(nPos);

	return nPos;
}

int CDropListBox::AddListItem( PLIST_ITEM pItem )
{
//	AfxTrace(_T("AddListItem: %d\n"), GetCount());
	ASSERT(pItem != NULL);

	int nPos = AtomicAddListItem(pItem);

	if(nPos >= 0)
	{
		//add children if needed
		if( !(pItem->state & ACBIS_COLLAPSED) && (pItem->GetChildCount() > 0) )
		{
			int i, nCount = pItem->GetChildCount();
			for(i = 0; i < nCount; i++)
			{
				PLIST_ITEM pChild = pItem->GetChildAt(i);
				AddListItem(pChild);
			}
		}
	}

	return nPos;
}

int CDropListBox::InsertListItem(int nIndex, PLIST_ITEM pItem)
{
//	AfxTrace(_T("InsertListItem: %d\n"), nIndex);
	ASSERT(pItem != NULL);

	if(nIndex < 0 || nIndex >= GetCount())
	{
		return AddListItem(pItem);
	}

	//add children firstly
	if( !(pItem->state & ACBIS_COLLAPSED) && (pItem->GetChildCount() > 0) )
	{
		PLIST_ITEM pChild;
		for(int i = pItem->GetChildCount() - 1; i >= 0; i--)
		{
			pChild = pItem->GetChildAt(i);
			AtomicInsertListItem(nIndex, pChild);
		}
	}

	//insert parent at last
	int nPos = AtomicInsertListItem(nIndex, pItem);
	return nPos;
}

int CDropListBox::DeleteListItem( UINT nIndex )
{
	if(nIndex < 0 || nIndex >= GetCount())
	{
		return LB_ERR;
	}

	int nRemain = DeleteString(nIndex);
	return nRemain;
}

void DrawBitmap(CRect& rect, HBITMAP hbmp, CDC* pDC, int nGap = 2)
{
	if(hbmp == NULL)
	{
		return;
	}

	BITMAP bmp;
	::GetObject(hbmp, sizeof(bmp), &bmp);

	int top = 0, height = 0;
	if(rect.Height() >= bmp.bmHeight)
	{
		top = rect.top + (rect.Height() - bmp.bmHeight) / 2;
		height = bmp.bmHeight;
	}
	else
	{
		top = rect.top;
		height = rect.Height();
	}

	HDC hMemDC = ::CreateCompatibleDC(pDC->GetSafeHdc());
	::SelectObject(hMemDC, hbmp);	

	::BitBlt(pDC->GetSafeHdc(), rect.left + nGap, top, bmp.bmWidth, height, hMemDC, 0, 0, SRCCOPY);
	rect.left += bmp.bmWidth + nGap * 2;

	::DeleteDC(hMemDC);
}

int CDropListBox::DrawImage(CRect& rect, CDC* pDC, CImageList* pImageList, int nImage, int nGap)
{
	if(rect.Width() <= 0 || pImageList == NULL || pImageList->GetImageCount() <= 0)
	{
		return 0;
	}
	if(nImage < 0 || nImage >= pImageList->GetImageCount())
	{
		return 0;
	}
	IMAGEINFO info;
	if(!pImageList->GetImageInfo(nImage, &info))
	{
		return 0;
	}
	SIZE sizeImage;
	sizeImage.cx = info.rcImage.right - info.rcImage.left;
	sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
	if(sizeImage.cx <= 0 || sizeImage.cy <= 0)
	{
		return 0;
	}

	POINT point;
	point.y = rect.top + ((rect.Height() - sizeImage.cy) >> 1);
	point.x = rect.left + nGap;
	
	SIZE size;
	size.cx = rect.Width() < sizeImage.cx ? rect.Width() : sizeImage.cx;
	size.cy = rect.Height() < sizeImage.cy ? rect.Height() : sizeImage.cy;

	pImageList->DrawIndirect(pDC, nImage, point, size, CPoint(0, 0));
	
	int nWidth = size.cx + 2 * nGap;
	return nWidth;
}

int CDropListBox::DrawStateImage(CRect& rect, CDC* pDC, PLIST_ITEM pItem, int nGap)
{
	if(pItem == NULL || pItem->GetChildCount() <= 0)
	{
		return 0;
	}
	CImageList* pImageList = m_pComboParent->GetImageList(ACBSIL_LISTBOX_STATE);
	if(pImageList == NULL)
	{
		return 0;
	}

	int nImage = (pItem->state & ACBIS_COLLAPSED) ? 1 : 0;

	return DrawImage(rect, pDC, pImageList, nImage, nGap);
}

int CDropListBox::DrawItemImage(CRect& rect, CDC* pDC, PLIST_ITEM pItem, int nGap)
{
	if(pItem == NULL || pItem->iImage < 0)
	{
		return 0;
	}
	CImageList* pImageList = m_pComboParent->GetImageList(ACBSIL_LISTBOX_NORMAL);
	if(pImageList == NULL)
	{
		return 0;
	}
	
	return DrawImage(rect, pDC, pImageList, pItem->iImage, nGap);
}

void CDropListBox::DrawItem(LPDRAWITEMSTRUCT pDIStruct) 
{
	//
	// Set var's
	BOOL			bIsSelected = FALSE;
	BOOL			bIsDisabled = FALSE;
	BOOL			bIsChecked = FALSE;
	COLORREF		clrNormal = ::GetSysColor(COLOR_WINDOW);
	COLORREF		clrSelected = ::GetSysColor(COLOR_HIGHLIGHT);
	COLORREF		clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	COLORREF		clrTextHilight = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	COLORREF		clrTextDisabled = ::GetSysColor(COLOR_GRAYTEXT);

	CDC* pDC = CDC::FromHandle(pDIStruct->hDC);
	PLIST_ITEM pItem = (PLIST_ITEM)GetItemDataPtr(pDIStruct->itemID);

	bIsDisabled = (pItem->state & ACBIS_DISABLED);
	bIsChecked = (pItem->state & ACBIS_CHECKED);
	if( /*(pDIStruct->itemAction | ODA_SELECT) &&*/ (pDIStruct->itemState & ODS_SELECTED) )
		bIsSelected = TRUE;

	CRect rcItem = pDIStruct->rcItem;
	CRect rcText = pDIStruct->rcItem;
	CRect rcCheck = pDIStruct->rcItem;

	int nSavedDC = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT);
	if( m_pComboParent->GetACBStyle() & ACBS_CHECKED )
	{
		if( bIsChecked )
		{
			rcCheck.right = rcCheck.left + rcCheck.Height();
			pDC->DrawFrameControl( &rcCheck, DFC_MENU, DFCS_MENUCHECK );
		}
		rcText.left += rcItem.Height()-4;
		rcItem.left += rcItem.Height()-4;
	}

	//disabled
	if(bIsDisabled)
	{
		pDC->FillSolidRect( rcItem, clrNormal);

		pDC->SetTextColor(clrTextDisabled);
	}
	//selected
	else if(bIsSelected)
	{
		pDC->FillSolidRect( rcItem, clrSelected);

		pDC->SetTextColor( clrTextHilight );
	}
	//normal
	else
	{
		if(pItem->GetChildCount() > 0)
		{
			CRect rcTemp = rcItem;
			rcTemp.bottom -= 1;
			pDC->FillSolidRect(rcTemp, RGB(245, 245, 245));

			rcTemp = rcItem;
			rcTemp.top = rcTemp.bottom - 1;
			pDC->FillSolidRect(rcTemp, RGB(200, 216, 255));
		}
		else
		{
			pDC->FillSolidRect( rcItem, clrNormal);
		}

		pDC->SetTextColor(clrText);
	}

	int nWidth = DrawStateImage(rcText, pDC, pItem, m_nSpaceX);
	rcText.left += nWidth;

	nWidth = DrawItemImage(rcText, pDC, pItem, m_nSpaceX);
	rcText.left += nWidth;

	//Draw Text
	rcText.left += 2;
	pDC->DrawText( pItem->strText, &rcText, DT_SINGLELINE|DT_VCENTER);

	//restore dc
	pDC->RestoreDC(nSavedDC);
}

int CDropListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Replace the next line with your message handler code
	return 0;
}

void CDropListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
//	AfxTrace(_T("MeasureItem: %d\n"), lpMeasureItemStruct->itemID);
	//only setting for variable
	if((GetStyle() & LBS_OWNERDRAWVARIABLE) == LBS_OWNERDRAWVARIABLE)
	{
		PLIST_ITEM pItem = m_pDropWnd->GetListBoxItem(lpMeasureItemStruct->itemID);/*GetListItem(lpMeasureItemStruct->itemID);*/
		ASSERT(pItem != NULL);

		if(pItem->GetChildCount() > 0)
		{
			lpMeasureItemStruct->itemHeight = 40;
		}
		else
		{
			lpMeasureItemStruct->itemHeight = 20;
		}
	}
}

BOOL CDropListBox::DestroyWindow() 
{	
	return CListBox::DestroyWindow();
}

int CDropListBox::SetCurSel(int nSelect)
{
	PLIST_ITEM pItem = NULL;
	int nCur = GetCurSel();
	int nWay = nSelect - nCur;
	int nTmp = nSelect;

	// Select the next in list the is NOT disabled
	if( nWay < 0 )
	{
		// Select previous in list
		pItem = (PLIST_ITEM)GetItemDataPtr( nTmp );
		while( (DWORD)pItem != -1 )
		{
			if( !(pItem->state & ACBIS_DISABLED) )
			{
				nSelect = nTmp;
				break;
			}
			nTmp--;
			pItem = (PLIST_ITEM)GetItemDataPtr( nTmp );
		}
	}
	else
		if( nWay > 0 )
		{
			// Select next in list
			pItem = (PLIST_ITEM)GetItemDataPtr( nTmp );
			while( (DWORD)pItem != -1 )
			{
				if( !(pItem->state & ACBIS_DISABLED) )
				{
					nSelect = nTmp;
					break;
				}
				nTmp++;
				pItem = (PLIST_ITEM)GetItemDataPtr( nTmp );
			}
		}

	pItem = (PLIST_ITEM)GetItemDataPtr( nSelect);
	if( (DWORD)pItem != -1 )
	{
		if(pItem->state & ACBIS_DISABLED)
		{
			return nSelect;
		}
	}
	int nr = CListBox::SetCurSel( nSelect );

	if( nr != -1 )
	{
		//
		// Set scrollbar
		int nTopIdx = GetTopIndex();

		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		if( m_pDropWnd->GetScrollBarPtr()->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPos = nTopIdx;
			m_pDropWnd->GetScrollBarPtr()->SetScrollInfo( &info );
		}
	}
	return nr;
}

int CDropListBox::SetCurSel(PLIST_ITEM pItem)
{
	int nSelected = -1, i = 0, nCount = GetCount();
	for(i = 0; i < nCount; i++)
	{
		PLIST_ITEM pListItem = (PLIST_ITEM)GetItemDataPtr( i );
		if(pListItem == pItem)
		{
			nSelected = i;
			break;
		}
	}
	if((nSelected < 0) || (pItem->state & ACBIS_DISABLED))
	{
		return nSelected;
	}

	int nr = CListBox::SetCurSel( nSelected );
	if( nr != -1 )
	{
		//
		// Set scrollbar
		int nTopIdx = GetTopIndex();
		
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		if( m_pDropWnd->GetScrollBarPtr()->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPos = nTopIdx;
			m_pDropWnd->GetScrollBarPtr()->SetScrollInfo( &info );
		}
	}
	return nr;
}

int CDropListBox::GetTotalItemHeight(int nStartIndex, int nCount)
{
	ASSERT(nStartIndex >= 0);
	if(nCount < 0)
	{
		nCount = GetCount();
	}
	if(nCount - nStartIndex > GetCount())
	{
		nCount = GetCount() - nStartIndex;
	}

	int nHeight, nTotal = 0;
	//variable height
	if((GetStyle() & LBS_OWNERDRAWVARIABLE) == LBS_OWNERDRAWVARIABLE)
	{
		for(int i = nStartIndex; nCount > 0; i++, nCount--)
		{
			if((nHeight = GetItemHeight(i)) == LB_ERR)
			{
				return LB_ERR;
			}
			nTotal += nHeight;
		}
		return nTotal;
	}
	//non-variable style
	else
	{
		if((nHeight = GetItemHeight(0)) == LB_ERR)
		{
			return LB_ERR;
		}
		return nHeight * nCount;
	}
}

/* Return the item index of the point locate in */
int CDropListBox::PointTest(CPoint point)
{
	int i = GetTopIndex(), nCount = GetCount(), nResult;
	if(i == LB_ERR)
	{
		return LB_ERR;
	}

	CRect rect;
	for(; i < nCount; i++)
	{
		if((nResult = GetItemRect(i, &rect)) == LB_ERR)
		{
			return LB_ERR;
		}
		if(PtInRect(&rect, point))
		{
			return i;
		}
	}
	return LB_ERR;
}

BOOL CDropListBox::PointTestState(CPoint point, CRect& rcItem)
{
	if(!rcItem.PtInRect(point))
	{
		return FALSE;
	}
	CRect rc = rcItem;
	rc.left += 4;
	rc.right = rc.left + 16;
	return rc.PtInRect(point);
}

int CDropListBox::GetItemIndex(PLIST_ITEM pItem)
{
	PLIST_ITEM ptr;
	for(int i = GetCount() - 1; i >= 0; i--)
	{
		ptr = (PLIST_ITEM)GetItemDataPtr(i);

		ASSERT(ptr != NULL && ((int)ptr != -1));
		if(ptr != NULL && ((int)ptr != -1))
		{
			if(ptr == pItem)
			{
				return i;
			}
		}
	}
	return -1;
}

BOOL CDropListBox::Expand(PLIST_ITEM pItem, UINT nCode)
{
	if(pItem == NULL)
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	if(nCode == LBE_COLLAPSE)
	{
		bResult = Collapse(pItem);
	}
	else if(nCode == LBE_EXPAND)
	{
		bResult = Expand(pItem);
	}
	else if(nCode == LBE_TOGGLE)
	{
		if(pItem->state & ACBIS_COLLAPSED)
		{
			bResult = Expand(pItem);
		}
		else
		{
			bResult = Collapse(pItem);
		}
	}

	if(bResult)
	{
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		if( m_pDropWnd->GetScrollBarPtr()->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPage = GetBottomIndex() - GetTopIndex();
			info.nMax = GetCount()-1;
			info.nMin = 0;
			m_pDropWnd->GetScrollBarPtr()->SetScrollInfo( &info );
		}
	}
	return bResult;
}

BOOL CDropListBox::Expand(PLIST_ITEM pItem)
{
	int i, nPos, nParentPos = GetItemIndex(pItem);
	ASSERT(nParentPos >= 0);
	if(nParentPos < 0 || pItem->GetChildCount() <= 0)
	{
		return FALSE;
	}
	for(i = pItem->GetChildCount() - 1; i >= 0; i--)
	{
		PLIST_ITEM pChild = pItem->GetChildAt(i);
		ASSERT(pChild);

		nPos = GetItemIndex(pChild);
		//already in the listbox
		if(nPos >= 0)
		{
			continue;
		}
		
		InsertListItem(nParentPos + 1, pChild);
	}

	pItem->state &= ~ACBIS_COLLAPSED;
	return TRUE;
}
BOOL CDropListBox::Collapse(PLIST_ITEM pItem)
{
	int i, nCount = pItem->GetChildCount();
	for(i = 0; i < nCount; i++)
	{
		PLIST_ITEM pChild = pItem->GetChildAt(i);
		ASSERT(pChild);

		AtomicDeleteListItem(pChild);
	}
	pItem->state |= ACBIS_COLLAPSED;
	return TRUE;
}

void CDropListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//Click in the client area?
	CRect rcClient;
	GetClientRect( rcClient );
	if( !rcClient.PtInRect( point ) )
	{
		ReleaseCapture();
		GetParent()->SendMessage( WM_VRC_SETCAPTURE );
	}

	int nPos = PointTest(point);
	if(nPos != LB_ERR)
	{
		PLIST_ITEM pItem = (PLIST_ITEM)GetItemDataPtr(nPos);
		if((int)pItem != -1 && pItem != NULL)
		{
			if(pItem->state & ACBIS_DISABLED)
			{
				return;
			}
			if(pItem->GetChildCount() > 0)
			{
				Expand(pItem, LBE_TOGGLE);
				return;
			}
		}
	}
	CListBox::OnLButtonDblClk(nFlags, point);
}


