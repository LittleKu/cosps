#ifndef _GTB_4_WIN_H_
#define _GTB_4_WIN_H_

#include "gtbutil.h"

BEGIN_NAMESPACE(gtb)

enum EPaintBmpType
{
	PBT_STRETCH = 0,
	PBT_TILE,
	PBT_COPY,
	PBT_CENTER
};

AFX_EXT_API BOOL IsFontExist(LPTSTR lpFaceName, HDC hdc);

AFX_EXT_API BOOL PaintBmp(CDC *pDC, CRect* pRect, CBitmap *pCBitmap, int nPositionType, 
						  COLORREF fillColor = RGB(128, 128, 128));

AFX_EXT_API BOOL TransparentBlt(HDC hdcDst, int nXOriginDst, int nYOriginDst, int nWidthDst, int nHeightDst, 
								HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, 
								UINT crTransparent, int nReserved = -1);

//Same with TransparentBlt, but with the CDC interface
AFX_EXT_API BOOL DrawTransparentBitmap(CDC *pDC, CBitmap *pCBitmap, CRect &rect, COLORREF crTransparent, 
									   BOOL bDoubleBuffered = FALSE);

/**
* Fills a rectangle with a two color gradient.
* \param pDC the device context to draw on
* \param rect the rectangle to fill
* \param colorStart the starting color. This is either the color used on the left (if bHorz == TRUE) or top
* \param colorEnd the ending color. This is either the color used on the right (if bHorz == TRUE) or bottom
* \param bHorz if TRUE then the gradient is drawn from left to right, otherwise from top to bottom
* \param nSteps the steps the gradient shall have. The more the smoother the gradient will be but also slower.
*/
AFX_EXT_API void DrawGradient(CDC * pDC, const CRect& rect, COLORREF colorStart, COLORREF colorEnd, BOOL bHorz = TRUE, UINT nSteps = 64);

END_NAMESPACE()

#endif