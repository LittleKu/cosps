// DragSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DragSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragSliderCtrl

CDragSliderCtrl::CDragSliderCtrl()
{
}

CDragSliderCtrl::~CDragSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CDragSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CDragSliderCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragSliderCtrl message handlers

void CDragSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcThumb;
	GetThumbRect(&rcThumb);

	//click the thumb
	if(rcThumb.PtInRect(point))
	{
		m_bDragging = TRUE;

		SetCapture();

		AfxTrace(_T("[OnLButtonDown]: clicked the thumb, dragging start.\n"));;
	}
	//click the slider but not thumb
	else 
	if(m_rcClient.PtInRect(point))
	{
		int nMin = 0, nMax = 0;
		GetRange(nMin, nMax);
		if(point.x < m_rcChannel.left)
		{
			point.x = m_rcChannel.left;
		}
		if(point.x > m_rcChannel.right)
		{
			point.x = m_rcChannel.right;
		}
		int nPos = (nMax - nMin) * (point.x - m_rcChannel.left) / m_rcChannel.Width();
		nPos += nMin;

		SetPos(nPos);
		AfxTrace(_T("[OnLButtonDown]: clicked the channel\n"));
		//return;
	}
	else
	{
		AfxTrace(_T("[OnLButtonDown]: none thumb nor channel\n"));
		//return;
	}
	
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CDragSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		m_bDragging = FALSE;
		ReleaseCapture();
		
		AfxTrace(_T("[OnLButtonUp]: ReleaseCapture\n"));
	}
	CSliderCtrl::OnLButtonUp(nFlags, point);
}

void CDragSliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDragging && (nFlags & MK_LBUTTON))
	{
		if (point.x > m_rcChannel.right)
		{
			point.x = m_rcChannel.right;
		}
		if (point.x < m_rcChannel.left)
		{
			point.x = m_rcChannel.left;
		}
		
		int nMax = 0;  
		int nMin = 0;  
		GetRange(nMin, nMax);  
		int nPos = (nMax - nMin) * (point.x - m_rcChannel.left) / m_rcChannel.Width();
		nPos += nMin;
		SetPos(nPos);
	}
	else
	{
		CSliderCtrl::OnMouseMove(nFlags, point);
	}
}

void CDragSliderCtrl::PreSubclassWindow() 
{
	SetRange(0, 100);
 	SetPageSize(1);
 	SetLineSize(1);
	// TODO: Add your specialized code here and/or call the base class
	GetClientRect(&m_rcClient);
	GetChannelRect(&m_rcChannel);
	
	CSliderCtrl::PreSubclassWindow();
}
