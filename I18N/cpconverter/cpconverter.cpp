#include "cpconverter.h"


CP_CONVERTER_API int MultiByteToMultByte(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, UINT nCPSrc, 
						LPSTR lpMultiByteStrDst, int cbMultiByteDst, UINT nCPDst,
						LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar, DWORD dwFlags)
{
	LPWSTR lpWideCharStr = new WCHAR[cbMultiByteDst];
	int nResult = 0;
	nResult = ::MultiByteToWideChar(nCPSrc, dwFlags, lpMultiByteStrSrc, cbMultiByteSrc, lpWideCharStr, cbMultiByteDst);
	nResult = ::WideCharToMultiByte(nCPDst, 0, lpWideCharStr, nResult, lpMultiByteStrDst, cbMultiByteDst, lpDefaultChar, lpUsedDefaultChar);
	return nResult;
}

CP_CONVERTER_API int Big5ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	BOOL bUsedDefaultChar = TRUE;
	int nResult = 0;
	nResult = ::MultiByteToMultByte(lpMultiByteStrSrc, cbMultiByteSrc, 950, 
		lpMultiByteStrDst, cbMultiByteDst, 936, "?", &bUsedDefaultChar);
	return nResult;
}

CP_CONVERTER_API int GBKToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	BOOL bUsedDefaultChar = TRUE;
	int nResult = 0;
	nResult = ::MultiByteToMultByte(lpMultiByteStrSrc, cbMultiByteSrc, 936, 
		lpMultiByteStrDst, cbMultiByteDst, 950, "?", &bUsedDefaultChar);
	return nResult;
}

CP_CONVERTER_API int GB2312ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	DWORD dwLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nResult = 0;
	nResult = ::LCMapString(dwLCID, LCMAP_TRADITIONAL_CHINESE, lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
	return nResult;
}
CP_CONVERTER_API int GBKToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	DWORD dwLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
	int nResult = 0;
	nResult = ::LCMapString(dwLCID, LCMAP_SIMPLIFIED_CHINESE, lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
	return nResult;
}

CP_CONVERTER_API int Big5ToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	LPSTR lpTempStr = new CHAR[cbMultiByteDst];
	ZeroMemory(lpTempStr, cbMultiByteDst);

	Big5ToGBK(lpMultiByteStrSrc, cbMultiByteSrc, lpTempStr, cbMultiByteDst);
	int nResult = GBKToGB2312(lpTempStr, cbMultiByteDst, lpMultiByteStrDst, cbMultiByteDst);

	delete lpTempStr;
	return nResult;
}
CP_CONVERTER_API int GB2312ToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	LPSTR lpTempStr = new CHAR[cbMultiByteDst];
	ZeroMemory(lpTempStr, cbMultiByteDst);
	
	GB2312ToGBK(lpMultiByteStrSrc, cbMultiByteSrc, lpTempStr, cbMultiByteDst);
	int nResult = GBKToBig5(lpTempStr, cbMultiByteDst, lpMultiByteStrDst, cbMultiByteDst);
	
	delete lpTempStr;
	return nResult;
}

CP_CONVERTER_API int Convert(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst, int nConvertWay)
{
	int nResult = -1;
	switch(nConvertWay)
	{
	case CW_BIG5_TO_GBK:
		{
			nResult = Big5ToGBK(lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
		}
		break;
	case CW_GBK_TO_BIG5:
		{
			nResult = GBKToBig5(lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
		}
		break;
	case CW_GB2312_TO_GBK:
		{
			nResult = GB2312ToGBK(lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
		}
		break;
	case CW_GBK_TO_GB2312:
		{
			nResult = GBKToGB2312(lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
		}
		break;
	case CW_BIG5_TO_GB2312:
		{
			nResult = Big5ToGB2312(lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
		}
		break;
	case CW_GB2312_TO_BIG5:
		{
			nResult = GB2312ToBig5(lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
		}
		break;
	}
	return nResult;
}
