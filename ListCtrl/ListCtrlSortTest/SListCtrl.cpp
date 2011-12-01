// SListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SListCtrl.h"
#include "Tools.h"

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

UINT WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED = ::RegisterWindowMessage(_T("WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED"));

CSListCtrl::CSListCtrl()
{
	m_crBtnFace             = ::GetSysColor(COLOR_BTNFACE);
	m_crHighLight           = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crHighLightText       = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crWindow              = ::GetSysColor(COLOR_WINDOW);
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
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
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
	
	CListCtrl::PreSubclassWindow();
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
		DrawItem(static_cast<int> (pLVCD->nmcd.dwItemSpec), pLVCD->iSubItem, pLVCD->nmcd.lItemlParam, pDC);
		
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
				SetItemCheckedState(nItem, nSubItem, nCheckedState, FALSE);
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
	CCheckListItemData* pData = (CCheckListItemData*)lParam;
	return pData->dwItemData;
}
void CSListCtrl::DrawItem(int nItem, int nSubItem, LPARAM lItemlParam, CDC* pDC)
{
	CCheckListItemData *pListCtrlData = (CCheckListItemData *)lItemlParam;
	ASSERT(pListCtrlData);
	
	COLORREF crText  = m_crWindowText;
	COLORREF crBkgnd = m_crWindow;
	//@TODO store the colors back in the NMLVCUSTOMDRAW struct
// 	pLVCD->clrText = crText;
// 	pLVCD->clrTextBk = crBkgnd;
	
	CRect rect;
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

	CListSubItemData* pSubItemData = GetSubItemData(nItem, nSubItem);
	ASSERT(pSubItemData);

	BOOL bOld = FALSE;
	if(bOld)
	{
		int nItemCheckedState = GetItemCheckedState(nItem, nSubItem);
		if(nItemCheckedState != SHC_NONE_CHECK_BOX)
		{
			BOOL bDrawMark = (nItemCheckedState == SHC_CHECKED);
			DrawCheckbox(nItem, nSubItem, pDC, crText, crBkgnd, rect, bDrawMark);
		}
		else
		{
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect);
		}

		return;
	}
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);
	pDC->FillSolidRect(&rect, crBkgnd);
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
		CTools::DrawCheckBox(pDC, &chkboxrect, bDrawMark, m_crWindow);
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
	ASSERT(pListImage != NULL);
	
	pListImage->m_imageList->DrawIndirect(pDC, pListImage->m_nImage, imgRect.TopLeft(), imgRect.Size(), CPoint(0, 0));
}
void CSListCtrl::DrawText(int nItem, int nSubItem, CDC *pDC)
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
// 	pDC->FillSolidRect(&rcText, crBkgnd);
	
	// get text justification
	HDITEM hditem;
	hditem.mask = HDI_FORMAT;
	m_HeaderCtrl.GetItem(nSubItem, &hditem);
	int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
	UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	if (nFmt & HDF_CENTER)
		nFormat |= DT_CENTER;
	else if (nFmt & HDF_RIGHT)
		nFormat |= DT_RIGHT;
	else
		nFormat |= DT_LEFT;
	
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

	//1. Draw Border
	pDC->Draw3dRect(&rcPrgs, RGB(0, 0, 0), RGB(0, 0, 0));
	rcPrgs.DeflateRect(1, 1);

	//2. Draw whole progress bar
	pDC->FillSolidRect(rcPrgs, RGB(255, 240, 240));


	//3. Draw the actual progress
	CListProgressBar* pListPrgsBar = GetSubItemData(nItem, nSubItem)->m_pListPrgsBar;
	ASSERT(pListPrgsBar != NULL && pListPrgsBar->m_maxvalue > 0);
	ASSERT(pListPrgsBar->m_value <= pListPrgsBar->m_maxvalue);

	int w = ::MulDiv(rcPrgs.Width(), pListPrgsBar->m_value, pListPrgsBar->m_maxvalue);
	rcPrgs.right = rcPrgs.left + w;

	pDC->FillSolidRect(rcPrgs, RGB(46, 211, 49));

	//4. Draw Progress Text
	DrawText(nItem, nSubItem, pDC);
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
	BOOL bResult = CTools::CalcCheckBoxRect(boundRect, chkboxrect, bCenter);
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
		rcItem.left = rcCheckBox.right + m_HeaderCtrl.m_nSpace;
	}
	//no more space
	if(rcItem.Width() <= 0)
	{
		return FALSE;
	}

	SIZE sizeImage;
	sizeImage.cx = sizeImage.cy = 0;

	IMAGEINFO info;
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
	
	if(pSubItemData->m_pListPrgsBar == NULL || pSubItemData->m_pListPrgsBar->m_maxvalue <= 0)
	{
		return FALSE;
	}

	return CalcTextRect(nItem, nSubItem, rcProgress);
}
void CSListCtrl::DrawCheckbox(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect, BOOL bDrawMark)
{
	ASSERT(pDC);
	
	if (rect.IsRectEmpty())
	{
		return;
	}
	
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);
	pDC->FillSolidRect(&rect, crBkgnd);

	CRect chkboxrect;
	if(CalcCheckBoxRect(nItem, nSubItem, chkboxrect))
	{
		CTools::DrawCheckBox(pDC, &chkboxrect, bDrawMark, m_crWindow);
	}
	
	CString str = GetItemText(nItem, nSubItem);
	if (!str.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		CRect textrect;
		textrect = rect;
		textrect.left = chkboxrect.right + m_HeaderCtrl.m_nSpace;
		
		UINT nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;	//+++
		BOOL m_bUseEllipsis = TRUE;
		if (m_bUseEllipsis)
			nFormat |= DT_END_ELLIPSIS;
		
		pDC->DrawText(str, &textrect, nFormat);
	}
	
}


