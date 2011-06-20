// MainStatusBarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "MainStatusBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainStatusBarCtrl

CMainStatusBarCtrl::CMainStatusBarCtrl()
{
}

CMainStatusBarCtrl::~CMainStatusBarCtrl()
{
}


BEGIN_MESSAGE_MAP(CMainStatusBarCtrl, CStatusBarCtrl)
	//{{AFX_MSG_MAP(CMainStatusBarCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
//	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainStatusBarCtrl message handlers
BOOL CMainStatusBarCtrl::OnEraseBkgnd(CDC* pDC)
{	
	CRect rect;
	GetClientRect(&rect);

	int crOldBk = pDC->GetBkColor();

	pDC->FillSolidRect(&rect, RGB(255, 128, 0));
	pDC->Draw3dRect(&rect, RGB(127, 157, 185), RGB(127, 157, 185));

	pDC->SetBkColor(crOldBk);

	return TRUE;
}

void CMainStatusBarCtrl::OnPaint()
{
	CStatusBarCtrl::OnPaint();
}

void CMainStatusBarCtrl::OnNcPaint() 
{
}
