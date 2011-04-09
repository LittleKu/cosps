// BitmapRW.cpp: implementation of the CBitmapRW functions.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResDoc.h"
#include "BitmapRW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL frame = TRUE;
int colorId = 2;
COLORREF bkColor = 0xE0E0E0;
BOOL bkColorUsed = FALSE;
static void DrawFrame(CDC *pDC, int x, int y, int sx, int sy, int hx, int hy);

///////////////////////////////////////////////////////////////////////////////

LPCTSTR FormatCompression(DWORD compression)
{
   switch (compression) {
   case BI_RGB :  return "RGB";
   case BI_RLE4 : return "RLE4";
   case BI_RLE8 : return "RLE8";
   case BI_BITFIELDS: return "BITFIELDS";
   case 4 : return "JPEG";
   case 5 :  return "PNG";
   default :      return " ";
   }
}

static DWORD NumColors(UINT BitCount, DWORD compression = 0)
{
   switch (BitCount) {
   case 1 : return 2;
   case 4 : return 16;
   case 8 : return 256;
      // The bmiColors member contains three DWORD color masks that specify
      // the red, green, and blue components, respectively, of each pixel.
   case 16:
   case 32: if (compression == BI_BITFIELDS) return 3;
   default : return 0;
   }
}

#if (WINVER < 0x0500)   // define only in Windows NT 5.0
typedef struct {
        DWORD        bV5Size;
        LONG         bV5Width;
        LONG         bV5Height;
        WORD         bV5Planes;
        WORD         bV5BitCount;
        DWORD        bV5Compression;
        DWORD        bV5SizeImage;
        LONG         bV5XPelsPerMeter;
        LONG         bV5YPelsPerMeter;
        DWORD        bV5ClrUsed;
        DWORD        bV5ClrImportant;
        DWORD        bV5RedMask;
        DWORD        bV5GreenMask;
        DWORD        bV5BlueMask;
        DWORD        bV5AlphaMask;
        DWORD        bV5CSType;
        CIEXYZTRIPLE bV5Endpoints;
        DWORD        bV5GammaRed;
        DWORD        bV5GammaGreen;
        DWORD        bV5GammaBlue;
        DWORD        bV5Intent;
        DWORD        bV5ProfileData;
        DWORD        bV5ProfileSize;
        DWORD        bV5Reserved;
} BITMAPV5HEADER;
#endif
///////////////////////////////////////////////////////////////////////////////
// Correct height of BITMAPINFOHEADER to convert ICON an BMP

PVOID CBuffer::Convert(LPBITMAPINFOHEADER pBmh,
                       BM_INFO *pBmi)
{
   LONG Height;

   if ( pVoid ) { free(pVoid); pVoid = NULL; }

   if ( pBmh && pBmi ) {
      if (pBmh->biSize == sizeof(BITMAPCOREHEADER))
         Height = ((BITMAPCOREHEADER*)pBmh)->bcHeight;
      else
         Height = pBmh->biHeight;

      if ( pBmi->bmHeight == (Height /= 2) ) {
         // ICON or CURSOR => corrected
         // make a copy of the corrected data
         pVoid = malloc(pBmi->bmSize);
         if ( pVoid ) {
            memcpy(pVoid, pBmh, pBmi->bmSize);
            if ( pBmi->bmType == sizeof(BITMAPCOREHEADER) ) {
               ((BITMAPCOREHEADER*)pVoid)->bcHeight = (WORD)Height;
            } else {
               ((BITMAPINFOHEADER*)pVoid)->biHeight = Height;
            }
            if ( colorId ) // set a bkColor
               IconCursToBm( pBmi, (LPBITMAPINFOHEADER)pVoid, colorId );
         }
         return pVoid;
      }
   }
   return (PVOID)pBmh;
}
/////////////////////////////////////////////////////////////////////////////
// Extract a piece of an Image BITMAP

PVOID CBuffer::CutBmp(HINSTANCE hinst, LPCTSTR resName,  // RT_BITMAP resource
                      CRect rect,                        // dim of the piece
                      BM_INFO *pBmi)                     // image infos
{
   if ( pVoid ) { free(pVoid); pVoid = NULL; }

   // Extract a piece of an image bitmap
   pVoid = GetTruncBitmap(hinst, resName, rect, pBmi);
   return pVoid;
}
/////////////////////////////////////////////////////////////////////////////
// Draw a title of a image

static CRect DrawTitle(CDC *pDC, const CString &name, CRect pos, bool in)
{
   CRect rect = pos;
   int x, y;

   rect.top = rect.bottom;
   pDC->DrawText(name, rect, DT_CALCRECT);

   y = (in)? -rect.Height(): 0;
   if (pos.left >= pos.right) x = 0; 
   else if ((x = (pos.Width() - rect.Width()) / 2) < 0) {
      // text truncate at the right limit
      x = 0;
      rect.right = pos.right;
   }
   rect.OffsetRect(x, y);
   pDC->DrawText(name, rect, 0);
   return rect;
}
/////////////////////////////////////////////////////////////////////////////
// Get a BITMAP info of the resource

