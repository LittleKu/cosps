// DialogRW.cpp : implementation file
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "DialogRW.h"
#include "Utilities.h"
#include "ResUtilities.h"
#include "ResInfo.h"
#include "MsgDecode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <winuser.h>

/////////////////////////////////////////////////////////////////////////////
// CDialogRW dialog

static PVOID    m_oldData= NULL;
static HWND     m_dstWnd = NULL;
static HWND     m_hDlg   = NULL;
static HWND     m_hAnim  = NULL;
static LPCTSTR  m_lpMenu = NULL;
static HICON    m_hiHand = NULL;
/////////////////////////////////////////////////////////////////////////////
// Affiche la resource dialog

#define styleMask (WS_BORDER|WS_CAPTION|WS_DLGFRAME|WS_HSCROLL|WS_MAXIMIZEBOX|\
     WS_MINIMIZEBOX|WS_SYSMENU|WS_THICKFRAME|WS_VSCROLL|DS_SETFONT|DS_MODALFRAME)
#define dlgStyle (WS_POPUP)

/////////////////////////////////////////////////////////////////////////////
//

void DlgDestroy()
{
   if (m_hAnim) {
      ::ShowWindow(m_hAnim, SW_HIDE);
      ::SendMessage(m_hAnim, WM_DESTROY, 0, 0);
      m_hAnim = NULL;
   }
   if (m_hDlg) {
      ::DestroyWindow(m_hDlg);
      m_hDlg = NULL;
      m_oldData = NULL;
      m_dstWnd = NULL;
   }
   m_oldData = NULL;
}

void DlgSetFocus()
{
   if (m_hDlg) ::SetFocus(m_hDlg);
}

CPoint DlgItemSetFocus(UINT item, HWND hView)
{
   int dx = 0, dy = 0;
   if (m_hDlg && item) {

      // recherche le Window correspondant au numero item
      HWND hWnd = ::GetWindow( m_hDlg, GW_CHILD );
      while ( hWnd && --item ) hWnd = ::GetWindow( hWnd, GW_HWNDNEXT );

      if (hWnd) {
         CRect cr, wr;
         ::GetWindowRect(hWnd,  &cr);     // item rect
         ::GetWindowRect(hView, &wr);     // View rect

         cr.right = cr.left + 40;         // Emplacement a montrer
         cr.bottom = cr.top + 40;         // au minimum la main

         // calcule les deplacements eventuels pour montrer le minimum
         if (cr.left < wr.left) dx = cr.left - wr.left;  // horizontal
         else if (cr.right > wr.right) dx = cr.right - wr.right;
         if (cr.top < wr.top) dy = cr.top - wr.top;      // vertical
         else if (cr.bottom > wr.bottom) dy = cr.bottom - wr.bottom;
      }
   }
   return CPoint(dx, dy);
}

void DlgItemSetFocus(CDC *pDC, UINT item, BOOL hand)
{
   if ( m_hDlg && item ) {
      if (m_hiHand == NULL)
         m_hiHand = (HICON)AfxGetApp()->LoadCursor(IDC_CURSHAND);

      // recherche le Window correspondant au numero item
      HWND hWnd = ::GetWindow( m_hDlg, GW_CHILD );
      while ( hWnd && --item ) hWnd = ::GetWindow( hWnd, GW_HWNDNEXT );

      if (hWnd) {
         CRect cr, wr;
         ::GetWindowRect(hWnd, &cr);         // item rect
         ::GetWindowRect(m_hDlg, &wr);       // Dlg rect
   
         cr += CPoint(5, 5) - wr.TopLeft();
         cr.DeflateRect(2, 2);

         // Wait a end of update
         ((CResExplorer*)AfxGetApp())->WaitEndOfWork();

         // Affiche le FocusRect et la designation de l'objet
         pDC->DrawFocusRect(cr);
         if (hand) pDC->DrawIcon(cr.left, cr.top + 2, m_hiHand);
      }
   }
}

#define AREA(r) ((r.right - r.left) * (r.bottom - r.top))

