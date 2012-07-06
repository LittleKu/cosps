// PreviewCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PreviewCtrl.h"
#include "memdc.h"

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

	m_nFrame = 0;
	m_nTotalFrames = 100;

	m_pBitmap1 = NULL;
	m_pBitmap2 = NULL;
}

CPreviewCtrl::~CPreviewCtrl()
{
	ReleaseBitmap(m_hBitmap1);
	ReleaseBitmap(m_hBitmap2);
}


BEGIN_MESSAGE_MAP(CPreviewCtrl, CStatic)
	//{{AFX_MSG_MAP(CPreviewCtrl)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewCtrl message handlers

void CPreviewCtrl::OnPaint() 
{
	ASSERT(m_pBitmap1 != NULL && m_pBitmap2 != NULL);

	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
	CRect clientRect;
	GetClientRect(&clientRect);
 	CMemDC memDC(&dc, &clientRect, TRUE);

	Draw(&memDC);	
}

/*
void CPreviewCtrl::Draw(CDC *pDC)
{
	pDC->SetStretchBltMode(HALFTONE);
	
	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	srcDC.SelectObject(m_pBitmap1);
	
	BITMAP bmp;
	m_pBitmap1->GetBitmap(&bmp);
	
	CRect rect;
	GetClientRect(&rect);
	
	int nBmpHeight2 = (int)((bmp.bmHeight * m_nFrame) / m_nTotalFrames);
	int nRectHeight2 = (int)((rect.Height() * m_nFrame) / m_nTotalFrames);
		
	//1
	pDC->StretchBlt(rect.left, rect.top + nRectHeight2, rect.Width(), rect.Height() - nRectHeight2, 
		&srcDC, 0, nBmpHeight2, bmp.bmWidth, bmp.bmHeight - nBmpHeight2, SRCCOPY);
	
	//2
	srcDC.SelectObject(m_pBitmap2);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), nRectHeight2, 
		&srcDC, 0, 0, bmp.bmWidth, nBmpHeight2, SRCCOPY);
	
	srcDC.DeleteDC();
}
*/

/*
void CPreviewCtrl::Draw(CDC *pDC)
{
	//Get the client area	
	CRect rect;
	GetClientRect(&rect);

	ASSERT(m_nFrame >= 0 && m_nFrame <= m_nTotalFrames);
	
	//The height of image 2
	int nHeight2 = (int)((rect.Height() * m_nFrame) / m_nTotalFrames);

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	
	//1. Draw the 1st image part
	::SelectObject(srcDC.GetSafeHdc(), m_hBitmap1);
	pDC->BitBlt(rect.left, rect.top + nHeight2, rect.Width(), rect.Height() - nHeight2, 
		&srcDC, 0, nHeight2, SRCCOPY);
	
	//2. Draw the 2nd image part
	::SelectObject(srcDC.GetSafeHdc(), m_hBitmap2);
	pDC->BitBlt(rect.left, rect.top, rect.Width(), nHeight2, &srcDC, 0, 0, SRCCOPY);
	
	//Release the DC resource
	srcDC.DeleteDC();
}
*/

void CPreviewCtrl::Draw(CDC *pDC)
{
	//Get the client area	
	CRect rect;
	GetClientRect(&rect);
	
	HBITMAP hbmp = GetFrameBitmap(pDC->GetSafeHdc(), m_nFrame, m_nTotalFrames, rect.Width(), rect.Height());

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(srcDC.GetSafeHdc(), hbmp);

	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &srcDC, 0, 0, SRCCOPY);

	::SelectObject(srcDC.GetSafeHdc(), hOldBmp);
	srcDC.DeleteDC();

	::DeleteObject(hbmp);
}

void CPreviewCtrl::OnTimer(UINT nIDEvent) 
{
	if(m_nFrame < 0)
	{
		m_nFrame = 0;
	}
	else
	{
		m_nFrame++;
	}
	if(m_nFrame > m_nTotalFrames)
	{
		m_nFrame = 0;
		StopPreview();
	}
	AfxTrace("OnTimer: m_nFrame=%d\n", m_nFrame);

	InvalidateRect(NULL, TRUE);
	
	CStatic::OnTimer(nIDEvent);
}

