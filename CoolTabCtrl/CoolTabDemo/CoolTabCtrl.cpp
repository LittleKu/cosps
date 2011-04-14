#include "stdafx.h"
#include "CoolTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolTabCtrl

IMPLEMENT_DYNCREATE(CCoolTabCtrl, CWnd)

CPen CCoolTabCtrl::PEN_WHITE(PS_SOLID, 1, RGB(255,255,255));

//COLOR_3DFACE : RGB(224, 223, 227)
//Default system skin color
CPen CCoolTabCtrl::PEN_3DFACE(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));

//COLOR_3DLIGHT : RGB(241, 239, 226)
CPen CCoolTabCtrl::PEN_3DLIGHT(PS_SOLID, 1, GetSysColor(COLOR_3DLIGHT));

//COLOR_3DSHADOW : RGB(157, 157, 161)
CPen CCoolTabCtrl::PEN_3DSHADOW(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));

//COLOR_3DDKSHADOW : RGB(113, 111, 100)
CPen CCoolTabCtrl::PEN_3DDKSHADOW(PS_SOLID, 1, GetSysColor(COLOR_3DDKSHADOW));

//CCoolTabItem
UINT CCoolTabCtrl::CalcWidth(CCoolTabItem* pItem, CDC *pDC)
{
	UINT width = pDC->GetTextExtent(pItem->m_sText).cx;
	if(pItem->m_hIcon)
		width += 18;
	return width + 9;
}

void CCoolTabCtrl::Draw(CCoolTabItem* pItem, CDC *pDC, UINT nStyle, BOOL bActive, BOOL bHovered, UINT nIndex)
{
	CRect rect = pItem->m_rect;
	
	COLORREF oldColor;
	if(bHovered)
		oldColor = pDC->SetTextColor(RGB(255, 64, 128));
	if(nStyle & TCS_BOTTOM)
	{
		rect.bottom -= 5;
		if(bActive)
		{
			if(nIndex == 0)
			{
				rect.InflateRect(0, 2, 2, 2);
			}
			else
			{
				rect.InflateRect(2, 2, 2, 2);
			}
		}

		//Outer border
		pDC->SelectObject(&PEN_WHITE);

		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom - 2);

		pDC->LineTo(rect.left + 2, rect.bottom);

		pDC->SelectObject(&PEN_3DDKSHADOW);
		pDC->LineTo(rect.right - 2, rect.bottom);
		pDC->LineTo(rect.right, rect.bottom - 2);

		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.right, rect.bottom - 1);


		//Inner border
		pDC->SelectObject(&PEN_3DLIGHT);
		
		pDC->MoveTo(rect.left + 1, rect.top);
		pDC->LineTo(rect.left + 1, rect.bottom - 1);
		
		pDC->SelectObject(&PEN_3DSHADOW);
		pDC->MoveTo(rect.left + 2, rect.bottom - 1);
		pDC->LineTo(rect.right - 1, rect.bottom - 1);

		pDC->MoveTo(rect.right - 1, rect.top);
		pDC->LineTo(rect.right - 1, rect.bottom - 1);

		//To show icon and text
		rect.left += 6;
		rect.right -= 2;
		
		if(bActive)
		{
			rect.top += 7;
		}
		else
		{
			rect.top += 3;
		}
	}
	else if(nStyle & TCS_TOP)
	{
		rect.top += 5;
		if(bActive)
		{
			if(nIndex == 0)
			{
				rect.InflateRect(0, 2, 2, 2);
			}
			else
			{
				rect.InflateRect(2, 2, 2, 2);
			}
		}
		//Outer border
		pDC->SelectObject(&PEN_WHITE);
		
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(rect.left, rect.top + 1);
		
		pDC->MoveTo(rect.left + 1, rect.top + 1);
		pDC->LineTo(rect.left + 2, rect.top);
		pDC->LineTo(rect.right - 1, rect.top);
		
		pDC->SelectObject(&PEN_3DDKSHADOW);
		pDC->MoveTo(rect.right - 1, rect.top + 1);
		pDC->LineTo(rect.right, rect.top + 2);
		pDC->LineTo(rect.right, rect.bottom + 1);		
		
		//Inner border
		pDC->SelectObject(&PEN_3DLIGHT);
		
		pDC->MoveTo(rect.left + 1, rect.bottom);
		pDC->LineTo(rect.left + 1, rect.top + 1);

		pDC->MoveTo(rect.left + 2, rect.top + 1);
		pDC->LineTo(rect.right - 1, rect.top + 1);
		
		pDC->SelectObject(&PEN_3DSHADOW);
		pDC->MoveTo(rect.right - 1, rect.bottom);
		pDC->LineTo(rect.right - 1, rect.top + 1);


		//To show icon and text
		rect.left += 6;
		rect.right -= 2;
		
		if(bActive)
		{
			rect.top += 3;
		}
		else
		{
			rect.top += 5;
		}
	}

	//Icon
	if(pItem->m_hIcon != NULL && rect.Width() > 16)
	{
		::DrawIconEx(pDC->m_hDC, rect.left, rect.top, pItem->m_hIcon, 16, 16, 0, NULL, DI_NORMAL);
		rect.left += 18;
	}
	
	//Text
	if (!pItem->m_sText.IsEmpty())
	{
		rect.top += 2;
		
		CString sText = pItem->m_sText;
		
		BOOL bTruncated = FALSE;
		int len;
		
		for(len = sText.GetLength(); len > 4 && pDC->GetTextExtent(sText).cx > rect.Width(); len -= 2)
		{
			sText = sText.Left(len - 2);
			bTruncated = TRUE;
		}
		if(bTruncated)
		{
			if(len > 3)
			{
				sText = sText.Left(len - 3);
			}
			else
			{
				sText.Empty();
			}
			sText += "...";
		}
		
		pDC->DrawText(sText, &rect, DT_CENTER | DT_SINGLELINE);
	}

	if(bHovered)
		pDC->SetTextColor(oldColor);
}

