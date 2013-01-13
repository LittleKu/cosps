// SListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SListCtrl.h"
#include "MemDC.h"
#include "gtb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSListCtrl
#ifdef _DEBUG
#define ASSERT_ROW_COUNT(nItem) ASSERT(nItem >= 0 && nItem < GetItemCount())
#define ASSERT_COL_COUNT(nItem) ASSERT(nItem >= 0 && nItem < GetHeaderCtrl()->GetItemCount())
#define ASSERT_ROW_COL_COUNT(nItem, nSubItem) \
	ASSERT_ROW_COUNT(nItem); \
ASSERT_COL_COUNT(nSubItem)
#else
#define ASSERT_ROW_COUNT(nItem)
#define ASSERT_COL_COUNT(nItem)
#define ASSERT_ROW_COL_COUNT(nItem, nSubItem)
#endif

#ifndef LVBKIF_TYPE_WATERMARK
#define LVBKIF_TYPE_WATERMARK   0x10000000
#endif

UINT WM_SLISTCTRL_CHECKBOX_CLICKED = ::RegisterWindowMessage(_T("WM_SLISTCTRL_CHECKBOX_CLICKED"));

CSListCtrl::CSListCtrl()
{
	m_bCustomDraw			= false;

	m_crBtnFace             = ::GetSysColor(COLOR_BTNFACE);
	m_crHighLight           = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crHighLightText       = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	//background color for lost focus selection items
	m_crBtnFace				= RGB(198, 215, 192);
	
	//background color for focus selection items
	m_crHighLight			= RGB(179, 200, 232);

	//text color for focus selection items
	m_crHighLightText       = ::GetSysColor(COLOR_WINDOWTEXT);

	//background color for non-selection items
	m_crWindow              = ::GetSysColor(COLOR_WINDOW);
	m_crWindowTextBk = m_crWindow;

	//text color for non-selection items
	m_crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);

	m_pSortable = new CSorter(this);
}

CSListCtrl::~CSListCtrl()
{
	if(m_pSortable != NULL)
	{
		delete m_pSortable;
		m_pSortable = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSListCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
//	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
// 	ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClick) 
//	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSListCtrl message handlers

void CSListCtrl::PreSubclassWindow() 
{
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	ASSERT(pHeader);
	if (pHeader)
	{
		VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));
	}
	
	SetColors();
	CListCtrl::PreSubclassWindow();
}

void CSListCtrl::SetColors()
{
	m_crBtnFace             = ::GetSysColor(COLOR_BTNFACE);
	m_crHighLight           = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crHighLightText       = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	//background color for lost focus selection items
	m_crBtnFace				= RGB(198, 215, 192);
	
	//background color for focus selection items
	m_crHighLight			= RGB(179, 200, 232);
	
	//text color for focus selection items
	m_crHighLightText       = ::GetSysColor(COLOR_WINDOWTEXT);
	
	//background color for non-selection items
	m_crWindow              = ::GetSysColor(COLOR_WINDOW);
	m_crWindowTextBk = m_crWindow;
	
	//text color for non-selection items
	m_crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);

	SetBkColor(m_crWindow);
	SetTextBkColor(m_crWindowTextBk);
	SetTextColor(m_crWindowText);
	
	// Must explicitly set a NULL watermark bitmap, to clear any already set watermark bitmap.
// 	LVBKIMAGE lvimg = {0};
// 	lvimg.ulFlags = LVBKIF_TYPE_WATERMARK;
// 	SetBkImage(&lvimg);
}

BOOL CSListCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if(message != WM_DRAWITEM) {
		//catch the prepaint and copy struct
		if(message == WM_NOTIFY && ((NMHDR*)lParam)->code == NM_CUSTOMDRAW &&
			((LPNMLVCUSTOMDRAW)lParam)->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
			
			m_bCustomDraw = CListCtrl::OnChildNotify(message, wParam, lParam, pResult);
			if(m_bCustomDraw)
				m_lvcd = *((LPNMLVCUSTOMDRAW)lParam);
			
			return m_bCustomDraw;
		}
		
		return CListCtrl::OnChildNotify(message, wParam, lParam, pResult);
	}
	
	ASSERT(pResult == NULL); // no return value expected
	
	DrawItem((LPDRAWITEMSTRUCT)lParam);
	return TRUE;
}

void CSListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rcItem(lpDrawItemStruct->rcItem);
	CDC *oDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CMemDC pDC(oDC, &rcItem, m_crWindow);
	CFont *pOldFont = pDC->SelectObject(GetFont());

	int iCount = GetHeaderCtrl()->GetItemCount();
	for(int i = 0; i < iCount; i++) 
	{
		DrawItem(lpDrawItemStruct->itemID, i, pDC);
	}

	//pDC->Flush();
	pDC->SelectObject(pOldFont);
}

