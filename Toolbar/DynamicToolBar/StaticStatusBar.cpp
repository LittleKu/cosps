// StaticStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "StaticStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticStatusBar

void DrawGradient(CDC * pDC, CRect rect, COLORREF colorStart, COLORREF colorEnd, BOOL bHorz/* = TRUE*/, UINT nSteps/* = 64*/)
{
    for (UINT i = 0; i < nSteps; i++)
    {
        BYTE bR = (BYTE) ((GetRValue(colorStart) * (nSteps - i) + GetRValue(colorEnd) * i) / nSteps);
        BYTE bG = (BYTE) ((GetGValue(colorStart) * (nSteps - i) + GetGValue(colorEnd) * i) / nSteps);
        BYTE bB = (BYTE) ((GetBValue(colorStart) * (nSteps - i) + GetBValue(colorEnd) * i) / nSteps);
		
		CBrush br (RGB(bR, bG, bB));
		
        CRect r2 = rect;
        if (!bHorz)
        {
            r2.top = rect.top + ((i * rect.Height()) / nSteps);
            r2.bottom = rect.top + (((i + 1) * rect.Height()) / nSteps);
            if (r2.Height() > 0)
                pDC->FillRect(r2, &br);
        }
        else
        {
            r2.left = rect.left + ((i * rect.Width()) / nSteps);
            r2.right = rect.left + (((i + 1) * rect.Width()) / nSteps);
            if (r2.Width() > 0)
                pDC->FillRect(r2, &br);
        }
    }
}

CStaticStatusBar::CStaticStatusBar()
{
	m_crBk = ::GetSysColor(COLOR_3DFACE);

	m_crTopSep = ::GetSysColor(COLOR_3DDKSHADOW);
	m_crTopLeft = ::GetSysColor(COLOR_3DSHADOW);
	m_crBottomRight = ::GetSysColor(COLOR_3DLIGHT);

	m_crBorderSepStart = RGB(203, 199, 181);
	m_crBorderSepEnd = RGB(199, 197, 179);

	m_nSepWidth = 2;

#ifdef _DEBUG
	m_crBk = RGB(255, 128, 0);
	m_crTopSep = RGB(0, 0, 255);
	m_crTopLeft = RGB(0, 32, 255);
	m_crBottomRight = RGB(0, 64, 64);
#endif
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

	//Change font to parent's font
	CFont* pOldFont = NULL;
	CWnd* pWndParent = GetParent();
	if(pWndParent)
	{
		pOldFont = pDC->SelectObject (pWndParent->GetFont());
	}
	//Change bk mode to transparent
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

	//Draw Background first
	DrawBk(pDC);

	//Then draw the text info
	DWORD dwFormat = DT_LEFT | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER;
	CRect rcPart;
	PartData* pPartData = NULL;

	int nPane, nCount = m_mapParts.GetCount();		
	for(nPane = 0; nPane < nCount; nPane++)
	{
		m_mapParts.Lookup(nPane, pPartData);
		GetRect(nPane, &rcPart);

		rcPart.left += 2;
		if(rcPart.left >= rcPart.right)
		{
			continue;
		}
		::DrawTextEx(pDC->m_hDC, pPartData->szText.GetBuffer(0), pPartData->szText.GetLength(), &rcPart, dwFormat, NULL);
		pPartData->szText.ReleaseBuffer();
	}

	//Restore DC
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

	//Background
	pDC->FillSolidRect(&rect, m_crBk);

	//Top line
	pDC->FillSolidRect(rect.left, rect.top, rect.Width(), 1, m_crTopSep);
	pDC->FillSolidRect(rect.left, rect.top + 1, rect.Width(), 1, m_crTopLeft);

	//Bottom line
	pDC->FillSolidRect(rect.left, rect.bottom, rect.Width(), 1, m_crBottomRight);

	PartData* pPartData = NULL;
	CRect rcPart;

	int nPane, nCount = m_mapParts.GetCount();
	for(nPane = 0; nPane < nCount; nPane++)
	{
		GetRect(nPane, &rcPart);

		m_mapParts.Lookup(nPane, pPartData);
		if(pPartData->nDrawType != SBT_NOBORDERS)
		{
			ASSERT((UINT)rcPart.Width() > m_nSepWidth);
			rcPart.left = rcPart.right - m_nSepWidth;
			DrawSep(pDC, &rcPart);
		}
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
	if(nPane < 0 || nPane >= m_mapParts.GetCount())
	{
		return FALSE;
	}

	PartData* pPartData = NULL;
	if(!m_mapParts.Lookup(nPane, pPartData))
	{
		return FALSE;
	}

	pPartData->szText = lpszText;
	pPartData->nDrawType = nType;
	
	CRect rcPart;
	GetRect(nPane, &rcPart);

	InvalidateRect(&rcPart);

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

//Get the Rectangle of the Pane, including the separator space
BOOL CStaticStatusBar::GetRect( int nPane, LPRECT lpRect ) const
{
	if(nPane < 0 || nPane >= m_mapParts.GetCount())
	{
		return FALSE;
	}
	
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
		lpRect->left = pPartData->nRightEdge + 1;
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

	if(bottom <= top)
	{
		return;
	}

	CRect rcSep;
	rcSep.left = lpRect->left;
	rcSep.top = top;
	rcSep.right = rcSep.left + 1;
	rcSep.bottom = bottom;
	DrawGradient(pDC, rcSep, m_crBorderSepStart, m_crBorderSepEnd, FALSE, rcSep.Height());

	pDC->FillSolidRect(lpRect->left + 1, top, 1, bottom - top, RGB(255, 255, 255));
}

COLORREF CStaticStatusBar::SetBkColor( COLORREF cr )
{
	COLORREF crOldBk = m_crBk;
	m_crBk = cr;

	Invalidate();

	return crOldBk;
}