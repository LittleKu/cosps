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
}

CMyReBar::~CMyReBar()
{
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
// 	
// 	return TRUE;
	return CReBarCtrl::OnEraseBkgnd(pDC);
}