BOOL CSListCtrl::OnEraseBkgnd(CDC* pDC)
{
// 	if(1)
// 	{
// 		RECT clientRect;
// 		GetClientRect(&clientRect);
// 		pDC->FillSolidRect(&clientRect,GetBkColor());
// 		return TRUE;
// 	}
	int itemCount = GetItemCount();
// 	if (!itemCount)
// 		return CListCtrl::OnEraseBkgnd(pDC);
	
	RECT clientRect;
	RECT itemRect;
	int topIndex = GetTopIndex();
	int maxItems = GetCountPerPage();
	int drawnItems = itemCount < maxItems ? itemCount : maxItems;
	CRect rcClip;
	
	//draw top portion
	GetClientRect(&clientRect);
	rcClip = clientRect;
	GetItemRect(topIndex, &itemRect, LVIR_BOUNDS);
	clientRect.bottom = itemRect.top;
	if (m_crWindowTextBk != CLR_NONE)
		pDC->FillSolidRect(&clientRect,GetBkColor());
	else
		rcClip.top = itemRect.top;
	
	//draw bottom portion if we have to
	if(topIndex + maxItems >= itemCount) {
		GetClientRect(&clientRect);
		GetItemRect(topIndex + drawnItems - 1, &itemRect, LVIR_BOUNDS);
		clientRect.top = itemRect.bottom;
		rcClip.bottom = itemRect.bottom;
		if (m_crWindowTextBk != CLR_NONE)
			pDC->FillSolidRect(&clientRect, GetBkColor());
	}
	
	//draw right half if we need to
	if (itemRect.right < clientRect.right) {
		GetClientRect(&clientRect);
		clientRect.left = itemRect.right;
		rcClip.right = itemRect.right;
		if (m_crWindowTextBk != CLR_NONE)
			pDC->FillSolidRect(&clientRect, GetBkColor());
	}
	
	if (m_crWindowTextBk == CLR_NONE){
		CRect rcClipBox;
		pDC->GetClipBox(&rcClipBox);
		rcClipBox.SubtractRect(&rcClipBox, &rcClip);
		if (!rcClipBox.IsRectEmpty()){
			pDC->ExcludeClipRect(&rcClip);
			CListCtrl::OnEraseBkgnd(pDC);
			InvalidateRect(&rcClip, FALSE);
		}
	}
	return TRUE;
}
void CSListCtrl::OnSize( UINT nType, int cx, int cy  )
{
	CListCtrl::OnSize(nType, cx, cy);
	
    GetClientRect(m_rectClient);
	
    CHeaderCtrl* pHC;
    pHC = GetHeaderCtrl();
    if (pHC != NULL)
    {
        CRect rectHeader;
        pHC->GetItemRect( 0, &rectHeader );
        m_rectClient.top += rectHeader.bottom;
    }
}
void CSListCtrl::OnPaint()
{
//	CPaintDC dc(this);
	
    // Paint to a memory device context to reduce screen flicker.
//    CMemDC memDC(&dc, &m_rectClient, FALSE);
		
	// Let the window do its default painting...
//    CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
//	CListCtrl::OnPaint();
    Default();
	if (GetItemCount() <= 0)
	{
		CDC* pDC = GetDC();
		int nSavedDC = pDC->SaveDC();

		//Get Content client area
		CRect rcClient;
		GetClientRect(&rcClient);

		CRect rcHeaderWindow;
		m_HeaderCtrl.GetWindowRect(&rcHeaderWindow);
		rcClient.top += rcHeaderWindow.Height();

		//Draw
		DrawEmptyBk(pDC, rcClient);

		//Restore
		pDC->RestoreDC(nSavedDC);
		ReleaseDC(pDC);
	}
}

void CSListCtrl::DrawEmptyBk(CDC* pDC, CRect rcClient)
{
	//1. Draw background
	pDC->FillSolidRect(&rcClient, /*m_crWindow*/RGB(255, 128, 0));

	//No valid row height, don't draw anything
	if(m_nRowHeight <= 0 || rcClient.Height() < (2 * m_nRowHeight))
	{
		return;
	}

	COLORREF crGridLine = RGB(236, 233, 216);

	//Draw horizontal lines
	for(int nRowY = rcClient.top + m_nRowHeight; nRowY < rcClient.bottom; nRowY += (m_nRowHeight + 1))
	{
		pDC->FillSolidRect(rcClient.left, nRowY, rcClient.Width(), 1, crGridLine);
	}

	//Get horizontal scroll bar distance
	int nMinPos, nMaxPos, nCurPos;
	GetScrollRange(SB_HORZ, &nMinPos, &nMaxPos);
	nCurPos = GetScrollPos(SB_HORZ);

	int nHorzDistance = nCurPos - nMinPos;

	//Draw vertical lines
	CRect rcHeaderCol;
	for(int nCol = 0; nCol < m_HeaderCtrl.GetItemCount(); nCol++)
	{
		m_HeaderCtrl.GetItemRect(nCol, &rcHeaderCol);
		if(rcHeaderCol.right >= nHorzDistance)
		{
			pDC->FillSolidRect(rcHeaderCol.right - nHorzDistance, rcClient.top, 1, rcClient.Height(), crGridLine);
		}
	}
}

