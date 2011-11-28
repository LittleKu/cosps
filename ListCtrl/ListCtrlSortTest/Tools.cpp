// Tools.cpp: implementation of the CTools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTools::CTools()
{

}

CTools::~CTools()
{

}

void CTools::DrawCheckBox(CDC* pDC, LPCRECT lpcRect, BOOL bDrawMark, COLORREF crBkg, COLORREF crBorder, COLORREF crMark)
{
	ASSERT( (lpcRect->right - lpcRect->left >= 13) && (lpcRect->bottom - lpcRect->top >= 13));
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

BOOL CTools::CalcCheckBoxRect(const CRect& boundRect, CRect& checkboxRect, BOOL bCenter, int h)
{
	if(boundRect.Width() < h || boundRect.Height() < h)
	{
		return FALSE;
	}

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

	return TRUE;
}