void CPreviewCtrl::SetBitmaps(CBitmap *pBitmap1, CBitmap *pBitmap2)
{
	m_pBitmap1 = pBitmap1;
	m_pBitmap2 = pBitmap2;

	CRect rect;
	GetClientRect(&rect);

	HDC hdc = ::GetDC(GetSafeHwnd());
	int nOldStretchMode = ::SetStretchBltMode(hdc, HALFTONE);

	m_hBitmap1 = GetStretchedBitmap(hdc, (HBITMAP)m_pBitmap1->GetSafeHandle(), rect.Width(), rect.Height());
	m_hBitmap2 = GetStretchedBitmap(hdc, (HBITMAP)m_pBitmap2->GetSafeHandle(), rect.Width(), rect.Height());

	::SetStretchBltMode(hdc, nOldStretchMode);
	::ReleaseDC(GetSafeHwnd(), hdc);
}

void CPreviewCtrl::StartPreview()
{
	if(m_nTimer != 0)
	{
		AfxTrace("StartPreview: m_nTimer=%d\n", m_nTimer);
		return;
	}
	m_nTimer = SetTimer(1, 40, 0);
}

void CPreviewCtrl::StopPreview()
{
	if(m_nTimer == 0)
	{
		AfxTrace("StopPreview: m_nTimer=0\n");
		return;
	}
	KillTimer(m_nTimer);

	m_nTimer = 0;
	m_nFrame = 0;
}


HBITMAP CPreviewCtrl::GetStretchedBitmap(HDC hdc, HBITMAP hSrcBmp, int nWidth, int nHeight)
{
	//Get the input bitmap information
	BITMAP bitmap;
	GetObject(hSrcBmp, sizeof(BITMAP), &bitmap);

	//Already same size with the target rectangle, just return
	if(bitmap.bmWidth == nWidth && bitmap.bmHeight == nHeight)
	{
		return hSrcBmp;
	}

	//Create a source DC
	HDC hSrcDC = ::CreateCompatibleDC(hdc);
	HBITMAP hSrcOldBmp = (HBITMAP)::SelectObject(hSrcDC, hSrcBmp);

	//Create a memory DC
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	HBITMAP hMemBmp = CreateCompatibleBitmap(hdc, nWidth, nHeight);
	HBITMAP hMemOldBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp);

	//blt
	int oldMode = ::SetStretchBltMode(hMemDC, HALFTONE);
	::StretchBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	::SetStretchBltMode(hMemDC, oldMode);

	//Restore Mem DC
	HBITMAP hBitmap = (HBITMAP)::SelectObject(hMemDC, hMemOldBmp);
	ASSERT(hBitmap == hMemBmp);
	::DeleteDC(hMemDC);

	//Restore SRC DC
	::SelectObject(hSrcDC, hSrcOldBmp);
	::DeleteDC(hSrcDC);
  
	return hBitmap;
}

HBITMAP CPreviewCtrl::GetFrameBitmap(HDC hdc, int nFrame, int nTotalFrames, int nWidth, int nHeight)
{
	ASSERT(nFrame >= 0 && nFrame <= nTotalFrames);
	
	//The height of image 2
	int nHeight2 = (int)((nHeight * nFrame) / nTotalFrames);
		
	//Create a memory DC
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	HBITMAP hMemBmp = CreateCompatibleBitmap(hdc, nWidth, nHeight);
	HBITMAP hMemOldBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp);

	//Create a source DC
	HDC hSrcDC = ::CreateCompatibleDC(hdc);
	
	//1. Draw the 1st image part
	HBITMAP hSrcOldBmp = (HBITMAP)::SelectObject(hSrcDC, m_hBitmap1);
	::BitBlt(hMemDC, 0, nHeight2, nWidth, nHeight - nHeight2, hSrcDC, 0, nHeight2, SRCCOPY);
	
	//2. Draw the 2nd image part
	::SelectObject(hSrcDC, m_hBitmap2);
	::BitBlt(hMemDC, 0, 0, nWidth, nHeight2, hSrcDC, 0, 0, SRCCOPY);
	
	//Release the SRC DC resource
	::SelectObject(hSrcDC, hSrcOldBmp);
	::DeleteDC(hSrcDC);

	//Release the Mem DC resource
	HBITMAP hBitmap = (HBITMAP)::SelectObject(hMemDC, hMemOldBmp);
	ASSERT(hBitmap == hMemBmp);
	::DeleteDC(hMemDC);

	return hBitmap;
}

