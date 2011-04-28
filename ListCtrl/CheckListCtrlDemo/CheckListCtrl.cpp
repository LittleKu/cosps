// CheckListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CheckListCtrlDemo.h"
#include "CheckListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
		
		if(pListCtrlData->pCheckStates[nSubItem] != CHECK_LIST_CTRL_NO_IMAGE)
		{
			BOOL bDrawMark = (pListCtrlData->pCheckStates[nSubItem] == CHECK_LIST_CTRL_CHECKED_IMAGE);
			DrawCheckbox(nItem, nSubItem, pDC, crText, crBkgnd, rect, bDrawMark);
			
			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}
	}
}

BOOL CCheckListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* pNMActivate = (NMITEMACTIVATE*)pNMHDR;
	*pResult = 0;

	return FALSE;
}

BOOL CCheckListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	return FALSE;
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
	
	CCheckHeaderCtrl::DrawCheckBox(pDC, &chkboxrect, bDrawMark, m_crWindow);
	
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
		pListCtrlData->pCheckStates[i] = CHECK_LIST_CTRL_NO_IMAGE;
	}
	pListCtrlData->dwItemData = pItem->lParam;
	
	CListCtrl::SetItemData(index, (DWORD)pListCtrlData);
	
	return index;
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
