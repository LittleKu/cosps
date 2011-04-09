// DrawView.cpp : implementation of the CDrawView class
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "BitmapRW.h"
#include "DialogRW.h"
#include "MsgDecode.h"
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h >       // MFC OLE automation classes
#include <afxpriv.h>

#include "ResDoc.h"
#include "DrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CreateAnimationCtrl(HWND hwndDlg, int nIDCtl, HINSTANCE hinst, LPCTSTR resName); 
/////////////////////////////////////////////////////////////////////////////
// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawView, CScrollView)
	//{{AFX_MSG_MAP(CDrawView)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDDV_COPY, OnEditCopy)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_ICON_BKCOLOR, ID_ICON_BKCOLOR2, OnIconBkcolor)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawView construction/destruction

CDrawView::CDrawView()
{
   m_pParam = NULL;
   m_font.CreatePointFont(86, "Tahoma");
   m_enCopy = FALSE;
   m_select = 0;
   m_nbImage= 8;
   m_timer  = 0;
   m_diag   = FALSE;
   m_rDraw  = 0;
   m_invalid= TRUE;
   m_cbItem = NULL;
}

CDrawView::~CDrawView()
{
   AnimErase();
}
/////////////////////////////////////////////////////////////////////////////
// CDrawView drawing

void CDrawView::OnDraw(CDC* pDC)
{
   if (m_pParam == NULL || m_pParam->pRID == NULL) return;
   LPCTSTR resName = m_pParam->resName;
   LPCTSTR resType = m_pParam->resType;
   HINSTANCE hLib  = m_pParam->hLib;
   m_type          = m_pParam->pRID->type;
   if (hLib == NULL) return;

   CSize sizeTotal(100, 100);
   CRect pos(0,0,0,0);
   CFont *pFont = pDC->SelectObject(&m_font);
   m_objects.RemoveAll();
   m_enCopy = FALSE;
   IPicture *pIPicture;

   switch (m_pParam->pRID->wId) {
   case RT_BITMAP:
      if (resName) {
         TOOLBARRES *pTool = &GetDocument()->m_toolbar;
         pos = DrawBitmap(pDC, hLib, resName, pTool->x);
         if (pTool->cnt > 0) {
            pos = DrawToolbar(pDC, hLib, resName, CPoint(5, pos.bottom+10),
                                                           &m_objects, pTool);
            m_invalid = TRUE;
         } else m_objects.Add(pos, resName, 3);
         m_enCopy = TRUE;
         sizeTotal = CSize(pos.right + 10, pos.bottom + 10);
      } else {
         HTREEITEM hit = NULL;
         int nb = (GetDocument()->m_largeIcon)? (m_nbImage*2)/3: m_nbImage;
         do {
            resName = m_pParam->NextResource(hit);
            sizeTotal = NextPosition(m_rDraw, pos, nb);
            DrawBitmap(pDC, hLib, resName, pos);
            m_objects.Add(pos, resName);
         } while (hit);
      }
      break;
   case RT_GROUP_ICON:
   case RT_GROUP_CURSOR:
      if (resName) {
         if (m_pParam->item == 0) {
            UINT selItem = LookupIconCursId(hLib, resName, resType);
            m_objects.SetSelect(LPCTSTR(selItem));
            m_pParam->item = selItem;
         }
         sizeTotal = DrawIconCurs(pDC, hLib, resName, resType, 100, &m_objects);
         m_enCopy = TRUE;
      } else {
         HTREEITEM hit = NULL;
         do {
            resName = m_pParam->NextResource(hit);
            sizeTotal = NextPosition(m_rDraw, pos, m_nbImage);
            if (resType == RT_GROUP_ICON)
               DrawIcon(pDC, hLib, resName, pos);
            else
               DrawCursor(pDC, hLib, resName, pos);
            m_objects.Add(pos, resName);
         } while (hit);
      }
      break;
   case RT_TOOLBAR:
      if (resName) {
         pos = DrawToolbar(pDC, hLib, resName, CPoint(5,5), &m_objects);
         sizeTotal = pos.BottomRight() + CPoint(5,5);
      } else {
         HTREEITEM hit = NULL;
         int cx = 0;
         do {
            resName = m_pParam->NextResource(hit);
            pos = DrawToolbar(pDC, hLib, resName, CPoint(5, pos.bottom + 5));
            m_objects.Add(pos, resName);
            if (pos.right > cx) cx = pos.right;
         } while (hit);
         sizeTotal = CSize(cx + 5, pos.bottom + 5);
      }
      break;
   case RT_DIALOG:
      if (resName) {
         sizeTotal = DlgDisplay(m_hWnd, hLib, resName,
                                 GetScrollPosition(), m_pParam->limited);
         DlgItemSetFocus(pDC, m_pParam->item, m_hand);
      } else {
         HTREEITEM hit = NULL;
         int cx = 0;
         do {
            resName = m_pParam->NextResource(hit);
            sizeTotal = NextPosition(m_rDraw, pos, 4);
            DlgDisplayFrame(pDC, hLib, resName, m_type, pos);
            m_objects.Add(pos, resName);
         } while (hit);
      }
      break;
   case RT_MENU:
      ScrollToPosition(CPoint(0,0));
      if (resName) {
         sizeTotal = MenuDisplay(m_hWnd, hLib, resName, m_rDraw);
      }
      break;
   case RT_DLGINIT:
      if (m_pParam->resName) {
         sizeTotal = DlgDisplay(m_hWnd, hLib, resName,
                                 GetScrollPosition(), m_pParam->limited);
      }
      break;
   case RT_STRING:
   case RT_ACCELERATOR:
   case RT_VERSION:
      break;

   case RT_ANICURSOR:
   case RT_ANIICON:
      if (m_pParam->pAni) {
         sizeTotal = DrawAniIcon(pDC, m_pParam->pAni, m_pParam->item);
      }
      break;
   case rt_avi:
      sizeTotal = AniDisplay(m_hWnd, hLib, resName, resType,
                                          CPoint(10,10)) + CPoint(20, 20);
      break;
   case rt_reg:
   case rt_txt:
   case rt_rtf:
   case RT_HTML:
   case RT_XML:
      // This fonction is not use, it's CEdtView that make this
      sizeTotal = DrawText(pDC, hLib, resName, resType);
      break;
   case rt_bmp:
      // Bitmap ecrite dans resource au format fichier
      sizeTotal = DrawBitmap(pDC, hLib, resName, resType);
      break;
   case rt_bmx:
   case rt_jpg:
   case rt_gif:
      if (pIPicture = Picture(hLib, resName, resType)) {
         pos = IPictureShow(pDC, pIPicture, CPoint(10,10));
         sizeTotal = CSize(pos.right + 10, pos.bottom + 10);
         m_objects.Add(pos, resName, 3);
      }
      break;
   }
   if (m_invalid) {
	   SetScrollSizes(MM_TEXT, sizeTotal);
      m_invalid = FALSE;
   }
   pDC->SelectObject(pFont);
}

