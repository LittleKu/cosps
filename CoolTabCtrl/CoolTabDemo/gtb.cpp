#include "StdAfx.h"
#include "gtb.h"
#include "gtbutil.h"

CMemoryDC::CMemoryDC(CDC *pDC, LPCRECT lpcRect, BOOL bAutoFlush)
 : m_pOldBit(NULL), m_pRealDC(pDC), m_rect(lpcRect), m_bAutoFlush(bAutoFlush)
{	
	//0.1 Set up memory DC
	m_memoryDC.CreateCompatibleDC(pDC);

	//0.2 Create canvas. Set up an CBitmap
	m_canvas.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());

	//1. Make the canvas to be selected by our memory DC
	m_pOldBit = m_memoryDC.SelectObject(&m_canvas);

	//2. Optional? When you draw bitmap with ROP SRCINVERT/SRCPAINT/..., this copy operation is needed.
	//For common use, don't copy for speed.
//	m_memoryDC.BitBlt(0, 0, m_rect.Width(), m_rect.Height(), pDC, m_rect.left, m_rect.top, SRCCOPY);
}

CMemoryDC::~CMemoryDC()
{
	if(m_bAutoFlush)
	{
		Flush();
	}
	//Restore old bitmap
	m_memoryDC.SelectObject(m_pOldBit);

	//Delete canvas
	m_canvas.DeleteObject();

	//Delete DC
	m_memoryDC.DeleteDC();
}

BOOL CMemoryDC::Flush()
{
	if(!IsOK())
	{
		return FALSE;
	}
	//Copy the content of memory DC to the real DC
	return m_pRealDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &m_memoryDC, 0, 0, SRCCOPY);
}

BOOL CMemoryDC::Flush(int x, int y)
{
	if(!IsOK())
	{
		return FALSE;
	}
	//Copy the content of memory DC to the real DC
	return m_pRealDC->BitBlt(x, y, m_rect.Width(), m_rect.Height(), &m_memoryDC, 0, 0, SRCCOPY);
}

int CALLBACK CheckFontExistProc(ENUMLOGFONTEX *lpelfe,    // logical-font data
								NEWTEXTMETRICEX *lpntme,  // physical-font data
								DWORD FontType,           // type of font
								LPARAM lParam             // application-defined data
								)
{
	BOOL* bExist = (BOOL*)lParam;
	*bExist = TRUE;
	//Stop enum
	return 0;
}

BOOL IsFontExist(LPTSTR lpFaceName, HDC hdc)
{
	LOGFONT lf;
	lf.lfCharSet = DEFAULT_CHARSET; // Initialize the LOGFONT structure
	_tcscpy(lf.lfFaceName, lpFaceName);
	lf.lfPitchAndFamily = 0;

	BOOL bExist = FALSE;
	::EnumFontFamiliesEx(hdc, &lf, (FONTENUMPROC)CheckFontExistProc, (LPARAM)(&bExist), 0);

	return bExist;
}

CSize GetBitmapSize(CBitmap* pBitmap)
{
	BITMAP bmp;
	pBitmap->GetBitmap(&bmp);

	CSize sz;
	sz.cx = bmp.bmWidth;
	sz.cy = bmp.bmHeight;

	return sz;
}