void CSListCtrl::OnDestroy() 
{
	DeleteAllItems();
	CListCtrl::OnDestroy();	
}

void CSListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.
		
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		DrawItem(static_cast<int> (pLVCD->nmcd.dwItemSpec), pLVCD->iSubItem, pDC);
		
		*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
	}
}

BOOL CSListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* pNMActivate = (NMITEMACTIVATE*)pNMHDR;
	*pResult = 0;

	int nItem = -1;
	
	//+++
	LVHITTESTINFO lvhit;
	lvhit.pt = pNMActivate->ptAction;
	SubItemHitTest(&lvhit);
	if (lvhit.flags & LVHT_ONITEM)
	{
		nItem = lvhit.iItem;
	}
	
	if(nItem != -1)
	{	
		CRect rect;
		int nSubItem = -1;
		
		// check if a subitem checkbox was clicked
		for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
		{
			GetSubItemRect(nItem, i, LVIR_BOUNDS, rect);
			if (rect.PtInRect(pNMActivate->ptAction))
			{
				nSubItem = i;
				break;
			}
		}
		
		if (nSubItem != -1)
		{
			int nChecked = GetItemCheckedState(nItem, nSubItem);
			if (nChecked != SHC_NONE_CHECK_BOX && IsPtInSubItemCheckBox(nItem, nSubItem, pNMActivate->ptAction))
			{
				nChecked = SwitchCheckedState(nChecked);
				
				SetItemCheckedStateByClick(nItem, nSubItem, nChecked, TRUE);
			}
		}
	}

	return FALSE;
}

BOOL CSListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	AfxTrace("[OnColumnClick]: iItem=%d\n", pNMListView->iSubItem);
	
	int nSubItem = pNMListView->iSubItem;

	//1. Check if clicked a check box in header
	if(!m_HeaderCtrl.IsClickedCheckBox(nSubItem))
	{
		//CTRL key is pressed
		m_pSortable->SortColumn(nSubItem, ::GetKeyState( VK_CONTROL ) < 0);
		return FALSE;
	}
	
	//2. Clicked check box in header
	int nCheckedState = GetHeaderCheckedState(nSubItem);
	
	// CHECK_LIST_CTRL_NO_IMAGE = no checkbox
	if (nCheckedState != SHC_NONE_CHECK_BOX)
	{
		nCheckedState = SwitchCheckedState(nCheckedState);

		SetHeaderCheckedState(nSubItem, nCheckedState);
		m_HeaderCtrl.UpdateWindow();
		
		for (int nItem = 0; nItem < GetItemCount(); nItem++)
		{
			if (GetItemCheckedState(nItem, nSubItem) != SHC_NONE_CHECK_BOX)
			{
				SetItemCheckedState(nItem, nSubItem, nCheckedState);
			}
		}
		UpdateWindow();
	}
	return FALSE;
}

void CSListCtrl::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	
	AfxTrace("[OnHeaderClick]: iItem=%d, iButton=%d\n", phdn->iItem, phdn->iButton);
	
	*pResult = 1;
}

CComparator* CSListCtrl::CreateComparator(CSortCondition* pSortCondtions, int nCount)
{
	return NULL;
}
LPARAM CSListCtrl::GetAppData(LPARAM lParam)
{
	if(lParam == NULL)
	{
		return NULL;
	}
	CListItemData* pData = (CListItemData*)lParam;
	return pData->dwItemData;
}

int  CSListCtrl::InsertItem( int nItem, LPCTSTR lpszItem )
{
	ASSERT(nItem >= 0);
	int index = CListCtrl::InsertItem(nItem, lpszItem);
	if (index < 0)
	{
		return index;
	}

	DWORD dwItemData = CListCtrl::GetItemData(index);

	CListItemData *pListCtrlData = new CListItemData;
	pListCtrlData->dwItemData = dwItemData;

	int nColCount = GetHeaderCtrl()->GetItemCount();
	int i;

	pListCtrlData->m_pSubItemDatas = new CListSubItemData[nColCount];
	for(i = 0; i < nColCount; i++)
	{
		pListCtrlData->m_pSubItemDatas[i].m_nCheckState = SHC_NONE_CHECK_BOX;
	}

	CListCtrl::SetItemData(index, (DWORD)pListCtrlData);
	return index;
}

