#include "StdAfx.h"
#include "CodePageConverter.h"


int MultiByteToMultByte(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, UINT nCPSrc, 
						LPSTR lpMultiByteStrDst, int cbMultiByteDst, UINT nCPDst, DWORD dwFlags)
{
	LPWSTR lpWideCharStr = new WCHAR[cbMultiByteDst];

	int nResult = 0;
	do 
	{
		nResult = ::MultiByteToWideChar(nCPSrc, dwFlags, lpMultiByteStrSrc, cbMultiByteSrc, lpWideCharStr, cbMultiByteDst);
		if(nResult == 0)
		{
			AfxTrace("MultiByteToWideChar Failed. Error code = %d\n", GetLastError());
			break;
		}

		BOOL bUsedDefaultChar = TRUE;
		nResult = ::WideCharToMultiByte(nCPDst, 0, lpWideCharStr, nResult, lpMultiByteStrDst, cbMultiByteDst, "?", &bUsedDefaultChar);
		if(nResult == 0)
		{
			AfxTrace("WideCharToMultiByte Failed. Error code = %d\n", GetLastError());
			break;
		}
		if(bUsedDefaultChar)
		{
			AfxTrace("Used default char when converting [%s]\n", lpMultiByteStrSrc);
			break;
		}

	} while (FALSE);

	//Clean up
	delete lpWideCharStr;
	return nResult;
}

int Big5ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	int nResult = ::MultiByteToMultByte(lpMultiByteStrSrc, cbMultiByteSrc, 950, lpMultiByteStrDst, cbMultiByteDst, 936);
	return nResult;
}

int GBKToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	int nResult = ::MultiByteToMultByte(lpMultiByteStrSrc, cbMultiByteSrc, 936, lpMultiByteStrDst, cbMultiByteDst, 950);
	return nResult;
}

int GB2312ToGBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	DWORD dwLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nResult = 0;
	nResult = ::LCMapString(dwLCID, LCMAP_TRADITIONAL_CHINESE, lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
	if(nResult == 0)
	{
		AfxTrace("LCMapString Failed. Error code = %d\n", GetLastError());
	}
	return nResult;
}
int GBKToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	DWORD dwLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
	int nResult = 0;
	nResult = ::LCMapString(dwLCID, LCMAP_SIMPLIFIED_CHINESE, lpMultiByteStrSrc, cbMultiByteSrc, lpMultiByteStrDst, cbMultiByteDst);
	if(nResult == 0)
	{
		AfxTrace("LCMapString Failed. Error code = %d\n", GetLastError());
	}
	return nResult;
}

int Big5ToGB2312(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	LPSTR lpTempStr = new CHAR[cbMultiByteDst];
	ZeroMemory(lpTempStr, cbMultiByteDst);

	Big5ToGBK(lpMultiByteStrSrc, cbMultiByteSrc, lpTempStr, cbMultiByteDst);
	int nResult = GBKToGB2312(lpTempStr, cbMultiByteDst, lpMultiByteStrDst, cbMultiByteDst);

	delete lpTempStr;
	return nResult;
}
int GB2312ToBig5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	LPSTR lpTempStr = new CHAR[cbMultiByteDst];
	ZeroMemory(lpTempStr, cbMultiByteDst);
	
	GB2312ToGBK(lpMultiByteStrSrc, cbMultiByteSrc, lpTempStr, cbMultiByteDst);
	int nResult = GBKToBig5(lpTempStr, cbMultiByteDst, lpMultiByteStrDst, cbMultiByteDst);
	
	delete lpTempStr;
	return nResult;
}

int Convert(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst, int nConvertWay)
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

int Convert(const CString& szSrc, CString& szDst, int nConvertWay)
{
	int nSrcLen = szSrc.GetLength();
	if(nSrcLen <= 0)
	{
		szDst.Empty();
		return 0;
	}

	int nResult = 0;
	nResult = Convert(szSrc, nSrcLen, szDst.GetBuffer(nSrcLen), nSrcLen, nConvertWay);
	ASSERT(nResult <= nSrcLen);
	szDst.ReleaseBuffer();
	return nResult;
}

int ConvertFile(LPCTSTR lpFileNameIn, LPCTSTR lpFileNameOut, int nConvertWay)
{
	CStdioFile fileIn;
	if(!fileIn.Open(lpFileNameIn, CFile::modeRead))
	{
		AfxTrace(_T("Failed to Open file %s\n"), lpFileNameIn);
		return 0;
	}

	CStdioFile fileOut;
	if(!fileOut.Open(lpFileNameOut, CFile::modeReadWrite | CFile::modeCreate))
	{
		fileIn.Close();
		AfxTrace(_T("Failed to Open file %s\n"), lpFileNameOut);
		return 0;
	}
	
	int nResult = 0;

	CString bufRead;	
	while(fileIn.ReadString(bufRead))
	{	
		CString bufWrite;
 		Convert(bufRead, bufWrite, nConvertWay);
		bufWrite += "\n";
		fileOut.WriteString(bufWrite);
	}
	fileIn.Close();
	fileOut.Close();
	return nResult;
}
