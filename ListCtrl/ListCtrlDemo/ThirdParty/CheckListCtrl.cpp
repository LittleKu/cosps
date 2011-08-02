// CheckListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CheckListCtrl.h"

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

UINT WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED = ::RegisterWindowMessage(_T("WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED"));

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl

CCheckListCtrl::CCheckListCtrl()
{
	m_crBtnFace             = ::GetSysColor(COLOR_BTNFACE);
	m_crHighLight           = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crHighLightText       = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crWindow              = ::GetSysColor(COLOR_WINDOW);
	m_crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);
}

CCheckListCtrl::~CCheckListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheckListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCheckListCtrl)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl message handlers
void CCheckListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));
	}
	
	CListCtrl::PreSubclassWindow();
}
void CCheckListCtrl::OnDestroy()
{
	DeleteAllItems();
	CListCtrl::OnDestroy();
}

void CCheckListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
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
		
		CCheckListItemData *pListCtrlData = (CCheckListItemData *) pLVCD->nmcd.lItemlParam;
		ASSERT(pListCtrlData);
		
		COLORREF crText  = m_crWindowText;
		COLORREF crBkgnd = m_crWindow;
		
		
		// store the colors back in the NMLVCUSTOMDRAW struct
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;
		
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		
		if(pListCtrlData->pCheckStates[nSubItem] != CL_NONE_CHECK_BOX)
		{
			BOOL bDrawMark = (pListCtrlData->pCheckStates[nSubItem] == CL_CHECKED);
			DrawCheckbox(nItem, nSubItem, pDC, crText, crBkgnd, rect, bDrawMark);
			
			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}
		else
		{
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect);
			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}
	}
}

BOOL CCheckListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
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
			int* pCheckStates = GetCheckedState(nItem);
			if (pCheckStates[nSubItem] != CL_NONE_CHECK_BOX && IsPtInSubItemCheckBox(nItem, nSubItem, pNMActivate->ptAction))
			{

				int nChecked = pCheckStates[nSubItem];
				nChecked = SwitchCheckedState(nChecked);
				
				SetItemCheckedStateByClick(nItem, nSubItem, nChecked, TRUE);
			}
		}
	}

	return FALSE;
}

BOOL CCheckListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	
	int nSubItem = pNMListView->iSubItem;
	AfxTrace("OnColumnClick %d\n", nSubItem);
	
	int nCheckedState = GetHeaderCheckedState(nSubItem);
	
	// CHECK_LIST_CTRL_NO_IMAGE = no checkbox
	if (nCheckedState != CL_NONE_CHECK_BOX)
	{
		nCheckedState = SwitchCheckedState(nCheckedState);

		SetHeaderCheckedState(nSubItem, nCheckedState);
		m_HeaderCtrl.UpdateWindow();
		
		for (int nItem = 0; nItem < GetItemCount(); nItem++)
		{
			int* pCheckStates = GetCheckedState(nItem);
			if (pCheckStates[nSubItem] != CL_NONE_CHECK_BOX)
			{
				pCheckStates[nSubItem] = nCheckedState;
				InvalidateSubItem(nItem, nSubItem);
			}
		}
		UpdateWindow();
	}
	return FALSE;
}