int CSListCtrl::InsertItem(const LVITEM* pItem, BOOL bVirgin)
{
	ASSERT(pItem->iItem >= 0);
	int index = CListCtrl::InsertItem(pItem);
	
	if (index < 0)
	{
		return index;
	}	
	//Application has the knowledge about the internal data
	if(bVirgin)
	{
		ASSERT( (pItem->mask & LVIF_PARAM) != 0 );
		return index;
	}
	
	CListItemData *pListCtrlData = new CListItemData;
	pListCtrlData->dwItemData = pItem->lParam;
	
	int nColCount = GetHeaderCtrl()->GetItemCount();
	int i;
	pListCtrlData->m_pSubItemDatas = new CListSubItemData[nColCount];
	for(i = 0; i < nColCount; i++)
	{
		pListCtrlData->m_pSubItemDatas[i].m_nCheckState = SHC_NONE_CHECK_BOX;
	}
	
	CListCtrl::SetItemData(index, (DWORD)pListCtrlData);
	
	return index;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteItem
BOOL CSListCtrl::DeleteItem(int nItem)
{
	ASSERT_ROW_COUNT(nItem);
	
	CListItemData *pXLCD = (CListItemData *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
		delete pXLCD;
	CListCtrl::SetItemData(nItem, 0);
	return CListCtrl::DeleteItem(nItem);
}

///////////////////////////////////////////////////////////////////////////////
// DeleteAllItems
BOOL CSListCtrl::DeleteAllItems()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		CListItemData *pXLCD = (CListItemData *) CListCtrl::GetItemData(i);
		if (pXLCD)
			delete pXLCD;
		CListCtrl::SetItemData(i, 0);
	}
	
	return CListCtrl::DeleteAllItems();
}

DWORD CSListCtrl::GetItemData(int nItem) const
{
	ASSERT_ROW_COUNT(nItem);
	
	CListItemData *pXLCD = (CListItemData *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return 0;
	}
	
	return pXLCD->dwItemData;
}

//Image Related functions
void CSListCtrl::SetItemImage(int nItem, int nSubItem, int nImage, CImageList* pImageList)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	
	//1. Update data: checked state
	CListItemData *pData = (CListItemData *) CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);
	
	if(pData->m_pSubItemDatas[nSubItem].m_pListImage == NULL)
	{
		pData->m_pSubItemDatas[nSubItem].m_pListImage = new CListImage();
	}
	pData->m_pSubItemDatas[nSubItem].m_pListImage->m_nImage = nImage;
	if(pImageList)
	{
		pData->m_pSubItemDatas[nSubItem].m_pListImage->m_imageList = pImageList;
	}
	
	//2. Update window
	//InvalidateSubItem(nItem, nSubItem);
}

//Progress Related functions
void CSListCtrl::SetItemProgress(int nItem, int nSubItem, int nCurrValue, int nMaxValue)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	
	//1. Update data: checked state
	CListItemData *pData = (CListItemData *) CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);
	
	if(pData->m_pSubItemDatas[nSubItem].m_pListPrgsBar == NULL)
	{
		pData->m_pSubItemDatas[nSubItem].m_pListPrgsBar = new CListProgress();
	}
	pData->m_pSubItemDatas[nSubItem].m_pListPrgsBar->m_nValue = nCurrValue;
	if(nMaxValue > 0)
	{
		pData->m_pSubItemDatas[nSubItem].m_pListPrgsBar->m_nMaxValue = nMaxValue;
	}
	
	//2. Update window
	//InvalidateSubItem(nItem, nSubItem);
}

void CSListCtrl::SetRowHeight(int nRowHeight)
{
	ASSERT(::IsWindow(m_hWnd));
	m_nRowHeight = nRowHeight;
	if(m_nRowHeight < 0)
	{
		return;
	}
	if(m_ILRowHeight.GetSafeHandle() != NULL)
	{
		m_ILRowHeight.DeleteImageList();
	}
	m_ILRowHeight.Create(m_nRowHeight, m_nRowHeight, ILC_COLOR4, 1, 1);
	SetImageList(&m_ILRowHeight, LVSIL_SMALL);
}
int  CSListCtrl::GetRowHeight()
{
	return m_nRowHeight;
}

