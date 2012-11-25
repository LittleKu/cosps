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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDropListBox

CDropListBox::CDropListBox( CWnd* pComboParent, CDropScrollBar* pScroll )
:
	m_pComboParent( pComboParent ),
	m_pScroll( pScroll )
{
// 	m_pListFont = new CFont;
// 	LOGFONT logFont;
// 	memset( &logFont, 0, sizeof(LOGFONT) );
// 	strcpy( logFont.lfFaceName, "MS Sans Serif" );
// 	logFont.lfHeight = 20;
// 	m_pListFont->CreateFontIndirect(&logFont);

	m_nLastTopIdx = 0;

	m_dwACBStyle = 0;
}

CDropListBox::~CDropListBox()
{
//	delete m_pListFont;
}


BEGIN_MESSAGE_MAP(CDropListBox, CListBox)
	//{{AFX_MSG_MAP(CDropListBox)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
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

	SetFont( static_cast<CAdvComboBox*>(m_pComboParent)->GetFont() ); 
	return 0;
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
		if( GetCurSel() != nPos && !pItem->bDisabled )
		{
			SetCurSel( nPos );
		}
	}

	//
	// Check if we have autoscrolled
	if( m_nLastTopIdx != GetTopIndex() )
	{
		int nDiff = m_nLastTopIdx - GetTopIndex();
		m_nLastTopIdx = GetTopIndex();

		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		if( m_pScroll->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPos = m_nLastTopIdx;
			m_pScroll->SetScrollInfo( &info );
		}
	}


//	OutputDebugString( "DropListBox MouseMove\n" );

	CListBox::OnMouseMove(nFlags, point);

}


void CDropListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnLButtonDown(nFlags, point);
//	CListBox::OnLButtonUp(nFlags, point);
}

void CDropListBox::OnLButtonDown(UINT nFlags, CPoint point) 
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
		if( pItem->bDisabled )
		{
			return;
		}
	}

	nPos = GetCurSel();

	//
	// Is selected item disabled
	if( nPos != LB_ERR )
	{
		PLIST_ITEM pItem;
		pItem = (PLIST_ITEM)GetItemDataPtr( nPos );
		if( pItem->bDisabled )
		{
			return;
		}
	}

// 	if(point.x < 5 && nPos % 5 == 0)
// 	{
// 		DeleteString(nPos+1);
// 		DeleteString(nPos+2);
// 		DeleteString(nPos+3);
// 		DeleteString(nPos+4);
// 		return;
// 	}

	//
	// Send current selection to comboedit
	if( nPos != -1 )
		m_pComboParent->PostMessage( WM_SELECTED_ITEM, (WPARAM)nPos, 0 );
//	CString str;
//	str.Format( "DropListWnd: Selected item: %d\n", nPos );
//	OutputDebugString( str );

	//
	// Destroy dropdown
	ReleaseCapture();
	//CAdvComboBox* pList = static_cast<CAdvComboBox*>(m_pComboParent);
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
	AfxTrace(_T("GetBottomIndex=%d\n"), nBottomIndex);
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
		if( m_pScroll->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPos = m_nLastTopIdx;
			m_pScroll->SetScrollInfo( &info );
		}
	}
}

void CDropListBox::GetTextSize(LPCTSTR lpszText, int nCount, CSize &size)
{
	CClientDC dc(this);
	int nSave = dc.SaveDC();
	dc.SelectObject( static_cast<CAdvComboBox*>(m_pComboParent)->GetFont() );
	size = dc.GetTextExtent( lpszText, nCount );
	if( m_dwACBStyle & ACBS_CHECKED )
	{
		size.cx += 14;
	}
	dc.RestoreDC(nSave);
}


int CDropListBox::AddListItem( LIST_ITEM& item )
{
	PLIST_ITEM pItem = new LIST_ITEM;
	*pItem = item;
	int nPos = AddString( pItem->strText.c_str() );
	SetItemDataPtr( nPos, (void*)pItem );
	return nPos;
}


