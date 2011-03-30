#ifndef _GTB_4_WIN_H_
#define _GTB_4_WIN_H_


class CMemoryDC
{
public:
	CMemoryDC(CDC *pDC, LPCRECT lpcRect, BOOL bAutoFlush = FALSE);
	virtual ~CMemoryDC();
	CDC* GetDC()
	{
		return &m_memoryDC;
	}
	operator CDC* ()
	{
		return &m_memoryDC;
	}
	CRect* GetRect()
	{
		return &m_rect;
	}
	BOOL IsOK()
	{
		return m_memoryDC.m_hDC != NULL && m_canvas.m_hObject != NULL && m_pOldBit != NULL;
	}
	BOOL Flush();
	BOOL Flush(int x, int y);
protected:
	CDC m_memoryDC;
private:
	CBitmap m_canvas;
	CBitmap *m_pOldBit;
	CDC *m_pRealDC;
	CRect m_rect;
	BOOL m_bAutoFlush;
};

int CALLBACK CheckFontExistProc(ENUMLOGFONTEX *lpelfe,    // logical-font data
							NEWTEXTMETRICEX *lpntme,  // physical-font data
							DWORD FontType,           // type of font
							LPARAM lParam             // application-defined data
);
BOOL IsFontExist(LPTSTR lpFaceName, HDC hdc);


// int CALLBACK CheckFontExist(LPENUMLOGFONT lpelf,
// 							 LPNEWTEXTMETRIC lpntm,DWORD nFontType,long lparam);

BOOL PaintBmp(CDC *pDC, CRect* pRect, CBitmap *pCBitmap, int nPositionType, COLORREF fillColor = RGB(128, 128, 128));
BOOL TransparentBlt2(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, 
					HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, 
					UINT crTransparent);
BOOL TransparentBlt3(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, 
					 HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, 
					UINT crTransparent);

/************************************************************************/
/* Same with TransparentBlt2/TransparentBlt3, but with the CDC interface*/
/************************************************************************/
BOOL DrawTransparentBitmap(CDC *pDC, CBitmap *pCBitmap, CRect &rect, COLORREF crTransparent);
/************************************************************************/
/* Use double buffer memory DC                                          */
/************************************************************************/
BOOL DrawTransparentBitmap2(CDC *pDC, CBitmap *pCBitmap, CRect &rect, COLORREF crTransparent);
#endif