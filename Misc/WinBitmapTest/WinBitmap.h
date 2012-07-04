#ifndef __WIN_BITMAP_H__
#define __WIN_BITMAP_H__

#include <windows.h>

class CWinBitmap
{
public:
	CWinBitmap();
	virtual ~CWinBitmap();

	//static member functions
public:
	static BOOL SaveBitmap(HDC hdc, DWORD dwWidth, DWORD dwHeight, const char* pFileName);

private:
	static BOOL SaveToFile(const char* pFileName, BITMAPFILEHEADER* pBFH, BITMAPINFOHEADER* pBIH, LPVOID lpDibData);
	HBITMAP MakeBitmap2(HDC hdc, DWORD dwWidth, DWORD dwHeight);
};

#endif