void CDropListBox::DrawItem(LPDRAWITEMSTRUCT pDIStruct) 
{
	//
	// Set var's
	BOOL			bIsSelected = FALSE;
	BOOL			bIsDisabled = FALSE;
	BOOL			bIsChecked = FALSE;
	BOOL			bIsNew = FALSE; // 2002-08-19
	COLORREF		clrNormal = ::GetSysColor(COLOR_WINDOW);
	COLORREF		clrSelected = ::GetSysColor(COLOR_HIGHLIGHT);
	COLORREF		clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	COLORREF		clrTextHilight = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	COLORREF		clrBackground = ::GetSysColor(COLOR_HIGHLIGHT);
	COLORREF		clrTextDisabled = ::GetSysColor(COLOR_GRAYTEXT);
	PLIST_ITEM		pItem;

	CDC* pDC = CDC::FromHandle(pDIStruct->hDC);
	pItem = (PLIST_ITEM)GetItemDataPtr(pDIStruct->itemID);

	bIsDisabled = pItem->bDisabled;
	bIsChecked = pItem->bChecked;
	if( (pDIStruct->itemAction | ODA_SELECT) && (pDIStruct->itemState & ODS_SELECTED) )
		bIsSelected = TRUE;

	CRect rcItem = pDIStruct->rcItem;
	CRect rcBitmap = pDIStruct->rcItem;
	CRect rcText = pDIStruct->rcItem;
	CRect rcCenteredText = pDIStruct->rcItem;
	CRect rcCheck = pDIStruct->rcItem;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(clrText);

	if( m_dwACBStyle & ACBS_CHECKED )
	{
		if( bIsChecked )
		{
			rcCheck.right = rcCheck.left + rcCheck.Height();
			pDC->DrawFrameControl( &rcCheck, DFC_MENU, DFCS_MENUCHECK );
		}
		rcText.left += rcItem.Height()-4;
		rcItem.left += rcItem.Height()-4;
	}

	//
	// Is item selected
	if( bIsSelected )
	{
		if( !bIsDisabled )
		{
			pDC->SetBkColor( clrBackground );
			pDC->SetTextColor( clrTextHilight );
			pDC->FillSolidRect( rcItem, clrSelected);
		}
	}
	else
	{
		pDC->SetBkColor( clrNormal );
		pDC->SetTextColor( clrText );
		pDC->FillSolidRect( rcItem, clrNormal);
		if(pDIStruct->itemID % 5 == 0)
		{
			pDC->FillSolidRect( rcItem, RGB(245, 245, 245));
		}
	}

	if( bIsDisabled )
	{
		pDC->SetTextColor(clrTextDisabled);
	}

	//
	// Draw text
	rcText.left += 2;
	pDC->DrawText( pItem->strText.c_str(), &rcText, DT_SINGLELINE|DT_VCENTER);
}

int CDropListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Replace the next line with your message handler code
	return 0;
}

void CDropListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	//only setting for variable
	if(GetStyle() & LBS_OWNERDRAWVARIABLE)
	{
		if(lpMeasureItemStruct->itemID % 5 == 0)
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
	PLIST_ITEM pItem;
	for( int i = 0; i < GetCount(); i++ )
	{
		pItem = (PLIST_ITEM)GetItemDataPtr( i );
		delete pItem;
	}
	
	return CListBox::DestroyWindow();
}

void CDropListBox::SetDLBStyle(DWORD dwStyle)
{
	m_dwACBStyle = dwStyle;
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
			if( !pItem->bDisabled )
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
				if( !pItem->bDisabled )
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
		if( pItem->bDisabled )
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
		if( m_pScroll->GetScrollInfo( &info, SIF_ALL|SIF_DISABLENOSCROLL ) )
		{
			info.nPos = nTopIdx;
			m_pScroll->SetScrollInfo( &info );
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