UINT GetSelectedItem(POINT point)   // point in screen coordinate
{
   UINT found = 0;
   if ( m_hDlg ) {
      RECT r;
      UINT area = UINT_MAX, tmp, item = 1;
      HWND hWnd = ::GetWindow( m_hDlg, GW_CHILD );

      while ( hWnd ) {
         ::GetWindowRect(hWnd, &r);
         // traite le cas des groupbox qui recouvrent plusieurs objets
         if (::PtInRect(&r, point) && (tmp = AREA(r)) < area ) {
            found = item, area = tmp;
         }
         hWnd = ::GetWindow( hWnd, GW_HWNDNEXT );
         item++;
      }
   }
   return found;
}

static POINT toPoint(DWORD dw)
{
   POINT p;
   p.x = LOWORD(dw), p.y = HIWORD(dw);
   return p;
}

static UINT GetDlgCtrlCnt(HWND hWnd, LPARAM lp)
{
   UINT item = 1;
   HWND hwn = ::GetWindow(m_hDlg, GW_CHILD);

   while ( hwn ) {
      if ( hwn == hWnd ) return item;
      hwn = ::GetWindow(hwn, GW_HWNDNEXT);
      item++;
   }
   // cas du menu (place en dernier item)
   if (hWnd == m_hDlg && LOWORD(lp) == 5) return item;
   return 0;
}

static CSize DlgSize()
{
   if (m_hDlg) {
      CRect rect;
      ::GetWindowRect(m_hDlg, rect);
      CPoint br = rect.BottomRight();
      CPoint tl = rect.TopLeft();
      br += CPoint(10,10);
      return CSize(br - tl);
   } else {
      return CSize(10,10);
   }
}

static void SkipString(PWORD& pt)
{
   WORD w;
   if (*pt == 0xFFFF) {
      pt++; w = *pt++;
   } else {
      while (w = *pt++);
   }
}

static LPCTSTR GetClass(HINSTANCE hinst, PWORD& pWord)
{
   WORD w = *pWord;
   if (w == 0xFFFF) {
      w = pWord[1];
      pWord+= 2;
      if (w >= 0x0080 && w <= 0x0085) return LPCTSTR(w);
   } else if (w != 0) {
      WNDCLASSEX ci;
      LPCTSTR className = ReadResId(pWord);

      if ( ::GetClassInfoEx(hinst, className, &ci)
         || ::GetClassInfoEx(NULL, className, &ci) )  return className;
   }
   return NULL;
}

static BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
//   TRACE("DlgProc %08x msg=%-18s wp=%08x lp=%08x\n", hWnd, decode(uMsg), wp, lp);
   static NMHDR nmhdr;
   nmhdr.hwndFrom = hWnd;
   nmhdr.code = NM_SETFOCUS;
   nmhdr.idFrom = 0;

   switch (uMsg) {
   case WM_LBUTTONDOWN: // detecte les item static sous le curseur
      nmhdr.idFrom = GetSelectedItem(toPoint(lp));
      break;

   case WM_SETCURSOR:   // detecte les no item des autres objets
      if (lp & 0x00010000) nmhdr.idFrom = GetDlgCtrlCnt((HWND)wp, lp);
      break;

   case WM_NCHITTEST:
      // pour empecher la capture par la souris et deplacer DlgWindow
      if (::DefWindowProc(hWnd, uMsg, wp, lp) == HTCAPTION) {
         // permet le focus lorqu'on passe sur le header
        //::PostMessage(hWnd, WM_SETFOCUS, 0, 0);
         return TRUE;
      }
      break;
   }

   // pour avertir le parent (designation d'objets)
   if (nmhdr.idFrom) ::SendMessage(m_dstWnd, WM_NOTIFY, 0, (LPARAM)&nmhdr);
   return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// Display CDialog