CCoolTabCtrl::CCoolTabCtrl()
{
	m_nStyle = TCS_TOP;
	m_nActivePage = -1;
	m_nFocusedPage = -1;
	m_bEraseBkgnd = TRUE;
	m_nTabItemHeight = 28;
	m_bMouseHovered = FALSE;
}

CCoolTabCtrl::~CCoolTabCtrl()
{
	POSITION pos;
	CCoolTabItem* pItem;
	for(pos = m_tabItemList.GetHeadPosition(); pos!=NULL; )
	{
		pItem = (CCoolTabItem*)m_tabItemList.GetNext(pos);
		if(pItem != NULL)
		{	
			//If this window is created dynamically, try to destroy this window
			if(pItem->m_nCreateType == TI_CT_DYNAMIC && pItem->m_pWnd != NULL)
			{
				pItem->m_pWnd->DestroyWindow();
				delete pItem->m_pWnd;
				pItem->m_pWnd = NULL;
			}
			delete pItem;
			pItem=NULL;
		}
	}
	m_tabItemList.RemoveAll();
}


BEGIN_MESSAGE_MAP(CCoolTabCtrl, CWnd)
	//{{AFX_MSG_MAP(CCoolTabCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_SETCURSOR()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCoolTabCtrl message handlers

BOOL CCoolTabCtrl::Create(UINT wStyle, const CRect &rect, CWnd *pParentWnd, UINT nID)
{
// 	m_nStyle = wStyle & TCS_MASK;
// 	if(m_nStyle == 0)
// 	{
// 		m_nStyle = TCS_TOP;
// 	}
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, 
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)GetStockObject(LTGRAY_BRUSH), NULL);
	
	BOOL bRet = CWnd::Create(lpszClassName, NULL, (wStyle /*& ~TCS_MASK*/) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		rect, pParentWnd, nID);
	
	return bRet;
}

BOOL CCoolTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	DrawBk(pDC);
	return TRUE;
}

void CCoolTabCtrl::DrawBk(CDC* pDC)
{
	if(m_bEraseBkgnd)
	{
		CRect rect;
		GetTabItemsRect(&rect);
		CBrush brush(GetSysColor(COLOR_3DFACE));
		pDC->FillRect(rect,&brush);
	}
}

void CCoolTabCtrl::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
}

void CCoolTabCtrl::Draw(CDC* pDC)
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
			Draw(pItem, pDC, m_nStyle, FALSE, m_nFocusedPage == nItemIndex);
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

