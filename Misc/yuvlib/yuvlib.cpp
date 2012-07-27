#include "yuvlib.h"
#include <string.h>

void CYuvUtils::YUV444toRGB888(uint8_t y, uint8_t u, uint8_t v, uint8_t* rgbBuffer)
{
	int r, g, b;
	
	int v_delta = v - 128;
	int u_delta = u - 128;
	
	r = (int)( y + 1.403f * v_delta);
	g = (int)( y - 0.344f * u_delta - 0.714f * v_delta);
	b = (int)( y + 1.770f * u_delta);
	
	rgbBuffer[0] = min(255, max(0, r));
	rgbBuffer[1] = min(255, max(0, g));
	rgbBuffer[2] = min(255, max(0, b));

}

int CYuvUtils::RGB888toRGB24(uint8_t *rgb888, int rgbSize, int width, int height)
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
	
	Flip(rgb888, rgbSize, width, height);
	
	return 0;
}

void CYuvUtils::Flip(uint8_t* rgb_buf, int rgbSize, int width, int height)
{
	int rowBytes = width * 3;
	
	uint8_t* lpRowData = new uint8_t[rowBytes];
	
	uint8_t *lpSrc, *lpDst;
	
	//first row
	lpSrc = rgb_buf;
	//last row
	lpDst = rgb_buf + rowBytes * (height - 1);
	
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

int CYuvUtils::YV12_to_RGB24(uint8_t* y_buf, uint8_t* u_buf, uint8_t* v_buf, uint8_t* rgb_buf, int rgbSize, int width, int height)
{
	if(rgbSize < width * height * 3)
	{
		//not enough memeory
		return 1;
	}
	uint8_t y, u, v;
	
	int posX, posY, posRGB;
	for(posY = 0; posY < height; posY++)
	{
		for(posX = 0; posX < width; posX++)
		{
			y = y_buf[posY * width + posX];
			u = u_buf[(posY / 2) * (width / 2) + (posX / 2)];
			v = v_buf[(posY / 2) * (width / 2) + (posX / 2)];
			
			posRGB = 3 * (posY * width + posX);
			YUV444toRGB888(y, u, v, rgb_buf + posRGB);
		}
	}
	
	
	RGB888toRGB24(rgb_buf, rgbSize, width, height);

	return 0;
}