CSize DlgDisplay(HWND hWnd, HINSTANCE hinst, LPCTSTR resName,
                            CPoint pos, BOOL limited)
{
   DWORD rsize;
   PWORD pWord = (PWORD)RawData(hinst, resName, RT_DIALOG, &rsize);
   PWORD pEnd = PWORD( PBYTE(pWord) + rsize );

   // le code du template est copie et modifie pour permettre l'affichage
   // dans les cas de tous les ojets. Les user class d'objets qui ne peuvent
   // etre affichees (necessitant un traitement de l'executable) sont
   // remplacees par une marque enveloppe

   if (pWord && m_oldData != (PVOID)pWord && m_hDlg == NULL) {
      m_oldData = (PVOID)pWord;     // deja trace
      PVOID pTmpl;
      PWORD pDst, pItem, pWold;
      DWORD style, exStyle;
      HMENU hMenu = NULL;
      UINT id;
      BOOL  exTmpl;  // TRUE=> DLGTEMPLATEEX, DLGITEMTEMPLATEEX
      int   cnt;     // nombre des items

      pTmpl = calloc(1, rsize+10);

      pDst = (PWORD)pTmpl;
      if ( (exTmpl = (((DLGTEMPLATEEX*)pWord)->signature == 0xFFFF)) ) {
         cnt = ((DLGTEMPLATEEX*)pWord)->cDlgItems;
         style = ((DLGTEMPLATEEX*)pWord)->style;
         *((DLGTEMPLATEEX*)pDst) = *((DLGTEMPLATEEX*)pWord);
         ((DLGTEMPLATEEX*)pDst)->style = (style & styleMask) | dlgStyle;
         pWord = (PWORD)((DLGTEMPLATEEX*)pWord + 1);
         pDst  = (PWORD)((DLGTEMPLATEEX*)pDst + 1);

      } else {
         cnt = ((DLGTEMPLATE*)pWord)->cdit;
         style = ((DLGTEMPLATE*)pWord)->style;
         *((DLGTEMPLATE*)pDst) = *((DLGTEMPLATE*)pWord);
         ((DLGTEMPLATE*)pDst)->style = (style & styleMask) | dlgStyle;
         pWord = (PWORD)((DLGTEMPLATE*)pWord + 1);
         pDst  = (PWORD)((DLGTEMPLATE*)pDst + 1);
      }
      if (pWord >= pEnd) return CSize(0,0);
                                             // menu
      *pDst++ = 0;                           // on elimine le menu car provoque une erreur
      if ((m_lpMenu = ReadResId(pWord))
                 && (hMenu = ::LoadMenu(hinst, m_lpMenu))) {
         if (exTmpl) ((DLGTEMPLATEEX*)pTmpl)->cy += 10;
         else ((DLGTEMPLATE*)pTmpl)->cy += 10;
      }

      *pDst++ = 0;                           // on elimine la class car provoque une erreur
      if (*pWord == 0xFFFF) pWord++;
      else        SkipString(pWord);         // class

      pWold = pWord;
      SkipString(pWord);                     // title
      if (style & DS_SETFONT) {
         pWord += (exTmpl? 3: 1);            // font
         SkipString(pWord);
      }
      while (pWord > pWold) *pDst++ = *pWold++;

      // lecture des Dlg Items
      while (--cnt >= 0) {
         pWord = ALIGN_DWORD( PWORD, pWord);
         pDst  = ALIGN_DWORD( PWORD, pDst);
         pWold = pWord;
         pItem = pDst;
         if (pWord >= pEnd) return CSize(0,0);
         if (exTmpl) {
            style  = ((DLGITEMTEMPLATEEX*)pWord)->style;
            exStyle= ((DLGITEMTEMPLATEEX*)pWord)->exStyle;
            id = ((DLGITEMTEMPLATEEX*)pWord)->id;
            pWord  = (PWORD)((DLGITEMTEMPLATEEX*)pWord + 1);
         } else {
            style  = ((DLGITEMTEMPLATE*)pWord)->style;
            exStyle= ((DLGITEMTEMPLATE*)pWord)->dwExtendedStyle;
            id = ((DLGITEMTEMPLATE*)pWord)->id;
            pWord  = (PWORD)((DLGITEMTEMPLATE*)pWord + 1);
         }
         while (pWord > pWold) *pDst++ = *pWold++;

         LPCTSTR Class = GetClass(hinst, pWord);   // class
         SkipString(pWord);                        // title
         WORD dataLen = *pWord++;
         pWord += dataLen / 2;                     // data
         if (Class) {
            if (Class == (LPCTSTR)UBUTTON
             && (style & (BS_OWNERDRAW|BS_CHECKBOX)) == (BS_OWNERDRAW|BS_CHECKBOX)) {
               // rend visible certains boutons
               style &= ~BS_OWNERDRAW;
            }
            while (pWord > pWold) *pDst++ = *pWold++;
         } else {
            // la class n'est pas definie dans le systeme, definit un placeholder
            style = WS_VISIBLE|BS_MULTILINE|BS_FLAT;
            exStyle = 0;
            *pDst++ = 0xFFFF;                      // class button
            *pDst++ = UBUTTON;
                                                   // title
            pDst +=::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "??", -1, pDst, 20);
            *pDst++ = 0;                           // data
         }
         if (exTmpl) {
            ((DLGITEMTEMPLATEEX*)pItem)->style = style;
            ((DLGITEMTEMPLATEEX*)pItem)->exStyle = exStyle;
         } else {
            ((DLGITEMTEMPLATE*)pItem)->style   = style;
            ((DLGITEMTEMPLATE*)pItem)->dwExtendedStyle = exStyle;
         }
      }

      // Creation du CDialog. DlgProc assure les traitements specifiques
      m_dstWnd = hWnd;
      if (limited)
         m_hDlg = ::CreateDialogIndirect( NULL, (DLGTEMPLATE*)pTmpl, NULL, DlgProc );
      else
         m_hDlg = ::CreateDialogIndirect( hinst, (DLGTEMPLATE*)pTmpl, NULL, DlgProc );

      // Wait a end of update
      ((CResExplorer*)AfxGetApp())->WaitEndOfWork();

      if (m_hDlg) {
         // Ajoute les donnees DlgInit
         DLGINITRES *pDlgIn = (DLGINITRES*)RawData(hinst, resName, RT_DLGINIT);
         if (pDlgIn) {
            while (pDlgIn->itemId) {
               ::SendDlgItemMessage(m_hDlg, pDlgIn->itemId,
                                            CB_ADDSTRING, 0, (LPARAM)pDlgIn->data);
               pDlgIn = (DLGINITRES*)(pDlgIn->data + pDlgIn->length);
            }
         }

         if (hMenu) {
            if ( !::SetMenu(m_hDlg, hMenu) ) DisplayErrorBox();
         }
         ::SetParent(m_hDlg, hWnd);
         // Place le Dlg Window et le rend visible
         ::SetWindowPos(m_hDlg, NULL, 5 - pos.x, 5 - pos.y, 0, 0,
                                            SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
      } else DisplayErrorBox();
      delete pTmpl;

   } else {
//      if (m_hDlg) ::SetFocus(m_hDlg);
   }

   return DlgSize();
}
/////////////////////////////////////////////////////////////////////////////
// Affichage d'un Dialog sous forme d'imagette