BOOL PaintBmp(CDC *pDC, CRect* pRect, CBitmap *pCBitmap, int nPositionType, COLORREF fillColor)
{
	//Bitmap DC
	BITMAP bmp;
	pCBitmap->GetBitmap(&bmp);
	
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(pCBitmap);	
	
	//Paint bmp in the memDC
	switch(nPositionType)
	{
	case STRETCH:
		{			
			pDC->StretchBlt(pRect->left, pRect->top, pRect->Width(), pRect->Height(), 
				&dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		}
		break;
	case TILE:
		{
			for(int w = pRect->left; w < pRect->Width(); w += bmp.bmWidth)
			{
				for(int h = pRect->top; h < pRect->Height(); h += bmp.bmHeight)
				{
					pDC->BitBlt(w, h, bmp.bmWidth, bmp.bmHeight, &dc, 0, 0, SRCCOPY);
				}
			}
		}
		break;
	case COPY:
		{
			pDC->BitBlt(pRect->left, pRect->top, bmp.bmWidth, bmp.bmHeight, &dc, 0, 0, SRCCOPY);
		}
		break;
	case CENTER:
		{
			int diff_width = pRect->Width() - bmp.bmWidth;
			int diff_height = pRect->Height() - bmp.bmHeight;
			
			pDC->BitBlt(pRect->left + (diff_width >> 1), pRect->top + (diff_height >> 1), bmp.bmWidth, bmp.bmHeight, 
				&dc, 0, 0, SRCCOPY);
			
			CRect blankRect;
			if(diff_width > 0)
			{			
				//1. left
				blankRect.SetRect(0, 0, diff_width >> 1, pRect->bottom);
				blankRect.OffsetRect(pRect->TopLeft());
				pDC->FillSolidRect(&blankRect, fillColor);
				
				//2. right
				blankRect.SetRect(bmp.bmWidth + (diff_width >> 1), 0, pRect->right, pRect->bottom);
				blankRect.OffsetRect(pRect->TopLeft());
				pDC->FillSolidRect(&blankRect, fillColor);
			}
			
			if(diff_height > 0)
			{
				//1. top
				blankRect.SetRect(0, 0, pRect->right, (diff_height >> 1));
				blankRect.OffsetRect(pRect->TopLeft());
				pDC->FillSolidRect(&blankRect, fillColor);
				
				//2. bottom
				blankRect.SetRect(0, bmp.bmHeight + (diff_height >> 1), pRect->right, pRect->bottom);
				blankRect.OffsetRect(pRect->TopLeft());
				pDC->FillSolidRect(&blankRect, fillColor);
			}
		}
		break;
	}

	//Release
	dc.DeleteDC();

	return TRUE;
}

BOOL TransparentBlt2(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, 
					 HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, 
					 UINT crTransparent)
{
	//0. Construct
	//0.1 Image DC and Image bitmap
	HDC hImageDC = CreateCompatibleDC(hdcDest);
	HBITMAP hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	
	HBITMAP hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);

	//0.2 Mask DC and Mask bitmap
	HDC hMaskDC = CreateCompatibleDC(hdcDest);
	HBITMAP hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);	//Create monochrome bitmap
	HBITMAP hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	
	//0.3 Copy the bitmap in the Source DC to the temporary DC
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	
	//1. Create a mask bitmap(monochrome), transparent color area = white color(bit 1), other area = black color(bit 0)
	SetBkColor(hImageDC, crTransparent);
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
	
	//2. Set the original bitmap's color: transparent color area = black, other area = remains as original
	SetBkColor(hImageDC, RGB(0, 0, 0));
	SetTextColor(hImageDC, RGB(255, 255, 255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	//3. Copy the mask bitmap to the dest DC, the transparent area = transparent(the content of dest DC), other area = black
	SetBkColor(hdcDest,RGB(255, 255, 255));
	SetTextColor(hdcDest,RGB(0, 0, 0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	//4. Raster OP = OR. Add the bitmap of [STEP 2] to the dest DC.
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
		
	//-1. Destruct
	//-0.1 Free Mask DC and Mask Bitmap
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteObject(hMaskBMP);
	DeleteDC(hMaskDC);
	//-0.2 Free Image DC and Image Bitmap
	SelectObject(hImageDC, hOldImageBMP);
	DeleteObject(hImageBMP);
	DeleteDC(hImageDC);

	return TRUE;
}
BOOL TransparentBlt3(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, 
					 HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, 
					UINT crTransparent)
{
	//0. Construct

	//0.1 Image DC and Image bitmap
	HDC hImageDC = CreateCompatibleDC(hdcDest);
	HBITMAP hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	
	HBITMAP hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);

	//0.2 Mask DC and Mask bitmap
	HDC hMaskDC = CreateCompatibleDC(hdcDest);
	HBITMAP hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);	//Create monochrome bitmap
	HBITMAP hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	
	//0.3 Copy the bitmap in the Source DC to the temporary DC
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	
	//1. Create a mask bitmap(monochrome), transparent color area = white color(bit 1), other area = black color(bit 0)
	SetBkColor(hImageDC, crTransparent);
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	/*
	Think about the following 3 steps like this:
	The 3 steps can be combined to a formula:  (((Dest xor A) and B) xor A)
	In the formula: Dest = hdcDest, A = hImageDC, B = hMaskDC.
    Because hMaskDC is a monochrome bitmap DC, that means it only contains 1 (white) and 0 (black),
	we can split hMaskDC into 2 parts, one is the 1 (white) area, the other one is 0 (black) area.
	The above formula will be equals to:

    When in the 1 area of B, let's say the B = 1;
    (((Dest xor A) and B) xor A) =  (((Dest xor A) and 1) xor A) = ((Dest xor A) xor A) = Dest.
	This means in the 1 area of B, the final bitmap data will completely be the data of Dest,
	in our case, it's the backgound bmp.

	When in the 0 area of B, let's say the B = 0;
	(((Dest xor A) and B) xor A) =  (((Dest xor A) and 0) xor A) = (0 xor A) = A.
	This means in the 0 area of B, the final bitmap data will completely be the data of A,
	in our case, it's the foreground bmp without transparent color area.

	*/
	//2. Raster oper = XOR
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCINVERT);
	//3. Raster oper = AND
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	//4. Raster oper = XOR
 	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCINVERT);
	
	//-1. Destruct
	//-0.1 Free Mask DC and Mask Bitmap
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteObject(hMaskBMP);
	DeleteDC(hMaskDC);
	//-0.2 Free Image DC and Image Bitmap
	SelectObject(hImageDC, hOldImageBMP);
	DeleteObject(hImageBMP);
	DeleteDC(hImageDC);
	return TRUE;
}