CSize CDrawView::DrawText(CDC *pDC, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   DWORD rsize;
   LPCTSTR pTxt = (LPCTSTR)RawData(hinst, resName, resType, &rsize);
   CRect rect(5,5,5,5);
   if (pTxt) {
      pDC->DrawText(pTxt, rsize, &rect, DT_CALCRECT|DT_EXPANDTABS);
      pDC->DrawText(pTxt, rsize, &rect, DT_EXPANDTABS);
   }
   return rect.Size() + CSize(5,5);
}

void CDrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

   SetWindowContextHelpId(0x20000+IDR_DRAWVIEW);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView printing

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CResDoc* CDrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResDoc)));
	return (CResDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawView message handlers

void CDrawView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   if (pHint) m_pParam = (CParams*)pHint;
   m_diag = TRUE;
   if (m_pParam) {
      if (lHint & RES_VIEW) {
         DlgDestroy();
         m_objects.RemoveAll();
         CRect r;
         GetClientRect(r);
         m_rDraw = r.right - 20;
         m_invalid = TRUE;
         ScrollToPosition(CPoint(0,0));

         SetWindowContextHelpId(0x20000 + (m_pParam->resName? IDR_DRAWVIEW: IDR_LISTVIEW));

         Invalidate();
      }
      if (lHint & RES_VIEW_ITEM) {
         // Selectionne l'objet designe
         UINT item = m_pParam->item;

         m_objects.SetSelect((LPCTSTR)item);
         CPoint scr = DlgItemSetFocus(item, m_hWnd);
         if (scr.x | scr.y) {
            
            ScrollToPosition( GetScrollPosition() + scr );
         }
         m_hand = TRUE;
         Invalidate();
      }
   }
}

