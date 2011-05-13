// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SPDemo.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

// void CMyListCtrl::OnPaint() 
// {
// 	CListCtrl::OnPaint();
// 	Default();
// 
// 	CPaintDC dc(this); // device context for painting
// 	
// 
// 	CDC* pDC = &dc;
// 
// 
//    // create and select a solid blue brush
//    CBrush brushBlue(RGB(0, 0, 255));
//    CBrush* pOldBrush = pDC->SelectObject(&brushBlue);
// 
//    // create and select a thick, black pen
//    CPen penBlack;
//    penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
//    CPen* pOldPen = pDC->SelectObject(&penBlack);
// 
//    // get our client rectangle
//    CRect rect;
//    GetClientRect(rect);
// 
//    // shrink our rect 20 pixels in each direction
// //   rect.DeflateRect(20, 20);
//    
//    // draw a thick black rectangle filled with blue
//    pDC->Rectangle(rect);
//    
//    // put back the old objects
//    pDC->SelectObject(pOldBrush);
//    pDC->SelectObject(pOldPen);
// 
// }

void CMyListCtrl::OnNcPaint() 
{
	// this is a sample drawing code :
// 	CRect rcWin;
// 	GetWindowRect(&rcWin);
// 
// 	CRect rcHeadCtrl;
// 	GetHeaderCtrl()->GetWindowRect(&rcHeadCtrl);
// 
// 	rcHeadCtrl.bottom += rcWin.Height();
// 
// 	CRect rcFrame(0, 0, rcWin.Width(), rcWin.Height());
// 	CPaintDC dc(this); 
// 	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
// 	CPen *pOld = dc.SelectObject(&pen);
// 	dc.Rectangle(&rcFrame);
// 	dc.SelectObject(pOld);

	// draw the inside.
	Default();
	
	// draw over the border.
	CBrush brush(RGB(255, 0, 0));
	CRect rc;
	GetWindowRect(rc);
	
	CDC *pDC = GetParent()->GetDC();
	GetParent()->ScreenToClient( &rc );
	// do the drawing ...
	pDC->FrameRect( &rc , &brush );
 	rc.DeflateRect( 1, 1, 1, 1 );
 	pDC->FrameRect( &rc, &brush );

	GetParent()->ReleaseDC(pDC);
}