///////////////////////////////////////////////////////////////////////////////
// DrawText
void CSListCtrl::DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect& rect)
{
	if (rect.IsRectEmpty())
	{
		return;
	}
	
	COLORREF oldBkColor = pDC->GetBkColor();
	
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);
	pDC->FillSolidRect(&rect, crBkgnd);

	CString str;
	str = GetItemText(nItem, nSubItem);
	if(str.IsEmpty())
	{
		pDC->SetBkColor(oldBkColor);
		return;
	}

	// get text justification
	HDITEM hditem;
	hditem.mask = HDI_FORMAT;
	m_HeaderCtrl.GetItem(nSubItem, &hditem);
	int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
	UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	if (nFmt == HDF_CENTER)
		nFormat |= DT_CENTER;
	else if (nFmt == HDF_LEFT)
		nFormat |= DT_LEFT;
	else
		nFormat |= DT_RIGHT;
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);
	pDC->SetBkColor(crBkgnd);

	int m_nPadding = m_HeaderCtrl.m_nSpace;
	rect.DeflateRect(m_nPadding, 0);
	pDC->DrawText(str, &rect, nFormat);
	rect.InflateRect(m_nPadding, 0);

	pDC->SetBkColor(oldBkColor);
}

void CSListCtrl::GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd)
{
	DWORD dwStyle    = GetStyle();
	DWORD dwExStyle  = GetExtendedStyle();
	
	COLORREF crText  = colorText;
	COLORREF crBkgnd = colorBkgnd;
	
	if (GetItemState(nItem, LVIS_SELECTED))
	{
		if (dwExStyle & LVS_EX_FULLROWSELECT)
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
		else	// not full row select
		{
			if (nSubItem == 0)
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
	}
	
	colorText = crText;
	colorBkgnd = crBkgnd;
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

	CCheckListItemData *pListCtrlData = new CCheckListItemData;
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
	
	CCheckListItemData *pListCtrlData = new CCheckListItemData;
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
	
	CCheckListItemData *pXLCD = (CCheckListItemData *) CListCtrl::GetItemData(nItem);
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
		CCheckListItemData *pXLCD = (CCheckListItemData *) CListCtrl::GetItemData(i);
		if (pXLCD)
			delete pXLCD;
		CListCtrl::SetItemData(i, 0);
	}
	
	return CListCtrl::DeleteAllItems();
}

DWORD CSListCtrl::GetItemData(int nItem) const
{
	ASSERT_ROW_COUNT(nItem);
	
	CCheckListItemData *pXLCD = (CCheckListItemData *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return 0;
	}
	
	return pXLCD->dwItemData;
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
	
	rect.InflateRect(2, 2);
	
	InvalidateRect(&rect);
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
		::SendMessage(pWnd->GetSafeHwnd(), WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED, nItem, nSubItem);
	}
}

void CSListCtrl::SetItemCheckedState(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateImmediately)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	ASSERT(nCheckedState >= SHC_NONE_CHECK_BOX && nCheckedState <= SHC_CHECKED);

	//1. Update data: checked state
	CCheckListItemData *pData = (CCheckListItemData *) CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);
	pData->m_pSubItemDatas[nSubItem].m_nCheckState = nCheckedState;
	
	//2. Update window
	InvalidateSubItem(nItem, nSubItem);
	if(bUpdateImmediately)
	{
		UpdateWindow();
	}	
}
int  CSListCtrl::GetItemCheckedState(int nItem, int nSubItem)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);

	CCheckListItemData *pData = (CCheckListItemData *) CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);

	return pData->m_pSubItemDatas[nSubItem].m_nCheckState;
}

CListSubItemData* CSListCtrl::GetSubItemData(int nItem, int nSubItem)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	CCheckListItemData* pData = (CCheckListItemData*)CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->m_pSubItemDatas);

	return &(pData->m_pSubItemDatas[nSubItem]);
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