static void DrawRItem(CDC *pDC, CSize size, CPoint org,
                      short x, short y, short cx, short cy)
{
   int r = size.cy / 2;
   int x0 = org.x + (size.cx * x + r) / size.cy, x1 = x0 + (size.cx * cx + r) / size.cy;
   int y0 = org.y + (size.cx * y + r) / size.cy, y1 = y0 + (size.cx * cy + r) / size.cy;
 
   pDC->MoveTo(x0, y0);
   pDC->LineTo(x1, y0); pDC->LineTo(x1, y1);
   pDC->LineTo(x0, y1); pDC->LineTo(x0, y0);
}

static void DrawSlider(CDC *pDC, CSize size, CPoint org,
                      short x, short y, short cx, short cy, DWORD flg)
{
   int r = size.cy / 2;
   int x0 = org.x + (size.cx * x + r) / size.cy, x1 = x0 + (size.cx * cx + r) / size.cy;
   int y0 = org.y + (size.cx * y + r) / size.cy, y1 = y0 + (size.cx * cy + r) / size.cy;

   if (flg & TBS_VERT) {
      // presentation verticale
      int k = (cx * size.cx / 2 - 1 + r) / size.cy, d = (2 * size.cx + r) / size.cy;

      pDC->MoveTo(x0, y0+d);
      pDC->LineTo(x1, y0+d); pDC->LineTo(x1, y0+d+d);
      pDC->LineTo(x0, y0+d+d); pDC->LineTo(x0, y0+d);

      x0 += k;
      pDC->MoveTo(x0, y0);
      pDC->LineTo(x0+d, y0); pDC->LineTo(x0+d, y1);
      pDC->LineTo(x0, y1); pDC->LineTo(x0, y0);
   } else {
      // presentation horizontale
      int k = (cy * size.cx / 2 - 1 + r) / size.cy, d = (2 * size.cx + r) / size.cy;

      pDC->MoveTo(x0+d, y0);
      pDC->LineTo(x0+d+d, y0); pDC->LineTo(x0+d+d, y1);
      pDC->LineTo(x0+d, y1); pDC->LineTo(x0+d, y0);

      y0 += k;
      pDC->MoveTo(x0, y0);
      pDC->LineTo(x1, y0); pDC->LineTo(x1, y0+d);
      pDC->LineTo(x0, y0+d); pDC->LineTo(x0, y0);
   }
}