void CCheckListCtrl::CalcCheckBoxRect(int nItem, int nSubItem, CRect& chkboxrect, BOOL bCenter, int h)
{
	CRect rect;
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

	CRect boundRect = rect;
	boundRect.DeflateRect(m_HeaderCtrl.m_nSpace, 0); // line up checkbox with header checkbox

	if(boundRect.Height() >= 13)
	{
		CCheckHeaderCtrl::CalcCheckBoxRect(boundRect, chkboxrect, bCenter);
		return;
	}
	
	chkboxrect = boundRect;
	chkboxrect.bottom -= 1;
	chkboxrect.right = chkboxrect.left + chkboxrect.Height() + 1;	// width = height
	
	if(bCenter)
	{
		// center the checkbox		
		chkboxrect.left = boundRect.left + boundRect.Width()/2 - chkboxrect.Height()/2 - 1;
		chkboxrect.right = chkboxrect.left + chkboxrect.Height() + 1;
	}
}
void CCheckListCtrl::DrawCheckbox(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect, BOOL bDrawMark)
{
	ASSERT(pDC);
	
	if (rect.IsRectEmpty())
	{
		return;
	}
	
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);
	pDC->FillSolidRect(&rect, crBkgnd);
	
	CString str = GetItemText(nItem, nSubItem);
	BOOL bCenter = str.IsEmpty();

	CRect chkboxrect;
	CalcCheckBoxRect(nItem, nSubItem, chkboxrect, bCenter);
	/*
	CRect boundRect = rect;
	boundRect.DeflateRect(m_HeaderCtrl.m_nSpace, 0); // line up checkbox with header checkbox

	CRect chkboxrect;
	chkboxrect = boundRect;
	chkboxrect.bottom -= 1;
	chkboxrect.right = chkboxrect.left + chkboxrect.Height() + 1;	// width = height
	
	if(bCenter)
	{
		// center the checkbox		
		chkboxrect.left = boundRect.left + boundRect.Width()/2 - chkboxrect.Height()/2 - 1;
		chkboxrect.right = chkboxrect.left + chkboxrect.Height() + 1;
	}
	*/
	
	CCheckHeaderCtrl::DrawCheckBox(pDC, &chkboxrect, bDrawMark, m_crWindow);
	
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
void CCheckListCtrl::DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect& rect)
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

void CCheckListCtrl::GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd)
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

int  CCheckListCtrl::InsertItem( int nItem, LPCTSTR lpszItem )
{
	ASSERT(nItem >= 0);
	int index = CListCtrl::InsertItem(nItem, lpszItem);
	if (index < 0)
	{
		return index;
	}

	DWORD dwItemData = CListCtrl::GetItemData(index);

	CCheckListItemData *pListCtrlData = new CCheckListItemData;
	pListCtrlData->pCheckStates = new int[GetHeaderCtrl()->GetItemCount()];
	for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
	{
		pListCtrlData->pCheckStates[i] = CL_NONE_CHECK_BOX;
	}
	pListCtrlData->dwItemData = dwItemData;

	CListCtrl::SetItemData(index, (DWORD)pListCtrlData);
	return index;
}

int CCheckListCtrl::InsertItem(const LVITEM* pItem, BOOL bVirgin)
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
	pListCtrlData->pCheckStates = new int[GetHeaderCtrl()->GetItemCount()];
	for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
	{
		pListCtrlData->pCheckStates[i] = CL_NONE_CHECK_BOX;
	}
	pListCtrlData->dwItemData = pItem->lParam;
	
	CListCtrl::SetItemData(index, (DWORD)pListCtrlData);
	
	return index;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteItem
BOOL CCheckListCtrl::DeleteItem(int nItem)
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
BOOL CCheckListCtrl::DeleteAllItems()
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

DWORD CCheckListCtrl::GetItemData(int nItem) const
{
	ASSERT_ROW_COUNT(nItem);
	
	CCheckListItemData *pXLCD = (CCheckListItemData *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return 0;
	}
	
	return pXLCD->dwItemData;
}

void CCheckListCtrl::InvalidateSubItem(int nItem, int nSubItem)
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
BOOL CCheckListCtrl::GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect)
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

int	 CCheckListCtrl::GetHeaderCheckedState(int nSubItem)
{
	ASSERT_COL_COUNT(nSubItem);
	HDITEM hditem;
	
	// use the image index (0 or 1) to indicate the checked status
	hditem.mask = HDI_IMAGE;
	m_HeaderCtrl.GetItem(nSubItem, &hditem);
	return hditem.iImage;
}
void CCheckListCtrl::SetHeaderCheckedState(int nSubItem, int nCheckedState)
{
	ASSERT_COL_COUNT(nSubItem);
	ASSERT(nCheckedState >= CL_NONE_CHECK_BOX && nCheckedState <= CL_CHECKED);
	
	HDITEM hditem;
	
	hditem.mask = HDI_IMAGE;
	hditem.iImage = nCheckedState;
	m_HeaderCtrl.SetItem(nSubItem, &hditem);
}

