// YUVLib.h: interface for the CYUVLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YUVLIB_H__16752342_5993_4CEA_AE42_9F0E7B43A045__INCLUDED_)
#define AFX_YUVLIB_H__16752342_5993_4CEA_AE42_9F0E7B43A045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYUVLib  
{
public:
	static void Flip(unsigned char* lpBuf, int width, int height);
	static int RGB888toRGB24(unsigned char* rgb888, int width, int height);
	static int clamp(int x, int min = 0, int max = 255);
	static void YUV444toRGB888(unsigned char y, unsigned char u, unsigned char v, unsigned char* rgbBuffer);
	static void YV12_to_RGB24(unsigned char *src0,unsigned char *src1,unsigned char *src2,unsigned char *dst_ori,int width,int height);
	CYUVLib();
	virtual ~CYUVLib();

};

#endif // !defined(AFX_YUVLIB_H__16752342_5993_4CEA_AE42_9F0E7B43A045__INCLUDED_)