CCoolTabItem* CCoolTabCtrl::AddTab(CWnd *pWnd, LPCTSTR sText, UINT nIconID)
{
	ASSERT(pWnd);
	ASSERT(IsWindow(pWnd->m_hWnd));

	CCoolTabItem *pItem = new CCoolTabItem();
	pItem->m_pWnd = pWnd;
	pItem->m_nCreateType = TI_CT_NORMAL;
	pItem->m_sText = sText;
	if(nIconID)
		pItem->m_hIcon = AfxGetApp()->LoadIcon(nIconID);
	else
		pItem->m_hIcon = NULL;

	CRect rect;
	GetViewClientRect(rect);
	pWnd->MoveWindow(rect);

	pWnd->ShowWindow(SW_HIDE);

	m_tabItemList.AddTail(pItem);
	return pItem;
}

BOOL CCoolTabCtrl::AddTab(CRuntimeClass* pClass,UINT nIDTemplate, LPCTSTR sText, UINT nIconID)
{
	CDialog *pDlg = (CDialog*)pClass->CreateObject();
	if(pDlg != NULL && pDlg->Create(nIDTemplate, this))
	{
		CCoolTabItem *pItem = AddTab(pDlg, sText, nIconID);
		if(pItem)
		{
			pItem->m_nCreateType = TI_CT_DYNAMIC;
			return TRUE;
		}
	}
	return FALSE;
}


void CCoolTabCtrl::GetViewClientRect(LPRECT lpRect)
{
	CWnd::GetClientRect(lpRect);
	if(m_nStyle & TCS_BOTTOM)
	{
		lpRect->bottom -= m_nTabItemHeight;
	}
	else if(m_nStyle & TCS_TOP)
	{
		lpRect->top += m_nTabItemHeight;		
	}
}

void CCoolTabCtrl::GetTabItemsRect(LPRECT lpRect)
{
	CWnd::GetClientRect(lpRect);
	if(m_nStyle & TCS_BOTTOM)
	{
		lpRect->top = lpRect->bottom - m_nTabItemHeight;
	}
	else if(m_nStyle & TCS_TOP)
	{
		lpRect->bottom = lpRect->top + m_nTabItemHeight;
	}
}

UINT CCoolTabCtrl::GetTotalRequiredWidth(CDC* pDC)
{
	UINT totalWidth = 0;

	CCoolTabItem *pItem;
	POSITION	 pos;

	for(pos = m_tabItemList.GetHeadPosition(); pos != NULL; )
	{
		pItem = (CCoolTabItem*)m_tabItemList.GetNext(pos);
		ASSERT(pItem);

		totalWidth += CalcWidth(pItem, pDC);
	}

	return totalWidth;
}

void CCoolTabCtrl::AutoSize()
{
	UINT nPageCount = m_tabItemList.GetCount();
	if(nPageCount == 0)
	{
		return;
	}

	BOOL bMonoSpace = ((m_nStyle & TCS_MONOSPACE) == TCS_MONOSPACE) ? TRUE : FALSE;

	CRect tabItemRect, viewClientRect, rect;
	GetTabItemsRect(&tabItemRect);
	GetViewClientRect(&viewClientRect);

	CDC* pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(&m_font);

	int nTotalWidth = (int)GetTotalRequiredWidth(pDC);
	int nAvgWidth = tabItemRect.Width() / nPageCount;
	int nRequiredWidth;


	POSITION	 pos = NULL;
	CCoolTabItem *pItem = NULL;
	rect = tabItemRect;


	//1. Set the width of each tab item
	//2. Move all the client window to the client area by the way
	for(pos = m_tabItemList.GetHeadPosition(); pos != NULL; )
	{
		pItem=(CCoolTabItem*)m_tabItemList.GetNext(pos);
		ASSERT(pItem);
		
		if(bMonoSpace)
		{
			rect.right = rect.left + nAvgWidth;
		}
		else
		{
			
			//Calculate the initial width every tab item required
			nRequiredWidth = CalcWidth(pItem, pDC);
					
			//if the actual width is less than the total with of tab items, adjust
			if(IsAutoAdjustWidth() && nTotalWidth > tabItemRect.Width())
			{
				//1. if the width of the tab item is greater than the average, the average width 
				//2. otherwise, the required width
				rect.right = rect.left + min(nRequiredWidth, nAvgWidth);
			}
			//No need to adjust
			else
			{
				rect.right = rect.left + nRequiredWidth;
			}
		}
		
		pItem->m_rect = rect;
		rect.left = rect.right + 1;
		
		//Move all the client window to the client area by the way
		if(pItem->m_pWnd != NULL)
		{
			pItem->m_pWnd->MoveWindow(viewClientRect);
		}
	}
	

	//Restore DC
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

void CCoolTabCtrl::SetActivePage(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_tabItemList.GetCount());
	if(nIndex == m_nActivePage)
	{
		return;
	}
	CCoolTabItem *pOldItem = NULL, *pItem = NULL;
	if(m_nActivePage >= 0)
	{
		pOldItem = (CCoolTabItem*)GetPageItem(m_nActivePage);
		if(pOldItem)
		{
			pOldItem->m_pWnd->ShowWindow(SW_HIDE);
		}
	}

	pItem = (CCoolTabItem*)GetPageItem(nIndex);
	ASSERT(pItem);

	m_nActivePage = nIndex;
	if(m_nStyle & TCS_ANIMATE)
	{
		SetTimer(100, 15, NULL);
	}
	else
	{
		pItem->m_pWnd->ShowWindow(SW_SHOW);
	}	
}

