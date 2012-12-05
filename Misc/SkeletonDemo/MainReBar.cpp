// MainReBar.cpp : implementation file
//

#include "stdafx.h"
#include "MainReBar.h"
#include "cflmfc/UiTheme.h"

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

	pDC->FillSolidRect(&rcClient, RGB(255, 255, 255));
	cfl::UiTheme* pTheme = cfl::GetSysThemeMgr()->GetThemeData(cfl::UTI_BLUE);
	pTheme->DrawThemeBackground(pDC, 0, cfl::UTSI_HOT, &rcClient);	

	return TRUE;
}