void CCheckListCtrl::SetItemCheckedStateByClick(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateHeader)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	ASSERT(nCheckedState >= CL_NONE_CHECK_BOX && nCheckedState <= CL_CHECKED);
	
	int* pCheckStates = GetCheckedState(nItem);

	//1. Update data: checked state
	pCheckStates[nSubItem] = nCheckedState;
	
	//2. Update window
	InvalidateSubItem(nItem, nSubItem);
	UpdateWindow();
	
	//3. Update header
	if(bUpdateHeader)
	{
		// now update checkbox in header
		if (GetHeaderCheckedState(nSubItem) != CL_NONE_CHECK_BOX)
		{
			int nCheckedCount = CountCheckedItems(nSubItem);
			
			if (nCheckedCount == GetItemCount())
				SetHeaderCheckedState(nSubItem, CL_CHECKED);
			else
				SetHeaderCheckedState(nSubItem, CL_UNCHECKED);
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

void CCheckListCtrl::SetItemCheckedState(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateImmediately)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	ASSERT(nCheckedState >= CL_NONE_CHECK_BOX && nCheckedState <= CL_CHECKED);
	
	int* pCheckStates = GetCheckedState(nItem);
	
	//1. Update data: checked state
	pCheckStates[nSubItem] = nCheckedState;
	
	//2. Update window
	InvalidateSubItem(nItem, nSubItem);
	if(bUpdateImmediately)
	{
		UpdateWindow();
	}	
}
int  CCheckListCtrl::GetItemCheckedState(int nItem, int nSubItem)
{
	ASSERT_ROW_COL_COUNT(nItem, nSubItem);
	int* pCheckStates = GetCheckedState(nItem);
	return pCheckStates[nSubItem];
}

void CCheckListCtrl::ValidateCheck()
{
	int nColCount = GetHeaderCtrl()->GetItemCount();
	for(int nSubItem = 0; nSubItem < nColCount; nSubItem++)
	{
		if(GetHeaderCheckedState(nSubItem) == CL_NONE_CHECK_BOX)
		{
			continue;
		}

		//Column state		
		if (CountCheckedItems(nSubItem) == GetItemCount())
			SetHeaderCheckedState(nSubItem, CL_CHECKED);
		else
			SetHeaderCheckedState(nSubItem, CL_UNCHECKED);
	}
	m_HeaderCtrl.UpdateWindow();
	UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////
// CountCheckedItems
int CCheckListCtrl::CountCheckedItems(int nSubItem)
{
	ASSERT_COL_COUNT(nSubItem);
	
	int nCount = 0;
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		if(GetItemCheckedState(nItem, nSubItem) == CL_CHECKED)
		{
			nCount++;
		}			
	}
	
	return nCount;
}

int* CCheckListCtrl::GetCheckedState(int nItem)
{
	CCheckListItemData *pData = (CCheckListItemData *) CListCtrl::GetItemData(nItem);
	ASSERT(pData && pData->pCheckStates);
	return pData->pCheckStates;
}
int CCheckListCtrl::SwitchCheckedState(int nCheckedState)
{
	ASSERT(nCheckedState == CL_CHECKED || nCheckedState == CL_UNCHECKED);

	if(nCheckedState == CL_CHECKED)
	{
		nCheckedState = CL_UNCHECKED;
	}
	else
	{
		nCheckedState = CL_CHECKED;
	}
	return nCheckedState;
}

BOOL CCheckListCtrl::IsPtInSubItemCheckBox(int nItem, int nSubItem, POINT pt)
{
	CString sText = GetItemText(nItem, nSubItem);
	
	CRect checkboxRect;
	CalcCheckBoxRect(nItem, nSubItem, checkboxRect, sText.IsEmpty());

	BOOL bResult = checkboxRect.PtInRect(pt);
	AfxTrace("IsPtInSubItemCheckBox: %d\n", bResult);
	return bResult;
}

void CCheckListCtrl::SetRowHeight(int nRowHeight)
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
int  CCheckListCtrl::GetRowHeight()
{
	return m_nRowHeight;
}
