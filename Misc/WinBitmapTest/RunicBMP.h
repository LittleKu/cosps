#ifndef __RUNIC_BMP_H__
#define __RUNIC_BMP_H__

BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, int height, long* newsize );
BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height );
bool LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile );
bool SaveBMP ( BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile );
BYTE* LoadBMP ( int* width, int* height, long* size, LPCTSTR bmpfile );

void TestBMPCopy (LPCTSTR input, LPCTSTR output);
void TestBMPCopy2(LPCTSTR input, LPCTSTR output);

#endif