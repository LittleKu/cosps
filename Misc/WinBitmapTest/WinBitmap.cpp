#include "stdafx.h"
#include "WinBitmap.h"

CWinBitmap::CWinBitmap()
{
}

CWinBitmap::~CWinBitmap()
{
}

BOOL CWinBitmap::SaveBitmap(HDC hdc, DWORD dwWidth, DWORD dwHeight, const char* pFileName)
{
	if(hdc == NULL)
	{
		return FALSE;
	}

	//Create memory DC
	HDC hMemDC = ::CreateCompatibleDC(hdc);

	//Current Bitmap
	HBITMAP hBmpCurr = CreateCompatibleBitmap(hdc, dwWidth, dwHeight);
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hMemDC, hBmpCurr);

	//Copy the original DC contents into the memory DC
	BitBlt(hMemDC, 0, 0, dwWidth, dwHeight, hdc, 0, 0, SRCCOPY);
	
	//Restore the DC contents to the original one
	SelectObject(hMemDC, hBmpOld);

	// You can get access to the DC contents in device-specific format by calling GetObject
	// This structure holds information about bitmap
	BITMAP bitmap;
	// Retrieve bitmap information
	GetObject(hBmpCurr, sizeof(bitmap), &bitmap);

	
	//Start the Save logic
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
	HBITMAP hbmDIB = CreateDIBSection(hMemDC, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &lpDibData, 0, 0);

	//Get the DIB Bitmap information by the below call
//	DIBSECTION dibSection;
//	GetObject(hbmDIB, sizeof(dibSection), &dibSection);

	// Copy device image into our DIB
	GetDIBits(hMemDC, hBmpCurr, 0, dwHeight, lpDibData, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	ASSERT(bi.biSizeImage == dwSizeImage);

	//You can now use lpDibData to store contents of DC in a file, etc.

	//Init the bitmap file head
	BITMAPFILEHEADER bfh;
	bfh.bfType = 0x4d42;  //BM
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bfh.bfSize = bfh.bfOffBits + dwSizeImage;

	BOOL bOK = SaveToFile(pFileName, &bfh, &bi, lpDibData);

	// When you''re done, delete the DIB section
	//Delete the DIB Bitmap so the system allocated buffer "lpDibData" will be closed.
	DeleteObject(hbmDIB);

	//Delete the bitmap we created
	DeleteObject(hBmpCurr);

	//Delete the memory DC
	DeleteDC(hMemDC);
		
	return bOK;
}

BOOL CWinBitmap::SaveToFile(const char* pFileName, BITMAPFILEHEADER* pBFH, BITMAPINFOHEADER* pBIH, LPVOID lpDibData)
{
	HANDLE hFile = CreateFile(pFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

//Just for referrence
//http://www.gamedev.net/topic/101674-hdc---gt-hbitmap/
HBITMAP CWinBitmap::MakeBitmap2(HDC hdc, DWORD dwWidth, DWORD dwHeight)
{
	if(hdc == NULL)
	{
		return NULL;
	}
	
	//Create memory DC
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	
	// Need this temporary bitmap so that we can select it
	HBITMAP hbmTemp = CreateCompatibleBitmap(hMemDC, 1, 1);
	
	// SelectBitmap will return current bitmap in DC, which has the DC contents
	HBITMAP hbmCurrent = (HBITMAP)SelectObject(hMemDC, hbmTemp);
	
	// You can get access to the DC contents in device-specific format by calling GetObject
	// This structure holds information about bitmap
	BITMAP bitmap;
	// Retrieve bitmap information
	GetObject(hbmCurrent, sizeof(bitmap), &bitmap);
	
	// lpBitmapData will point to the bitmap data in DC-specific format
	LPVOID lpBitmapData = bitmap.bmBits;
	
	// Alternatively, you can store the data in a device-independent bitmap,
	// whose internal format is known (array of RGBQUADs)
	// First, we need to fill BITMAPINFO struct
	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = bitmap.bmWidth;
	bih.biHeight = bitmap.bmHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	
	BITMAPINFO bi;
	bi.bmiHeader = bih;
	
	// This variable will point to the DIB image data
	LPVOID lpDibData = NULL;
	
	// Create device independent bitmap
	HBITMAP hbmDIB = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &lpDibData, 0, 0);
	// 	// Copy device image into our DIB
	// 	GetDIBits(hdc, hbmCurrent, 0, bitmap.bmHeight, lpDibData, &bi, DIB_RGB_COLORS);
	// 
	// 	// You can now use lpDibData to store contents of DC in a file, etc.
	// 	// When you''re done, delete the DIB section
	// 	DeleteObject(hbmDIB);
	
	// When you''re done with the DC image, select it back to the DC
	SelectObject(hdc, hbmCurrent);
	// Delete temporary bitmap now
	DeleteObject(hbmTemp);
	DeleteDC(hMemDC);
	
	return hbmDIB;
}