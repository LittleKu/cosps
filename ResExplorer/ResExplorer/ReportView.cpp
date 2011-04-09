// ReportView.cpp : implementation file
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "ReportView.h"
#include "ResDoc.h"
#include "BitmapRW.h"
#include "VersionRD.h"
#include "SymbolDlg.h"
#include "Utilities.h"
#include <mmsystem.h>
#include <AVIFMT.H>
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h >       // MFC OLE automation classes
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportView

static CNodTbl *styleTypeTbl[255];
CNodTbl CReportView::m_typeDlg[1] = { CNodTbl(styleTypeTbl, 0, 255) };
BOOL CReportView::m_details = FALSE;

IMPLEMENT_DYNCREATE(CReportView, CListView)

CReportView::CReportView()
{
   m_pParam = NULL;
   m_pAicon = NULL;
   m_menuItem = 0;
   m_trueText = TRUE;
   m_quotText = FALSE;
}

CReportView::~CReportView()
{
}

BOOL CReportView::PreCreateWindow(CREATESTRUCT& cs) 
{
   cs.style |= LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS|HDS_FULLDRAG;
	
	return CListView::PreCreateWindow(cs);
}

BOOL CReportView::PreTranslateMessage(MSG* pMsg) 
{
	m_toolView.RelayEvent(pMsg);
	return CListView::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CReportView, CListView)
	//{{AFX_MSG_MAP(CReportView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINRDRAG, OnBegindrag)
	ON_COMMAND(ID_EDRV_COPY, OnEditCopy)
	ON_COMMAND(ID_EDRV_QUOT, OnEdrvQuot)
	ON_COMMAND(ID_EDRV_TRUE, OnEdrvTrue)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
   ON_MESSAGE(TTM_GETTEXT, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportView diagnostics

#ifdef _DEBUG
void CReportView::AssertValid() const
{
	CListView::AssertValid();
}

void CReportView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CResDoc* CReportView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResDoc)));
	return (CResDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportView message handlers

int CReportView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
   CListCtrl &list = GetListCtrl();
   list.SetImageList( &m_image, LVSIL_SMALL );
	list.SetExtendedStyle(list.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
   m_toolView.Create(this, &list);

   SetWindowContextHelpId(0x20000 + IDR_REPORTVIEW);

	return 0;
}

LRESULT CReportView::OnToolTipText(WPARAM id, LPARAM lParam)
{
   static CString msg;
   TOOLINFO* pInfo = (TOOLINFO*)lParam;
   int iItem = HIWORD(id), iSubItem = LOWORD(id);

   if (m_pParam) {

      if (iItem == 0xFFFF) {
         // First, get tooltips for the List Header
         msg.LoadString(IDS_TTPSREPORTHD);
      } else {
         // Second, get special tooltips (Style and ExStyle for RT_DIALOG for example)
         msg = m_typeDlg->GetSring(&GetListCtrl(), iItem, iSubItem);
         if (msg.IsEmpty()) {
            LPCTSTR resName = m_pParam->resName;

            // Three, get general tooltips (depending restType, iItem and/or iSubItem)
            switch (DWORD(m_pParam->resType)) {
            case RT_DIALOG:
               if (resName == 0)    msg.LoadString(IDS_TTPSDLGGLIST);
               else if (iItem == 0) msg.LoadString(IDS_TTPSDLGLLIST0);
               else                 msg.LoadString(IDS_TTPSDLGLLIST);
               break;
            case RT_MENU:
               if (resName == 0)    msg.LoadString(IDS_TTPSMENGLIST);
               else                 msg.LoadString(IDS_TTPSMENLLIST);
               break;
            }
         }
      }
      pInfo->lpszText = msg.GetBuffer(0);
   }
   return 0;
}

void CReportView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CListCtrl& list = GetListCtrl();
   CResDoc* pDoc = GetDocument();
   int iItem = ((NM_LISTVIEW*)pNMHDR)->iItem;
   int iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
   if (iItem >= 0) {
      DWORD data = list.GetItemData( iItem );
      if (data) {
         m_pParam->symbol = list.GetItemText(iItem, iSubItem);
         if ( data == m_menuItem ) {
            pDoc->SetCurrent( NULL, RT_MENU, m_lpMenu,
                                        RES_VIEW|LIST_VIEW|LEFT_VIEW_SELECT );
         } else if ( m_pParam->resName ) {
            pDoc->SelectItem( this, data );
         } else {
            pDoc->SetCurrent( NULL, m_pParam->resType, (LPCTSTR)data,
                                        RES_VIEW|LIST_VIEW|LEFT_VIEW_SELECT );
         }
      }
   }
   *pResult = 0;
}

void CReportView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
   // Performs a drag-and-drop operation with the selected text
   CacheTextCopy(FALSE);

	*pResult = 0;
}

void CReportView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   if ( GetListCtrl().GetFirstSelectedItemPosition() ) {
      CMenu menu; // Load float edit menu
      menu.LoadMenu( IDR_RVCOPYPAST );
      CMenu *pSbm = menu.GetSubMenu(0);

      // Update check marks
      if (m_quotText) pSbm->CheckMenuItem(ID_EDRV_QUOT, MF_BYCOMMAND|MF_CHECKED);
      if (m_trueText) pSbm->CheckMenuItem(ID_EDRV_TRUE, MF_BYCOMMAND|MF_CHECKED);

      // Display a menu at the cursor position
      pSbm->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x,point.y, this);
   }
}

// Commands to modifie check marks
void CReportView::OnEdrvQuot() { m_quotText = ! m_quotText; }
void CReportView::OnEdrvTrue() { m_trueText = ! m_trueText;	}

void CReportView::OnEditCopy() { CacheTextCopy(TRUE); }
///////////////////////////////////////////////////////////////////////////////
// Virtual Functions

