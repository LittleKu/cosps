// YUVLib.cpp: implementation of the CYUVLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YUVLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYUVLib::CYUVLib()
{

}

CYUVLib::~CYUVLib()
{

}

void CYUVLib::YV12_to_RGB24(unsigned char *yBuffer, unsigned char *uBuffer, unsigned char *vBuffer, 
							unsigned char *rgbBuffer, int width, int height)
{
	unsigned char y, u, v;

	int posX, posY, posRGB;
	for(posX = 0; posX < width; posX++)
	{
		for(posY = 0; posY < height; posY++)
		{
			y = yBuffer[posY * width + posX];
			u = uBuffer[(posY / 2) * (width / 2) + (posX / 2)];
			v = vBuffer[(posY / 2) * (width / 2) + (posX / 2)];

			posRGB = 3 * (posY * width + posX);
			YUV444toRGB888(y, u, v, rgbBuffer + posRGB);
		}
	}

	RGB888toRGB24(rgbBuffer, width, height);
}

/*

  http://en.wikipedia.org/wiki/YUV
  
	From YUV to RGB:
	R =     Y + 1.13983 V
	G =     Y - 0.39466 U - 0.58060 V
	B =     Y + 2.03211 U
	
*/
void CYUVLib::YUV444toRGB888(unsigned char y, unsigned char u, unsigned char v, unsigned char *rgbBuffer)
{
	int r, g, b;

	int vv = v - 128;
	int uu = u - 128;

	r = (int)(1.13983 * vv);
	g = (int)(-(0.39466 * uu) - (0.58060 * vv));
	b = (int)(2.03211 * uu);

	rgbBuffer[0] = clamp(y + r);
	rgbBuffer[1] = clamp(y + g);
	rgbBuffer[2] = clamp(y + b);
}

int CYUVLib::clamp(int x, int min, int max)
{
	if(x < min)
	{
		x = min;
	}
	if(x > max)
	{
		x = max;
	}
	return x;
}

/*
Convert RGB888 sequences into a Bottom-Up 24-bits little-endian bitmap.
The output will be the following sequences 
BGRBGRBGR...
*/
int CYUVLib::RGB888toRGB24(unsigned char *rgb888, int width, int height)
{
	if((width * 24) % 32 != 0)
	{
		//NOT DWORD aligned
		return 1;
	}
	//convert RGB to BGR
	int size = width * height * 3;
	unsigned char temp = 0;

	for(int i = 0; i < size; i += 3)
	{
		temp = rgb888[i];
		rgb888[i] = rgb888[i + 2];
		rgb888[i + 2] = temp;
	}

	Flip(rgb888, width, height);

	return 0;
}

void CYUVLib::Flip(unsigned char *lpBuf, int width, int height)
{
	int rowBytes = width * 3;

	unsigned char* lpRowData = new unsigned char[rowBytes];

	/*
	int nRowUp = 0, nRowDown = height - 1;
	while(nRowUp < nRowDown)
	{
		memcpy(lpRowData, lpBuf + nRowUp * rowBytes, rowBytes);
		memcpy(lpBuf + nRowUp * rowBytes, lpBuf + nRowDown * rowBytes, rowBytes);
		memcpy(lpBuf + nRowDown * rowBytes, lpRowData, rowBytes);
		nRowUp++;
		nRowDown--;
	}
	*/

	unsigned char *lpSrc, *lpDst;

	//first row
	lpSrc = lpBuf;
	//last row
	lpDst = lpBuf + rowBytes * (height - 1);

	while(lpSrc < lpDst)
	{
		memcpy(lpRowData, lpSrc, rowBytes);
		memcpy(lpSrc, lpDst, rowBytes);
		memcpy(lpDst, lpRowData, rowBytes);
		lpSrc += rowBytes;
		lpDst -= rowBytes;
	}

	delete lpRowData;
}
