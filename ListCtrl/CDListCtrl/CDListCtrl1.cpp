// CDListCtrl1.cpp : implementation file
//

#include "stdafx.h"
#include "CDListCtrl.h"
#include "CDListCtrl1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

/////////////////////////////////////////////////////////////////////////////
// CCDListCtrl

CCDListCtrl::CCDListCtrl()
{
	m_crBtnFace             = ::GetSysColor(COLOR_BTNFACE);
	m_crHighLight           = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crHighLightText       = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crWindow              = ::GetSysColor(COLOR_WINDOW);
	m_crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);
}

CCDListCtrl::~CCDListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCDListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCDListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT_EX(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CCDListCtrl::InsertItem(const LVITEM* pItem)
{
	ASSERT(pItem->iItem >= 0);
	
	int index = CListCtrl::InsertItem(pItem);
	
	if (index < 0)
		return index;
	
	CCDListCtrlData *pListCtrlData = new CCDListCtrlData;
	pListCtrlData->pCheckStates = new int[GetHeaderCtrl()->GetItemCount()];
	for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
	{
		pListCtrlData->pCheckStates[i] = -1;
	}
	pListCtrlData->dwItemData = pItem->lParam;
	
	CListCtrl::SetItemData(index, (DWORD) pListCtrlData);

	return index;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteItem
BOOL CCDListCtrl::DeleteItem(int nItem)
{
	ASSERT_ROW_COUNT(nItem);
	
	CCDListCtrlData *pXLCD = (CCDListCtrlData *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
		delete pXLCD;
	CListCtrl::SetItemData(nItem, 0);
	return CListCtrl::DeleteItem(nItem);
}

///////////////////////////////////////////////////////////////////////////////
// DeleteAllItems
BOOL CCDListCtrl::DeleteAllItems()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		CCDListCtrlData *pXLCD = (CCDListCtrlData *) CListCtrl::GetItemData(i);
		if (pXLCD)
			delete pXLCD;
		CListCtrl::SetItemData(i, 0);
	}
	
	return CListCtrl::DeleteAllItems();
}

DWORD CCDListCtrl::GetItemData(int nItem) const
{
	ASSERT_ROW_COUNT(nItem);
		
	CCDListCtrlData *pXLCD = (CCDListCtrlData *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return 0;
	}
	
	return pXLCD->dwItemData;
}

/////////////////////////////////////////////////////////////////////////////
// CCDListCtrl message handlers

///////////////////////////////////////////////////////////////////////////////
// OnDestroy
void CCDListCtrl::OnDestroy()
{
	DeleteAllItems();
	
	CListCtrl::OnDestroy();
}
BOOL CCDListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE* pNMActivate = (NMITEMACTIVATE*)pNMHDR;
	*pResult = 0;

	int nItem = -1;

	//+++
	LVHITTESTINFO lvhit;
	lvhit.pt = pNMActivate->ptAction;
	SubItemHitTest(&lvhit);
	if (lvhit.flags & LVHT_ONITEMLABEL)
	{
		nItem = lvhit.iItem;
	}

	if(nItem != -1)
	{
		CCDListCtrlData *pXLCD = (CCDListCtrlData *) CListCtrl::GetItemData(nItem);
		if (!pXLCD)
		{
			return FALSE;
		}
		
		CRect rect;
		
		int nSubItem = -1;
		
		// check if a subitem checkbox was clicked
		
		for (int i = 0; i < this->GetHeaderCtrl()->GetItemCount(); i++)
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
			if (pXLCD->pCheckStates[nSubItem] >= 0)
			{
				int nChecked = pXLCD->pCheckStates[nSubItem];
				
				nChecked = (nChecked == 0) ? 1 : 0;
				
				pXLCD->pCheckStates[nSubItem] = nChecked;
				
				UpdateSubItem(nItem, nSubItem);
				
				CWnd *pWnd = GetParent();
				if (!pWnd)
					pWnd = GetOwner();
				if (pWnd && ::IsWindow(pWnd->m_hWnd))
					pWnd->SendMessage(0, nItem, nSubItem);
				
				// now update checkbox in header
				
				// -1 = no checkbox in column header
				if (GetHeaderCheckedState(nSubItem) != XHEADERCTRL_NO_IMAGE)
				{
					int nCheckedCount = CountCheckedItems(nSubItem);
					
					if (nCheckedCount == GetItemCount())
						SetHeaderCheckedState(nSubItem, XHEADERCTRL_CHECKED_IMAGE);
					else
						SetHeaderCheckedState(nSubItem, XHEADERCTRL_UNCHECKED_IMAGE);
				}
			}
		}
	}

	return FALSE;		// return FALSE to send message to parent also -
						// NOTE:  MSDN documentation is incorrect
}

///////////////////////////////////////////////////////////////////////////////
// CountCheckedItems
int CCDListCtrl::CountCheckedItems(int nSubItem)
{
	ASSERT_COL_COUNT(nSubItem);

	int nCount = 0;
	
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		CCDListCtrlData *pXLCD = (CCDListCtrlData *) CListCtrl::GetItemData(nItem);
		if (!pXLCD)
		{
			continue;
		}
		
		if (pXLCD->pCheckStates[nSubItem] == 1)
			nCount++;
	}
	
	return nCount;
}
///////////////////////////////////////////////////////////////////////////////
// GetHeaderCheckedState
//
// The GetHeaderCheckedState and SetHeaderCheckedState may be used to toggle
// the checkbox in a column header.
//     0 = no checkbox
//     1 = unchecked
//     2 = checked
//
int CCDListCtrl::GetHeaderCheckedState(int nSubItem)
{
	ASSERT_COL_COUNT(nSubItem);
	
	HDITEM hditem;
	
	// use the image index (0 or 1) to indicate the checked status
	hditem.mask = HDI_IMAGE;
	m_HeaderCtrl.GetItem(nSubItem, &hditem);
	return hditem.iImage;
}