LPBITMAPINFOHEADER GetImageInfo(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType,
                                BM_INFO *pBi)
{
   BM_INFO bmi;
   memset(&bmi, 0, sizeof(bmi));
   PVOID pData = RawData(hinst, resName, resType, &bmi.bmSize);
   if (pData) {
      BOOL curOfs = FALSE, dblBmap = FALSE;
      
      switch (DWORD(resType)) {
      case RT_CURSOR : curOfs  = TRUE;
      case RT_ICON :   dblBmap = TRUE;
      case RT_BITMAP : break;
      default : {
            // BITMAP coded in file format (raw data)
            BITMAPFILEHEADER *pBfh = (BITMAPFILEHEADER*)pData;

            // Check a bmp valid format ("BM...")
            if ((pBfh->bfType^0x4d42)|pBfh->bfReserved1|pBfh->bfReserved2)
               return NULL;

            // skip BITMAPFILEHEADER for BITMAP acces
            bmi.bmSize -= sizeof(BITMAPFILEHEADER);
            bmi.bmOffBits = pBfh->bfOffBits - sizeof(BITMAPFILEHEADER);
            pData = PVOID( pBfh + 1 );
         } 
      }

      if (curOfs) {
         // CURSOR : read hot spot
         bmi.bmHotx = PWORD(pData)[0];
         bmi.bmHoty = PWORD(pData)[1];
         bmi.bmSize -= 4;
         pData = (PVOID)(PWORD(pData)+2);
         if (bmi.bmOffBits) bmi.bmOffBits -= 4;
      } else {
         // BITMAP or ICON
         bmi.bmHotx = bmi.bmHoty = -1;
      }

      bmi.bmType = PDWORD(pData)[0];
      if (bmi.bmType == sizeof(BITMAPCOREHEADER)) {
         bmi.bmWidth      = ((BITMAPCOREHEADER*)pData)->bcWidth;
         bmi.bmHeight     = ((BITMAPCOREHEADER*)pData)->bcHeight;
         bmi.bmBitsPixel  = ((BITMAPCOREHEADER*)pData)->bcBitCount;
         bmi.bmPlanes     = ((BITMAPCOREHEADER*)pData)->bcPlanes;
         bmi.bmCompression= BI_RGB;   // An uncompressed format.
         bmi.bmSizePal    = NumColors(bmi.bmBitsPixel, 0) * sizeof(RGBTRIPLE);
      } else if (bmi.bmType == sizeof(BITMAPINFOHEADER) ||
                     bmi.bmType == sizeof(BITMAPV4HEADER) ||
                           bmi.bmType == sizeof(BITMAPV5HEADER)) {
         bmi.bmWidth      = ((BITMAPINFOHEADER*)pData)->biWidth;
         bmi.bmHeight     = ((BITMAPINFOHEADER*)pData)->biHeight;
         bmi.bmBitsPixel  = ((BITMAPINFOHEADER*)pData)->biBitCount;
         bmi.bmPlanes     = ((BITMAPINFOHEADER*)pData)->biPlanes;
         bmi.bmCompression= ((BITMAPINFOHEADER*)pData)->biCompression;
         bmi.bmSizeImage  = ((BITMAPINFOHEADER*)pData)->biSizeImage;
         bmi.bmSizePal    = ((BITMAPINFOHEADER*)pData)->biClrUsed;
         if (bmi.bmSizePal == 0)
           bmi.bmSizePal  = NumColors( bmi.bmBitsPixel, bmi.bmCompression );
         bmi.bmSizePal   *= sizeof(RGBQUAD);
      } else {
         // Invalid Image
         pData = NULL;
      }

      if (dblBmap) bmi.bmHeight /= 2; // update for ICON or CURSOR
      if (bmi.bmSizeImage == 0) {
         bmi.bmSizeImage = (((bmi.bmWidth * bmi.bmBitsPixel + 31)>> 5)<< 2)
                                         * bmi.bmHeight;
         if (dblBmap) bmi.bmSizeImage += (((bmi.bmWidth + 31)>> 5)<< 2)
                                         * bmi.bmHeight;
      }
      if (bmi.bmOffBits == 0) bmi.bmOffBits = bmi.bmType + bmi.bmSizePal;
   }
   if (pBi) *pBi = bmi;
   return (LPBITMAPINFOHEADER)pData;
}
///////////////////////////////////////////////////////////////////////////////
// Extract a piece of the resource image

LPBITMAPINFOHEADER GetTruncBitmap(HINSTANCE hinst, LPCTSTR resName,
                                  CRect rect,   // piece of image
                                  BM_INFO *pBi)
{
   BM_INFO bmi;
   LPBITMAPINFOHEADER pNew = NULL;
   LPBITMAPINFOHEADER pBih = GetImageInfo(hinst, resName, RT_BITMAP, &bmi);

   // Clip a requested rect to image size
   rect &= CRect(0, 0, bmi.bmWidth, bmi.bmHeight);

   if (pBih && !rect.IsRectEmpty()) {
      // For the new, reuse the format of source Bitmap but uncompressed
      bmi.bmWidth = rect.Width(), bmi.bmHeight = rect.Height();
      bmi.bmCompression = BI_RGB;   // An uncompressed format
      bmi.bmSizeImage = (((bmi.bmWidth * bmi.bmBitsPixel + 31)>> 5)<< 2)
                                         * bmi.bmHeight;
      bmi.bmSize = bmi.bmType + bmi.bmSizePal + bmi.bmSizeImage;

      // Allocate and fill out a BITMAPINFO struct for the new DIB
      pNew = (LPBITMAPINFOHEADER)malloc(bmi.bmSize);
      if (pNew) {
         HBITMAP hSrcBmp, hDstBmp, hOldDstBmp, hOldSrcBmp;
         HDC hDC = ::GetDC( NULL ), hSrcDC, hDstDC;
         PVOID pDstBits = NULL;

         // Copy in new bitmap the BITMAPINFOHEADER and the palette
         memcpy(pNew, pBih, bmi.bmType + bmi.bmSizePal);

         // Update BITMAPINFOHEADER for the new bitmap dimensions
         if (bmi.bmType == sizeof(BITMAPCOREHEADER)) {
            ((BITMAPCOREHEADER*)pNew)->bcWidth  = (WORD)bmi.bmWidth;
            ((BITMAPCOREHEADER*)pNew)->bcHeight = (WORD)bmi.bmHeight;
         } else {
            ((BITMAPINFOHEADER*)pNew)->biWidth  = bmi.bmWidth;
            ((BITMAPINFOHEADER*)pNew)->biHeight = bmi.bmHeight;
            ((BITMAPINFOHEADER*)pNew)->biCompression = bmi.bmCompression;
            ((BITMAPINFOHEADER*)pNew)->biSizeImage = bmi.bmSizeImage;
            ((BITMAPINFOHEADER*)pNew)->biXPelsPerMeter = 0;
            ((BITMAPINFOHEADER*)pNew)->biYPelsPerMeter = 0;
         }

         // Use DIBSections and BitBlt() to do the conversion, so make 'em
         hSrcBmp = ::CreateDIBitmap( hDC, pBih, CBM_INIT,
                 PBYTE(pBih)+bmi.bmOffBits, (BITMAPINFO*)pBih, DIB_RGB_COLORS );
         hDstBmp = ::CreateDIBSection( hDC, (BITMAPINFO*)pNew, DIB_RGB_COLORS,
                                                           &pDstBits, NULL, 0 );
         hSrcDC = ::CreateCompatibleDC( hDC );
         hDstDC = ::CreateCompatibleDC( hDC );

         // Select DIBSections into DCs
         hOldSrcBmp = (HBITMAP)::SelectObject( hSrcDC, hSrcBmp );
         hOldDstBmp = (HBITMAP)::SelectObject( hDstDC, hDstBmp );

         // Draw a new image with a cut a source
         BitBlt( hDstDC, 0, 0, bmi.bmWidth, bmi.bmHeight, hSrcDC,
                                              rect.left, rect.top, SRCCOPY );
         // Clean up and delete the DCs
         SelectObject( hSrcDC, hOldSrcBmp );
         SelectObject( hDstDC, hOldDstBmp );
         DeleteDC( hSrcDC );
         DeleteDC( hDstDC );
         ReleaseDC( NULL, hDC );

         // Copy a bitmap datas
         memcpy(PBYTE(pNew)+bmi.bmType+bmi.bmSizePal, pDstBits, bmi.bmSizeImage);

         // Final cleanup
         DeleteObject( hDstBmp );
         DeleteObject( hSrcBmp );
//         free( lpbmi );
      } 
   }
   if (pBi) *pBi = bmi;
   return pNew;
}
///////////////////////////////////////////////////////////////////////////////
// Display a Bitmap

