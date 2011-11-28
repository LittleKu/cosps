// SHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SHeaderCtrl.h"
#include "MemDC.h"
#include "Tools.h"
#include "gtb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl

CSHeaderCtrl::CSHeaderCtrl()
{
	m_cr3DHighLight   = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow      = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace        = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText       = ::GetSysColor(COLOR_BTNTEXT);
	m_rgbText         = m_crBtnText;

	m_crBorder		  = RGB(127, 157, 185);
//	m_crBorder		  = RGB(0, 59, 153);
	
	m_nFormat         = DT_DEFAULT;
	m_bDividerLines   = TRUE;
	m_nSpace          = 6;
	m_sizeImage.cx    = 0;
	m_sizeImage.cy    = 0;
	
	m_nHeight         = -1; //-1 means use the default height
}

CSHeaderCtrl::~CSHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSHeaderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl message handlers

BOOL CSHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CSHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);
	CMemDC memDC(&dc);
	DrawCtrl(&memDC);
}

void CSHeaderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CHeaderCtrl::OnLButtonDblClk(nFlags, point);
	SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CSHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
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

/*
This code is from: http://www.codeproject.com/KB/list/Emery-Emerald.aspx
*/
LRESULT CSHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
{
	LPHDLAYOUT pHL = reinterpret_cast<LPHDLAYOUT>(lParam);
	
	//*** Sends HDM_LAYOUT message to the base class
	if(m_nHeight <= 0)
	{
		return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
	}
	
	//*** The table list rectangle
	RECT *pRect = pHL->prc;
	
	//*** The table header rectangle
	WINDOWPOS *pWPos = pHL->pwpos;
	
	//*** Here's equivalent code for the code which follows after
	/*
	pWPos->hwndInsertAfter = NULL;
	
	//*** Moves the table header to the righ
	pWPos->x = pRect->left;
	pWPos->y = pRect->top;
	pWPos->cx = pRect->right - pRect->left;
	  
	//*** New table header height
	pWPos->cy = m_nHeight;
		
	pWPos->flags = SWP_NOACTIVATE|SWP_NOZORDER;
		  
	//*** Decreases the table list height on the table header height
	pRect->top += m_nHeight;
			
	return TRUE;
	*/
	
	// Method 2
	int nRet = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
	pWPos->cy = m_nHeight;
	pRect->top = m_nHeight;
	return nRet;
}

void CSHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;
	
	CRect rectClient, rectItem;
	GetClientRect(&rectClient);
	
//    pDC->FillSolidRect(rectClient, m_cr3DFace);
	COLORREF colorStart = RGB(255, 255, 255);
	COLORREF colorEnd	= RGB(199, 210, 226);
	gtb::DrawGradient(pDC, &rectClient, colorStart, colorEnd, FALSE);
	
	int iItems = GetItemCount();
	ASSERT(iItems >= 0);
	

	CPen* pOldPen = pDC->GetCurrentPen();
	CFont* pOldFont = pDC->SelectObject(GetFont());
	
	COLORREF oldBkColor = pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crBtnText);

	CPen penBorder(PS_SOLID, 1, m_crBorder);
	
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
			if(m_bDividerLines)
			{				
				pDC->SelectObject(&penBorder);
				pDC->MoveTo(rectItem.right, rectItem.top);
				pDC->LineTo(rectItem.right, rectItem.bottom);
			}
		}
		
		iWidth += hditem.cxy;
	}
	
	//Draw Border: bottom only
	pDC->FillSolidRect(rectClient.left, rectClient.bottom, rectClient.Width(), -1, m_crBorder);	
	
	pDC->SetBkColor(oldBkColor);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	
	penBorder.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CSHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
	ASSERT(FALSE);  // must override for self draw header controls
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CSHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	ASSERT( (lphdi->mask & HDI_FORMAT) && (lphdi->mask & HDI_IMAGE) );

	//From left to right: check box, text, sort image
	int iWidth = 0;

	//1. Draw CheckBox
	iWidth = DrawCheckBox(pDC, rect, lphdi);
	rect.left += (iWidth != 0) ? iWidth + m_nSpace : 0;
	//int r = rect.right;

	int nSortImage = ((lphdi->iImage >> 2) & 3);
	CImageList* pImageList = GetImageList();
	BOOL bHasSortImage = (nSortImage != SHC_NONE_SORT && pImageList != NULL);
	//Reserve the space for sort image
	if(bHasSortImage)
	{
		rect.right -= (m_sizeImage.cx + m_nSpace);
	}

	//2. Draw Text
	iWidth = DrawText(pDC, rect, lphdi);
	rect.left += (iWidth != 0) ? iWidth + m_nSpace : 0;

	if(bHasSortImage)
	{
		rect.right += (m_sizeImage.cx + m_nSpace);

		//3. Draw Sort image
		iWidth = DrawSortImage(pDC, rect, lphdi);
		rect.left += (iWidth != 0) ? iWidth + m_nSpace : 0;
	}
}