void CReportView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   static LVFINDINFO fi;
   CListCtrl &list = GetListCtrl();
   m_pWave = NULL;

   if (pHint) m_pParam = (CParams*)pHint;

   if (m_pParam && m_pParam->pRID) {
      if (m_hWnd && (lHint & LIST_VIEW)) {

         list.DeleteAllItems();
         m_pAicon = NULL;
         m_menuItem = 0;

         InitListHeader();
         LPCTSTR resName = m_pParam->resName;
         LPCTSTR resType = m_pParam->resType;
         HINSTANCE hLib  = m_pParam->hLib;
         if (hLib) {
            DWORD dwHId = 0x20000 + (resName? IDR_REPORTVIEW: IDR_REPORTVIEWLS);

            SetWindowContextHelpId(dwHId);
            ModifyStyle(0, LVS_SINGLESEL);
            m_type = m_pParam->pRID->type;

            switch ( m_pParam->pRID->wId ) {
            case RT_DIALOG :    DialogList(hLib, resName); break;
            case RT_TOOLBAR:    ToolbarList(hLib, resName); break;
            case RT_MENU :      MenuList( hLib, resName); break;
            case RT_STRING :    StringList(hLib, resName); break;
            case RT_BITMAP :    ImagesList(hLib, resName); break;
            case RT_GROUP_CURSOR:
            case RT_GROUP_ICON: IconesList(hLib, resName, resType); break;
            case RT_DLGINIT:    DlgInitList(hLib, resName); break;
            case RT_VERSION :   VersionList(hLib, resName); break;
            case RT_ACCELERATOR:AccelList( hLib, resName); break;
            case RT_ANICURSOR:
            case RT_ANIICON:
            case rt_avi :
            case rt_wav :       InfoRIFF(hLib, resName, resType); break;
            default:            HexaList(hLib, resType, resName);
            }

            m_pParam->FreeAni( m_pAicon );

            // Set reasonable widths for our columns
            ColumnWidthAdjust(list);
         }
      }

      if ( lHint & LIST_VIEW_ITEM && m_pParam->item ) {

         // selectionne l'item designe
         fi.flags = LVFI_PARAM;
         fi.lParam = m_pParam->item;

         int index = list.FindItem(&fi);
         if (index >= 0) {
            list.EnsureVisible(index, FALSE);
            list.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
         }
      }
      if ( lHint & LIST_VIEW_SYMBOL ) {
         SearchSymbol(m_pParam->symbol);
      }
   }
}
///////////////////////////////////////////////////////////////////////////////
// Subroutines

void CReportView::CacheTextCopy(BOOL clipboard)
{
   // It is used either during Drag/Drop operation
   // or during a Copy/Paste operation
   CString text;
   CListCtrl& list = GetListCtrl();
   int nCol = list.GetHeaderCtrl()->GetItemCount();
   POSITION pos = list.GetFirstSelectedItemPosition();

   while (pos) {
      int item = list.GetNextSelectedItem(pos);

      for (int n = 0; n < nCol; n++) {
         CString tmp = list.GetItemText(item, n);

         if (!ExtractTrueStr(tmp, m_trueText, m_quotText)) tmp = "-";
         if (n != 0) text += " \t";
         text += tmp;
      }
      text += "\r\n";
   }

   if (!text.IsEmpty()) {
      CSharedFile	sfile( GMEM_MOVEABLE|GMEM_DDESHARE );
      COleDataSource* pSource = new COleDataSource();

      sfile.Write( text, text.GetLength() + 1 );

      if (pSource) {
         pSource->CacheGlobalData( CF_TEXT, sfile.Detach() );
         if (clipboard) {
            pSource->SetClipboard();
         } else {
            pSource->DoDragDrop(DROPEFFECT_COPY);
            delete pSource;
         }
      }
   }
}
///////////////////////////////////////////////////////////////////////////////
// Initialisation du header (titre des colonnes)

void CReportView::InitListHeader()
{
#define PT_IS_NULL(pt) ((BOOL)(pt == NULL)) 
   CListCtrl& list = GetListCtrl();
   CString inId;
   LPCTSTR resName = m_pParam->resName;
   static LPCTSTR rType = NULL;
   static BOOL rNameNull= FALSE;
   
   m_typeDlg->EntryDelete();

   if ( rType == m_pParam->resType  && PT_IS_NULL(resName) ==  rNameNull)
      return;

   rNameNull = PT_IS_NULL(resName);
   rType = m_pParam->resType;

   list.GetHeaderCtrl()->SetWindowContextHelpId(0x20000 + IDR_REPORTVIEWHD);
   m_toolView.ListChange();

   switch (m_pParam->pRID->wId) {
   case RT_DIALOG :     InitHeader(list, resName? IDS_RER_DIALOG: IDS_REX_DIALOG); break;
   case RT_MENU :       InitHeader(list, resName? IDS_RER_MENU:   IDS_REX_MENU);   break;
   case RT_STRING :     InitHeader(list, IDS_RER_STRING); break;
   case RT_BITMAP :     InitHeader(list, IDS_RER_BITMAP); break;
   case RT_GROUP_CURSOR:InitHeader(list, IDS_RER_CURSOR); break;
   case RT_GROUP_ICON:  InitHeader(list, IDS_RER_ICON); break;
   case RT_DLGINIT :    InitHeader(list, IDS_RER_DLGINIT); break; // ComboBox Data
   case RT_TOOLBAR :    InitHeader(list, resName? IDS_RER_TOOLBAR:IDS_REX_TOOLBAR);break;
   case RT_VERSION :    InitHeader(list, IDS_RER_VERSION); break;
   case RT_ACCELERATOR: InitHeader(list, resName? IDS_RER_ACCEL:  IDS_REX_ACCEL); break;
   case RT_ANICURSOR:
   case RT_ANIICON:
   case rt_avi :
   case rt_wav :        InitHeader(list, IDS_RER_ANI); break;
   default:             InitHeader(list, IDS_RER_DEFAULT); break;
   }
}
/////////////////////////////////////////////////////////////////////////////
// Search a requested symbol in a displayed list

