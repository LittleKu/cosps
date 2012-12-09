// FlickerFreeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FlickerFreeListCtrlDemo.h"
#include "FlickerFreeListCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MLC_BLEND(A, B, X) ((A + B * (X-1) + ((X+1)/2)) / X)

#define MLC_RGBBLEND(A, B, X) (                   \
	RGB(MLC_BLEND(GetRValue(A), GetRValue(B), X), \
	MLC_BLEND(GetGValue(A), GetGValue(B), X),     \
	MLC_BLEND(GetBValue(A), GetBValue(B), X))     \
)

// Be carefull with that offsets, they are supposed to match *exactly* the Windows built-in metric.
// If it does not match that value, column auto-sizeing (double clicking on header divider) will
// give inaccurate results.
const int CFlickerFreeListCtrl::sm_iIconOffset = 4;	// Offset from left window border to icon (of 1st column)
const int CFlickerFreeListCtrl::sm_iLabelOffset = 2;	// Offset between right icon border and item text (of 1st column)
const int CFlickerFreeListCtrl::sm_iSubItemInset = 4;	// Offset from left and right column border to item text

/////////////////////////////////////////////////////////////////////////////
// CFlickerFreeListCtrl

CFlickerFreeListCtrl::CFlickerFreeListCtrl()
{
	m_bCustomDraw = false;
	
	//just in case
    m_crWindow = 0;
    m_crWindowText = 0;
	m_crWindowTextBk = m_crWindow;
    m_crHighlight = 0;
    m_crHighlightText = m_crWindowText;
	m_crGlow=0;
    m_crFocusLine = 0;
    m_crNoHighlight = 0;
    m_crNoFocusLine = 0;
}

CFlickerFreeListCtrl::~CFlickerFreeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CFlickerFreeListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFlickerFreeListCtrl)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlickerFreeListCtrl message handlers

void CFlickerFreeListCtrl::SetColors()
{
	m_crWindow       = ::GetSysColor(COLOR_WINDOW);
	m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crWindowTextBk = m_crWindow;

	COLORREF crHighlight = ::GetSysColor(COLOR_HIGHLIGHT);

	CString strBkImage;

	SetBkColor(m_crWindow);
	SetTextBkColor(m_crWindowTextBk);
	SetTextColor(m_crWindowText);
	
	// Must explicitly set a NULL watermark bitmap, to clear any already set watermark bitmap.
	LVBKIMAGE lvimg = {0};
	lvimg.ulFlags = LVBKIF_TYPE_WATERMARK;
	SetBkImage(&lvimg);

	m_crFocusLine = crHighlight;
	if (g_bLowColorDesktop) {
		m_crNoHighlight		= crHighlight;
		m_crNoFocusLine		= crHighlight;
		m_crHighlight		= crHighlight;
		m_crHighlightText	= GetSysColor(COLOR_HIGHLIGHTTEXT);
		m_crGlow			= crHighlight;
	} else {
		m_crNoHighlight		= MLC_RGBBLEND(crHighlight, m_crWindow, 8);
		m_crNoFocusLine		= MLC_RGBBLEND(crHighlight, m_crWindow, 2);
		m_crHighlight		= MLC_RGBBLEND(crHighlight, m_crWindow, 4);
		m_crHighlightText	= m_crWindowText;
		m_crGlow			= MLC_RGBBLEND(crHighlight, m_crWindow, 3);
	}
}

BOOL CFlickerFreeListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CListCtrl::PreTranslateMessage(pMsg);
}

BOOL CFlickerFreeListCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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

void CFlickerFreeListCtrl::PreSubclassWindow() 
{
	ModifyStyle(0, LVS_OWNERDRAWFIXED);
	SetColors();
	CListCtrl::PreSubclassWindow();
	// Win98: Explicitly set to Unicode to receive Unicode notifications.
	SendMessage(CCM_SETUNICODEFORMAT, TRUE);
	SetExtendedStyle(LVS_EX_HEADERDRAGDROP);
	
	// Vista: Reduce flickering in header control
	ModifyStyle(0, WS_CLIPCHILDREN);

	// DEFAULT_GUI_FONT: Vista: "MS Shell Dlg" with 8 pts (regardless of system applet settings !!!)
	// SYSTEM_FONT:		 Vista: Good old Windows 3.11 System Font
	// NULL				 Vista: Font ('Symbol') with the face and size which is configured in System applet.
	if (TRUE)
		SendMessage(WM_SETFONT, NULL, FALSE);
}

BOOL CFlickerFreeListCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CListCtrl::OnWndMsg(message, wParam, lParam, pResult);
}


void CFlickerFreeListCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CFlickerFreeListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


BOOL CFlickerFreeListCtrl::OnEraseBkgnd(CDC* pDC) 
{
//	if (m_crWindowTextBk == CLR_NONE) // this creates a lot screen flickering
//		return CListCtrl::OnEraseBkgnd(pDC);
	
	int itemCount = GetItemCount();
	if (!itemCount)
		return CListCtrl::OnEraseBkgnd(pDC);
	
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

void CFlickerFreeListCtrl::OnSysColorChange() 
{
	CListCtrl::OnSysColorChange();
	SetColors();
}

void CFlickerFreeListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//set up our flicker free drawing
	CRect rcItem(lpDrawItemStruct->rcItem);
	CDC *oDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CMemDC pDC(oDC, &rcItem, m_crWindow);
	CFont *pOldFont = pDC->SelectObject(GetFont());
	CRect rcClient;
	GetClientRect(&rcClient);

	int iItem = lpDrawItemStruct->itemID;
	CImageList* pImageList;
	CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();

	//gets the item image and state info
	LVITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = iItem;
	lvi.iSubItem = 0;
	lvi.stateMask = LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED | LVIS_GLOW; 
	GetItem(&lvi);

	//see if the item be highlighted
	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) || (lvi.state & LVIS_SELECTED));
	BOOL bCtrlFocused = ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS));
	BOOL bGlowing = ( lvi.state & LVIS_GLOW );

	COLORREF crOldTextColor;
	if (m_bCustomDraw) {
		if (bHighlight)
			crOldTextColor = pDC->SetTextColor(g_bLowColorDesktop ? m_crHighlightText : m_lvcd.clrText);
		else
			crOldTextColor = pDC->SetTextColor(m_lvcd.clrText);
	}
	else {
		if (bHighlight)
			crOldTextColor = pDC->SetTextColor(m_crHighlightText);
		else
			crOldTextColor = pDC->SetTextColor(m_crWindowText);
	}

	//get rectangles for drawing
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(iItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(iItem, rcLabel, LVIR_LABEL);
	GetItemRect(iItem, rcIcon, LVIR_ICON);
	CRect rcCol(rcBounds);

	//the label!
	CString sLabel = GetItemText(iItem, 0);
	//labels are offset by a certain amount
	//this offset is related to the width of a space character
	CRect rcHighlight;
	CRect rcWnd;

	//should I check (GetExtendedStyle() & LVS_EX_FULLROWSELECT) ?
	rcHighlight.top    = rcBounds.top;
	rcHighlight.bottom = rcBounds.bottom;
	rcHighlight.left   = rcBounds.left;
	rcHighlight.right  = rcBounds.right;

	//draw the background color
	if(bHighlight) 
	{
		if(bCtrlFocused) 
		{
			pDC->FillRect(rcHighlight, &CBrush(m_crHighlight));
			pDC->SetBkColor(m_crHighlight);
		}
		else if(bGlowing)
		{
			pDC->FillRect(rcHighlight, &CBrush(m_crGlow));
			pDC->SetBkColor(m_crGlow);
		}
		else 
		{
			pDC->FillRect(rcHighlight, &CBrush(m_crNoHighlight));
			pDC->SetBkColor(m_crNoHighlight);
		}
	} 
	else
	{
		if(bGlowing)
		{
			pDC->FillRect(rcHighlight, &CBrush(m_crGlow));
			pDC->SetBkColor(m_crGlow);
		}
		else
		{
			if (m_crWindowTextBk == CLR_NONE)
				DefWindowProc(WM_ERASEBKGND, (WPARAM)pDC->m_hDC, 0);
			else
				pDC->FillRect(rcHighlight, &CBrush(m_crWindow));
			pDC->SetBkColor(m_crWindow);
		}
	}

	//update column
	rcCol.right = rcCol.left + GetColumnWidth(0);

	//draw state icon
	if(lvi.state & LVIS_STATEIMAGEMASK) 
	{
		int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
		pImageList = GetImageList(LVSIL_STATE);
		if(pImageList) 
		{
			pImageList->Draw(pDC, nImage, rcCol.TopLeft(), ILD_TRANSPARENT);
		}
	}

	//draw the item's icon
	pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList) 
	{
		int iIconPosY = (rcItem.Height() > 16) ? ((rcItem.Height() - 16) / 2) : 0;
		pImageList->Draw(pDC, lvi.iImage, CPoint(rcIcon.left, rcIcon.top + iIconPosY), ILD_TRANSPARENT);
	}

	if (m_crWindowTextBk == CLR_NONE)
		pDC->SetBkMode(TRANSPARENT);

	//draw item label (column 0)
	rcLabel.left += sm_iLabelOffset;
	rcLabel.right -= sm_iLabelOffset;
	pDC->DrawText(sLabel, -1, rcLabel, MLC_DT_TEXT | DT_NOCLIP);

	//draw labels for remaining columns
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right : rcBounds.right;

	int iCount = pHeaderCtrl->GetItemCount();
	for(int iCurrent = 1; iCurrent < iCount; iCurrent++) 
	{
		int iColumn = pHeaderCtrl->OrderToIndex(iCurrent);
		//don't draw column 0 again
		if(iColumn == 0)
			continue;

		GetColumn(iColumn, &lvc);
		//don't draw anything with 0 width
		if(lvc.cx == 0)
			continue;

		rcCol.left = rcCol.right;
		rcCol.right += lvc.cx;

		if (rcCol.left < rcCol.right && HaveIntersection(rcClient, rcCol))
		{
			sLabel = GetItemText(iItem, iColumn);
			if (sLabel.GetLength() == 0)
				continue;

			//get the text justification
			UINT nJustify = DT_LEFT;
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK) 
			{
				case LVCFMT_RIGHT:
					nJustify = DT_RIGHT;
					break;
				case LVCFMT_CENTER:
					nJustify = DT_CENTER;
					break;
				default:
					break;
			}

			rcLabel = rcCol;
			rcLabel.left += sm_iLabelOffset + sm_iSubItemInset;
			rcLabel.right -= sm_iLabelOffset + sm_iSubItemInset;

			pDC->DrawText(sLabel, -1, rcLabel, MLC_DT_TEXT | nJustify);
		}
	}