void CSListCtrl::InvalidateSubItem(int nItem, int nSubItem)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	
	CRect rect;
	if (nSubItem == -1)
	{
		GetItemRect(nItem, &rect, LVIR_BOUNDS);
	}
	else
	{
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	}
	
	rect.InflateRect(1, 1);
	
	InvalidateRect(&rect);
}

void CSListCtrl::InvalidateSubItems(int nItem, int pSubItems[], int nSubItemCount)
{
	ASSERT_ROW_COUNT(nItem);

	CRgn rgnTotal;
	rgnTotal.CreateRectRgn(0, 0, 0, 0);

	CRect rect;
	for(int i = 0; i < nSubItemCount; i++)
	{
		ASSERT_COL_COUNT(pSubItems[i]);
		GetSubItemRect(nItem, pSubItems[i], LVIR_BOUNDS, rect);

		CRgn rgnTemp;
		rgnTemp.CreateRectRgnIndirect(&rect);

		rgnTotal.CombineRgn(&rgnTotal, &rgnTemp, RGN_OR);
	}

	InvalidateRgn(&rgnTotal);
}
///////////////////////////////////////////////////////////////////////////////
// GetSubItemRect
BOOL CSListCtrl::GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect)
{	
	BOOL bRC = CListCtrl::GetSubItemRect(nItem, nSubItem, nArea, rect);
	
	// if nSubItem == 0, the rect returned by CListCtrl::GetSubItemRect
	// is the entire row, so use left edge of second subitem
	
	if (nSubItem == 0)
	{
		if (GetHeaderCtrl()->GetItemCount() > 1)
		{
			CRect rect1;
			bRC = GetSubItemRect(nItem, 1, LVIR_BOUNDS, rect1);
			rect.right = rect1.left;
		}
	}
	
	//+++
	if (nSubItem == 0)
	{
		if (GetHeaderCtrl()->GetItemCount() > 1)
		{
			CRect rect1;
			// in case 2nd col width = 0
			for (int i = 1; i < GetHeaderCtrl()->GetItemCount(); i++)
			{
				bRC = GetSubItemRect(nItem, i, LVIR_BOUNDS, rect1);
				if (rect1.Width() > 0)
				{
					rect.right = rect1.left;
					break;
				}
			}
		}
	}
	
	return bRC;
}

int	 CSListCtrl::GetHeaderCheckedState(int nSubItem)
{
	ASSERT_COL_COUNT(nSubItem);
	
	return m_HeaderCtrl.GetCheckBoxState(nSubItem);
}
void CSListCtrl::SetHeaderCheckedState(int nSubItem, int nCheckedState)
{
	ASSERT_COL_COUNT(nSubItem);
	ASSERT(nCheckedState >= SHC_NONE_CHECK_BOX && nCheckedState <= SHC_CHECKED);
	
	m_HeaderCtrl.SetCheckBoxState(nSubItem, nCheckedState);
}

void CSListCtrl::SetItemCheckedStateByClick(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateHeader)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	ASSERT(nCheckedState >= SHC_NONE_CHECK_BOX && nCheckedState <= SHC_CHECKED);

	//1. Update data: checked state
	SetItemCheckedState(nItem, nSubItem, nCheckedState);
	InvalidateSubItem(nItem, nSubItem);
	
	//3. Update header
	if(bUpdateHeader)
	{
		// now update checkbox in header
		if (GetHeaderCheckedState(nSubItem) != SHC_NONE_CHECK_BOX)
		{
			int nCheckedCount = CountCheckedItems(nSubItem);
			
			if (nCheckedCount == GetItemCount())
				SetHeaderCheckedState(nSubItem, SHC_CHECKED);
			else
				SetHeaderCheckedState(nSubItem, SHC_UNCHECKED);
			m_HeaderCtrl.UpdateWindow();
		}
	}
	
	//4. Send message to parent
	CWnd *pWnd = GetParent();
	if (pWnd == NULL)
	{
		pWnd = GetOwner();
	}
	if(pWnd != NULL && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		::SendMessage(pWnd->GetSafeHwnd(), WM_SLISTCTRL_CHECKBOX_CLICKED, nItem, nSubItem);
	}
}
void CSListCtrl::SetItemCheckedState(int nItem, int nSubItem, int nCheckedState)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	ASSERT(nCheckedState >= SHC_NONE_CHECK_BOX && nCheckedState <= SHC_CHECKED);

	//1. Update data: checked state
	CListItemData *pData = (CListItemData *) CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);
	pData->m_pSubItemDatas[nSubItem].m_nCheckState = nCheckedState;
	
	//2. Update window
	//InvalidateSubItem(nItem, nSubItem);
}
int  CSListCtrl::GetItemCheckedState(int nItem, int nSubItem)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);

	CListItemData *pData = (CListItemData *) CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);

	return pData->m_pSubItemDatas[nSubItem].m_nCheckState;
}

