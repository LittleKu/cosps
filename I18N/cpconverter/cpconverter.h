#ifndef _CP_CONVERTER_DLL_
#define _CP_CONVERTER_DLL_

#include <afx.h>
// #ifndef _INC_WINDOWS
// #include <windows.h>
// #endif

#define CW_BIG5_TO_GBK						0
#define CW_GBK_TO_BIG5						1
#define CW_GB2312_TO_GBK					2
#define CW_GBK_TO_GB2312					3
#define CW_BIG5_TO_GB2312					4
#define CW_GB2312_TO_BIG5					5

#ifdef CPCONVERTER_EXPORTS
#define CP_CONVERTER_API __declspec(dllexport)
#else
#define CP_CONVERTER_API __declspec(dllimport)
#endif

CP_CONVERTER_API int MultiByteToMultByte(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, UINT nCPSrc, 
										 LPSTR  lpMultiByteStrDst, int cbMultiByteDst, UINT nCPDst,
										 LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar, DWORD dwFlags);
CP_CONVERTER_API int MultiByteToMultByte(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, UINT nCPSrc, 
										 LPSTR lpMultiByteStrDst, int cbMultiByteDst, UINT nCPDst,
										 LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar, DWORD dwFlags = 0);

typedef CP_CONVERTER_API int (*ConvertWay)(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);

CP_CONVERTER_API int Big5ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
CP_CONVERTER_API int GBKToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
CP_CONVERTER_API int GB2312ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
CP_CONVERTER_API int GBKToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
CP_CONVERTER_API int Big5ToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
CP_CONVERTER_API int GB2312ToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);

CP_CONVERTER_API int Convert(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst, int nConvertWay);

#endif