//	DrawFocusRect(pDC, rcHighlight, lvi.state & LVIS_FOCUSED, bCtrlFocused, lvi.state & LVIS_SELECTED);

	pDC->Flush();
	pDC->SelectObject(pOldFont);
}

void CFlickerFreeListCtrl::DrawFocusRect(CDC *pDC, const CRect &rcItem, BOOL bItemFocused, BOOL bCtrlFocused, BOOL bItemSelected)
{
	//draw focus rectangle if item has focus
	if (bItemFocused && (bCtrlFocused || bItemSelected)) 
	{
		if (!bCtrlFocused || !bItemSelected)
			pDC->FrameRect(rcItem, &CBrush(m_crNoFocusLine));
		else
			pDC->FrameRect(rcItem, &CBrush(m_crFocusLine));
	}
}

void CFlickerFreeListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	Default();
	
	if (TRUE)
	{
		CDC *pDC = GetDC();
		if (pDC)
		{
			CFont *pFont = GetFont();
			if (pFont)
			{
				CFont *pFontOld = pDC->SelectObject(pFont);
				TEXTMETRIC tm;
				pDC->GetTextMetrics(&tm);
				int iNewHeight = tm.tmHeight + tm.tmExternalLeading + 1;
				lpMeasureItemStruct->itemHeight = max(18, iNewHeight);
				pDC->SelectObject(pFontOld);
			}
			ReleaseDC(pDC);
		}
	}
	lpMeasureItemStruct->itemHeight += 10;
}

void CFlickerFreeListCtrl::InitItemMemDC(CMemDC *dc, LPDRAWITEMSTRUCT lpDrawItemStruct, BOOL &bCtrlFocused)
{
	bCtrlFocused = ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS));
	
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		if (bCtrlFocused)
			dc->FillBackground(m_crHighlight);
		else
			dc->FillBackground(m_crNoHighlight);
	}
	else
	{
		if (m_crWindowTextBk == CLR_NONE)
		{
			DefWindowProc(WM_ERASEBKGND, (WPARAM)(HDC)*dc, 0);
			dc->SetBkMode(TRANSPARENT);
		}
		else
		{
			ASSERT( m_crWindowTextBk == GetBkColor() );
			dc->FillBackground(m_crWindowTextBk);
		}
	}
	
	dc->SetTextColor((lpDrawItemStruct->itemState & ODS_SELECTED) ? m_crHighlightText : m_crWindowText);
	dc->SetFont(GetFont());
}

void CFlickerFreeListCtrl::SendMeasureItemMsg()
{
	CRect rc;
	GetWindowRect(&rc);
	
	WINDOWPOS wp;
	wp.hwnd = this->m_hWnd;
	wp.cx = rc.Width() ;
	wp.cy = rc.Height() ;
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	
	SendMessage(WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM> (&wp));
}