void CSListCtrl::ValidateCheck()
{
	int nColCount = GetHeaderCtrl()->GetItemCount();
	for(int nSubItem = 0; nSubItem < nColCount; nSubItem++)
	{
		if(GetHeaderCheckedState(nSubItem) == SHC_NONE_CHECK_BOX)
		{
			continue;
		}

		//Column state		
		if (CountCheckedItems(nSubItem) == GetItemCount())
			SetHeaderCheckedState(nSubItem, SHC_CHECKED);
		else
			SetHeaderCheckedState(nSubItem, SHC_UNCHECKED);
	}
	m_HeaderCtrl.UpdateWindow();
	UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////
// CountCheckedItems
int CSListCtrl::CountCheckedItems(int nSubItem)
{
	ASSERT_COL_COUNT(nSubItem);
	
	int nCount = 0;
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		if(GetItemCheckedState(nItem, nSubItem) == SHC_CHECKED)
		{
			nCount++;
		}			
	}
	
	return nCount;
}

int CSListCtrl::SwitchCheckedState(int nCheckedState)
{
	ASSERT(nCheckedState == SHC_CHECKED || nCheckedState == SHC_UNCHECKED);

	if(nCheckedState == SHC_CHECKED)
	{
		nCheckedState = SHC_UNCHECKED;
	}
	else
	{
		nCheckedState = SHC_CHECKED;
	}
	return nCheckedState;
}

BOOL CSListCtrl::IsPtInSubItemCheckBox(int nItem, int nSubItem, POINT pt)
{
	BOOL bResult = FALSE;
	
	CRect checkboxRect;
	if(CalcCheckBoxRect(nItem, nSubItem, checkboxRect))
	{
		bResult = checkboxRect.PtInRect(pt);
	}

	AfxTrace("IsPtInSubItemCheckBox: %d\n", bResult);
	return bResult;
}

CSListCtrl::CListSubItemData* CSListCtrl::GetSubItemData(int nItem, int nSubItem)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	CListItemData* pData = (CListItemData*)CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);
	
	return &(pData->m_pSubItemDatas[nSubItem]);
}

void CSListCtrl::DrawItem(int nItem, int nSubItem, CDC* pDC)
{
	//Color
	COLORREF crText  = m_crWindowText;
	COLORREF crBkgnd = m_crWindow;
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);
	
	//Rectangle
	CRect rect;
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

	//Paint background
	CRect rcTemp = rect;
	rcTemp.bottom -= 1;
	pDC->FillSolidRect(&rcTemp, crBkgnd);

	//Sub Item data
	CListSubItemData* pSubItemData = GetSubItemData(nItem, nSubItem);
	ASSERT(pSubItemData);

	//1. Draw Check Box
	if(pSubItemData->m_nCheckState != SHC_NONE_CHECK_BOX)
	{
		DrawCheckBox(nItem, nSubItem, pDC);
	}

	//2. Draw Image
	if(pSubItemData->m_pListImage != NULL)
	{
		DrawImage(nItem, nSubItem, pDC);
	}

	//3. Draw Text or Progress
	if(pSubItemData->m_pListPrgsBar != NULL)
	{
		DrawProgressBar(nItem, nSubItem, pDC);
	}
	else
	{
		DrawText(nItem, nSubItem, pDC);
	}	
}