static void DrawChkBx(CDC *pDC, CSize size, CPoint org,
                      short x, short y, short cx, short cy, DWORD flg)
{
   int r = size.cy / 2;
   int x0 = org.x + (size.cx * x + r) / size.cy, x1 = x0 + (size.cx * cx + r) / size.cy;
   int y0 = org.y + (size.cx * y + r) / size.cy, y1 = y0 + (size.cx * cy + r) / size.cy;
   int k = (6 * size.cx + r) / size.cy, d = (2 * size.cx + r) / size.cy;

   y0 = (y0 + y1 - k) / 2, y1 = y0 + k;
   if (flg & BS_LEFTTEXT) x1 = x1 - d, x0 = x1 - k;
   else                   x0 = x0 + d, x1 = x0 + k;
  
   if (flg & 0x10) {
      // Radio Box
      pDC->MoveTo(x0, y0+d);
      pDC->LineTo(x0+d, y0); pDC->LineTo(x1-d, y0);
      pDC->LineTo(x1, y0+d); pDC->LineTo(x1, y1-d);
      pDC->LineTo(x1-d, y1); pDC->LineTo(x0+d, y1);
      pDC->LineTo(x0, y1-d); pDC->LineTo(x0, y0+d);
   } else {
      // Check Box
      pDC->MoveTo(x0, y0);
      pDC->LineTo(x1, y0); pDC->LineTo(x1, y1);
      pDC->LineTo(x0, y1); pDC->LineTo(x0, y0);
   }

}

