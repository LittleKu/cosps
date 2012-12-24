// MainReBar.cpp : implementation file
//

#include "stdafx.h"
#include "MainReBar.h"
#include "cflmfc/gdi_utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainReBar

CMainReBar::CMainReBar()
{
}

CMainReBar::~CMainReBar()
{
}


BEGIN_MESSAGE_MAP(CMainReBar, CReBarCtrl)
	//{{AFX_MSG_MAP(CMainReBar)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainReBar message handlers

BOOL CMainReBar::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcClient;
	GetClientRect(&rcClient);

	cfl::DrawGradient4(pDC, rcClient, RGB(223, 237, 244), RGB(191, 217, 232), 
		RGB(178, 209, 228), RGB(220, 234, 242), FALSE);

	return TRUE;
}
