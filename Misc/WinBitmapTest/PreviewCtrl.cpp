// PreviewCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PreviewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewCtrl

CPreviewCtrl::CPreviewCtrl()
{
	m_nTimer = 0;
	m_nFrame = -1;
	m_nTotalFrames = 60;
}

CPreviewCtrl::~CPreviewCtrl()
{
}


BEGIN_MESSAGE_MAP(CPreviewCtrl, CStatic)
	//{{AFX_MSG_MAP(CPreviewCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewCtrl message handlers
void CPreviewCtrl::OnPaint() 
{
	ASSERT(m_pBmp1 != NULL && m_pBmp2 != NULL);

	//AfxTrace("OnPaint: %d\n", m_nFrame);
	CClientDC dc(this);

	CDC& memDC = dc;
//	memDC.CreateCompatibleDC(&dc);
	memDC.SetStretchBltMode(HALFTONE);

	CDC srcDC;
	srcDC.CreateCompatibleDC(&dc);
	srcDC.SelectObject(m_pBmp1);
	
	BITMAP bmp;
	m_pBmp1->GetBitmap(&bmp);

	CRect rect;
	GetClientRect(&rect);

	int nBmpHeight2 = (int)((bmp.bmHeight * m_nFrame) / m_nTotalFrames);
	int nRectHeight2 = (int)((rect.Height() * m_nFrame) / m_nTotalFrames);

	//1
	memDC.StretchBlt(rect.left, rect.top + nRectHeight2, rect.Width(), rect.Height() - nRectHeight2, 
		&srcDC, 0, nBmpHeight2, bmp.bmWidth, bmp.bmHeight - nBmpHeight2, SRCCOPY);

	//2
	srcDC.SelectObject(m_pBmp2);
	memDC.StretchBlt(rect.left, rect.top, rect.Width(), nRectHeight2, 
		&srcDC, 0, 0, bmp.bmWidth, nBmpHeight2, SRCCOPY);

	//3
	//dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	srcDC.DeleteDC();
}

void CPreviewCtrl::SetBitmaps(CBitmap *pBmp1, CBitmap *pBmp2)
{
	m_pBmp1 = pBmp1;
	m_pBmp2 = pBmp2;
}

void CPreviewCtrl::StartPreview()
{
	if(m_nTimer != 0)
	{
		return;
	}
	m_nFrame = 0;
	m_nTimer = SetTimer(1, 40, NULL);
}

void CPreviewCtrl::StopPreview()
{
	if(m_nTimer == 0)
	{
		return;
	}
	KillTimer(m_nTimer);
	m_nTimer = 0;
	m_nFrame = -1;
}

void CPreviewCtrl::OnTimer(UINT nIDEvent)
{
	ASSERT(m_nFrame >= 0);
	m_nFrame++;
	AfxTrace("OnTimer: %d\n", m_nFrame);

	InvalidateRect(NULL, TRUE);
}