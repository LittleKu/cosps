// StaticStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "StaticStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticStatusBar

CStaticStatusBar::CStaticStatusBar()
{
	m_crBk = ::GetSysColor(COLOR_3DFACE);
	m_crTopLeft = ::GetSysColor(COLOR_3DSHADOW);
	m_crBottomRight = ::GetSysColor(COLOR_3DLIGHT);
	m_crSep = ::GetSysColor(COLOR_3DHIGHLIGHT);
// 	m_crBk = RGB(192, 192, 192);
// 	m_crTopLeft = RGB(127, 157, 185);
// 	m_crBottomRight = RGB(127, 157, 185);
// 	m_crSep = RGB(199, 197, 179);
	m_bDrawBorder = TRUE;
}

CStaticStatusBar::~CStaticStatusBar()
{
	RemoveAllPartData();
}


BEGIN_MESSAGE_MAP(CStaticStatusBar, CStatic)
	//{{AFX_MSG_MAP(CStaticStatusBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticStatusBar message handlers

void CStaticStatusBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = &dc;

	CFont* pOldFont = NULL;
	CWnd* pWndParent = GetParent();
	if(pWndParent)
	{
		pOldFont = pDC->SelectObject (pWndParent->GetFont());
	}
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);	

	DrawBk(pDC);

	CRect rect;
	GetClientRect(&rect);

	CRect rcPart = rect;

	int nIndex, nCount = m_mapParts.GetCount();
	PartData* pPartData = NULL;
	DWORD dwFormat = DT_LEFT | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER;
	for(nIndex = 0; nIndex < nCount; nIndex++)
	{
		m_mapParts.Lookup(nIndex, pPartData);

		GetRect(nIndex, &rcPart);

		rcPart.left += 2;
		if(rcPart.left >= rcPart.right)
		{
			continue;
		}
		::DrawTextEx(pDC->m_hDC, pPartData->szText.GetBuffer(0), pPartData->szText.GetLength(), &rcPart, dwFormat, NULL);
		pPartData->szText.ReleaseBuffer();
	}

	pDC->SetBkMode(nOldBkMode);
	if(pWndParent)
	{
		pDC->SelectObject(pOldFont);
	}
}

BOOL CStaticStatusBar::OnEraseBkgnd(CDC* pDC) 
{
	DrawBk(pDC);
	
	return TRUE;
}

void CStaticStatusBar::DrawBk(CDC *pDC)
{
	int crOldBk = pDC->GetBkColor();

	CRect rect;
	GetClientRect(&rect);

	pDC->FillSolidRect(&rect, m_crBk);

	pDC->Draw3dRect(&rect, m_crTopLeft, m_crBottomRight);

	PartData* pPartData = NULL;
	CRect rcPart = rect;
	int nIndex, nCount = m_mapParts.GetCount();
	for(nIndex = 0; nIndex < nCount; nIndex++)
	{
		GetRect(nIndex, &rcPart);

		m_mapParts.Lookup(nIndex, pPartData);
		if(pPartData->nDrawType != SBT_NOBORDERS)
		{
			rcPart.DeflateRect(0, 1);
			pDC->Draw3dRect(&rcPart, m_crTopLeft, m_crBottomRight);
		}

		rcPart.left = rcPart.right;
		rcPart.right = rcPart.left + 2;
		DrawSep(pDC, &rcPart);
	}
	
	pDC->SetBkColor(crOldBk);
}

BOOL CStaticStatusBar::SetParts( int nParts, int* pWidths )
{
	PartData* pPartData = NULL;
	int i;
	for(i = 0; i < nParts; i++)
	{
		if(m_mapParts.Lookup(i, pPartData))
		{
			pPartData->nRightEdge = pWidths[i];
		}
		else
		{
			pPartData = new PartData();
			pPartData->nIndex = i;
			pPartData->nRightEdge = pWidths[i];
			
			m_mapParts.SetAt(i, pPartData);
		}
	}
	int nCount = m_mapParts.GetCount();
	for(i = nParts; i < nCount; i++)
	{
		m_mapParts.Lookup(i, pPartData);

		delete pPartData;
		m_mapParts.RemoveKey(i);
	}

	return TRUE;
}

BOOL CStaticStatusBar::SetText( LPCTSTR lpszText, int nPane, int nType )
{
	ASSERT(nType >= 0 && nPane < m_mapParts.GetCount());

	PartData* pPartData = NULL;
	if(m_mapParts.Lookup(nPane, pPartData))
	{
		pPartData->szText = lpszText;
		pPartData->nDrawType = nType;

		CRect rcPart;
		GetRect(nPane, &rcPart);

		InvalidateRect(&rcPart);
	}
	return TRUE;
}



void CStaticStatusBar::RemoveAllPartData()
{
	POSITION pos = m_mapParts.GetStartPosition();
	int nIndex;
	PartData* pPartData = NULL;
	while(pos != NULL)
	{
		m_mapParts.GetNextAssoc( pos, nIndex, pPartData );
		if(pPartData != NULL)
		{
			delete pPartData;
		}
	}
	m_mapParts.RemoveAll();
}

BOOL CStaticStatusBar::GetRect( int nPane, LPRECT lpRect ) const
{
	ASSERT(nPane >= 0 && nPane < m_mapParts.GetCount());
	
	CRect rcClient;
	GetClientRect(&rcClient);

	lpRect->top = rcClient.top;
	lpRect->bottom = rcClient.bottom;

	//Left
	PartData* pPartData = NULL;
	if(nPane == 0)
	{
		lpRect->left = 0;
	}
	else
	{
		m_mapParts.Lookup(nPane - 1, pPartData);
		lpRect->left = pPartData->nRightEdge + 2;
	}

	//Right
	if(m_mapParts.Lookup(nPane, pPartData))
	{
		if(pPartData->nRightEdge >= 0)
		{
			lpRect->right = pPartData->nRightEdge;
		}
		else
		{
			lpRect->right = rcClient.right;
		}
	}
	return TRUE;
}

void CStaticStatusBar::DrawSep(CDC *pDC, LPCRECT lpRect)
{
	int top = lpRect->top + 4;
	int bottom = lpRect->bottom - 2;
	ASSERT(bottom > top);

	pDC->FillSolidRect(lpRect->left, top, 1, bottom - top, m_crSep);
	pDC->FillSolidRect(lpRect->left + 1, top, 1, bottom - top, RGB(255, 255, 255));
}

COLORREF CStaticStatusBar::SetBkColor( COLORREF cr )
{
	COLORREF crOldBk = m_crBk;
	m_crBk = cr;

	Invalidate();

	return crOldBk;
}