BOOL DrawTransparentBitmap(CDC *pDC, CBitmap *pCBitmap, CRect &rect, COLORREF crTransparent)
{
	//0. Construct

	//0.1 Image DC
	CDC imageDC;
	CBitmap imageBitmap, *pOldImageBitmap;

	imageDC.CreateCompatibleDC(pDC);
	imageBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldImageBitmap = imageDC.SelectObject(&imageBitmap);
	
	//0.2 Mask DC
	CDC maskDC;
	CBitmap maskBitmap, *pOldMaskBitmap;

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(rect.Width(), rect.Height(), 1, 1, NULL);
	pOldMaskBitmap = maskDC.SelectObject(&maskBitmap);

	//0.3 Copy the bitmap in the Source DC to the temporary DC
	{
		CDC tempDC;
		tempDC.CreateCompatibleDC(pDC);
		CBitmap* pOldTempBitmap = tempDC.SelectObject(pCBitmap);

		imageDC.BitBlt(0, 0, rect.Width(), rect.Height(), &tempDC, 0, 0, SRCCOPY);

		tempDC.SelectObject(pOldTempBitmap);
		tempDC.DeleteDC();
	}


	//1. Create a mask bitmap(monochrome), transparent color area = white color(bit 1), other area = black color(bit 0)
	imageDC.SetBkColor(crTransparent);
	maskDC.BitBlt(0, 0, rect.Width(), rect.Height(), &imageDC, 0, 0, SRCCOPY);
	
	//2. Set the original bitmap's color: transparent color area = black, other area = remains as original
	imageDC.SetBkColor(RGB(0, 0, 0));
	imageDC.SetTextColor(RGB(255, 255, 255));
	imageDC.BitBlt(0, 0, rect.Width(), rect.Height(), &maskDC, 0, 0, SRCAND);

	//3. Copy the mask bitmap to the dest DC, the transparent area = transparent(the content of dest DC), other area = black
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &maskDC, 0, 0, SRCAND);

	//4. Raster OP = OR. Add the bitmap of [STEP 2] to the dest DC.
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &imageDC, 0, 0, SRCPAINT);

	
	//-1. Destruct
	maskDC.SelectObject(pOldMaskBitmap);
	maskBitmap.DeleteObject();
	maskDC.DeleteDC();

	imageDC.SelectObject(pOldImageBitmap);
	imageDC.DeleteDC();
 
	return TRUE;
}

BOOL DrawTransparentBitmap2(CDC *pDC, CBitmap *pCBitmap, CRect &rect, COLORREF crTransparent)
{
	BOOL ret;
	CRect rect2;
	rect2.SetRect(0, 0, rect.Width(), rect.Height());

	CMemoryDC memDC(pDC, &rect2);	
	memDC.GetDC()->BitBlt(0, 0, rect2.Width(), rect2.Height(), pDC, rect.left, rect.top, SRCCOPY);

	ret = DrawTransparentBitmap(memDC.GetDC(), pCBitmap, rect2, crTransparent);
	if(!ret)
	{
		return FALSE;
	}

	ret = memDC.Flush(rect.left, rect.top);
	if(!ret)
	{
		return FALSE;
	}
	return TRUE;
}