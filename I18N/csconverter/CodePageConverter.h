#ifndef _CODE_PAGE_CONVERTER_H_
#define _CODE_PAGE_CONVERTER_H_

#pragma once

#define CW_BIG5_TO_GBK						0
#define CW_GBK_TO_BIG5						1
#define CW_GB2312_TO_GBK					2
#define CW_GBK_TO_GB2312					3
#define CW_BIG5_TO_GB2312					4
#define CW_GB2312_TO_BIG5					5

int MultiByteToMultByte(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, UINT nCPSrc, 
						LPSTR lpMultiByteStrDst, int cbMultiByteDst, UINT nCPDst, DWORD dwFlags = 0);

typedef int (*ConvertWay)(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);

int Big5ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
int GBKToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
int GB2312ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
int GBKToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
int Big5ToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
int GB2312ToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);

int Convert(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst, int nConvertWay);
int Convert(const CString& szSrc, CString& szDst, int nConvertWay);
int ConvertFile(LPCTSTR lpFileNameIn, LPCTSTR lpFileNameOut, int nConvertWay);

#endif