CRect DrawBitmap(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, int width)
{
   CDC dcMem;
   dcMem.CreateCompatibleDC( pDC );
   BITMAP bm;
   HBITMAP hBm = ::LoadBitmap(hinst, resName);
   if (hBm) {
      ::GetObject(hBm, sizeof(bm), &bm);
      int sx = bm.bmWidth, sy = bm.bmHeight;

      // Draw bitmap limits
      DrawFrame(pDC, 10, 10, sx, sy, width, -1);

      // Draw the bitmap
      HBITMAP oldBm = (HBITMAP)dcMem.SelectObject( hBm );
      pDC->BitBlt(10, 10, sx, sy, &dcMem, 0, 0, SRCCOPY);
      dcMem.SelectObject( oldBm );
      ::DeleteObject( hBm );

      // Draw name and info of the bitmap
      CString text;
      text.Format("%s\n%d x %d bits", FormatId(resName, TYP_IDB), bm.bmWidth, bm.bmHeight);
      CRect rect = DrawTitle(pDC, text, CRect(10, 10, 0, 20 + sy), false);
      return CRect(10, 10, sx + 10, rect.bottom);
   } else {
      return DrawTitle(pDC, "image error", CRect(10,10,10,10), false);
   }
}

BOOL DrawBitmap(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, const CRect& pos)
{
   BITMAP bm;
   HBITMAP hBm = ::LoadBitmap(hinst, resName);
   if (hBm == NULL) return FALSE;

   ::GetObject(hBm, sizeof(bm), &bm);
   int w = pos.Width(), h = pos.Height();
   int sx = bm.bmWidth, sy = bm.bmHeight;
   if (sx > w) sy = (sy * w) / sx, sx = w;
   if (sy > h) sx = (sx * h) / sy, sy = h;
   int x = (pos.left + pos.right - sx) / 2;
   int y = (pos.bottom + pos.top - sy) / 2;

   CDC dcMem;
   dcMem.CreateCompatibleDC( pDC );
   HBITMAP oldBm = (HBITMAP)dcMem.SelectObject( hBm );
//   pDC->FillSolidRect(pos, 0xFFF0FF);
   pDC->FillSolidRect(pos, bkColor);
   bkColorUsed = TRUE;
   if (sx == bm.bmWidth) {
      pDC->BitBlt(x, y, sx, sy, &dcMem, 0, 0, SRCCOPY);
   } else {
      pDC->StretchBlt(x, y, sx, sy, &dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
   }
   dcMem.SelectObject( oldBm );
   ::DeleteObject( hBm );

   // Draw name of the bitmap
   DrawTitle(pDC, FormatId(resName, TYP_IDB), pos, true);
   return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// Bitmap display

CSize DrawBitmap(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType,
                  const t_IMAGEDSC *pDIm, int toolWidth)
{
   BM_INFO bmi;
   LPBITMAPINFOHEADER pBih = GetImageInfo( hinst, resName, resType, &bmi );
   if ( pBih ) {

      HBITMAP hBm = ::CreateDIBitmap( pDC->m_hDC, pBih, CBM_INIT,
                  PBYTE(pBih) + bmi.bmOffBits, (BITMAPINFO*)pBih, DIB_RGB_COLORS );

      if ( pDIm == NULL ) pDIm = &defImageDsc;

      if ( hBm ) {
         CDC dcMem;
         int marg = pDIm->marg;
         if (marg < 0) marg = -marg;
         // bitmap position and dimensions
         int x = marg + pDIm->x, y = marg + pDIm->y;
         int sx = bmi.bmWidth, sy = bmi.bmHeight;
         // Draw the bitmap
         dcMem.CreateCompatibleDC( pDC );
         HBITMAP oldBm = (HBITMAP)dcMem.SelectObject( hBm );
         if ( pDIm->size > 0 ) {
            int size = pDIm->size;
            if (sx > sy) {
               if (sx > size) sy = (sy * size) / sx, sx = size;
            } else if (sy > size) sx = (sx * size) / sy, sy = size;
            x += (size-sx) / 2, y += (size-sy) / 2; 
            pDC->StretchBlt(x, y, sx, sy, &dcMem, 0, 0, bmi.bmWidth, bmi.bmHeight, SRCCOPY);
         } else {
            // Trace les limites de l'image
            if ( pDIm->marg >= 10 ) {
               if ( (bmi.bmHotx & bmi.bmHoty) != -1 )
                  // CURSOR
                  DrawFrame(pDC, x, y, sx, sy, bmi.bmHotx, bmi.bmHoty);
               else
                  // BITMAP or ICON
                  DrawFrame(pDC, x, y, sx, sy, toolWidth, -1);
            }
            pDC->BitBlt(x , y, sx, sy, &dcMem, 0, 0, SRCCOPY );
         }

         // Affiche les informations de l'image
         CString text;
         text.Format( "%d x %d bits,\n %s %d bits/pixel", bmi.bmWidth, bmi.bmHeight,
                              FormatCompression(bmi.bmCompression), bmi.bmBitsPixel );
         CRect rect = DrawTitle(pDC, text, CRect(x, 0, 0, y + sy + marg), false);

         dcMem.SelectObject( oldBm );
         ::DeleteObject( hBm );

         return CSize( sx + 2*marg, rect.bottom - y + 2*marg );
      }
   }
   return CSize(0, 0);
}
///////////////////////////////////////////////////////////////////////////////
// Cursors or Icons display

BOOL DrawIcon(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, const CRect& pos)
{
   HICON hic = ::LoadIcon(hinst, resName);
   if (hic == NULL) return FALSE;

   int cx = ::GetSystemMetrics(SM_CXICON);
   int x = (pos.left + pos.right - cx) / 2;
   int y = (pos.bottom + pos.top - cx) / 2;
   pDC->DrawIcon(x, y, hic);
   ::DestroyIcon(hic);

   DrawTitle(pDC, FormatId(resName, TYP_IDI), pos, true);
   return TRUE;
}

BOOL DrawCursor(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, const CRect& pos)
{
   HCURSOR hcr = ::LoadCursor(hinst, resName);
   if (hcr == NULL) return FALSE;

   int cx = ::GetSystemMetrics(SM_CXICON);
   int x = (pos.left + pos.right - cx) / 2;
   int y = (pos.bottom + pos.top - cx) / 2;
   pDC->DrawIcon(x, y, hcr);
   ::DestroyCursor(hcr);

   DrawTitle(pDC, FormatId(resName, TYP_IDC), pos, true);
   return TRUE;
}

CSize DrawIconCurs(CDC *pDC,
                   HINSTANCE hinst, LPCTSTR resName,
                   LPCTSTR resType,    // only for RT_GROUP_ICON or RT_GROUP_CURSOR
                   int width, CPosObj *pObj)
{
   DWORD rSize;
   LPMEMICONDIR pMId = (LPMEMICONDIR)RawData(hinst, resName, resType, &rSize);
   DWORD type = (resType == RT_GROUP_CURSOR)? TYP_IDC: TYP_IDI;
   if (pMId && DWORD(pMId->idEntries + pMId->idCount) <= DWORD(pMId) + rSize) {

      for (int no = 0; no < pMId->idCount; no++) {
         LPCTSTR resId = (LPCTSTR)pMId->idEntries[no].nID;
         int left = width * no;
         BM_INFO bmi;
         PBYTE pB = (PBYTE)GetImageInfo(hinst, resId, resType-DIFFERENCE, &bmi);
         if (pB) {
            // traite les cursor comme les icon
            HICON hi = ::CreateIconFromResourceEx( pB, bmi.bmSize, TRUE,
                               0x00030000, bmi.bmWidth, bmi.bmHeight, LR_DEFAULTCOLOR );
            int x = 5 + left + (width - bmi.bmWidth) / 2;
            int y = 10 + (width - bmi.bmHeight) / 2;

            // Trace les limites de l'image et le centre du curseur
            DrawFrame(pDC, x, y, bmi.bmWidth, bmi.bmHeight, bmi.bmHotx, bmi.bmHoty);

            // Trace icon ou curseur avec la taille d'origine
            if (hi)
               ::DrawIconEx(pDC->m_hDC, x, y, hi, 0, 0, 0, 0, DI_NORMAL);

            // Affiche les informations de l'image
            CString name = FormatId(resName, type) + "\n / " + GetResStr(resId);
            DrawTitle(pDC, name, CRect(left, 0, left+width, width + 10), false);
            if (pObj) {
               // Trace un rectangle de selection autour de l'icon designee
               CRect rect(CPoint(x, y), CSize(bmi.bmWidth, bmi.bmHeight));
               rect.InflateRect(7, 7);
               if (pObj->Add(rect, resId, 2)) pDC->DrawFocusRect(rect);
            }
         }
      }
      return CSize(width * pMId->idCount + 10, width + 20);
   }
   return CSize(10, 10);
}

CSize DrawIconCurs(CDC *pDC, WORD id, PBYTE pData, int width, CPosObj *pObj)
{
   LPICONDIR pId = (LPICONDIR)pData;
   for (int no = 0; no < pId->idCount; no++) {
      PBYTE pB = PBYTE(pData) + pId->idEntries[no].dwImageOffset;
      DWORD size = pId->idEntries[no].dwBytesInRes;
      HICON hi = ::CreateIconFromResource( pB, size, TRUE, 0x00030000 );
      int cx = pId->idEntries[no].bWidth, cy = pId->idEntries[no].bHeight;
      int left = width * no;
      int x = 5 + left + (width - cx) / 2;
      int y = 10 + (width - cy) / 2;

      // Trace les limites de l'image et le centre du curseur
      DrawFrame(pDC, x, y, cx, cy, -1, -1);

      // Trace icon ou curseur avec la taille d'origine
      if (hi)
         ::DrawIconEx(pDC->m_hDC, x, y, hi, 0, 0, 0, 0, DI_NORMAL);

      // Affiche les informations de l'image
      if (pObj) {
         // Trace un rectangle de selection autour de l'icon designee
         CRect rect(CPoint(x, y), CSize(cx, cy));
         rect.InflateRect(7, 7);
         if (pObj->Add(rect, LPCTSTR(id | (no << 8)), 2)) pDC->DrawFocusRect(rect);
      }
   }
   return CSize(width * pId->idCount + 10, width + 20);
}
///////////////////////////////////////////////////////////////////////////////
// Check a empty ICON or CURSOR bitmap

int IconCursEmpty(PBITMAPINFOHEADER pBih)
{
   LONG Width, Height, BitCount, SizePal;

   if ( pBih->biSize == sizeof(BITMAPCOREHEADER) ) {
      Width  = ((BITMAPCOREHEADER*)pBih)->bcWidth;
      Height = ((BITMAPCOREHEADER*)pBih)->bcHeight / 2;
      BitCount= ((BITMAPCOREHEADER*)pBih)->bcBitCount;
      SizePal= NumColors(BitCount , 0) * sizeof(RGBTRIPLE);
   } else {
      Width  = pBih->biWidth;
      Height = pBih->biHeight / 2;
      BitCount= pBih->biBitCount;
      SizePal= (pBih->biClrUsed? pBih->biClrUsed:
           NumColors(BitCount , pBih->biCompression )) * sizeof(RGBQUAD);
   }

   // Check a BITMAP data (-1 => no empty)
   LONG LineLen = ((Width*BitCount+31)/ 32); // line length in DWORD count
   LONG SizeBitmap= LineLen * Height * 4;    // BITMAP size in BYTE count
   PDWORD pW = PDWORD(PBYTE(pBih)+ pBih->biSize + SizePal), pT;
   DWORD bmMask = (1 << (LineLen * 32 - Width * BitCount)) - 1;
   LONG i, j;
   for ( i = 0; i < Height; i++, pW += LineLen ) {
      for (j = ((Width*BitCount)/32), pT = pW; --j >= 0; pT++)
         if (*pT != 0) return -1;
      if (bmMask && (bmMask & *pT) != 0) return -1;
   }

   // Check BITMAP AND mask (0 => image in the mask)
   DWORD cmp = *pW? 0xFFFFFFFF: 0;
   LineLen = (Width+31)/32;
   bmMask = (1 << (LineLen * 32 - Width)) - 1;
   for ( i = 0; i < Height; i++, pW += LineLen ) {
      for (j = Width/32, pT = pW; --j >= 0; pT++) if (*pT != cmp) return 0;
      if (bmMask && (bmMask & (*pT ^ cmp)) != 0) return 0;
   }
   return 1;                     // 1 => image and mask empty
}
///////////////////////////////////////////////////////////////////////////////
static IStream  *pIStream = NULL;
static IPicture *pIPicture= NULL;
static IMalloc  *pIMalloc = NULL;

void IPictureRelease()
{
   return;
   if (pIStream)  pIMalloc->Free(pIStream), pIStream = NULL;//pStream->Release();
   if (pIPicture) pIMalloc->Free(pIPicture), pIPicture = NULL;//pIPicture->Release();
   if (pIMalloc)  pIMalloc->Release();
}
///////////////////////////////////////////////////////////////////////////////
// Creation d'un IPicture a partir d'une resource (de type image)

IPicture* Picture(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
//   if (pIMalloc == NULL) {
//      CoGetMalloc(1, &pIMalloc);
//      if (pIMalloc == NULL) return NULL;
//   }

   DWORD rSize;   // resource size
   PVOID pRes = RawData(hinst, resName, resType, &rSize);
   if (pRes) {

      // Alloue la memoire pour IStream et y copie les data
      HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE|GMEM_SHARE, rSize );
	   memcpy( ::GlobalLock( hGlobal ), pRes, rSize );
      ::GlobalUnlock( hGlobal );

      // Creation de IPicture a partir des data de la resource
      if (::CreateStreamOnHGlobal( hGlobal, TRUE, &pIStream ) == S_OK ) {
         HRESULT hr = ::OleLoadPicture( pIStream, rSize, FALSE,
                                        IID_IPicture, (LPVOID*)&pIPicture );
         pIStream->Release();
//            pIMalloc->Free(pData);
         GlobalFree(hGlobal);
		   if (hr == S_OK) return pIPicture;
      }
   }
   return NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Draw images in IPicture format

CRect IPictureShow(CDC *pDC, IPicture *pIPicture, CPoint ofs)
{
#define HIMETRIC_INCH 2540
   if (pIPicture) {
      CRect DrawRect, txtRect;
      CString text;
      long width  = 0;
      long height = 0;

      pIPicture->get_Width(&width);
      pIPicture->get_Height(&height);
      // Get Current DPI - Dot Per Inch
      int CurrentDPI_X = pDC->GetDeviceCaps(LOGPIXELSX);
      int CurrentDPI_Y = pDC->GetDeviceCaps(LOGPIXELSY);
      // Picture out size in pixels
      long h = MulDiv(height, CurrentDPI_Y, HIMETRIC_INCH);
      long w = MulDiv(width,  CurrentDPI_X, HIMETRIC_INCH);

      // Draw a image limits
      DrawFrame(pDC, ofs.x, ofs.y, w, h, -1, -1);

      // Draw image
      HRESULT hrP = pIPicture->Render(pDC->m_hDC, ofs.x, ofs.y, w, h,
                                              0, height, width, -height,
                                                          &DrawRect);
      // Draw image info
      HGLOBAL hGlobal = IPictureToHGlobal(pIPicture, NULL);
      LPBITMAPINFOHEADER pBih = (LPBITMAPINFOHEADER)::GlobalLock(hGlobal);
      switch (pBih->biSize) {
      case sizeof(BITMAPINFOHEADER):
         text.Format("BITMAP %d x %d pixels\n   %d bit(s)/pixel",
                        pBih->biWidth, pBih->biHeight, pBih->biBitCount); 
         break;
      }
      ::GlobalUnlock( hGlobal );
      pIPicture->Release();
      txtRect = DrawTitle(pDC, text, CRect(ofs.x, 0, 0, ofs.y + h + 10), false);

      if (SUCCEEDED(hrP)) return CRect(ofs.x, ofs.y, max(w + ofs.x + 10, txtRect.right),
                                       max(h + ofs.y + 10, txtRect.bottom));
   }
   return CRect(ofs.x, ofs.y, ofs.x, ofs.y);
}
///////////////////////////////////////////////////////////////////////////////
// Get a HGlobal ptr to image data of an IPicture object (and allocate it)

HGLOBAL IPictureToHGlobal(IPicture *pIPicture, PDWORD pSize)
{
   IStream  *pIStream = NULL;
   HGLOBAL  hGlobal = NULL;
   DWORD    ofs = 0;
   long     lSize;

   if (pIPicture) {
      // Creates a stream object stored in global memory
      ::CreateStreamOnHGlobal(NULL, FALSE, &pIStream);

      if (pIStream) {
         // Saves the picture's data into a stream in the same format that it
         // would save itself into a file
         pIPicture->SaveAsFile( pIStream, TRUE, &lSize );
         
         // Retrieves the global memory handle used by the stream object
         ::GetHGlobalFromStream( pIStream, &hGlobal );

         // Suppress the File Header (if req.)
         if (pSize == NULL) {
            // Locks a global memory object and returns a pointer to the first
            // byte of the object's memory block
            PBYTE pByte = (PBYTE)::GlobalLock(hGlobal);

            if (PBITMAPFILEHEADER(pByte)->bfType == 0x4D42 )// "BM"
               ofs = sizeof(BITMAPFILEHEADER);
            // ajouter ici les autres formats si necessaire (Icon, ...)

            lSize -= ofs;

            // Translate bmp data to suppress Fileheader
            memmove(pByte, pByte + ofs, lSize);
            ::GlobalUnlock( hGlobal );

         } else *pSize = lSize;
//         pIStream->Release();

         return hGlobal;
      }
   }
   return NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Check if a resource can be read with a IPicture

t_ResID *SearchResEntry(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   IPicture *pIPicture = Picture(hinst, resName, resType);
   if (pIPicture) {
      short type = -2;
      WORD rType = 0;
      pIPicture->get_Type(&type);
      pIPicture->Release();

      switch (type) {
      case PICTYPE_BITMAP :      rType = rt_bmx; break;
      case PICTYPE_METAFILE :    rType = rt_wmf; break;
      case PICTYPE_ICON :        rType = rt_icon;break;
      case PICTYPE_ENHMETAFILE : rType = rt_emf; break;
      default: return NULL;
      }
      return GetResEntry((LPCTSTR)rType);
   }
   return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// Subroutine to display toolbar

CRect DrawToolbar(CDC *pDC,
                  HINSTANCE hinst,     // handle de la librairie
                  LPCTSTR resName,     // Name of the resource
                  CPoint& org,         // Origine de la toolbar
                  CPosObj *pObj,       // Descripteur des objets a designer
                  TOOLBARRES *pTool)   // Table descripteur de toolbar 
{
   // Extrait la bitmap des tools dans resource
   HBITMAP hBm = ::LoadBitmap(hinst, resName);
   // Extrait la description des tools
   TOOLBARRES *pT = pTool? pTool: (TOOLBARRES*)RawData(hinst, resName, RT_TOOLBAR);
   if (hBm == NULL || pT == NULL) return CRect(0,0,0,0);

   // Calcule les dimensions de la toolbar
   int btnWidth = pT->x + 7, btnHeight = pT->y + 6;
   CRect btnRect(0, 0, btnWidth, btnHeight);
   int width, i;
   if (pTool) {
      width = pT->cnt * btnWidth;
   } else {
      for (width = i = 0; i < pT->cnt; i++)
         width += pT->toolId[i]? btnWidth: btnWidth / 3;
   }
   CRect frame(org, CSize(width + 20, btnHeight + 20));

   CDC dcMem;
   dcMem.CreateCompatibleDC( pDC );
   HBITMAP oldBm = (HBITMAP)dcMem.SelectObject( hBm );

   pDC->DrawFrameControl( frame, DFC_BUTTON, DFCS_BUTTONPUSH );

   // Comput and draw a tool bar
   CPoint btnOrg = frame.TopLeft() + CPoint(10, 10);
   int cx = pT->x, cy = pT->y, ox = 0;
   for (i = 0; i < pT->cnt; i++) {
      WORD id = pTool? i + 1: pT->toolId[i];  // read resource
      if ( id ) {
         // Button
         CRect br = btnRect + btnOrg;
         int x = (br.left + br.right - cx) / 2;
         int y = (br.top + br.bottom - cy) / 2;
         UINT stat = DFCS_BUTTONPUSH;
         if ( pObj && (pObj->Add(br, (LPCTSTR)id, 1)) ) stat |= DFCS_PUSHED;
         // Draw a button
         pDC->DrawFrameControl( br, DFC_BUTTON, stat );
         pDC->BitBlt( x, y, cx, cy, &dcMem, ox, 0, SRCCOPY );
         // Position of the next
         btnOrg.x += btnWidth, ox += cx;
      } else {
         // Separator
         btnOrg.x += btnWidth / 3;
      }
   }
   dcMem.SelectObject( oldBm );
   ::DeleteObject( hBm );

   return frame;
}
///////////////////////////////////////////////////////////////////////////////
// Comput a bitmap piece Rect corresponding at the tool button

CRect GetToolRect(HINSTANCE hinst,     // handle de la librairie
                  LPCTSTR resName,     // Name of the resource
                  LPCTSTR item,        // item cherche
                  TOOLBARRES *pTool)   // Table descripteur de toolbar
{
   TOOLBARRES *pT = pTool? pTool: (TOOLBARRES*)RawData(hinst, resName, RT_TOOLBAR);
   int cx = pT->x, cy = pT->y, ox = 0, i;

   // Search a tool there has the god id
   for (i = 0; i < pT->cnt; i++) {
      WORD id = pTool? i + 1: pT->toolId[i];  // read resource
      if (id) {   // Null id => separator
         if (id == WORD(item)) return CRect(ox, 0, ox + cx, cy);
         ox += cx;
      }
   }
   return CRect(0,0,0,0);        // exit not found
}
///////////////////////////////////////////////////////////////////////////////
// Trace les limites et le reticule d'une image

static void DrawFrame(CDC *pDC,
                      int x, int y,    // position de l'image
                      int sx, int sy,  // dimensions de l'image
                      int hx, int hy)  // position du reticule ou des separateurs
{
   // Trace les limites de l'image
   COLORREF bkC = pDC->GetBkColor();
   if (frame) {
      if (bkColor) pDC->FillSolidRect(x-10,y-10, sx+20, sy+20, bkColor);
      pDC->MoveTo(x-6,y-1); pDC->LineTo(x-1,y-1); pDC->LineTo(x-1,y-6);
      pDC->MoveTo(x+sx+5,y-1); pDC->LineTo(x+sx,y-1); pDC->LineTo(x+sx,y-6);
      pDC->MoveTo(x-6,y+sy); pDC->LineTo(x-1,y+sy); pDC->LineTo(x-1,y+sy+5);
      pDC->MoveTo(x+sx+5,y+sy); pDC->LineTo(x+sx,y+sy); pDC->LineTo(x+sx,y+sy+5);
   }
   pDC->SetBkColor(bkC);
   bkColorUsed = TRUE;

   if (hx > 0 && hx <= sx && hy == -1) {
      // Trace les separations des tooltips
      int ox = x, y0 = y - 3, y1 = y0 + sy + 7;
      for (; ox <= x + sx; ox += hx)
         pDC->MoveTo(ox, y0), pDC->LineTo(ox, y1);
   } else if (frame && hx >= 0 && hy >= 0) {
      // trace le centre du curseur
      pDC->MoveTo(x-10, y+hy); pDC->LineTo(x+sx+10, y+hy);
      pDC->MoveTo(x+hx, y-10); pDC->LineTo(x+hx, y+sy+10);
   }
}
///////////////////////////////////////////////////////////////////////////////
// Mise en page d'une serie d'imagettes

CSize NextPosition(int dim, CRect & pos, int n)
{
   int w = dim / n;
   if (pos.IsRectEmpty()) {
      pos.top = pos.left = w / 12, pos.bottom = pos.right = (w * 11)/ 12;
   } else {
      if (pos.right > (dim - w)) {
         pos.left = w / 12, pos.right = (w * 11)/ 12, pos.top += w, pos.bottom += w;
      } else {
         pos.left += w, pos.right += w;
      }
   }
   return CSize(w * n + 5, pos.bottom + 5);
}
///////////////////////////////////////////////////////////////////////////////
// Extraction des informations de la directory des icon / cursor

UINT LookupIconCursId(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   return GetIconCursId(hinst, resName, resType, -2);
}

UINT GetIconCursId(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType, int no)
{
   // pour eviter de relire les informations
   static LPCTSTR rName = NULL, rType = NULL;
   static PBYTE lpMID = NULL;
   static DWORD key = 0;

   DWORD nKey = GetLibKey(hinst);
   if (lpMID == NULL || key != nKey || rName != resName || rType != resType) {
      key = nKey, rName = resName, rType = resType;
      lpMID = (PBYTE)RawData(hinst, resName, resType);
   }

   if (lpMID) {
      BOOL icon = (resType == RT_GROUP_ICON);
      UINT noMax = LPMEMICONDIR(lpMID)->idCount;
      if (noMax > 16) return 0;

      switch (no) {
      case -2:
         return ::LookupIconIdFromDirectoryEx(lpMID, icon, 0, 0, LR_DEFAULTCOLOR);
      case -3:
         return ::LookupIconIdFromDirectoryEx(lpMID, icon, 0, 0, LR_MONOCHROME);
      case -1:
         return noMax;
      default:
         if (UINT(no) < noMax) return LPMEMICONDIR(lpMID)->idEntries[no].nID;
      }
   }
   return 0;
}

DWORD GetLibKey(HINSTANCE hinst)
{
   TCHAR buffer[MAX_PATH];
   DWORD key = DWORD(hinst);
   int len = ::GetModuleFileName(hinst, buffer, MAX_PATH);
   while (--len >= 0) key += buffer[len];
   return key;
}
///////////////////////////////////////////////////////////////////////////////
// Ecriture d'une resource BITMAP dans un fichier

UINT WriteBitmap( const CString& fnam,
                 HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType )
{
   BM_INFO bmi;
   LPBITMAPINFOHEADER pBmi = GetImageInfo( hinst, resName, resType, &bmi);
   if (pBmi) {
      CBuffer bf;          // correction Height for CURSOR or ICON
      BITMAPFILEHEADER bfh;
      bfh.bfType = 0x4d42;
      bfh.bfReserved1 = 0;
      bfh.bfReserved2 = 0;
      bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + bmi.bmOffBits;
      bfh.bfSize = sizeof(BITMAPFILEHEADER) + bmi.bmSize;

      // Write the header
      CFile file(fnam, CFile::modeCreate|CFile::modeWrite);
      file.Write( &bfh, sizeof(BITMAPFILEHEADER) );

      // Write the bitmap
      file.Write( bf.Convert(pBmi, &bmi), bmi.bmSize );
      file.Close();

      return 0;
   }
   return IDS_READERROR;
}
///////////////////////////////////////////////////////////////////////////////
// Ecriture d'une BITMAP dans un fichier à partir d'une resource Gif, Jpg...

UINT WriteCvrtBitmap( const CString& fnam,
                 HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType )
{
   // Use an IPicture objet to convert in bitmap format
   IPicture *pIPicture = Picture(hinst, resName, resType);

   if (pIPicture) {
      DWORD bmSize;
      HGLOBAL hGlobal = IPictureToHGlobal(pIPicture, &bmSize);
      if (hGlobal) {

         CFile file(fnam, CFile::modeCreate|CFile::modeWrite);

         // Write the bitmap
         file.Write( ::GlobalLock(hGlobal), bmSize );
         ::GlobalUnlock(hGlobal);

         file.Close();

         GlobalFree( hGlobal );

         return 0;
      }
   }
   return IDS_READERROR;
}
///////////////////////////////////////////////////////////////////////////////
// Ecriture d'une resource ICON / CURSOR dans un fichier

UINT WriteIconCurs(const CString& fnam,
                   HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   WORD count, n, idType;
   LPMEMICONDIR pMid = (LPMEMICONDIR)RawData(hinst, resName, resType);
   if (pMid == NULL || (count = pMid->idCount)-1 > 16 ) return FALSE;

   // Creation de la directory des ICON ou CURSOR
   UINT bfSize = count * sizeof(LPBITMAPINFOHEADER);
   UINT headLen = sizeof(ICONDIR) + sizeof(ICONDIRENTRY) * (count - 1);
   LPBITMAPINFOHEADER *ppBi = (LPBITMAPINFOHEADER*)malloc(bfSize + headLen);
   LPICONDIR pId = (LPICONDIR)(PBYTE(ppBi) + bfSize);
   if (ppBi == NULL) return FALSE;

   resType -= DIFFERENCE;  // RT_GROUP_ICON/CURSOR => RT_ICON/CURSOR
   pId->idReserved = 0;
   // le type depend de l'extension du nom du fichier (1=> ICON, 2=> CURSOR)
   pId->idType = idType = fnam.Right(4).CompareNoCase(".cur") != 0? 1: 2;
   pId->idCount = count;

   BM_INFO bmi;
   BOOL ok = TRUE;
   DWORD ofs = headLen;
   ICONDIRENTRY *pIde = pId->idEntries;

   for (n = 0; n < count; n++, ofs += bmi.bmSize, pIde++) {

      // read the ICONs or CURSORs resources
      if (!(ppBi[n] = GetImageInfo(hinst, (LPCTSTR)pMid->idEntries[n].nID,
                                      resType, &bmi)) ) ok = FALSE;

      pIde->bWidth = (BYTE)bmi.bmWidth;
      pIde->bHeight = (BYTE)bmi.bmHeight;
      pIde->bColorCount = (idType == 2)? 0: BYTE(1 << bmi.bmBitsPixel);
      pIde->bReserved = 0;
      if (bmi.bmHotx == -1) {
         // set default Hotspot at the image center
         pIde->wCursHotX = WORD(bmi.bmWidth / 2);
         pIde->wCursHotY = WORD(bmi.bmHeight / 2);
      } else {
         // Hotspot is define
         pIde->wCursHotX = bmi.bmHotx;
         pIde->wCursHotY = bmi.bmHoty;
      }
      pIde->dwBytesInRes = bmi.bmSize;
      pIde->dwImageOffset = ofs;

      TRACE("WriteIconCurs typ=%d (%dx%d) color=%d\n",
                  idType, pIde->bWidth, pIde->bHeight, pIde->bColorCount);
   }

   // l'analyse des resources est terminee, on ecrit tout
   if (ok) {
      CFile file(fnam, CFile::modeCreate|CFile::modeWrite);

      // Ecriture de la Directory
      file.Write( pId, headLen );

      // Ecriture des images des ICONs ou CURSORs
      for (n = 0; n < count; n++)
         file.Write( ppBi[n], pId->idEntries[n].dwBytesInRes );

      file.Close();
   }
   free( ppBi );

   return 0;
}
///////////////////////////////////////////////////////////////////////////////
// Conversion d'un CURSOR/ICON en BITMAP

typedef struct {
   PBYTE pBm, pMsk;
   DWORD linSize;
   WORD  mlSize;
   WORD  bitSize;
   WORD  and;
   WORD  xor;
} t_CBM;

static void SetCBM(t_CBM *pCBM, BM_INFO *pBi, LPBITMAPINFOHEADER pBih,
                   WORD color)      // couleur de fond
{
   pCBM->pBm     = PBYTE(pBih) + pBi->bmOffBits;
   pCBM->linSize = ((pBi->bmBitsPixel * pBi->bmWidth + 31)>> 5)<< 2;
   pCBM->mlSize  = WORD(((pBi->bmWidth + 31)>> 5)<< 2);
   pCBM->pMsk    = pCBM->pBm + pCBM->linSize * pBi->bmHeight;
   pCBM->bitSize = pBi->bmBitsPixel;
   pCBM->and     = (0xFF00 >> pBi->bmBitsPixel) & 0x00FF;
   pCBM->xor     = pCBM->and & color;
}

static void PixelModify(t_CBM *pCBM, short x, short y)
{
   WORD posb = x * pCBM->bitSize;
   WORD posx = posb >> 3;
   WORD shift= (posb - (posx << 3));
   WORD and  = pCBM->and, xor = pCBM->xor;
   if (shift) and >>= shift, xor >>= shift;
   PBYTE pB  = pCBM->pBm + pCBM->linSize * y + posx;
   *pB = (*pB & ~and) ^ xor;
}

static BOOL GetBitMask(t_CBM *pCBM, short x, short y)
{
   UINT xy = pCBM->mlSize * y + (x >> 3);
   return 1 & (pCBM->pMsk[xy] >> ((7-x) & 7));
}

void IconCursToBm(BM_INFO *pBi, LPBITMAPINFOHEADER pBih, int colorId)
{
   t_CBM cbm;

   SetCBM(&cbm, pBi, pBih, colorId == 2? 0xFFFF: 0);

   for (short y = 0; y < pBi->bmHeight; y++) {
      for (short x = 0; x < pBi->bmWidth; x++) {
         if (GetBitMask(&cbm, x, y))
            PixelModify(&cbm, x, y);
      }
   }
}
///////////////////////////////////////////////////////////////////////////////
// Creation d'une resource toolbar correspondant a celle cree localement

UINT WriteBmToolbarRes(CFile &file, TOOLBARRES *pTool, LPCTSTR resName)
{
   static BYTE toolres[sizeof(TOOLBARRES)+sizeof(WORD)*255];
   DWORD size = sizeof(TOOLBARRES) - sizeof(WORD) + sizeof(WORD) * pTool->cnt;

   if (pTool->cnt > 255 || pTool->cnt <= 0) return FALSE;

   *((TOOLBARRES*)toolres) = *pTool;

   for (WORD i = 1; i < pTool->cnt; i++)
      ((TOOLBARRES*)toolres)->toolId[i] = pTool->toolId[0] + i;

   return WriteResBuffer(file, toolres, size, resName, RT_TOOLBAR);
}
///////////////////////////////////////////////////////////////////////////////
// Creation d'un fichier de donnes brutes a partir d'une resource

UINT WriteRawData(const CString& fnam,
                  HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType,
                  LPCTSTR filter)
{
   DWORD size;
   PVOID pt = RawData(hinst, resName, resType, &size);
   if (pt == NULL) return FALSE;
   if (filter) {
      int len = strlen(filter);
      if (strncmp(filter, (LPCTSTR)pt, len) != 0) return FALSE;
   }
   CFile file(fnam, CFile::modeCreate|CFile::modeWrite);
   file.Write(pt, size);
   file.Close();
   return 0;
}
///////////////////////////////////////////////////////////////////////////////
// Subroutines to create a resource (*.res) type file
///////////////////////////////////////////////////////////////////////////////
// Write resource data buffer

UINT WriteResBuffer(CFile &file,                      // file
                    PVOID pData, DWORD size,          // data buffer
                    LPCTSTR resName, LPCTSTR resType, // resource definition
                    WORD lang)                        // UNICODE support for NLS.
{
   int NameSize = (LenResId(resType) + LenResId(resName) + 3) & ~3;
   int HeaderSize = sizeof(RES) + NameSize + sizeof(RES2);
   PWORD pHeader = (PWORD)malloc(HeaderSize), pW = pHeader;

   memset(pHeader, 0, HeaderSize);
   ((RES*)pW)->HeaderSize = HeaderSize;
   ((RES*)pW)->DataSize = size;

   pW = PWORD(((RES*)pW) + 1);
   WriteResId(pW, resType);
   WriteResId(pW, resName);

   pW = pHeader + (sizeof(RES) + NameSize)/ 2;

   // Resource memory flags.
   ((RES2*)pW)->MemoryFlags = MMF_MOVEABLE|MMF_PURE|MMF_DISCARDABLE;
   ((RES2*)pW)->LanguageId = lang;

   size = (size + 3) & ~3;
   file.Write(pHeader, HeaderSize);
   if (size) file.Write(pData, size);

   free(pHeader);
   return 0;
}
///////////////////////////////////////////////////////////////////////////////
// Copy resource in an (*.res) file

UINT WriteResData(CFile &file,
                  HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   DWORD rsize;
   PVOID pt = RawData(hinst, resName, resType, &rsize);
   if (pt) return WriteResBuffer(file, pt, rsize, resName, resType);
   else return IDS_READERROR;
}