BOOL CReportView::SearchSymbol(const CString &symbol)
{
   static LVFINDINFO fi;
   CResDoc *pDoc = GetDocument();
   CListCtrl& list = GetListCtrl();
   int ix, cnt, ccnt, si;

   // selectionne l'item designe
   fi.flags = LVFI_STRING;
   fi.psz = symbol;

   ix = list.FindItem(&fi);
   if (ix >= 0) {
      if (symbol.Left(4).Compare("IDS_") == 0) {
         // string resource
         list.EnsureVisible(ix, FALSE);
         list.SetItemState(ix, LVIS_SELECTED, LVIS_SELECTED);
      } else {
         // other resource
         pDoc->SelectItem( NULL, list.GetItemData(ix) );
      }
      return TRUE;
   }

   // search in subitems
   cnt = list.GetItemCount();
   ccnt = list.GetHeaderCtrl()->GetItemCount();
   for (ix = 0; ix < cnt; ix++) {
      for (si = 1; si < ccnt; si++) {
         if (symbol.Compare(list.GetItemText(ix, si)) != 0) continue;
         pDoc->SelectItem( NULL, list.GetItemData(ix) );
         return TRUE;
      }
   }
   return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// List des items de la boite de dialogue affichee

void CReportView::DialogList(HINSTANCE hinst, LPCTSTR resName)
{
   CResDoc *pDoc = GetDocument();
   CString namTxt, clsTxt;
   CListCtrl& list = GetListCtrl();
   DWORD rsize, helpID;
   UINT   id;
   int no, cnt, x, y, cx, cy;
   DWORD  style, styleEx;
   LPCTSTR nameId;

   if (resName) {
      // CDialog items list
      PWORD pWord = (PWORD)RawData(hinst, resName, RT_DIALOG, &rsize);
      PWORD pEnd = PWORD( PBYTE(pWord) + rsize );
      bool  exTmpl;

      if (pWord == NULL) return;

      if ( (exTmpl =((DLGTEMPLATEEX*)pWord)->signature == 0xFFFF) ) {
         cnt    = ((DLGTEMPLATEEX*)pWord)->cDlgItems;
         style  = ((DLGTEMPLATEEX*)pWord)->style;
         styleEx= ((DLGTEMPLATEEX*)pWord)->exStyle;
         helpID = ((DLGTEMPLATEEX*)pWord)->helpID;
         x      = ((DLGTEMPLATEEX*)pWord)->x;
         y      = ((DLGTEMPLATEEX*)pWord)->y;
         cx     = ((DLGTEMPLATEEX*)pWord)->cx;
         cy     = ((DLGTEMPLATEEX*)pWord)->cy;
         pWord  = (PWORD)((DLGTEMPLATEEX*)pWord + 1);
      } else {
         cnt    = ((DLGTEMPLATE*)pWord)->cdit;
         style  = ((DLGTEMPLATE*)pWord)->style;
         styleEx= ((DLGTEMPLATE*)pWord)->dwExtendedStyle;
         helpID = 0;
         x      = ((DLGTEMPLATE*)pWord)->x;
         y      = ((DLGTEMPLATE*)pWord)->y;
         cx     = ((DLGTEMPLATE*)pWord)->cx;
         cy     = ((DLGTEMPLATE*)pWord)->cy;
         pWord  = (PWORD)((DLGTEMPLATE*)pWord + 1);
      }
      if (pWord >= pEnd) return;
      m_lpMenu  = ReadResId(pWord);                      // menu

      if (*pWord == 0xFFFF) pWord++, clsTxt = "";
      else clsTxt = ReadString(pWord);                   // class
      if (exTmpl) clsTxt += "/Ex";

      namTxt = StrFilter(ReadString(pWord), FALSE);      // title

      if (style & DS_SETFONT) {                          // font
         if (exTmpl) pWord += 3; else pWord++;
         SkipResId(pWord);
      }
      no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
      list.SetItemText(no, 1, FormatHexa(style));
      list.SetItemText(no, 2, FormatHexa(styleEx));
      list.SetItemText(no, 3, FormatDeci(x));
      list.SetItemText(no, 4, FormatDeci(y));
      list.SetItemText(no, 5, FormatDeci(cx));
      list.SetItemText(no, 6, FormatDeci(cy));
      list.SetItemText(no, 7, clsTxt);
      list.SetItemText(no, 8, FormatId(helpID, TYP_HIR));
      list.SetItemText(no, 9, namTxt);

      // Help and tooltips on this line
      m_typeDlg->SetEntry(&DlgStyle, no);

      while (--cnt >= 0) {
         // Lecture composants du CDialog
         pWord = ALIGN_DWORD( PWORD, pWord );
         if (pWord >= pEnd) return;
         if (exTmpl) {
            id     = ((DLGITEMTEMPLATEEX*)pWord)->id;
            style  = ((DLGITEMTEMPLATEEX*)pWord)->style;
            styleEx= ((DLGITEMTEMPLATEEX*)pWord)->exStyle;
            helpID = ((DLGITEMTEMPLATEEX*)pWord)->helpID;
            x      = ((DLGITEMTEMPLATEEX*)pWord)->x;
            y      = ((DLGITEMTEMPLATEEX*)pWord)->y;
            cx     = ((DLGITEMTEMPLATEEX*)pWord)->cx;
            cy     = ((DLGITEMTEMPLATEEX*)pWord)->cy;
            pWord  = (PWORD)((DLGITEMTEMPLATEEX*)pWord + 1);
         } else {
            id     = ((DLGITEMTEMPLATE*)pWord)->id;
            style  = ((DLGITEMTEMPLATE*)pWord)->style;
            styleEx= ((DLGITEMTEMPLATE*)pWord)->dwExtendedStyle;
            helpID = 0;
            x      = ((DLGITEMTEMPLATE*)pWord)->x;
            y      = ((DLGITEMTEMPLATE*)pWord)->y;
            cx     = ((DLGITEMTEMPLATE*)pWord)->cx;
            cy     = ((DLGITEMTEMPLATE*)pWord)->cy;
            pWord  = (PWORD)((DLGITEMTEMPLATE*)pWord + 1);
         }
         LPCTSTR classId  = ReadResId(pWord);            // class
         t_ClassStyle *pClass = ClassStyle(classId);
         clsTxt.Format("%s:%s", ClassName(pClass),
                             ClassTypeName(pClass, style, TRUE));
//---------------------------------------------------------------------------
         if (nameId = ReadResId(pWord)) {                // title
            if (clsTxt == "Static:ICON")
               namTxt = FormatId(nameId, TYP_IDI);
            else if (clsTxt == "Static:BITMAP")
               namTxt = FormatId(nameId, TYP_IDB);
            else namTxt = StrFilter(nameId, FALSE);
         } else namTxt.Empty();
//---------------------------------------------------------------------------
//       namTxt = StrFilter(ReadString(pWord), FALSE);   // title
//---------------------------------------------------------------------------
         WORD dataLen = *pWord++;                        // data
         pWord = (PWORD)((BYTE*)pWord + dataLen);
         if (pWord > pEnd) return;

         // update list
         no = list.InsertItem(0x7FFF, FormatId(id, TYP_IDC), pClass->image);
         list.SetItemText(no, 1, FormatHexa(style));
         list.SetItemText(no, 2, FormatHexa(styleEx));
         list.SetItemText(no, 3, FormatDeci(x));
         list.SetItemText(no, 4, FormatDeci(y));
         list.SetItemText(no, 5, FormatDeci(cx));
         list.SetItemText(no, 6, FormatDeci(cy));
         list.SetItemText(no, 7, clsTxt);
         list.SetItemText(no, 8, FormatId(helpID, TYP_HIC));
         list.SetItemText(no, 9, namTxt);
         list.SetItemData(no, no);

         m_typeDlg->SetEntry(pClass->pStyle, no);
      }
      if (m_lpMenu) {
         no = list.InsertItem(0x7FFF, FormatId(m_lpMenu, TYP_IDM), RI_MENU);
         list.SetItemText(no, 3, "1");
         list.SetItemText(no, 4, "1");
         HMENU hMenu = ::LoadMenu(hinst, m_lpMenu);
         if (hMenu) {
            CMenu *pMenu = CMenu::FromHandle(hMenu);

            pMenu->GetMenuString(0, clsTxt, MF_BYPOSITION);
            list.SetItemText(no, 1, FormatHexa(pMenu->GetMenuState(0, MF_BYPOSITION)));
            namTxt.Format("%s... %d entries", clsTxt, MenuCount(hMenu));
            list.SetItemText(no, 8, namTxt);
            DestroyMenu(hMenu);
         }
         list.SetItemText(no, 7, "Menu");
         list.SetItemData(no, no);
         m_menuItem = no;
         m_typeDlg->SetEntry(&MenuStyle, no);
      }
   } else {
      // CDialog list
      HTREEITEM hit = NULL;
      do {
         static LPCTSTR item = "item", itemX = "itemX";
         LPCTSTR resName = m_pParam->NextResource(hit);
         LPCTSTR itemQ;

         no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
         PWORD pWord = (PWORD)RawData(hinst, resName, RT_DIALOG, &rsize);
         if (pWord) {
            if ( ((DLGTEMPLATEEX*)pWord)->signature == 0xFFFF ) {
               style  = ((DLGTEMPLATEEX*)pWord)->style;
               styleEx= ((DLGTEMPLATEEX*)pWord)->exStyle;
               cnt    = ((DLGTEMPLATEEX*)pWord)->cDlgItems;
               cx     = ((DLGTEMPLATEEX*)pWord)->cx;
               cy     = ((DLGTEMPLATEEX*)pWord)->cy;
               itemQ  = itemX;
               pWord  = PWORD((DLGITEMTEMPLATEEX*)pWord + 1);
            } else {
               style  = ((DLGTEMPLATE*)pWord)->style;
               styleEx= ((DLGTEMPLATE*)pWord)->dwExtendedStyle;
               cnt    = ((DLGTEMPLATE*)pWord)->cdit;
               cx     = ((DLGTEMPLATE*)pWord)->cx;
               cy     = ((DLGTEMPLATE*)pWord)->cy;
               itemQ  = item;
               pWord  = PWORD((DLGITEMTEMPLATE*)pWord + 1);
            }
            SkipResId(pWord);                // menu
            if (*pWord == 0xFFFF) pWord++;
            else SkipResId(pWord);           // class
            namTxt = ReadString(pWord);      // title

            list.SetItemText(no, 1, FormatHexa(style));
            list.SetItemText(no, 2, FormatHexa(styleEx));
            list.SetItemText(no, 3, FormatFloat(double(rsize)/1000.0, "Ko"));
            list.SetItemText(no, 4, FormatDeci(cnt, itemQ));
            list.SetItemText(no, 5, FormatDeci(cx));
            list.SetItemText(no, 6, FormatDeci(cy));
            list.SetItemText(no, 7, namTxt);
            list.SetItemData(no, DWORD(resName));
            m_typeDlg->SetEntry(&DlgStyle, no);
         }
      } while (hit);
   }
}

CString CReportView::ReadStringDec(PWORD &pt)
{
   CString text;
   WORD w = *pt++;
   if (w == 0xFFFF) {
      text.Format("%d", *pt++);
   } else {
      while (w) text += (char)w, w = *pt++;
   }
   return text;
}
/////////////////////////////////////////////////////////////////////////////
// Affichage des informations sur les DLGINIT

void CReportView::DlgInitList(HINSTANCE hinst, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   DWORD rsize;
   bool loop = (resName == NULL);
   HTREEITEM hit = NULL;

   do {
      if (loop && !(resName = m_pParam->NextResource(hit)) ) break;

      DLGINITRES *pDlgIn = (DLGINITRES*)RawData(hinst, resName, RT_DLGINIT, &rsize);
      if (pDlgIn) {
         int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
         list.SetItemData(no, (DWORD)resName);
         WORD last = 0, n = 0;
         while (pDlgIn->itemId) {
            no = list.InsertItem(0x7FFF, "", RI_DLGINIT);
            if (pDlgIn->itemId != last) last = pDlgIn->itemId, n = 0;
            list.SetItemText(no, 1, FormatId(last, TYP_IDC));
            list.SetItemText(no, 2, FormatDeci(n++));
            list.SetItemText(no, 3, FormatDeci(pDlgIn->uMsg));
            list.SetItemText(no, 4, (char*)pDlgIn->data);
            list.SetItemData(no, last);

            pDlgIn = (DLGINITRES*)(pDlgIn->data + pDlgIn->length);
         }
      }
   } while (hit);
}
/////////////////////////////////////////////////////////////////////////////
// Affichage des informations sur les TEXTES

void CReportView::StringList(HINSTANCE hinst, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   DWORD rsize;
   bool loop = (resName == NULL);
   HTREEITEM hit = NULL;

   ModifyStyle(LVS_SINGLESEL, 0); 
   do {
      if (loop && !(resName = m_pParam->NextResource(hit)) ) break;

      PWORD pWord = (PWORD)RawData(hinst, resName, RT_STRING, &rsize);
      if (pWord) {
         DWORD id = DWORD(resName) * 16 - 16;
         int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
         list.SetItemData(no, (DWORD)resName);
         int size = rsize / 2;
         while (size > 0) {
            CString buffer = ExtractString(pWord, size);
            if (!buffer.IsEmpty()) {
               no = list.InsertItem(0x7FFF, FormatId(id, TYP_IDS), RI_INDEX_1);
               list.SetItemText(no, 1, buffer);
               list.SetItemData(no, loop? DWORD(resName) :id);//(DWORD)resName);
            }
            id++;
         }
      }
   } while (hit);
}
///////////////////////////////////////////////////////////////////////////////
// Affichage de la tables de racourci

void CReportView::AccelList(HINSTANCE hinst, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   DWORD rsize;

   ModifyStyle(LVS_SINGLESEL, 0); 

   if (resName) {
      RACCEL *pAc = (RACCEL*)RawData(hinst, resName, RT_ACCELERATOR, &rsize);
      if (pAc) {
         list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
         for (int nb = rsize / sizeof(RACCEL); --nb >= 0; pAc++) {
            int no = list.InsertItem(0x7FFF, FormatId(pAc->cmd, TYP_ID_), RI_MENUCMD);
            list.SetItemText(no, 1, DisplayKbStr(pAc));
            list.SetItemText(no, 2, DisplayKbType(pAc));
            list.SetItemText(no, 3, GetToolHelpMessage(hinst, pAc->cmd, 1)); 
            list.SetItemData(no, pAc->cmd);
         }
      }
   } else {
      HTREEITEM hit = NULL;
      do {
         if ((resName = m_pParam->NextResource(hit, RT_ACCELERATOR)) == NULL) break;
         if ( RawData(hinst, resName, RT_ACCELERATOR, &rsize) ) {
            int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
            list.SetItemText(no, 1, FormatDeci( rsize / sizeof(RACCEL) ));
            list.SetItemData(no, (DWORD)resName);
         }
      } while (hit);
   }
}
/////////////////////////////////////////////////////////////////////////////
// Affichage des informations sur les IMAGES (.BMP)

static INT NumColors(LPBITMAPINFOHEADER pHdr)
{
   int bitCount;
   if (pHdr->biSize == sizeof(BITMAPCOREHEADER))
      bitCount = ((BITMAPCOREHEADER*)pHdr)->bcBitCount;
   else if (pHdr->biSize == sizeof(BITMAPINFOHEADER))
      bitCount = pHdr->biBitCount;
   else return 0;

   switch (bitCount) {
   case 1 : return 2;
   case 4 : return 16;
   case 8 : return 256;
   default: return bitCount;
   }
}

static LPCTSTR FormatCompression(WORD compress)
{

   switch (compress) {
   case BI_RGB :     return "RGB";
   case BI_RLE4 :    return "RLE4";
   case BI_RLE8 :    return "RLE8";
   case BI_BITFIELDS:return "BITFIELDS";
   case 4 :          return "JPEG";
   case 5 :          return "PNG";
   default :         return " ";
   }
}

void CReportView::ImagesList(HINSTANCE hinst, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   bool loop = (resName == NULL);
   HTREEITEM hit = NULL;

   do {
      if (loop && !(resName = m_pParam->NextResource(hit)) ) break;

      BM_INFO bmi;
      LPBITMAPINFOHEADER lpBMIH = GetImageInfo(hinst, resName, RT_BITMAP, &bmi);
      int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
      if (lpBMIH) {
         list.SetItemText(no, 1, FormatDeci(bmi.bmWidth));
         list.SetItemText(no, 2, FormatDeci(bmi.bmHeight));
         list.SetItemText(no, 3, FormatDeci(bmi.bmBitsPixel));
         list.SetItemText(no, 4, FormatCompression(bmi.bmCompression));
         list.SetItemData(no, (DWORD)resName);
      } else {
         list.SetItemText(no, 4, ErrorMsg());
      }
   } while (hit);
}
/////////////////////////////////////////////////////////////////////////////
// Affichages des informations sur les CURSEURS et les ICONES

void CReportView::IconesList(HINSTANCE hinst,
                             LPCTSTR resName,
                             LPCTSTR resType)// RT_GROUP_CURSOR or RT_GROUP_ICON
{
   CListCtrl& list = GetListCtrl();
   HTREEITEM hit = NULL;
   bool loop = (resName == NULL);
   LPCSTR resNameOld = NULL;
   UINT resId, cnt, selItem;
   int no;

   do {
      if (loop && !(resName = m_pParam->NextResource(hit)) ) break;

      cnt = 0, selItem = LookupIconCursId(hinst, resName, resType);
      CString item = FormatId(resName, m_type);

      while (resId = GetIconCursId(hinst, resName, resType, cnt++)) {
         BM_INFO bmi;
         LPBITMAPINFOHEADER lpBMIH = GetImageInfo(hinst, LPCSTR(resId), resType-11, &bmi);

         no = list.InsertItem(0x7FFF, item, TICON(m_type));
         list.SetItemText(no, 1, GetResStr(resId) + (resId == selItem? "*": ""));
         if (lpBMIH) {
            int last = 6, flg;
            list.SetItemText(no, 2, FormatDeci(bmi.bmWidth));
            list.SetItemText(no, 3, FormatDeci(bmi.bmHeight));
            list.SetItemText(no, 4, FormatDeci(bmi.bmBitsPixel));
            list.SetItemText(no, 5, FormatCompression(bmi.bmCompression));
            if (resType == RT_GROUP_CURSOR) {
               list.SetItemText(no, 6, FormatDeci(bmi.bmHotx));
               list.SetItemText(no, 7, FormatDeci(bmi.bmHoty));
               last = 8;
            }
            const UINT warningMsg[2] = { IDS_WHITEIMAGE, IDS_EMPTYMSKIM };
            if ((flg = IconCursEmpty(lpBMIH)) >= 0)
               list.SetItemText(no, last, LoadString(warningMsg[flg]));

            list.SetItemData(no, loop? (DWORD)resName: resId);

         } else {
            list.SetItemText(no, 6, ErrorMsg());
         }
         item = " ---- ";
      }
      if (cnt == 1) {
         no = list.InsertItem(0x7FFF, item, TICON(m_type));
         list.SetItemText(no, 1, LoadString(IDS_EMPTY));
      }
   } while (hit);
}
/////////////////////////////////////////////////////////////////////////////
// Affichage des informations sur les TOOLBARS

void CReportView::ToolbarList(HINSTANCE hinst, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   DWORD rsize;
   if (resName) {
      TOOLBARRES *pTool = (TOOLBARRES*)RawData(hinst, resName, RT_TOOLBAR, &rsize);
      if (pTool) {
         CString wthTxt = FormatDeci(pTool->x);
         CString higTxt = FormatDeci(pTool->y);
         for (int i = 0, t = 0; i < pTool->cnt; i++) {
            WORD id = pTool->toolId[i];
            int no = list.InsertItem(0x7FFF, FormatId(id, TYP_ID_|TYP_SEP), RI_TOOLBAR);
            if (id) {
               list.SetItemText(no, 1, FormatDeci(t++));
               list.SetItemText(no, 2, wthTxt);
               list.SetItemText(no, 3, higTxt);
               list.SetItemText(no, 4, GetToolHelpMessage(hinst, id, 2));
               list.SetItemText(no, 5, GetToolHelpMessage(hinst, id, 1));
               list.SetItemData(no, id);
            }
         }
      }
   } else {
      HTREEITEM hit = NULL;
      do {
         resName = m_pParam->NextResource(hit);
         TOOLBARRES *pTool = (TOOLBARRES*)RawData(hinst, resName, RT_TOOLBAR, &rsize);
         if (resName && pTool) {
            int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
            list.SetItemText(no, 1, FormatDeci(pTool->cnt));
            list.SetItemText(no, 2, FormatDeci(pTool->x));
            list.SetItemText(no, 3, FormatDeci(pTool->y));
            list.SetItemData(no, (DWORD)resName);
         }
      } while (hit);
   }
}
/////////////////////////////////////////////////////////////////////////////
// Affichage des informations MENUS

void CReportView::MenuList(HINSTANCE hinst, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   if (resName) {
      HMENU hMenu = ::LoadMenu(hinst, resName);
      if (hMenu == NULL) return;
      SubmenuList(hinst, hMenu, TICON(m_type));
      DestroyMenu(hMenu);
   } else {
      HTREEITEM hit = NULL;
      do {
         if (resName = m_pParam->NextResource(hit)) {
            HMENU hMenu = ::LoadMenu(hinst, resName);
            if (hMenu) {
               CMenu *pMenu = CMenu::FromHandle(hMenu);
               DWORD helpId = pMenu->GetMenuContextHelpId();
               int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
               list.SetItemText(no, 1, FormatHexa(pMenu->GetMenuState(0, MF_BYPOSITION)));
               list.SetItemText(no, 2, FormatId(helpId, TYP_HIC));
               list.SetItemText(no, 3, FormatDeci(MenuCount(hMenu)));
               list.SetItemData(no, (DWORD)resName);
               DestroyMenu(hMenu);
               m_typeDlg->SetEntry(&MenuStyle, no);
            }
         }
      } while (hit);
   }
}

void CReportView::SubmenuList(HINSTANCE hinst, HMENU hMenu, int image)
{
   CListCtrl& list = GetListCtrl();
   CMenu *pMenu = CMenu::FromHandle(hMenu);
   CString hiTxt = FormatId(pMenu->GetMenuContextHelpId(), TYP_HIC);
   int n = pMenu->GetMenuItemCount();
   for (int i = 0; i < n; i++) {
      WORD id = pMenu->GetMenuItemID(i);
      UINT st = pMenu->GetMenuState(i, MF_BYPOSITION);
      if (st & MF_SEPARATOR) id = 0;
      CString name;
      pMenu->GetMenuString(i, name, MF_BYPOSITION);
      int no = list.InsertItem(0x7FFF, FormatId(id, TYP_ID_|TYP_SEP), image);
      list.SetItemText(no, 1, FormatHexa(st));
      list.SetItemText(no, 2, hiTxt);
      list.SetItemText(no, 3, ExtractStr(name.GetBuffer(0)));
      list.SetItemText(no, 4, GetToolHelpMessage(hinst, id, 2));// tooltip message
      list.SetItemText(no, 5, GetToolHelpMessage(hinst, id, 1));// help message
      list.SetItemData(no, id);
      m_typeDlg->SetEntry(&MenuStyle, no);

      HMENU hsm = ::GetSubMenu(hMenu, i);
      if (hsm) SubmenuList(hinst, hsm, image + 1);
   }
}

int CReportView::MenuCount(HMENU hMenu)
{
   int i = ::GetMenuItemCount(hMenu), sum = i;
   while ( --i >= 0) {
      HMENU hsm = ::GetSubMenu(hMenu, i);
      if (hsm) sum += MenuCount(hsm);
   }
   return sum;
}

CString CReportView::GetToolHelpMessage(HINSTANCE hinst, UINT id,
                                        int m)  // m=0=> complet
{                                               // m=1=> debut avant \n (help)
   static TCHAR buffer[512], *pt;               // m=2=> fin apres \n (tooltip)

   buffer[0] = 0, buffer[511] = 0;
   ::LoadString(hinst, id, buffer, 511);
   if ( m != 0 && (pt = strchr(buffer, '\n'))!= NULL ) *pt = 0;
   if ( m != 2 ) return ExtractStr(buffer);     // m=0, m=1
   else return (pt)? ExtractStr(pt + 1): "";    // m=2
}
/////////////////////////////////////////////////////////////////////////////
// Affichage en Hexa des zones inconnues

void CReportView::HexaList(HINSTANCE hinst, LPCTSTR resType, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   DWORD rsize;
   bool loop = (resName == NULL);
   HTREEITEM hit = NULL;

   do {
      if (loop && !(resName = m_pParam->NextResource(hit, resType)) ) break;

      PWORD pWord = (PWORD)RawData(hinst, resName, resType, &rsize);

      if (pWord) {
         int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
         list.SetItemText(no, 1, FormatDeci(rsize, "bytes"));
         list.SetItemData(no, (DWORD)resName);

         DWORD maxLen = loop ? 8: 8*200;  // longueur max affichee
         rsize = (rsize + 1) >> 1;
         if (rsize > maxLen) rsize = maxLen;

         CString hStr[2];
         while ( rsize >= 8 ) {
            // Traite les lignes completes (avec 16 bytes visualises)
            hStr[0].Format(" %04x %04x %04x %04x ", pWord[0], pWord[1], pWord[2], pWord[3]);
            hStr[1].Format(" %04x %04x %04x %04x ", pWord[4], pWord[5], pWord[6], pWord[7]);
            no = list.InsertItem(0x7FFF, hStr[0], RI_INDEX_1);
            list.SetItemText(no, 1, hStr[1]);
            list.SetItemText(no, 2, ReadText(pWord, 8));
            list.SetItemData(no, (DWORD)resName);
            rsize -= 8;
         }
         if (rsize > 0) {
            // Derniere ligne incomplete
            hStr[0] = hStr[1] = " ";
            for (DWORD i = 0; i < rsize; i++) {
               CString tmp;
               tmp.Format("%04x ", pWord[i]);
               hStr[i >> 2] += tmp;
            }
            no = list.InsertItem(0x7FFF, hStr[0], RI_INDEX_1);
            list.SetItemText(no, 1, hStr[1]);
            list.SetItemText(no, 2, ReadText(pWord, rsize));
            list.SetItemData(no, (DWORD)resName);
         }
      }

   } while (hit);
}

CString CReportView::ReadText(PWORD &pt, int len)
{
   CString text;
   while (--len >= 0) {
      WORD w = *pt++;
      text += (isprint(w & 0x007F)) ? TCHAR(w & 0x007F) : '-';
      if (isprint(w >> 8)) text += TCHAR(w >> 8);
   }
   return text;
}
///////////////////////////////////////////////////////////////////////////////
// Affichage des informations de Version

void CReportView::VersionList(HINSTANCE hinst, LPCTSTR resName)
{
   CListCtrl& list = GetListCtrl();
   CString sizeTxt, fn, fi, name, item;
   int no;
   UINT n = 0;

   SetWindowContextHelpId(0x20000 + IDR_REPORTVIEW);
   ModifyStyle(LVS_SINGLESEL, 0);

   if (resName == NULL) {
      HTREEITEM hit = NULL;
      resName = m_pParam->NextResource(hit, RT_VERSION);
   }
   UINT size = VersionInit(hinst, resName, name, item);
   if (size > 0) {

      no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
      list.SetItemText(no, 2, FormatDeci(size, "bytes"));

      while ( FixedFileInfo(n, fn, fi) ) {
         // Elimine les lignes sans information
         if (!fi.IsEmpty()) {
            no = list.InsertItem(0x7FFF, "", RI_INDEX_1);
            list.SetItemText(no, 1, fn);
            list.SetItemText(no, 2, fi);
         }
      }

      if (! name.IsEmpty() ) {
         no = list.InsertItem(0x7FFF, "", RI_INDEX_2);
         list.SetItemText(no, 1, name);
         list.SetItemText(no, 2, item);

         PWORD pW = NULL;
         while ( VersionDecod(pW, name, item) ) {
            // Elimine les lignes sans information
            if (!item.IsEmpty()) {
               no = list.InsertItem(0x7FFF, "", RI_INDEX_2);
               list.SetItemText(no, 1, name);
               list.SetItemText(no, 2, item);
            }
         }
      }
   }
}
///////////////////////////////////////////////////////////////////////////////
// Affichages des informations des fichiers RIFF

BOOL CReportView::InfoRIFF(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   DWORD rsize;
   if (resName) {
      t_CHUNK* pChunk = (t_CHUNK*)RawData(hinst, resName, resType, &rsize);

      if (pChunk && pChunk->ckId == FOURCC_RIFF) {

         return Chunk((PDWORD)pChunk, rsize, (DWORD)resName, m_type & TYP_IMSK);
      }
   } else {
      CListCtrl& list = GetListCtrl();
      HTREEITEM hit = NULL;
      do {
         if ((resName = m_pParam->NextResource(hit, resType)) == NULL) break;
         t_CHUNK* pChunk = (t_CHUNK*)RawData(hinst, resName, resType, &rsize);
         if (pChunk && pChunk->ckId == FOURCC_RIFF) {
            int no = list.InsertItem(0x7FFF, FormatId(resName, m_type), TICON(m_type));
            list.SetItemText(no, 1, FourCCtoStr(pChunk->ckId));
            list.SetItemText(no, 2, FourCCtoStr(pChunk->ckData[0]));
            list.SetItemText(no, 3, FormatDeci(rsize, "bytes"));
            list.SetItemData(no, (DWORD)resName);
         }
      } while (hit);
      return TRUE;
   }
   return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
//
//       INFO LIST CHUNKS (from the Multimedia Programmer's Reference
//                           plus new ones)
typedef struct { DWORD riff; LPSTR name; } t_RIFFINFO;
static t_RIFFINFO infoRiffTbl[] = {
   { mmioFOURCC('I','A','R','L'), "Archival location"},
   { mmioFOURCC('I','A','R','T'), "Artist"},
   { mmioFOURCC('I','C','M','S'), "Commissioned"},
   { mmioFOURCC('I','C','M','T'), "Comments"},
   { mmioFOURCC('I','C','O','P'), "Copyright"},
   { mmioFOURCC('I','C','R','D'), "Creation date"},
   { mmioFOURCC('I','C','R','P'), "Cropped"},
   { mmioFOURCC('I','D','I','M'), "Dimensions"},
   { mmioFOURCC('I','D','P','I'), "Dots per inch"},
   { mmioFOURCC('I','E','N','G'), "Engineer"},
   { mmioFOURCC('I','G','N','R'), "Genre"},
   { mmioFOURCC('I','K','E','Y'), "Keywords"},
   { mmioFOURCC('I','L','G','T'), "Lightness settings"},
   { mmioFOURCC('I','M','E','D'), "Medium"},
   { mmioFOURCC('I','N','A','M'), "Name of subject"},
   { mmioFOURCC('I','P','L','T'), "No.of colors"},
   { mmioFOURCC('I','P','R','D'), "Product"},
   { mmioFOURCC('I','S','B','J'), "Subject description"},
   { mmioFOURCC('I','S','F','T'), "Name of package"},
   { mmioFOURCC('I','S','H','P'), "Sharpness"},
   { mmioFOURCC('I','S','R','C'), "Source"},
   { mmioFOURCC('I','S','R','F'), "Source Form."},
   { mmioFOURCC('I','T','C','H'), "Technician"},
// New INFO Chunks as of August 30, 1993:
   { mmioFOURCC('I','S','M','P'), "SMPTE time code"},
// ISMP: SMPTE time code of digitization start point expressed as a NULL
// terminated text string "HH:MM:SS:FF". If performing MCI capture in AVICAP,
// this chunk will be automatically set based on the MCI start time.
   { mmioFOURCC('I','D','I','T'), "Digitization Time"},
   { 0,                           NULL}   };

static LPSTR ChunkInfo(DWORD chunk)
{
   t_RIFFINFO* pRi = infoRiffTbl;
   while (pRi->riff != chunk && pRi->riff) pRi++;
   return pRi->name;
}

BOOL CReportView::Chunk(PDWORD pDWord, DWORD dwChLen, DWORD type, int image)
{
   CListCtrl& list = GetListCtrl();
   t_CHUNK *pChunk = (t_CHUNK*)pDWord;
   WAVEFORMATEX *pFmt = NULL;
   AVIStreamHeader *pAVIStreamHd = NULL;
   BITMAPINFO *pBi = NULL;
   LPSTR pStr, pEnd;
   DWORD size;
   double tmp;
   int no;
   WORD ne = 0;
   DWORD iconStp = 0;
   BOOL  ret = TRUE;

   dwChLen = (dwChLen+1) & ~1;
   do {
      size = pChunk->ckSize;

      dwChLen -= 8;
      if (size > dwChLen) {
         if (m_details) {
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, FourCCtoStr(pChunk->ckId));
            list.SetItemText(no, 3, FormatDeci(dwChLen, "<"));
            list.SetItemText(no, 4, FormatDeci(pChunk->ckSize, "=> length error"));
         }
         // Corrige la longueur du chunk
         size = dwChLen;
         dwChLen = 1;
      } else {
         dwChLen -= (pChunk->ckSize + 1) & ~1;
      }

      switch (pChunk->ckId) {
      case FOURCC_RIFF :
         // format type of the riff chunk
         no = list.InsertItem(0x7FFF, FormatId((LPCTSTR)type, m_type), image);
         list.SetItemText(no, 1, FourCCtoStr(pChunk->ckId));
         list.SetItemText(no, 2, FourCCtoStr(pChunk->ckData[0]));
         list.SetItemText(no, 3, FormatDeci(size+8, "bytes"));

         ret = Chunk(pChunk->ckData+1, size, pChunk->ckData[0], RI_INDEX_1);
         break;

      case FOURCC_LIST :
         // list type of the list chunk
         if (m_details) {
            no = list.InsertItem(0x7FFF, "", RI_AVI);
            list.SetItemText(no, 1, FourCCtoStr(pChunk->ckId));
            list.SetItemText(no, 2, FourCCtoStr(pChunk->ckData[0]));
            list.SetItemText(no, 3, FormatDeci(pChunk->ckSize+8, "bytes"));
         }
         ret = Chunk(pChunk->ckData+1, size, pChunk->ckData[0], RI_INDEX_1);
         break;

      case mmioFOURCC('f','m','t',' '):
         // wave header
         if (type == mmioFOURCC('W','A','V','E')) {

            AfxBeginThread(ThreadProc, m_pWave = LPVOID(pDWord - 3));

            pFmt = (WAVEFORMATEX*)pChunk->ckData;

            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Format");
            list.SetItemText(no, 3, DecodFlags(pFmt->wFormatTag, waveFormatTagID));
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Channels");
            list.SetItemText(no, 3, FormatDeci(pFmt->nChannels));
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Sample");
            list.SetItemText(no, 3, FormatDeci(pFmt->wBitsPerSample, "Bytes"));
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "vps");
            list.SetItemText(no, 3, FormatDeci(pFmt->nSamplesPerSec, "Samples/sec"));
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "BytesSec");
            list.SetItemText(no, 3, FormatDeci(pFmt->nAvgBytesPerSec, "Bytes/sec"));
         } else {
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "fmt Error");
            list.SetItemText(no, 3, FourCCtoStr(type));
         }
         break;

      case mmioFOURCC('d','a','t','a'):
         // data wave
         if (m_details) {
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, FourCCtoStr(type));
            list.SetItemText(no, 3, FourCCtoStr(pChunk->ckId));
            list.SetItemText(no, 4, FormatDeci(pChunk->ckSize, "bytes"));
         }
         if (type == mmioFOURCC('W','A','V','E') && pFmt) {
            no = list.InsertItem(0x7FFF, "", image);
            DWORD nbSampl = (8 * pChunk->ckSize) / pFmt->wBitsPerSample;
            list.SetItemText(no, 2, "Size");
            list.SetItemText(no, 3, FormatDeci(nbSampl, "samples"));
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Durée");
            list.SetItemText(no, 3, FormatFloat(double(nbSampl) / pFmt->nSamplesPerSec, "sec"));
         } else {
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "data Error");
            list.SetItemText(no, 3, FourCCtoStr(type));
         }
         break;

      case mmioFOURCC('a','v','i','h'):
         {
            MainAVIHeader*pMainAVIHd = (MainAVIHeader*)pChunk->ckData;
            tmp = double(pMainAVIHd->dwMicroSecPerFrame) * pMainAVIHd->dwTotalFrames;
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Frames");
            list.SetItemText(no, 3, FormatDeci(pMainAVIHd->dwTotalFrames, "frames"));
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Durée");
            list.SetItemText(no, 3, FormatFloat(tmp/1000000., "secondes"));
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Display");
            list.SetItemText(no, 3, FormatDeci(pMainAVIHd->dwHeight, "x ")
                                       + FormatDeci(pMainAVIHd->dwWidth, "pixels"));
         }
         break;

      case mmioFOURCC('a','n','i','h'):
         {
            ANIHEADER *pAh = (ANIHEADER*)pChunk->ckData;
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Frames");
            list.SetItemText(no, 3, FormatDeci(pAh->cFrames, "frames"));
            list.SetItemData(no, 0x2000);
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, "Steps");
            list.SetItemText(no, 3, FormatDeci(pAh->cSteps, "s/f"));
            list.SetItemData(no, 0x2000);


            if (m_pAicon == NULL) {
               m_pAicon = (ANI_ICON*)calloc( pAh->cFrames+1, sizeof(AI_ENT) );
               m_pAicon->count = (WORD)pAh->cFrames;
               for (WORD n = 0; n < m_pAicon->count; n++) {
                  m_pAicon->icon[n].seq = (BYTE)n;
                  m_pAicon->icon[n].step = (BYTE)pAh->cSteps;
               }
            }
         }
         break;

      case mmioFOURCC('i','c','o','n'):
         no = list.InsertItem(0x7FFF, FormatDeci(++iconStp), image);
         list.SetItemText(no, 2, FourCCtoStr(pChunk->ckId));
         list.SetItemText(no, 3, FormatDeci(pChunk->ckSize+8, "Bytes"));

         if ( m_pAicon && ne < m_pAicon->count ) {
            list.SetItemData(no, 0x1000 + ne);
            m_pAicon->icon[ne].pData = PBYTE(pChunk->ckData);
            m_pAicon->icon[ne].size = pChunk->ckSize;
            ne++;
         }
         break;

      case mmioFOURCC('s','t','r','h'):
         pAVIStreamHd = (AVIStreamHeader*)pChunk->ckData;
         break;

      case mmioFOURCC('s','t','r','f'):
         if (pAVIStreamHd) {
            switch (pAVIStreamHd->fccType) {
            case mmioFOURCC('v', 'i', 'd', 's'):
               pBi = (BITMAPINFO*)pChunk->ckData;
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "Compression");
               list.SetItemText(no, 3, DecodFlags(pBi->bmiHeader.biCompression, biCompression));
               break;
            case mmioFOURCC('a', 'u', 'd', 's'):
               pFmt = (WAVEFORMATEX*)pChunk->ckData;
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "Format");
               list.SetItemText(no, 3, DecodFlags(pFmt->wFormatTag, waveFormatTagID));
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "Channels");
               list.SetItemText(no, 3, FormatDeci(pFmt->nChannels));
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "Sample");
               list.SetItemText(no, 3, FormatDeci(pFmt->wBitsPerSample, "Bytes"));
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "vps");
               list.SetItemText(no, 3, FormatDeci(pFmt->nSamplesPerSec, "Samples/sec"));
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "BytesSec");
               list.SetItemText(no, 3, FormatDeci(pFmt->nAvgBytesPerSec, "Bytes/sec"));
               break;
            case mmioFOURCC('t', 'x', 't', 's'):
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "txts");
               break;
            }
         }
         break;

      default :
         if (pStr = ChunkInfo(pChunk->ckId)) {
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, FourCCtoStr(pChunk->ckId));
            list.SetItemText(no, 3, pStr);
            pStr = (LPSTR)pChunk->ckData;
            pEnd = pStr + pChunk->ckSize;
            list.SetItemText(no, 4, ReadBfText(pStr, pEnd));
            while (pStr < pEnd) {
               no = list.InsertItem(0x7FFF, "", image);
               list.SetItemText(no, 2, "----");
               list.SetItemText(no, 4, ReadBfText(pStr, pEnd));
            }
         } else if (m_details) {
            no = list.InsertItem(0x7FFF, "", image);
            list.SetItemText(no, 2, FourCCtoStr(pChunk->ckId));
            list.SetItemText(no, 3, FormatDeci(pChunk->ckSize+8, "Bytes"));
         }
//         TRACE("Chunk ?? %s\n", FourCCtoStr(pChunk->ckId));
      }
      if (!ret) return FALSE;

      // next chunk 
      pChunk = (t_CHUNK*)(DWORD(pChunk->ckData) + ((pChunk->ckSize + 1) & ~1));
   } while (dwChLen > 8);
   return TRUE;
}

//  Loads an animatied cursor from a RIFF file.  The RIFF file format for
//  animated cursors looks like this:
//
//  RIFF( 'ACON'
//      LIST( 'INFO'
//          INAM( <name> )
//          IART( <artist> )
//      )
//      anih( <anihdr> )
//      [rate( <rateinfo> )  ]
//      ['seq '( <seq_info> )]
//      LIST( 'fram' icon( <icon_file> ) )
//  )

UINT CReportView::ThreadProc(LPVOID pParam)
{
   sndPlaySound((LPCTSTR)pParam, SND_MEMORY|SND_SYNC|SND_NODEFAULT);
   return TRUE;
}

void CReportView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_pWave) AfxBeginThread(ThreadProc, m_pWave);
	CListView::OnLButtonDown(nFlags, point);
}