CCoolTabItem* CCoolTabCtrl::GetPageItem(UINT nIndex)
{
	CCoolTabItem *pItem = NULL;
	POSITION pos = m_tabItemList.FindIndex(nIndex);
	if(pos)
		pItem = (CCoolTabItem*)m_tabItemList.GetAt(pos);
	return pItem;
}

int CCoolTabCtrl::GetPageIndex(CCoolTabItem* pItem)
{
	CCoolTabItem *pCurr = NULL;
	POSITION pos = NULL;
	int nIndex = 0;
	for(pos = m_tabItemList.GetHeadPosition(); pos != NULL; nIndex++)
	{
		pCurr = (CCoolTabItem*)m_tabItemList.GetNext(pos);
		if(pItem == pCurr)
		{
			return nIndex;
		}
	}
	return -1;
}

void CCoolTabCtrl::UpdateWindow()
{
 	AutoSize();
	
	if(m_nActivePage < 0)
	{
		SetActivePage(0);
	}

 	Invalidate();
}
int CCoolTabCtrl::GetPageIndex(CPoint point)
{
	int nItemIndex=0;
	POSITION pos;
	CCoolTabItem *pItem;
	for(pos=m_tabItemList.GetHeadPosition();pos!=NULL;nItemIndex++)
	{
		pItem=(CCoolTabItem*)m_tabItemList.GetNext(pos);
		if(pItem)
		{
			if(pItem->m_rect.PtInRect(point))
			{
				return nItemIndex;
			}
		}
	}
	return -1;
}

BOOL CCoolTabCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//THIS code also works for focus page setting!

// 	CPoint pt;
// 	GetCursorPos(&pt);
// 	ScreenToClient(&pt);
// 	
// 	int nPageIndex = GetPageIndex(pt);
// 	if(m_nFocusedPage != nPageIndex)
// 	{
// 		m_nFocusedPage = nPageIndex;
// 		Invalidate();
// 	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
void CCoolTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nItemIndex = GetPageIndex(point);
	if(nItemIndex >= 0 && m_nActivePage != nItemIndex)
	{
		SetActivePage(nItemIndex);
		Invalidate();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CCoolTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_bMouseHovered)
	{
		TRACKMOUSEEVENT tme; 
		tme.cbSize      = sizeof(tme); 
		tme.dwFlags     = TME_LEAVE; 
		tme.hwndTrack   = GetSafeHwnd();
		tme.dwHoverTime = HOVER_DEFAULT;
		::_TrackMouseEvent(&tme);
		
		m_bMouseHovered = TRUE;
	}

	int nPageIndex = GetPageIndex(point);
	if(m_nFocusedPage != nPageIndex)
	{
		m_nFocusedPage = nPageIndex;
		Invalidate();
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CCoolTabCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_nFocusedPage != -1)
	{
		m_nFocusedPage = -1;
		Invalidate();
	}
	m_bMouseHovered = FALSE;

	return 0;
}

LRESULT CCoolTabCtrl::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	//This event only occured once if the mouse doesn't move out of the erea
	/*
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	int nPageIndex = GetPageIndex(pt);
	TRACE2("Focus=%d, now=%d\n", m_nFocusedPage, nPageIndex);
	if(m_nFocusedPage != nPageIndex)
	{
		m_nFocusedPage = nPageIndex;
		Invalidate();
	}
	*/
	return 0;
}

void CCoolTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	m_bEraseBkgnd = TRUE;
	AutoSize();
	Invalidate();
}

void CCoolTabCtrl::OnTimer(UINT nIDEvent) 
{	
	POSITION pos = m_tabItemList.FindIndex(m_nActivePage);
	ASSERT(pos);
	
	static	int step, times = 10;
	UINT	width;
	CRect	rect;
	GetViewClientRect(&rect);
	
	CCoolTabItem *pItem = (CCoolTabItem*)m_tabItemList.GetAt(pos);
	ASSERT(pItem);
	
	width = rect.Width();
	if(times == 10)
	{
		rect.right = 0;
		rect.left = 0;
		step = width / times;
		times--;
		m_bEraseBkgnd = FALSE;
		pItem->m_pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		if(times <= 0)
		{
			times = 10;
			KillTimer(nIDEvent);
			m_bEraseBkgnd = TRUE;
		}
		else
		{
			rect.right = width - times*step;
			rect.left = rect.right - width;
			times--;
		}
	}
	pItem->m_pWnd->MoveWindow(rect);
}

void CCoolTabCtrl::SetStyleAnimate(BOOL bEnable)
{
	UINT style = GetStyle();
	if(bEnable)
		style |= TCS_ANIMATE;
	else
		style &= ~TCS_ANIMATE;
	SetStyle(style);
}

void CCoolTabCtrl::SetTabReplacement(int replacement)
{
	UINT style = GetStyle();

	style &= ~TCS_REPLACEMENT_MASK;
	style |= replacement;

	SetStyle(style);
}

void CCoolTabCtrl::SetMonoSpace(BOOL bEnable)
{
	UINT style = GetStyle();
	if(bEnable)
		style |= TCS_MONOSPACE;
	else
		style &= ~TCS_MONOSPACE;
	SetStyle(style);
}

void CCoolTabCtrl::SetAutoAdjustWidth(BOOL bEnable) 
{ 
	UINT style = GetStyle();
	if(bEnable)
		style |= TCS_AUTOADJUST;
	else
		style &= ~TCS_AUTOADJUST;
	SetStyle(style);
}

BOOL CCoolTabCtrl::IsAutoAdjustWidth()
{
	UINT style = GetStyle();

	return style & TCS_AUTOADJUST;
}

int CCoolTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LOGFONT logFont;
	ZeroMemory((void*)&logFont,sizeof(logFont));
	strcpy(logFont.lfFaceName,"Arial");
	logFont.lfHeight = -12;
	logFont.lfWeight = 400;
	logFont.lfCharSet = GB2312_CHARSET;
	logFont.lfOutPrecision = 3;
	logFont.lfClipPrecision = 2; 
	logFont.lfQuality = 1;
	logFont.lfPitchAndFamily = 2;
	m_font.CreateFontIndirect(&logFont);
	SetFont(&m_font);

	EnableToolTips(TRUE);   // enable tool tips for view
	return 0;
}

int CCoolTabCtrl::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	POSITION pos = m_tabItemList.GetHeadPosition();
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
			tip.Format("tips for %d", i);
			pTI->lpszText = (LPTSTR)malloc(sizeof(TCHAR)*(tip.GetLength()+1));
			_tcscpy(pTI->lpszText, tip);
//			pTI->lpszText = LPSTR_TEXTCALLBACK;
			return i;
		}
	}
	return -1;
}

BOOL CCoolTabCtrl::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
									  LRESULT *pResult)
{
	TOOLTIPTEXT *pText = (TOOLTIPTEXT *)pNMHDR;
	UINT nID =pNMHDR->idFrom;

	CString str;
#ifdef _DEBUG
	static int count = 0;	
	str.Format("(%d) hwndFrom=%d, idFrom=%d, code=%d, lpszText=%d, szText=%d, hinst=%d, uflags=%d\n", count++,
		pNMHDR->hwndFrom, pNMHDR->idFrom, pNMHDR->code, pText->lpszText, pText->szText, pText->hinst, pText->uFlags);
	TRACE(str);
#endif
	str.Format("tips for %d", nID);
	_tcscpy(pText->szText, str);
	return TRUE;
}