int  CSHeaderCtrl::DrawCheckBox(CDC* pDC, const CRect& rect, LPHDITEM lphdi)
{
	int iWidth = 0;
	/*
	if (rect.Width() > 0)
	{
		int nCheckBox = (lphdi->iImage & 3);
		if(nCheckBox != SHC_NONE_CHECK_BOX)
		{
			//draw checkbox
			BOOL bCenter = (lstrlen(lphdi->pszText) == 0);
			
			CRect chkboxrect;
			if(CTools::CalcCheckBoxRect(rect, chkboxrect, bCenter))
			{
				CTools::DrawCheckBox(pDC, &chkboxrect, lphdi->iImage == SHC_CHECKED, ::GetSysColor(COLOR_WINDOW));	
				iWidth = chkboxrect.right - rect.left;
			}
		}
		
	}
	*/
	CRect chkboxRect;
	if(GetCheckBoxRect(rect, lphdi, chkboxRect))
	{
		CTools::DrawCheckBox(pDC, &chkboxRect, lphdi->iImage == SHC_CHECKED, ::GetSysColor(COLOR_WINDOW));	
		iWidth = chkboxRect.right - rect.left;
	}
	
	return iWidth;
}

int  CSHeaderCtrl::DrawSortImage(CDC* pDC, const CRect& rect, LPHDITEM lphdi)
{
	int iWidth = 0;
	
	if (rect.Width() > 0)
	{
		int nSortImage = ((lphdi->iImage >> 2) & 3);
		CImageList* pImageList = GetImageList();
		if(nSortImage != SHC_NONE_SORT && pImageList != NULL)
		{
			POINT point;
			point.y = rect.CenterPoint().y - ((m_sizeImage.cy+1) >> 1);
			point.x = rect.left;
			
			SIZE size;
			size.cx = rect.Width() < m_sizeImage.cx ? rect.Width() : m_sizeImage.cx;
			size.cy = m_sizeImage.cy;
			
			// save image list background color
			COLORREF rgb = pImageList->GetBkColor();
			
			// set image list background color to same as header control
			pImageList->DrawIndirect(pDC, nSortImage, point, size, CPoint(0, 0));
			
			iWidth = size.cx;
		}
		
	}
	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int CSHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	CSize size = pDC->GetTextExtent(lphdi->pszText);
	if(rect.Width() > 0 && (lphdi->mask & HDI_TEXT) && (lphdi->fmt & HDF_STRING))
	{
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF crOldTextColor = pDC->SetTextColor(m_rgbText);
		
		UINT nFormat = m_nFormat;
		if (nFormat == DT_DEFAULT)
		{
			// default to whatever alignment the column is set to
			if (lphdi->fmt & HDF_CENTER)
				nFormat = DT_CENTER;
			else if (lphdi->fmt & HDF_RIGHT)
				nFormat = DT_RIGHT;
			else
				nFormat = DT_LEFT;
		}
		
		pDC->DrawText(lphdi->pszText, -1, rect, nFormat | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkMode(nOldBkMode);
	}
	
	size.cx = rect.Width() > (size.cx) ? size.cx : rect.Width();
	
	return size.cx > 0 ? size.cx : 0;
}

void CSHeaderCtrl::SetHeight(int nHeight)
{ 
	m_nHeight = nHeight;
	Invalidate();
	UpdateWindow();
}
int  CSHeaderCtrl::GetHeight() 
{ 
	return m_nHeight; 
}

BOOL CSHeaderCtrl::GetCheckBoxRect(int nSubItem, CRect& chkboxRect)
{
	TCHAR szText[FLATHEADER_TEXT_MAX];
	
	HDITEM hditem;
	hditem.mask = HDI_FORMAT|HDI_TEXT|HDI_IMAGE;
	hditem.pszText = szText;
	hditem.cchTextMax = sizeof(szText);
	VERIFY(GetItem(nSubItem, &hditem));

	CRect rectItem;
	VERIFY(GetItemRect(nSubItem, rectItem));
	rectItem.DeflateRect(m_nSpace, 0);

	return GetCheckBoxRect(rectItem, &hditem, chkboxRect);
}

BOOL CSHeaderCtrl::GetCheckBoxRect(const CRect& rectItem, LPHDITEM lphdi, CRect& chkboxRect)
{
	int nCheckBox = (lphdi->iImage & 3);
	//None CheckBox
	if(nCheckBox == SHC_NONE_CHECK_BOX)
	{
		return FALSE;
	}
	
	if(rectItem.Width() <= 0)
	{
		return FALSE;
	}
	
	BOOL bResult = CTools::CalcCheckBoxRect(rectItem, chkboxRect, (lstrlen(lphdi->pszText) == 0));
	
	return bResult;
}