BOOL CDrawView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
   // Reception des messages issus des CDlg (dialogues ou menus)
   NMHDR *pNMHdr = (NMHDR*)lParam;
   GetDocument()->SelectItem(this, pNMHdr->idFrom);

   m_hand = FALSE;
   Invalidate();

	return CScrollView::OnNotify(wParam, lParam, pResult);
}
///////////////////////////////////////////////////////////////////////////////
// Copy/past or Drag/drop operations

void CDrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   m_nFlags = 0xFFFF;
   point += GetScrollPosition();
   m_cbItem = m_objects.GetObj(point);

	CScrollView::OnLButtonDown(nFlags, point);

   if (m_cbItem && m_pParam) {
      LPARAM view = 0;

      switch (m_objects.Type()) {
      case 0 :
         m_pParam->resName = m_cbItem, m_pParam->item = 0;
         view = LEFT_VIEW_SELECT|LIST_VIEW|RES_VIEW;
         break;
      case 1 :
      case 2 :
         // to detect drag and drop operations
         if (m_pParam->resName) m_nFlags = nFlags;
         m_pParam->item = (DWORD)m_cbItem;
         view = LIST_VIEW_ITEM;
         Invalidate();
         break;
      case 3 :
         // to detect drag and drop operations
         if (m_pParam->resName) m_nFlags = nFlags;
         break;
      }

      if (view) GetDocument()->UpdateAllViews(NULL, view);
   }
}

void CDrawView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   m_nFlags = 0xFFFF;

   switch (m_objects.Type()) {
   case 1 :
   case 2 :
   case 3 :
      // to detect copy/past and drag/drop operations
      if (m_pParam->resName) m_nFlags = nFlags;
      point += GetScrollPosition();
      m_cbItem = m_objects.GetObj(point);
      break;
   }
	CScrollView::OnRButtonDown(nFlags, point);
}

void CDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CScrollView::OnMouseMove(nFlags, point);
   if (m_nFlags == nFlags) {
      TRACE("CDrawView::OnMouseMove\n");
      // Drag/Drop operation
      CacheBitmapCopy(FALSE);
   }
   m_nFlags = 0xFFFFF;
}

void CDrawView::CacheBitmapCopy(BOOL cb)
{
   // It is used either during Drag/Drop operation
   // or during a Copy/Paste operation
   if (m_cbItem && m_pParam) {
      TOOLBARRES *pTool = &GetDocument()->m_toolbar;
      CSharedFile sf( GMEM_MOVEABLE|GMEM_DDESHARE );
      LPCTSTR resName = m_pParam->resName;
      LPCTSTR resType = m_pParam->resType;
      HINSTANCE hLib  = m_pParam->hLib;
      HGLOBAL hGlob = NULL;
      PVOID pVoid = NULL;
      CBuffer buf;
      CRect rect;
      BM_INFO bmi;

      TRACE("CDrawView::CacheBitmapCopy %08x\n", m_cbItem);

      // Select a good bitmap format for a resource type
      switch ( m_pParam->pRID->wId ) {
      case RT_BITMAP :
         if (pTool->cnt > 0) {
            rect = GetToolRect(NULL, NULL, m_cbItem, pTool);
            pVoid = buf.CutBmp(hLib, resName, rect, &bmi);
         } else pVoid = GetImageInfo(hLib, m_cbItem, resType, &bmi);
         break;
      case RT_TOOLBAR :
         rect = GetToolRect(hLib, resName, m_cbItem, NULL);
         pVoid = buf.CutBmp(hLib, resName, rect, &bmi);
         break;
      case RT_GROUP_ICON :
      case RT_GROUP_CURSOR :
         pVoid = GetImageInfo(hLib, m_cbItem, resType-DIFFERENCE, &bmi);
         pVoid = buf.Convert( (LPBITMAPINFOHEADER)pVoid, &bmi );
         break;
      case rt_bmp :
         pVoid = GetImageInfo(hLib, m_cbItem, resType, &bmi);
         break;
      case rt_bmx :
      case rt_jpg :
      case rt_gif :
         hGlob = IPictureToHGlobal(Picture(hLib, m_cbItem, resType), NULL);
         break;
      }

      if (pVoid) {   // Make a hGlobal ptr
         sf.Write( pVoid, bmi.bmSize );
         hGlob = sf.Detach();
      }
 
      if (hGlob) {
         // The cache for Clipboard or drag-and-drop operations.
         COleDataSource* pSource = new COleDataSource();

         if (pSource) {
            pSource->CacheGlobalData( CF_DIB, hGlob );
            if (cb) {
               pSource->SetClipboard();
            } else {
               pSource->DoDragDrop();
               delete pSource;
            }
         }
      } else AfxMessageBox(IDS_CBWRITEERROR);
      m_cbItem = NULL;
   }
}

