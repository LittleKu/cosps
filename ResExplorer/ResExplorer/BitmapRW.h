// BitmapRW.h: header file of BitmapRW.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPRW_H__E98A8616_B808_4658_B707_195FEB023F9C__INCLUDED_)
#define AFX_BITMAPRW_H__E98A8616_B808_4658_B707_195FEB023F9C__INCLUDED_

#include "ResUtilities.h"
#include "ResInfo.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum { II_CURSOR, II_ICON, II_IMAGE};
typedef struct {
   short x, y;
   short size;
   short marg;
} t_IMAGEDSC;
const t_IMAGEDSC defImageDsc = {0, 0, 0, 10};

typedef struct {
   LONG   bmType;       // length of BITMAPINFOHEADER
   LONG   bmWidth;      // Width of BITMAP
   LONG   bmHeight;     // Height of BITMAP
	WORD   bmPlanes;     // 1
   WORD   bmBitsPixel; 
	DWORD  bmCompression;
   short  bmHotx;       // cursor hot spot x (-1 pour BITMAP et ICON)
   short  bmHoty;       // cursor hot spot y
   LONG   bmSizePal;    // taille palette
   LONG   bmOffBits;    // offset debut de la bitmap (HD + PAL)
   LONG   bmSizeImage;  // taille de la bitmap
   DWORD  bmSize;       // taille totale de l'image (HD+PAL+BM)
} BM_INFO;

class CBuffer
{
   PVOID pVoid;
public:
   CBuffer() { pVoid = NULL; };
   PVOID Convert(LPBITMAPINFOHEADER pBmh, BM_INFO* pBmi);
   PVOID CutBmp(HINSTANCE hLib, LPCTSTR resName, CRect rect, BM_INFO *pBmi);
   ~CBuffer() { if (pVoid) free(pVoid); }
};
extern DWORD GetLibKey(HINSTANCE hinst);

extern CRect DrawBitmap(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, int width);
extern CSize DrawBitmap(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType,
                         const t_IMAGEDSC *pDIm = &defImageDsc, int toolWidth = -1);
extern BOOL DrawBitmap(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, const CRect& pos);
extern BOOL DrawIcon(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, const CRect& pos);
extern BOOL DrawCursor(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, const CRect& pos);
extern CSize DrawIconCurs(CDC *pDC, WORD id, PBYTE pData, int width, CPosObj *pObj = NULL);
extern CSize DrawIconCurs(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType,
                                                int width, CPosObj* pObj = NULL);
extern IPicture* Picture(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
extern void IPictureRelease();
extern CRect IPictureShow(CDC *pDC, IPicture *pIPicture, CPoint ofs);
extern HGLOBAL IPictureToHGlobal(IPicture *pIPicture, PDWORD pSize);
extern t_ResID *SearchResEntry(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);

extern CRect DrawToolbar(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, CPoint& pos,
                                     CPosObj *pObj = NULL, TOOLBARRES *pTool = NULL);
extern CRect GetToolRect(HINSTANCE hinst, LPCTSTR resName, LPCTSTR item, TOOLBARRES *pTool);
extern CSize NextPosition(int dim, CRect& pos, int n);
extern LPBITMAPINFOHEADER GetImageInfo(HINSTANCE hinst,
                               LPCTSTR resName, LPCTSTR resType, BM_INFO* pBi = NULL);
extern LPBITMAPINFOHEADER GetTruncBitmap(HINSTANCE hinst, LPCTSTR resName, CRect rect,
                                BM_INFO *pBi);
extern LPCTSTR FormatCompression(DWORD compression);
extern UINT GetIconCursId(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType, int no =-1);
extern UINT LookupIconCursId(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
extern UINT WriteBitmap( const CString& fnam,
                        HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType );
extern UINT WriteCvrtBitmap( const CString& fnam,
                 HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType );
extern UINT WriteIconCurs( const CString& fnam,
                          HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
extern UINT WriteRawData( const CString& fnam, HINSTANCE hinst, LPCTSTR resName,
                         LPCTSTR resType, LPCTSTR filter = NULL);
extern UINT WriteResBuffer(CFile &file, PVOID pData, DWORD size,
                           LPCTSTR resName, LPCTSTR resType, WORD lang = 0x040c);
extern UINT WriteResData(CFile &file, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
extern DWORD WriteResAlign(PWORD& pWord);
extern PBITMAPINFOHEADER IconCursBitmap(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
extern UINT WriteBmToolbarRes(CFile &file, TOOLBARRES *pTool, LPCTSTR resName);
extern int IconCursEmpty(PBITMAPINFOHEADER pBih);
extern void IconCursToBm(BM_INFO *pBi, LPBITMAPINFOHEADER pBih, int colorId);

extern BOOL frame;
extern int colorId;
extern COLORREF bkColor;
extern BOOL bkColorUsed;

#endif // !defined(AFX_BITMAPRW_H__E98A8616_B808_4658_B707_195FEB023F9C__INCLUDED_)