void CSListCtrl::DrawCheckBox(int nItem, int nSubItem, CDC *pDC)
{
	BOOL bDrawMark = (GetItemCheckedState(nItem, nSubItem) == SHC_CHECKED);
	
	CRect chkboxrect;
	if(CalcCheckBoxRect(nItem, nSubItem, chkboxrect))
	{
		gtb::DrawCheckBox(pDC, &chkboxrect, bDrawMark, m_crWindow);
	}
}
void CSListCtrl::DrawImage(int nItem, int nSubItem, CDC *pDC)
{
	CRect imgRect;
	
	if(!CalcImageRect(nItem, nSubItem, imgRect))
	{
		return;
	}
	
	CListImage* pListImage = GetSubItemData(nItem, nSubItem)->m_pListImage;
	ASSERT(pListImage != NULL && pListImage->m_imageList != NULL);
	pListImage->m_imageList->DrawIndirect(pDC, pListImage->m_nImage, imgRect.TopLeft(), imgRect.Size(), CPoint(0, 0));
	/*
	//Picked up from WINCTRL7.CPP	
	IMAGELISTDRAWPARAMS drawing;
	
	drawing.i = pListImage->m_nImage;
	drawing.hdcDst = pDC->m_hDC;
	drawing.x = imgRect.left;
	drawing.y = imgRect.top;
	drawing.cx = imgRect.Width();
	drawing.cy = imgRect.Height();
	drawing.xBitmap = 0;
	drawing.yBitmap = 0;
	drawing.rgbBk = CLR_NONE;
	drawing.rgbFg = CLR_DEFAULT;
	drawing.fStyle = ILD_NORMAL;
	drawing.dwRop = SRCCOPY;

	ASSERT_POINTER(&drawing, IMAGELISTDRAWPARAMS);	
	drawing.cbSize = sizeof(IMAGELISTDRAWPARAMS);
	drawing.himl = pListImage->m_hImageList;
	BOOL bResult = ImageList_DrawIndirect(&drawing);
	AfxTrace("%d\n", bResult);
	*/
}
void CSListCtrl::DrawText(int nItem, int nSubItem, CDC *pDC, BOOL bProgressText)
{
	CString str = GetItemText(nItem, nSubItem);
	if(str.IsEmpty())
	{
		return;
	}

	CRect rcText;
	if(!CalcTextRect(nItem, nSubItem, rcText))
	{
		return;
	}

	COLORREF crText = m_crWindowText, crBkgnd = m_crWindow;
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);
	
	// get text justification
	UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

	if(bProgressText)
	{
		nFormat |= DT_CENTER;
		crText = m_crWindowText;
	}
	else
	{
		HDITEM hditem;
		hditem.mask = HDI_FORMAT;
		m_HeaderCtrl.GetItem(nSubItem, &hditem);
		int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
		
		if (nFmt & HDF_CENTER)
			nFormat |= DT_CENTER;
		else if (nFmt & HDF_RIGHT)
			nFormat |= DT_RIGHT;
		else
			nFormat |= DT_LEFT;
	}
	
	int nOldDC = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);
	pDC->SetBkColor(crBkgnd);

	pDC->DrawText(str, &rcText, nFormat);

	pDC->RestoreDC(nOldDC);
}
void CSListCtrl::DrawProgressBar(int nItem, int nSubItem, CDC *pDC)
{
	CRect rcPrgs;
	if(!CalcProgressRect(nItem, nSubItem, rcPrgs))
	{
		return;
	}
	rcPrgs.DeflateRect(0, 3);

	//1. Draw Border
	pDC->Draw3dRect(&rcPrgs, RGB(0, 0, 0), RGB(0, 0, 0));
	rcPrgs.DeflateRect(1, 1);

	//2. Draw whole progress bar
	pDC->FillSolidRect(rcPrgs, RGB(255, 240, 240));
//	gtb::DrawGradient2(pDC, rcPrgs, RGB(226, 226, 226), RGB(164, 164, 164), RGB(213, 213, 213), FALSE);

	//3. Draw the actual progress
	CListProgress* pListPrgsBar = GetSubItemData(nItem, nSubItem)->m_pListPrgsBar;
	ASSERT(pListPrgsBar != NULL);

	int w = 0;
	if(pListPrgsBar->m_nMaxValue > 0 && pListPrgsBar->m_nValue > 0)
	{
		// <= 100%
		if(pListPrgsBar->m_nMaxValue >= pListPrgsBar->m_nValue)
		{
			w = ::MulDiv(rcPrgs.Width(), pListPrgsBar->m_nValue, pListPrgsBar->m_nMaxValue);
		}
		// > 100%, abnormal case
		else
		{
			w = rcPrgs.Width();
		}
	}
	rcPrgs.right = rcPrgs.left + w;

//	pDC->FillSolidRect(rcPrgs, RGB(46, 211, 49));
	gtb::DrawGradient2(pDC, rcPrgs, RGB(205, 255, 205), RGB(0, 200, 38), RGB(90, 233, 107), FALSE);

	//4. Draw Progress Text
	DrawText(nItem, nSubItem, pDC, TRUE);
}
BOOL CSListCtrl::CalcCheckBoxRect(int nItem, int nSubItem, CRect& chkboxrect)
{
	int nCheckedState = GetItemCheckedState(nItem, nSubItem);
	if(nCheckedState != SHC_CHECKED && nCheckedState != SHC_UNCHECKED)
	{
		return FALSE;
	}
	CRect rect;
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

	CRect boundRect = rect;
	boundRect.DeflateRect(m_HeaderCtrl.m_nSpace, 0); // line up checkbox with header checkbox

	BOOL bCenter = GetItemText(nItem, nSubItem).IsEmpty();
	BOOL bResult = gtb::CalcCheckBoxRect(boundRect, chkboxrect, bCenter);
	if(bResult)
	{
		CRect rcTemp;
		if(m_HeaderCtrl.GetCheckBoxRect(nSubItem, rcTemp))
		{
			ASSERT(rcTemp.Width() == chkboxrect.Width());
			m_HeaderCtrl.ClientToScreen(&rcTemp);
			ScreenToClient(&rcTemp);

			chkboxrect.left = rcTemp.left;
			chkboxrect.right = rcTemp.right;
		}
	}
	return bResult;
}

