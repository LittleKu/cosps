#include "CodePageConverter.h"

int WideCharToMultiByte1(
						 UINT CodePage,            // code page
						 DWORD dwFlags,            // performance and mapping flags
						 LPCWSTR lpWideCharStr,    // wide-character string
						 int cchWideChar,          // number of chars in string
						 LPSTR lpMultiByteStr,     // buffer for new string
						 int cbMultiByte,          // size of buffer
						 LPCSTR lpDefaultChar,     // default for unmappable chars
						 LPBOOL lpUsedDefaultChar  // set when default char used
						)
{
	return 0;
}

int MultiByteToMultByte(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, UINT nCPSrc, 
						LPSTR lpMultiByteStrDst, int cbMultiByteDst, UINT nCPDst,
						LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar, DWORD dwFlags)
{
//	ASSERT(cbMultiByteSrc > 0);
	int nStrLen = strlen(lpMultiByteStrSrc);
	LPWSTR lpWideCharStr = new WCHAR[nStrLen + 1];

	int nReturn = MultiByteToWideChar(nCPSrc, dwFlags, lpMultiByteStrSrc, nStrLen, lpWideCharStr, nStrLen + 1);
//	ASSERT(nReturn > 0);

	nReturn = WideCharToMultiByte(nCPDst, 0, lpWideCharStr, nReturn, lpMultiByteStrDst, nStrLen + 1, lpDefaultChar, lpUsedDefaultChar);
	
	delete lpWideCharStr;

	return nReturn;
}

int BIG52GBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	BOOL bUsedDefaultChar = TRUE;
	int nResult = MultiByteToMultByte(lpMultiByteStrSrc, cbMultiByteSrc, 950, 
		lpMultiByteStrDst, cbMultiByteDst, 936, 
		"?", &bUsedDefaultChar);
	return nResult;
}

int GBK2BIG5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst)
{
	BOOL bUsedDefaultChar = TRUE;
	int nResult = MultiByteToMultByte(lpMultiByteStrSrc, cbMultiByteSrc, 936, 
		lpMultiByteStrDst, cbMultiByteDst, 950, 
		"?", &bUsedDefaultChar);
	return nResult;
}

//----------------------------------------------------------------------------
// GB2312 => GBK
void GB2GBK(char *szBuf)
{
  if(!strcmp(szBuf, ""))
   return;
  int nStrLen = strlen(szBuf);
  WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
  int nReturn = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nStrLen, NULL, 0);
  if(!nReturn)
   return;
  char *pcBuf = new char[nReturn + 1];
  __try
  {
   wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
   LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
   strncpy(szBuf, pcBuf, nReturn);
  }
  __finally
  {
   delete[] pcBuf;
  }
}
//---------------------------------------------------------------------------
// GBK =¡µGB2312
void GBK2GB(char *szBuf)
{
  if(!strcmp(szBuf, ""))
   return;
  int nStrLen = strlen(szBuf);
  WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
  int nReturn = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nStrLen, NULL, 0);
  if(!nReturn)
   return;
  char *pcBuf = new char[nReturn + 1];
  __try
  {
   wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
   LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
   strncpy(szBuf, pcBuf, nReturn);
  }
  __finally
  {
   delete []pcBuf;
  }
}