void CDrawView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   if (m_cbItem && m_objects.Type() > 0) {
      CMenu menu; // Load float edit menu
      menu.LoadMenu( IDR_DVCOPYPAST );
      CMenu *pSbm = menu.GetSubMenu(0);

      // Update check marks
      if (colorId >= 0 && colorId < 3) 
         pSbm->CheckMenuItem(ID_ICON_BKCOLOR+colorId, MF_BYCOMMAND|MF_CHECKED);

      // Enable entries
      if (m_pParam && (m_pParam->resType == RT_GROUP_ICON
         || m_pParam->resType == RT_GROUP_CURSOR)) {
         for (UINT id = ID_ICON_BKCOLOR; id <= ID_ICON_BKCOLOR2; id++)
            pSbm->EnableMenuItem(id, MF_BYCOMMAND|MF_ENABLED);
      }

      // Display a menu at the cursor position
      pSbm->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x,point.y, pWnd);
   }
}

void CDrawView::OnIconBkcolor(UINT id) { colorId = id - ID_ICON_BKCOLOR; }

void CDrawView::OnEditCopy() { CacheBitmapCopy(TRUE); }
///////////////////////////////////////////////////////////////////////////////
// CUROR et ICON animes

CSize CDrawView::DrawAniIcon(CDC *pDC, ANI_ICON *pAni, DWORD item)
{
   CSize size(10, 10);
   WORD n, x0, id = (WORD)(item & 0xF0FF);
   if ((item & 0xFFFFF000) == 0x00001000) {
      if (m_timer) {
         KillTimer(m_timer);
         m_timer = 0;
      }
      
      if ( (n = WORD(item & 0x000000FF)) < pAni->count && pAni->icon[n].pData ) 
         size = DrawIconCurs(pDC, id, pAni->icon[n].pData, 100, &m_objects);

   } else if (item == 0x00002000 || item == 0) {
      if (pAni->type == 0) {
         pAni->type = 1;
         for (n = 0; n < pAni->count; n++) {
            PBYTE pB = pAni->icon[n].pData;
            if (pB) {
               int ofs = ::LookupIconIdFromDirectory(pB, TRUE);
               pB += ofs;
               pAni->icon[n].hIcon = ::CreateIconFromResource(pB,
                                            pAni->icon[n].size, TRUE, 0x00030000);
            }
         }
      }

      m_timer = SetTimer(100, 100, NULL);

      for (n = 0, x0 = 30; n < pAni->count; n++, x0 += 50) {
         CRect r(0, 0, 32, 32);
         r.OffsetRect(x0 - 16, 110);
         if (pAni->icon[n].hIcon) pDC->DrawIcon(r.TopLeft(), pAni->icon[n].hIcon);
         m_objects.Add(r, LPCTSTR(0x1000 | n), 2);
      }

      size.cx = pAni->count * 50 + 20;
      size.cy = 220;
   }
   return size;
}

void CDrawView::OnTimer(UINT nIDEvent) 
{
   static WORD n = 0;
   if (m_pParam && m_pParam->pAni) {
      if (n >= m_pParam->pAni->count) n = 0;
      if (m_pParam->pAni->icon[n].hIcon) {
         CClientDC dc(this);
         CPoint org = CPoint(44, 44) - GetScrollPosition();
         dc.FillSolidRect(org.x-2, org.y-2, 36, 36, 0xFFFFFF);
         dc.DrawIcon(org, m_pParam->pAni->icon[n].hIcon);
         n++;
      }
   } else if (m_timer) {
      KillTimer(m_timer);
      m_timer = 0;
   }
	CScrollView::OnTimer(nIDEvent);
}
