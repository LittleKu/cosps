// CheckHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CheckHeaderCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckHeaderCtrl

CCheckHeaderCtrl::CCheckHeaderCtrl()
{
	m_cr3DHighLight   = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow      = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace        = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText       = ::GetSysColor(COLOR_BTNTEXT);
	m_rgbText         = m_crBtnText;

	m_nFormat         = DT_DEFAULT;
	m_bDividerLines   = TRUE;
	m_nSpace        = 6;
	m_sizeImage.cx    = 0;
	m_sizeImage.cy    = 0;
}

CCheckHeaderCtrl::~CCheckHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheckHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CCheckHeaderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckHeaderCtrl message handlers

BOOL CCheckHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CCheckHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);
	CMemDC memDC(&dc);
	DrawCtrl(&memDC);
}

void CCheckHeaderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CHeaderCtrl::OnLButtonDblClk(nFlags, point);
	SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CCheckHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
{
	CImageList* pImageList;
	pImageList = CImageList::FromHandle((HIMAGELIST)lParam);
	
	IMAGEINFO info;
	if (pImageList->GetImageInfo(0, &info))
	{
		m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
		m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
	}
	
	return Default();
}
void CCheckHeaderCtrl::CalcCheckBoxRect(const CRect& boundRect, CRect& checkboxRect, BOOL bCenter, int h)
{
	checkboxRect = boundRect;

	//left, right
	checkboxRect.right     = checkboxRect.left + h;
	//top, bottom
	checkboxRect.top       = boundRect.top + (boundRect.Height() - h) / 2;
	checkboxRect.bottom    = checkboxRect.top + h;

	// center the checkbox
	if(bCenter)
	{
		checkboxRect.left = boundRect.left + boundRect.Width() / 2 - checkboxRect.Height() / 2 - 1;
		checkboxRect.right = checkboxRect.left + checkboxRect.Height();
	}
}
void CCheckHeaderCtrl::DrawCheckBox(CDC* pDC, LPCRECT lpcRect, BOOL bDrawMark, COLORREF crBkg, COLORREF crBorder, COLORREF crMark)
{
	//Backup BkColor
	COLORREF oldBkColor = pDC->GetBkColor();

	// fill rect around check box with background color
	pDC->FillSolidRect(lpcRect, crBkg);
	
	// draw border
	CBrush brush(crBorder);
	pDC->FrameRect(lpcRect, &brush);
	
	if(bDrawMark)
	{		
		CPen markPen(PS_SOLID, 1, crMark);
		CPen *pOldPen = pDC->SelectObject(&markPen);
		
		// draw the check mark
		int x = lpcRect->left + 9;
		ASSERT(x < lpcRect->right);
		int y = lpcRect->top + 3;
		int i;
		for (i = 0; i < 4; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y + 3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y + 3);
			x--;
			y--;
		}
		
		pDC->SelectObject(pOldPen);
	}
	
	//Restore BkColor
	pDC->SetBkColor(oldBkColor);
}

void CCheckHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

    pDC->FillSolidRect(rectClip, m_cr3DFace);

	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen penShadow(PS_SOLID, 1, m_cr3DShadow);

	CPen* pOldPen = pDC->GetCurrentPen();
	CFont* pOldFont = pDC->SelectObject(GetFont());

	COLORREF oldBkColor = pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crBtnText);

	int iWidth = 0;
	CRect rectInter;
	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rectItem));
		
		//The item should be repaint
		rectClip.InflateRect(1, 0);  //Inflate 1 pixel for the separator
		if(rectInter.IntersectRect(&rectItem, &rectClip))
		{
			//Owner draw
			if (hditem.fmt & HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;

				DrawItem(&disItem);
			}
			//Draw by this
			else
			{
				rectItem.DeflateRect(m_nSpace, 0);
				DrawItem(pDC, rectItem, &hditem);
				rectItem.InflateRect(m_nSpace, 0);
			}

			//Draw separator
			if(m_bDividerLines /*&& i < iItems - 1*/)
			{
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.right-1, rectItem.top+2);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-2);
				
				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right, rectItem.top+2);
				pDC->LineTo(rectItem.right, rectItem.bottom-2);
			}
		}

		iWidth += hditem.cxy;
	}

	//Don't draw the right side
	pDC->FillSolidRect(rectClient.left, rectClient.top, rectClient.Width() - 1, 1, m_cr3DHighLight);
	pDC->FillSolidRect(rectClient.left, rectClient.top, 1, rectClient.Height() - 1, m_cr3DHighLight);
	pDC->FillSolidRect(rectClient.left, rectClient.bottom, rectClient.Width(), -1, m_cr3DShadow);
