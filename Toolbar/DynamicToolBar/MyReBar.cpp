// MyReBar.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "MyReBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MULE_TOOLBAR_BAND_NR 0
/////////////////////////////////////////////////////////////////////////////
// CMyReBar

CMyReBar::CMyReBar()
{
	m_bmpBack = NULL;
}

CMyReBar::~CMyReBar()
{
	if (m_bmpBack)
		VERIFY( ::DeleteObject(m_bmpBack) );
}


BEGIN_MESSAGE_MAP(CMyReBar, CReBarCtrl)
	//{{AFX_MSG_MAP(CMyReBar)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyReBar message handlers

BOOL CMyReBar::OnEraseBkgnd(CDC* pDC) 
{
// 	CBrush brush(RGB(255, 128, 0));
// 
// 	CRect rect;
// 	GetClientRect(&rect);
// 	pDC->FillRect(&rect, &brush);
	
//	return TRUE;
	return CReBarCtrl::OnEraseBkgnd(pDC);
}

void CMyReBar::UpdateBackground()
{
	HBITMAP hbmp = (HBITMAP)::LoadImage(NULL, _T("abc.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbmp)
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_STYLE;
		if (GetBandInfo(MULE_TOOLBAR_BAND_NR, &rbbi))
		{
			rbbi.fMask = RBBIM_STYLE | RBBIM_BACKGROUND;
			rbbi.fStyle |= RBBS_FIXEDBMP;
			rbbi.hbmBack = hbmp;
			if (SetBandInfo(MULE_TOOLBAR_BAND_NR, &rbbi))
			{
				if (m_bmpBack)
					VERIFY(::DeleteObject(m_bmpBack));
				m_bmpBack = hbmp;
				hbmp = NULL;
			}
		}
		if (hbmp)
			VERIFY( ::DeleteObject(hbmp) );
	}
	else
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_STYLE;
		if (GetBandInfo(MULE_TOOLBAR_BAND_NR, &rbbi))
		{
			rbbi.fMask = RBBIM_STYLE | RBBIM_BACKGROUND;
			rbbi.fStyle &= ~RBBS_FIXEDBMP;
			rbbi.hbmBack = NULL;
			if (SetBandInfo(MULE_TOOLBAR_BAND_NR, &rbbi))
			{
				if (m_bmpBack)
					VERIFY(::DeleteObject(m_bmpBack));
				m_bmpBack = NULL;
			}
		}
	}
}