BOOL CPreviewCtrl::ReleaseBitmap(HBITMAP hbmp)
{
	if(hbmp != NULL)
	{
		return ::DeleteObject(hbmp);
	}
	return TRUE;
}

BOOL CPreviewCtrl::SaveBitmap(HDC hdc, HBITMAP hbmp, DWORD dwWidth, DWORD dwHeight, LPCTSTR lpszFileName)
{
	BITMAPINFOHEADER   bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = dwWidth;    
    bi.biHeight = dwHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;
	
	DWORD dwSizeImage = ((dwWidth * bi.biBitCount + 31) / 32) * 4 * dwHeight;
	bi.biSizeImage = dwSizeImage;

	// This variable will point to the DIB image data
	LPVOID lpDibData = NULL;
	// Create device independent bitmap
	HBITMAP hbmDIB = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &lpDibData, 0, 0);

	// Copy device image into our DIB
	GetDIBits(hdc, hbmp, 0, dwHeight, lpDibData, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	ASSERT(bi.biSizeImage == dwSizeImage);


	//Init the bitmap file head
	BITMAPFILEHEADER bfh;
	bfh.bfType = 0x4d42;  //BM
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bfh.bfSize = bfh.bfOffBits + dwSizeImage;
	
	BOOL bOK = SaveToFile(&bfh, &bi, lpDibData, lpszFileName);
	
	// When you''re done, delete the DIB section
	//Delete the DIB Bitmap so the system allocated buffer "lpDibData" will be closed.
	DeleteObject(hbmDIB);

	return TRUE;
}

BOOL CPreviewCtrl::SaveToFile(BITMAPFILEHEADER* pBFH, BITMAPINFOHEADER* pBIH, LPVOID lpDibData, LPCTSTR lpszFileName)
{
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
	
	BOOL bOK = FALSE;
	DWORD dwWrite = 0;
	do 
	{
		if(!WriteFile(hFile, pBFH, sizeof(BITMAPFILEHEADER), &dwWrite, NULL))
		{
			break;
		}
		
		if(!WriteFile(hFile, pBIH, sizeof(BITMAPINFOHEADER), &dwWrite, NULL))
		{
			break;
		}
		if(!WriteFile(hFile, lpDibData, pBIH->biSizeImage, &dwWrite, NULL))
		{
			break;
		}
		bOK = TRUE;
	} while (FALSE);
	
	CloseHandle(hFile);
	
	if(!bOK)
	{
		DWORD dwErrorCode = GetLastError();
		AfxTrace("%08X\n", dwErrorCode);
	}
	
	return bOK;
}

void CPreviewCtrl::SaveFrames(int nTotalFrames)
{
	HDC hdc = ::GetDC(GetSafeHwnd());

	//Get the client area	
	CRect rect;
	GetClientRect(&rect);
	
	CString szFileName;
	int i;
	for(i = 0; i <= nTotalFrames; i++)
	{
		HBITMAP hbmp = GetFrameBitmap(hdc, i, nTotalFrames, rect.Width(), rect.Height());

		szFileName.Format("Frame%03d.bmp", i);

		SaveBitmap(hdc, hbmp, rect.Width(), rect.Height(), szFileName);

		DeleteObject(hbmp);
	}
	

	::ReleaseDC(GetSafeHwnd(), hdc);
}

BOOL CPreviewCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	
//	return CStatic::OnEraseBkgnd(pDC);
}