// 	if(iWidth > 0)
// 	{		
// 		rectClient.right = rectClient.left + iWidth;
// 		pDC->Draw3dRect(&rectClient, m_cr3DHighLight, m_cr3DShadow);
// 	}
	
	pDC->SetBkColor(oldBkColor);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

	penHighLight.DeleteObject();
	penShadow.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CCheckHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
	ASSERT(FALSE);  // must override for self draw header controls
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CCheckHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	ASSERT(lphdi->mask & HDI_FORMAT);

	int iWidth = 0;

	//Draw Image
	iWidth = DrawImage(pDC, rect, lphdi, FALSE);
	rect.left += (iWidth != 0) ? iWidth + m_nSpace : 0;
	
	//Draw Bitmap
	if(lphdi->fmt & HDF_BITMAP)
	{
		ASSERT(lphdi->mask & HDI_BITMAP);
		ASSERT(lphdi->hbm);
		
		iWidth = DrawBitmap(pDC, rect, lphdi, lphdi->hbm, TRUE);
		rect.right -= (iWidth != 0) ? iWidth + m_nSpace : 0;
	}
	//Draw Text
	DrawText(pDC, rect, lphdi);
}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CCheckHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{	
	//No need to draw image
	if(lphdi->iImage == CL_NONE_CHECK_BOX)
	{
		return 0;
	}
	int iWidth = 0;

	//
	CImageList* pImageList = GetImageList();
	if (pImageList && (rect.Width() > 0))
	{
		POINT point;
		
		point.y = rect.CenterPoint().y - ((m_sizeImage.cy+1) >> 1);
		
		if (bRight)
			point.x = rect.right - m_sizeImage.cx;
		else
			point.x = rect.left;
		
		SIZE size;
		size.cx = rect.Width() < m_sizeImage.cx ? rect.Width() : m_sizeImage.cx;
		size.cy = m_sizeImage.cy;
		
		// save image list background color
		COLORREF rgb = pImageList->GetBkColor();
		
		// set image list background color to same as header control
		pImageList->SetBkColor(pDC->GetBkColor());
		pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));
		pImageList->SetBkColor(rgb);
		
		iWidth = m_sizeImage.cx;
	}
	else if (rect.Width() > 0)
	{
		// no image list, just draw checkbox

		BOOL bCenter = (strlen(lphdi->pszText) == 0);

		CRect chkboxrect;
		CalcCheckBoxRect(rect, chkboxrect, bCenter);
		DrawCheckBox(pDC, &chkboxrect, lphdi->iImage == CL_CHECKED, ::GetSysColor(COLOR_WINDOW));	
		iWidth = chkboxrect.Width();
	}
	else
	{
		// width = 0, do nothing
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap
int CCheckHeaderCtrl::DrawBitmap(CDC* pDC, CRect rect, LPHDITEM lphdi, HBITMAP hb, BOOL bRight)
{
	if(hb == NULL)
	{
		return 0;
	}
	CBitmap* pBitmap = CBitmap::FromHandle(hb);
	if(pBitmap == NULL)
	{
		return 0;
	}
	BITMAP bmp;
	pBitmap->GetBitmap(&bmp);

	int iWidth = bmp.bmWidth;
	if(iWidth <= rect.Width() && rect.Width() > 0)
	{
		POINT point;
		
		point.y = rect.CenterPoint().y - (bmp.bmHeight >> 1);
		
		if (bRight)
			point.x = rect.right - iWidth;
		else
			point.x = rect.left;
		
		CDC dc;
		if(dc.CreateCompatibleDC(pDC)) 
		{
			CBitmap * pOldBitmap = dc.SelectObject(pBitmap);
			iWidth = pDC->BitBlt(point.x, point.y, bmp.bmWidth, bmp.bmHeight, &dc, 0, 0, SRCCOPY) ? iWidth : 0;
			dc.SelectObject(pOldBitmap);
		}	
	}
	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int CCheckHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	CSize size = pDC->GetTextExtent(lphdi->pszText);
	if(rect.Width() > 0 && (lphdi->mask & HDI_TEXT) && (lphdi->fmt & HDF_STRING))
	{
		pDC->SetTextColor(m_rgbText);

		UINT nFormat = m_nFormat;

		if (nFormat == DT_DEFAULT)
		{
			// default to whatever alignment the column is set to

			if (lphdi->fmt & LVCFMT_CENTER)
				nFormat = DT_CENTER;
			else if (lphdi->fmt & LVCFMT_RIGHT)
				nFormat = DT_RIGHT;
			else
				nFormat = DT_LEFT;
		}

		pDC->DrawText(lphdi->pszText, -1, rect, nFormat | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
	}

	size.cx = rect.Width() > size.cx ? size.cx : rect.Width();

	return size.cx > 0 ? size.cx : 0;
}
