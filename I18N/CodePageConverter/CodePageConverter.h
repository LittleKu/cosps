#ifndef _CODE_PAGE_CONVERTER_H_
#define _CODE_PAGE_CONVERTER_H_

#pragma once

#include <windows.h>

int WideCharToMultiByte1(
						UINT CodePage,            // code page
						DWORD dwFlags,            // performance and mapping flags
						LPCWSTR lpWideCharStr,    // wide-character string
						int cchWideChar,          // number of chars in string
						LPSTR lpMultiByteStr,     // buffer for new string
						int cbMultiByte,          // size of buffer
						LPCSTR lpDefaultChar,     // default for unmappable chars
						LPBOOL lpUsedDefaultChar  // set when default char used
						);

int MultiByteToMultByte(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, UINT nCPSrc, 
						LPSTR lpMultiByteStrDst, int cbMultiByteDst, UINT nCPDst,
						LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar, DWORD dwFlags = 0);

int BIG52GBK(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);
int GBK2BIG5(LPCSTR lpMultiByteStrSrc, int cbMultiByteSrc, LPSTR lpMultiByteStrDst, int cbMultiByteDst);

void GB2GBK(char *szBuf);
void GBK2GB(char *szBuf);

#endif