BOOL DlgDisplayFrame(CDC *pDC,
                     HINSTANCE hinst,
                     LPCTSTR resName,
                     DWORD type,
                     CRect &pos)
{
   int cnt, dim = pos.Width();
   DWORD  rSize;
   PWORD  pWord = (PWORD)RawData(hinst, resName, RT_DIALOG, &rSize);
   PWORD  pEnd = PWORD(PBYTE(pWord) + rSize);
   DWORD  style, styleEx;
   BOOL   exTmpl;
   WORD   id;
   CSize  size;
   CPoint org;
   int x, y, dcx, dcy, cx, cy, cap;

   if (pWord == NULL) return FALSE;

   if ( (exTmpl = ((DLGTEMPLATEEX*)pWord)->signature == 0xFFFF) ) {
      cnt    = ((DLGTEMPLATEEX*)pWord)->cDlgItems;
      style  = ((DLGTEMPLATEEX*)pWord)->style;
      styleEx= ((DLGTEMPLATEEX*)pWord)->exStyle;
      dcx    = ((DLGTEMPLATEEX*)pWord)->cx;
      dcy    = ((DLGTEMPLATEEX*)pWord)->cy;
      pWord  = (PWORD)((DLGTEMPLATEEX*)pWord + 1);
   } else {
      cnt    = ((DLGTEMPLATE*)pWord)->cdit;
      style  = ((DLGTEMPLATE*)pWord)->style;
      styleEx= ((DLGTEMPLATE*)pWord)->dwExtendedStyle;
      dcx    = ((DLGTEMPLATE*)pWord)->cx;
      dcy    = ((DLGTEMPLATE*)pWord)->cy;
      pWord  = (PWORD)((DLGTEMPLATE*)pWord + 1);
   }
   if (pWord >= pEnd) return FALSE;
   dcy += (cap = (style & WS_CAPTION)? (styleEx & WS_EX_TOOLWINDOW)? 12: 14: 0);
   size.cx = dim, size.cy = (dcx > dcy)? dcx : dcy;
   if (size.cx >= size.cy) size.cx = size.cy = 1;
   int sx = (dcx * size.cx)/ size.cy, sy = (dcy * size.cx)/ size.cy;
   org.x = (pos.left + pos.right - sx) / 2;
   org.y = (pos.top + pos.bottom - sy) / 2;

   DrawRItem(pDC, size, org, 0, 0, dcx, dcy);
   DrawRItem(pDC, size, org, 0, 0, dcx, cap);

   org.y += (cap * size.cx + size.cy / 2) / size.cy, dcy -= cap;

   SkipString(pWord);                               // menu
   if (*pWord == 0xFFFF) pWord++;
   else        SkipString(pWord);                   // class
   SkipString(pWord);                               // title
   if (style & DS_SETFONT) {
      if (exTmpl) pWord += 3;
      else pWord++;
      SkipString(pWord);
   }

   while (--cnt >= 0) {
      pWord = ALIGN_DWORD( PWORD, pWord);
      if (pWord >= pEnd) return FALSE;

      if (exTmpl) {
         style = ((DLGITEMTEMPLATEEX*)pWord)->style;
         x  = ((DLGITEMTEMPLATEEX*)pWord)->x;
         y  = ((DLGITEMTEMPLATEEX*)pWord)->y;
         cx = ((DLGITEMTEMPLATEEX*)pWord)->cx;
         cy = ((DLGITEMTEMPLATEEX*)pWord)->cy;
         id = ((DLGITEMTEMPLATEEX*)pWord)->id;
         pWord = (PWORD)((DLGITEMTEMPLATEEX*)pWord + 1);
      } else {
         style = ((DLGITEMTEMPLATE*)pWord)->style;
         x  = ((DLGITEMTEMPLATE*)pWord)->x;
         y  = ((DLGITEMTEMPLATE*)pWord)->y;
         cx = ((DLGITEMTEMPLATE*)pWord)->cx;
         cy = ((DLGITEMTEMPLATE*)pWord)->cy;
         id = ((DLGITEMTEMPLATE*)pWord)->id;
         pWord = (PWORD)((DLGITEMTEMPLATE*)pWord + 1);
      }
      // gather the class info
      LPCTSTR classId  = ReadResId(pWord); // class
      t_ClassStyle *pClass = ClassStyle(classId);
      LPCTSTR pRCType = ClassTypeName(pClass, style, TRUE);
      SkipString(pWord);                           // title

      if (x >= 0 && y >= 0 && x <= dcx && y <= dcy && cx >= 0 && cy >= 0) {
         // limite les objets chevauchant les limites
         if ((x + cx) > dcx) cx = dcx - x;
         if ((y + cy) > dcy) cy = dcy - y;

         switch (pClass->image) {
         case RI_COMBOX:
            DrawRItem(pDC, size, org, x, y, cx, 14);
            break;
         case RI_STATIC: // ne sont pas affiches les textes statics
            break;
         case RI_CHECK :
            DrawChkBx(pDC, size, org, x, y, cx, cy, style & BS_LEFTTEXT);
            break;
         case RI_RADIO :
            DrawChkBx(pDC, size, org, x, y, cx, cy,(style & BS_LEFTTEXT)|0x10);
            break;
         case RI_SLIDER:
            DrawSlider(pDC, size, org, x, y, cx, cy, style);
            break;
         default:
            DrawRItem(pDC, size, org, x, y, cx, cy);
         }
      }
      WORD dataLen = *pWord++;
      pWord += dataLen / 2;// data
      if (pWord > pEnd) return FALSE;
   }
   CRect rt = CRect(-pos.Width(), -18, 0, 0) + pos.BottomRight();
   pDC->DrawText( FormatId(resName, type), rt, DT_CENTER );
   return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// Affichage des MENUS

static BOOL CALLBACK DlgMenuProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

CSize MenuDisplay(HWND hWnd, HINSTANCE hinst, LPCTSTR resName, int width)
{
   if (m_hDlg == NULL) {
      // Creation du Dlg sans item qui supportera le menu
      DLGTEMPLATE *pTmpl = (DLGTEMPLATE*)calloc(1, sizeof(DLGTEMPLATE)+10);
      pTmpl->style = WS_POPUP|WS_DLGFRAME;
      pTmpl->cx = 1000;
      pTmpl->cy = 10;

      m_dstWnd = hWnd;         // pour recevoir les messages
      m_hDlg = ::CreateDialogIndirect( NULL, pTmpl, hWnd, DlgMenuProc );

      int itemCnt = -1;                                  // item count
      // Affectation du menu au Dlg
      HMENU hmu = ::LoadMenu(hinst, resName);
      if (hmu && ::SetMenu(m_hDlg, hmu) && (itemCnt = ::GetMenuItemCount(hmu)) > 0) {

         RECT r0, rL;
         ::GetMenuItemRect(m_hDlg, hmu, 0,         &r0); // rectangle 1er item
         ::GetMenuItemRect(m_hDlg, hmu, itemCnt-1, &rL); // rectange dernier item
         int wm = rL.right - r0.left + itemCnt + 18;// largeure necessaire pour tous les items
         int hm = rL.bottom - r0.top + 5;          // hauteur necessaire
         ::SetParent( m_hDlg, hWnd );   // necessaire pour lier le wnd! critique!!!

         // Necessaire pour afficher un menu tout en ayant un dialog fixe a la fenetre
         int x = 5, y = 5;
         int cx = width - 10, cy = 20;
         if (wm < 40) wm = 40;
         if (hm < 22) hm = 22;
         ::SetWindowPos(m_hDlg, NULL, x, y, wm, hm, SWP_NOZORDER|SWP_SHOWWINDOW);

      } else {
         DisplayErrorBox();
      }
      delete pTmpl;
   } else {
      if (m_hDlg) ::SetFocus(m_hDlg);
   }
   return DlgSize();
}

static BOOL CALLBACK DlgMenuProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
   switch (uMsg) {
   case WM_MENUSELECT :       // signale la position du curseur sur un item
      if (wp & (MF_MOUSESELECT<<16)) {
         static NMHDR nmhdr;
         nmhdr.hwndFrom = hWnd;
         nmhdr.idFrom = wp & 0x0000FFFF;
         nmhdr.code = NM_SETFOCUS;
         ::SendMessage(m_dstWnd, WM_NOTIFY, 0, (LPARAM)&nmhdr);
      }
      return 0;
   }
   return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// AVI Display

CSize AniDisplay(HWND hWnd,
                 HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType,
                 CPoint pos) 
{

   static CRect rect;
   if (resName != (LPCTSTR)m_oldData && m_hAnim == NULL) {
      m_oldData = (PVOID)resName;
      m_dstWnd = hWnd;

      // Create the animation control. 
      m_hAnim = Animate_Create(hWnd, IDC_ANIMATE,
                  WS_BORDER|WS_CHILD|ACS_AUTOPLAY|ACS_TRANSPARENT, hinst);
 
      // Position the animation control below the Stop button. 
      SetWindowPos(m_hAnim, 0, pos.x, pos.y, 0, 0, SWP_NOZORDER|SWP_SHOWWINDOW); 

      // Open the AVI clip, and show the animation control. 
      Animate_Open(m_hAnim, resName);
      ::GetClientRect(m_hAnim, &rect);
   }
   return rect.Size(); 
}

void AnimErase()
{
   if (m_hAnim) {
//      Animate_Close(m_hAnim);
//      ::SendMessage(m_hAnim, WM_DESTROY, 0, 0);
//      m_hAnim = NULL;
   }
}

