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
	m_crBk = RGB(236, 233, 216);
	m_crTopLeft = RGB(172, 168, 153);
	m_crBottomRight = RGB(255, 255, 255);
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

	pDC->FillSolidRect(&rect, m_crBk);
	pDC->Draw3dRect(&rect, m_crTopLeft, m_crBottomRight);

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