BOOL CSListCtrl::CalcImageRect(int nItem, int nSubItem, CRect& rcImage)
{
	CListSubItemData* pSubItemData = GetSubItemData(nItem, nSubItem);
	ASSERT(pSubItemData);

	if(pSubItemData->m_pListImage == NULL || pSubItemData->m_pListImage->m_imageList == NULL
		|| pSubItemData->m_pListImage->m_nImage < 0)
	{
		return FALSE;
	}

	CRect rcItem, rcCheckBox;
	
	//1. Get SubItem Rect
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rcItem);

	//2. Check if there's a check box in the left side
	if(CalcCheckBoxRect(nItem, nSubItem, rcCheckBox))
	{
		rcItem.left = rcCheckBox.right;
	}
	rcItem.left += m_HeaderCtrl.m_nSpace;

	//no more space
	if(rcItem.Width() <= 0)
	{
		return FALSE;
	}

	SIZE sizeImage;
	sizeImage.cx = sizeImage.cy = 0;

	IMAGEINFO info;
//	if(ImageList_GetImageInfo(pSubItemData->m_pListImage->m_hImageList, pSubItemData->m_pListImage->m_nImage, &info))
	if(pSubItemData->m_pListImage->m_imageList->GetImageInfo(pSubItemData->m_pListImage->m_nImage, &info))
	{
		//if imageOnly we only return the rect of the image.
		//otherwise we also include area under and below image.
		sizeImage.cx = info.rcImage.right - info.rcImage.left;
		sizeImage.cy = info.rcImage.bottom - info.rcImage.top;

		//Real size
		SIZE size;
		size.cx = rcItem.Width() < sizeImage.cx ? rcItem.Width() : sizeImage.cx;
		size.cy = 1+rcItem.Height() < sizeImage.cy ? rcItem.Height() : sizeImage.cy;

		POINT point;
		int nDiffHalf = (rcItem.Height() - sizeImage.cy ) / 2;
		point.y = (nDiffHalf > 0) ? rcItem.top + nDiffHalf : rcItem.top;
		point.x = rcItem.left;

		rcImage = CRect(point, size);

		return TRUE;
	}

	return FALSE;
}

BOOL CSListCtrl::CalcTextRect(int nItem, int nSubItem, CRect& rcText)
{
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rcText);
	
	CRect rcTemp;
	//Move if image
	if( CalcImageRect(nItem, nSubItem, rcTemp) )
	{
		rcText.left = rcTemp.right;
	}
	else if( CalcCheckBoxRect(nItem, nSubItem, rcTemp) )
	{
		rcText.left = rcTemp.right;
	}

	rcText.DeflateRect(m_HeaderCtrl.m_nSpace, 0);
	if(rcText.IsRectEmpty())
	{
		return FALSE;
	}
	
	return TRUE;
}
BOOL CSListCtrl::CalcProgressRect(int nItem, int nSubItem, CRect& rcProgress)
{
	CListSubItemData* pSubItemData = GetSubItemData(nItem, nSubItem);
	ASSERT(pSubItemData);
	
	if(pSubItemData->m_pListPrgsBar == NULL)
	{
		return FALSE;
	}

	return CalcTextRect(nItem, nSubItem, rcProgress);
}

void CSListCtrl::GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd)
{
	DWORD dwStyle    = GetStyle();
	DWORD dwExStyle  = GetExtendedStyle();
	
	COLORREF crText  = colorText;
	COLORREF crBkgnd = colorBkgnd;
	
	if (GetItemState(nItem, LVIS_SELECTED))
	{
		//full row select or the first column
		if( (dwExStyle & LVS_EX_FULLROWSELECT) || nSubItem == 0 )
		{
			// selected?  if so, draw highlight background
			crText  = m_crHighLightText;
			crBkgnd = m_crHighLight;
			
			// has focus?  if not, draw gray background
			if (m_hWnd != ::GetFocus())
			{
				if (dwStyle & LVS_SHOWSELALWAYS)
				{
					crText  = m_crWindowText;
					crBkgnd = m_crBtnFace;
				}
				else
				{
					crText  = colorText;
					crBkgnd = colorBkgnd;
				}
			}
		}
	}
	
	colorText = crText;
	colorBkgnd = crBkgnd;
}