///////////////////////////////////////////////////////////////////////////////
// SetHeaderCheckedState
BOOL CCDListCtrl::SetHeaderCheckedState(int nSubItem, int nCheckedState)
{
	ASSERT_COL_COUNT(nSubItem);

	ASSERT(nCheckedState == 0 || nCheckedState == 1 || nCheckedState == 2);
	
	HDITEM hditem;
	
	hditem.mask = HDI_IMAGE;
	hditem.iImage = nCheckedState;
	m_HeaderCtrl.SetItem(nSubItem, &hditem);
	
	return TRUE;
}
BOOL CCDListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
	
	int nSubItem = pnmlv->iSubItem;
	
	int nCheckedState = GetHeaderCheckedState(nSubItem);
	
	// 0 = no checkbox
	if (nCheckedState != XHEADERCTRL_NO_IMAGE)
	{
		nCheckedState = (nCheckedState == 1) ? 2 : 1;
		SetHeaderCheckedState(nSubItem, nCheckedState);
		
		m_HeaderCtrl.UpdateWindow();
		
		for (int nItem = 0; nItem < GetItemCount(); nItem++)
		{
			CCDListCtrlData *pXLCD = (CCDListCtrlData *) CListCtrl::GetItemData(nItem);
			if (!pXLCD)
			{
				continue;
			}
			
			if (pXLCD->pCheckStates[nSubItem] != -1)
			{
				pXLCD->pCheckStates[nSubItem] = nCheckedState - 1;
				UpdateSubItem(nItem, nSubItem);
			}
		}
	}
	
	*pResult = 0;
	return FALSE;		// return FALSE to send message to parent also -
						// NOTE:  MSDN documentation is incorrect
}
void CCDListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
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
		
		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		int nSubItem = pLVCD->iSubItem;
		
		CCDListCtrlData *pListCtrlData = (CCDListCtrlData *) pLVCD->nmcd.lItemlParam;
		ASSERT(pListCtrlData);
		
		COLORREF crText  = m_crWindowText;
		COLORREF crBkgnd = m_crWindow;
		
		
		// store the colors back in the NMLVCUSTOMDRAW struct
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;
		
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

		if(pListCtrlData->pCheckStates[nSubItem] != -1)
		{
			DrawCheckbox(nItem, nSubItem, pDC, crText, crBkgnd, rect, pListCtrlData);
			
			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}
	}
}

void CCDListCtrl::DrawCheckbox(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect, CCDListCtrlData *pXLCD)
{
	ASSERT(pDC);
	ASSERT(pXLCD);
	
	if (rect.IsRectEmpty())
	{
		return;
	}
	
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);
	
	pDC->FillSolidRect(&rect, crBkgnd);
	
	CRect chkboxrect;
	chkboxrect = rect;
	chkboxrect.bottom -= 1;
	chkboxrect.left += 9;		// line up checkbox with header checkbox
	chkboxrect.right = chkboxrect.left + chkboxrect.Height();	// width = height
	
	CString str;
	str = GetItemText(nItem, nSubItem);
	
	if (str.IsEmpty())
	{
		// center the checkbox
		
		chkboxrect.left = rect.left + rect.Width()/2 - chkboxrect.Height()/2 - 1;
		chkboxrect.right = chkboxrect.left + chkboxrect.Height();
	}
	
	// fill rect around checkbox with white
	pDC->FillSolidRect(&chkboxrect, m_crWindow);
	
	// draw border
	CBrush brush(RGB(51,102,153));
	pDC->FrameRect(&chkboxrect, &brush);
	
	if (pXLCD->pCheckStates[nSubItem] == 1)
	{
		CPen *pOldPen = NULL;
		
		CPen blackpen(PS_SOLID, 1, RGB(51,153,51));
		
		pOldPen = pDC->SelectObject(&blackpen);
		
		// draw the checkmark
		int x = chkboxrect.left + 9;
		ASSERT(x < chkboxrect.right);
		int y = chkboxrect.top + 3;
		int i;
		for (i = 0; i < 4; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y--;
		}
		
		if (pOldPen)
			pDC->SelectObject(pOldPen);
	}
	
	if (!str.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		CRect textrect;
		textrect = rect;
		textrect.left = chkboxrect.right + 4;
		
		UINT nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;	//+++
		BOOL m_bUseEllipsis = TRUE;
		if (m_bUseEllipsis)
			nFormat |= DT_END_ELLIPSIS;
		
		pDC->DrawText(str, &textrect, nFormat);
	}
}

void CCDListCtrl::SetItemCheckedState(int nItem, int nSubItem, int nCheckedState)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	
	CCDListCtrlData *pCDListCtrlData = (CCDListCtrlData *) CListCtrl::GetItemData(nItem);
	if (pCDListCtrlData)
	{
		// update checked state
		pCDListCtrlData->pCheckStates[nSubItem] = nCheckedState;
		
		UpdateSubItem(nItem, nSubItem);
	}
}

int CCDListCtrl::GetItemCheckedState(int nItem, int nSubItem)		//+++
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	
	CCDListCtrlData *pXLCD = (CCDListCtrlData *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return -1;
	}
	
	// return checked state
	return pXLCD->pCheckStates[nSubItem];
}

void CCDListCtrl::UpdateSubItem(int nItem, int nSubItem)
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
//	UpdateWindow();
}

BOOL CCDListCtrl::GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	
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

void CCDListCtrl::GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd)
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

void CCDListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));
	}

	CListCtrl::PreSubclassWindow();
}
