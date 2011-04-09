// ResDoc.cpp : implementation of the CResDoc class
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "MainFrm.h"
#include "ReportView.h"
#include "Utilities.h"
#include "BitmapRW.h"
#include "DialogRW.h"
#include "DlgDataRW.h"
#include "VersionRD.h"
#include <shlwapi.h>

#include "ResDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CResDoc

CParams CResDoc::m_param;

IMPLEMENT_DYNCREATE(CResDoc, CDocument)

BEGIN_MESSAGE_MAP(CResDoc, CDocument)
	//{{AFX_MSG_MAP(CResDoc)
	ON_COMMAND(ID_MAKE_TOOLBAR, OnMakeToolbar)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_PANE_BACK, OnPaneBack)
	ON_UPDATE_COMMAND_UI(ID_MAKE_TOOLBAR, OnUpdateMakeToolbar)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_PANE_BACK, OnUpdatePaneBack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LARGEICON, OnUpdateViewLargeicon)
	ON_COMMAND(ID_VIEW_LARGEICON, OnViewLargeicon)
	ON_COMMAND(ID_BITMAP_CVRT, OnBitmapCvrt)
	ON_UPDATE_COMMAND_UI(ID_BITMAP_CVRT, OnUpdateBitmapCvrt)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrint)
	ON_COMMAND(ID_DSPL_BKCOLOR, OnDsplBkcolor)
	ON_UPDATE_COMMAND_UI(ID_DSPL_BKCOLOR, OnUpdateDsplBkcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResDoc construction/destruction

CResDoc::CResDoc()
{
   m_toolbar.cnt = m_toolbar.y = m_toolbar.x = 0;
   m_toolbar.toolId[0] = 1000;
   m_largeIcon = TRUE;

   m_pToolbar = NULL;
   m_pOutFile = NULL;
}

CResDoc::~CResDoc()
{
   if (m_pToolbar) delete m_pToolbar; //m_pToolbar->DestroyWindow();
   if (m_pOutFile) delete m_pOutFile;
}
/////////////////////////////////////////////////////////////////////////////
// CResDoc diagnostics

#ifdef _DEBUG
void CResDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResDoc commands

BOOL CResDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{  // Lance la lecture d'une liste de fichiers contenant des resources
   SetPathName(lpszPathName);
   m_param.filesPath = lpszPathName;
   UpdateAllViews(NULL, UPPER_VIEW, &m_param);
	return TRUE;
}

void CResDoc::OnMakeToolbar() 
{
   if (m_pToolbar == NULL) {
      m_pToolbar = new CToolbarDlg(AfxGetMainWnd(), this);
      m_pToolbar->Create(AfxGetMainWnd());
   }
   m_pToolbar->ShowWindow(SW_SHOW);
}

void CResDoc::OnFileSaveAs() 
{
   UINT err;
   if (m_pOutFile == NULL) m_pOutFile = new CFileDialogSa();

   // Definition du nom de fichier par defaut, et des extensions possibles
   m_pOutFile->SetNames(m_param.GetFilter(), m_param.GetDefName());

   // Demande du nom de fichier et ecriture des informations
   if (m_pOutFile->DoModal() == IDOK) {
      if (err = FileSave(m_pOutFile->GetPathName())) AfxMessageBox(err);
   }
}

void CResDoc::OnPaneBack() 
{
   if ( m_param.Restore() ) {

      ((CMainFrame*)AfxGetMainWnd())->CreateView( m_param.GetView() );

      WPARAM wp = LEFT_VIEW_SELECT|RES_VIEW|RES_VIEW_ITEM|LIST_VIEW|LIST_VIEW_ITEM;
      UpdateAllViews( NULL, wp, &m_param );
   }
}

void CResDoc::OnEditCopy()       // Copy into Clipboard
{
   if ( m_param.pRID ) {
      BM_INFO  bmi;
      CBuffer  buf;
      CString *pBuf;
      PVOID    pVoid;
      IPicture*pIPicture;

      HINSTANCE hLib  = m_param.hLib;
      LPCTSTR resName = m_param.resName;
      LPCTSTR resType = m_param.resType;

      switch ( m_param.pRID->wId ) {
      case RT_BITMAP :
         pVoid = GetImageInfo(hLib, resName, RT_BITMAP, &bmi);
         CopyOnClipbord(CF_DIB, pVoid, bmi.bmSize);
         break;
      case RT_GROUP_ICON :
      case RT_GROUP_CURSOR :
         pVoid = GetImageInfo(hLib, (LPCTSTR)m_param.item, resType-DIFFERENCE, &bmi);
         CopyOnClipbord(CF_DIB, buf.Convert((LPBITMAPINFOHEADER)pVoid, &bmi), bmi.bmSize);
         break;
      case RT_DIALOG :
         pBuf = DumpDialog(hLib, resName);
         CopyOnClipbord(CF_TEXT, pBuf->GetBuffer(0), pBuf->GetLength() + 1);
         break;
      case RT_MENU :
         pBuf = DumpMenu(hLib, resName);
         CopyOnClipbord(CF_TEXT, pBuf->GetBuffer(0), pBuf->GetLength() + 1);
         break;
      case RT_ACCELERATOR :
         pBuf = DumpAccel(hLib, resName);
         CopyOnClipbord(CF_TEXT, pBuf->GetBuffer(0), pBuf->GetLength() + 1);
         break;
      case RT_STRING :
         BufferEmpty();
         pBuf = DumpStringTable(hLib, resName);
         CopyOnClipbord(CF_TEXT, pBuf->GetBuffer(0), pBuf->GetLength() + 1);
         break;
      case rt_bmp :
         pVoid = GetImageInfo(hLib, resName, resType, &bmi);
         CopyOnClipbord(CF_DIB, pVoid, bmi.bmSize);
         break;
      case rt_txt :
      case rt_rtf :
      case rt_reg :
         pVoid = RawData(hLib, resName, resType, &bmi.bmSize);
         CopyOnClipbord(CF_TEXT, pVoid, bmi.bmSize - 1);
         break;
      case rt_wav :
      case rt_avi :// ne fonctionne pas
         pVoid = RawData(hLib, resName, resType, &bmi.bmSize);
         CopyOnClipbord(CF_RIFF, pVoid, bmi.bmSize);
         break;
      case rt_bmx:
      case rt_jpg :
      case rt_gif :
         pIPicture = Picture(hLib, resName, resType);
         CopyOnClipbord(CF_DIB, IPictureToHGlobal(pIPicture, NULL));
         break;
      default :
         AfxMessageBox(IDS_INVALIDFMT);
      }
   }
}

void CResDoc::OnViewLargeicon() 
{
   m_largeIcon = !m_largeIcon;
   UpdateAllViews(NULL, RES_VIEW);
}

void CResDoc::OnDsplBkcolor() 
{
   static COLORREF colors[16] = {0xE0E0E0,0xE0E0E0,0xE0E0E0,0xE0E0E0,
                                 0xE0E0E0,0xE0E0E0,0xE0E0E0,0xE0E0E0,
                                 0xE0E0E0,0xE0E0E0,0xE0E0E0,0xE0E0E0,
                                 0xE0E0E0,0xE0E0E0,0xE0E0E0,0xE0E0E0};

   CColorDialog cdlg(bkColor, CC_RGBINIT);

   cdlg.m_cc.lpCustColors = colors;
   colors[15] = 0xE0E0E0;

   if (cdlg.DoModal() == IDOK) {
      bkColor = cdlg.GetColor();
      UpdateAllViews(NULL, RES_VIEW);
   }
}

void CResDoc::OnBitmapCvrt() 
{
   // Couleur de fond apres conversion des curors et icons en bitmap 
   //      pour l'ecriture dans le presse papier ou un fichier
   CBitmapDlg bmDlg;

   bmDlg.m_bkColor = colorId;
   if (bmDlg.DoModal() == IDOK) colorId = bmDlg.m_bkColor;

}
/////////////////////////////////////////////////////////////////////////////
// Presence des differentes commandes

void CResDoc::OnUpdateMakeToolbar(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_param.resType == RT_BITMAP && m_param.resName != NULL);
}

void CResDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable( m_param.GetView() & WRT );
}

void CResDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable( m_param.GetView() & EDT );
}

void CResDoc::OnUpdatePaneBack(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable( m_param.IsUndo() );
}

void CResDoc::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable( m_param.GetView() & PRN );
}

void CResDoc::OnUpdateViewLargeicon(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_param.resName == 0 && m_param.resType == RT_BITMAP);
   pCmdUI->SetCheck(m_largeIcon);
}

void CResDoc::OnUpdateBitmapCvrt(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable( m_param.GetView() & ICO );
}

void CResDoc::OnUpdateDsplBkcolor(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable( bkColorUsed );
}
/////////////////////////////////////////////////////////////////////////////
// Subroutines
/////////////////////////////////////////////////////////////////////////////
// Un nouveau fichier ou une directory vient d'etre designee

void CResDoc::SetPath(CView* pSender, LPCTSTR lpszPathName, LPCTSTR selPath)
{  
   SetPathName(lpszPathName);
   m_param.selPath = selPath;
   m_param.filesPath = lpszPathName;
   UpdateAllViews(pSender, UPPER_VIEW, &m_param);
}
/////////////////////////////////////////////////////////////////////////////
// Lecture des resources d'un fichier

void CResDoc::ReadDocument(LPCTSTR lpszPathName)
{
   int cnt = 0;
   // Elimine la precedente librairie, en informe les fenetres
   m_param.FreeLibrary();
   UpdateAllViews(NULL, RES_VIEW|LIST_VIEW|LEFT_VIEW, &m_param);

   // Wait a end of update
   ((CResExplorer*)AfxGetApp())->WaitEndOfWork();
/*   MSG msg;
   while (::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE )) {
      cnt++;
      if (!AfxGetApp()->PumpMessage()) {
         ::PostQuitMessage(0);
         break;
      }
   }*/
   // Charge la nouvelle et informe les fenetres
   if ( m_param.NewLibrary(lpszPathName) ) {
      // Wait a end of update
      ((CResExplorer*)AfxGetApp())->WaitEndOfWork();
/*      while (::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE )) {
         cnt++;
         if (!AfxGetApp()->PumpMessage()) {
            ::PostQuitMessage(0);
            break;
         }
      }*/

      UpdateAllViews(NULL, LEFT_VIEW|LEFT_VIEW_SELECT, &m_param);
      ((CMainFrame*)AfxGetMainWnd())->SymbolSelect("");

      TRACE("CResDoc::ReadDocument cnt=%d\n", cnt);
   } else {
      DisplayErrorBox();
   }
}
/////////////////////////////////////////////////////////////////////////////
// Mise a jour des vues en fonction de la resource designee

void CResDoc::SetCurrent(CView* pSender,
                         LPCTSTR resType, LPCTSTR resName,
                         LPARAM view)
{
//   TRACE("CResDoc::SetCurrent rt=%08x rn=%08x\n", resType, resName);
   DWORD type = 0;
   m_toolbar.y = m_toolbar.x = m_toolbar.cnt = 0;
   if (m_pToolbar) m_pToolbar->ShowWindow(SW_HIDE);

   m_param.Save(resName, resType);
   m_param.FreeAni();
   m_param.item = 0;

   bkColorUsed = FALSE;

   if (resType) {
      m_param.pRID = GetResEntryEx( resType );
      type = m_param.pRID->type;
   } else {
      m_param.pRID = NULL;
   }

   if (resName && DWORD(resType) & 0xFFFF0000)
      ResourceVerify(m_param.hLib, m_param.resName, m_param.resType);

   if (resName && (m_param.pRID->vInst & DAT)) TypeModifie();

   ((CMainFrame*)AfxGetMainWnd())->CreateView( m_param.GetView() );
   UpdateAllViews( pSender, view, &m_param );

   if (resType && (view & LIST_VIEW_SYMBOL) == 0) {
      if (m_param.symbol.IsEmpty()) {
         LPCTSTR resId = (resName)? resName: resType;
         ((CMainFrame*)AfxGetMainWnd())->SymbolSelect(FormatId(resId, type));
      } else {
         ((CMainFrame*)AfxGetMainWnd())->SymbolSelect(m_param.symbol);
      }
   }
}

void CResDoc::SelectItem(CView* pSender, UINT item,
                         LPARAM view/*=RES_VIEW_ITEM|LIST_VIEW_ITEM*/)
{
   if (m_param.item != item) {
      m_param.item = item;
      UpdateAllViews(pSender, view, &m_param);
   }
   if (!m_param.symbol.IsEmpty()) {
      if ((view & LIST_VIEW_SYMBOL) == 0)
         ((CMainFrame*)AfxGetMainWnd())->SymbolSelect(m_param.symbol);
   }
}
/////////////////////////////////////////////////////////////////////////////
// Ajuste le type en fonction des donnees contenues dans la resource

void CResDoc::TypeModifie()
{
   DWORD rsize;
   PBYTE pByte = (PBYTE)RawData(m_param.hLib, m_param.resName, m_param.resType, &rsize);
   t_ResID *pRID;

   if (pByte) {
      WORD wrd = *((PWORD)pByte);

      switch (wrd) {
      case 'IR':     // RIFF
         if (((t_CHUNK*)pByte)->ckId == FOURCC_RIFF) {
            switch (((t_CHUNK*)pByte)->ckData[0]) {
            case mmioFOURCC('W','A','V','E'):
               m_param.pRID = GetResEntry(LPCTSTR(rt_wav)); break;
            case mmioFOURCC('A','V','I',' '):
               m_param.pRID = GetResEntry(LPCTSTR(rt_avi)); break;
            case mmioFOURCC('A','C','O','N'):
               m_param.pRID = GetResEntry(RT_ANIICON); break;
            }
         }
         break;
      case 'MB':     // BM
         m_param.pRID = GetResEntry(LPCTSTR(rt_bmp)); break;
      case 'PJ':     // JPG
         m_param.pRID = GetResEntry(LPCTSTR(rt_jpg)); break;
      case 'IG':     // GIF
         m_param.pRID = GetResEntry(LPCTSTR(rt_gif)); break;
      case 0xFFD8:   // JPEG
         m_param.pRID = GetResEntry(LPCTSTR(rt_jpg)); break;
      case 0xD8FF:   // JPEG
         m_param.pRID = GetResEntry(LPCTSTR(rt_jpg)); break;
      default  :
         pRID = SearchResEntry(m_param.hLib, m_param.resName, m_param.resType);
         if (pRID) {
            m_param.pRID = pRID;
         } else {
            if ((wrd & 0xFF) == '<' || (wrd >> 8) == '<') {
               m_param.pRID = GetResEntry(LPCTSTR(rt_txt));
            } else {
               rsize--;
               for (DWORD i = 0; i < rsize; i++)
                  if ( !(isprint(pByte[i]) || isspace(pByte[i])) ) return;
                     m_param.pRID = GetResEntry(LPCTSTR(rt_txt));
            }
         }
      }
   }
}
/////////////////////////////////////////////////////////////////////////////
// Sauvegarde la resource dans un fichier (suivant l'extension du nom
//                 et la resource)

UINT CResDoc::FileSave(CString fnam)
{

   HINSTANCE hLib  = m_param.hLib;
   LPCTSTR resName = m_param.resName;
   LPCTSTR resType = m_param.resType;

   switch (GetFileNameExt(fnam)) {
   case '.res':
      return WriteRes(fnam, hLib, resName, resType);
   case '.bmp':
      switch (m_param.pRID->wId) {
      case RT_GROUP_CURSOR:
      case RT_GROUP_ICON :
         return WriteBitmap(fnam, hLib, (LPCTSTR)m_param.item, resType-DIFFERENCE);
      case RT_BITMAP :
         return WriteBitmap(fnam, hLib, resName, resType);
      case rt_bmp :
         return WriteRawData(fnam, hLib, resName, resType);
      case rt_bmx :
      case rt_jpg :        // convert this format in bmp
      case rt_gif :
         return WriteCvrtBitmap(fnam, hLib, resName, resType);
      default :
         return WriteBitmap(fnam, hLib, resName, resType);
      }
      break;
   case '.ico':
   case '.cur':
      switch (m_param.pRID->wId) {
      case RT_GROUP_ICON :
      case RT_GROUP_CURSOR:
         return WriteIconCurs(fnam, hLib, resName, resType);
      }
      break;
   case '.dlg':
      switch (m_param.pRID->wId) {
      case RT_DIALOG:
         return WriteDlg(fnam, hLib, resName);
      case RT_MENU :
         return WriteMenu(fnam, hLib, resName);
      case RT_STRING :
         return WriteStringTable(fnam, hLib, resName);
//      case RT_ACCELERATOR :
      }
      break;
   default :
      return WriteRawData(fnam, hLib, resName, resType);
   }
   return IDS_INVALIDFMT;
}
/////////////////////////////////////////////////////////////////////////////
// Ecriture fichier au format *.res

UINT CResDoc::WriteRes(CString fnam, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   CFile file;
   if (!file.Open(fnam, CFile::modeCreate|CFile::modeWrite)) return IDS_WRITEERROR;
   // Write the resource file header
   if (WriteResBuffer(file, NULL, 0, 0, 0, 0)) return IDS_WRITEERROR;
   // Write the resources
   return WriteRes(file, hinst, resName, resType);
}

UINT CResDoc::WriteRes(CFile &file, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   UINT err = 0;
   CResList list;

   switch (DWORD(resType)) {
   case 0   :
      // ecrit les icon et cursor utilises par les groupes
      err = WriteResIconLoop(file, hinst, resName, RT_GROUP_ICON);
      err = WriteResIconLoop(file, hinst, resName, RT_GROUP_CURSOR);
      break;
   case RT_GROUP_CURSOR :
   case RT_GROUP_ICON :
      err = WriteResIconLoop(file, hinst, resName, resType);
      break;
   case RT_TOOLBAR :
      // ecrit les bitmaps utilisees par les toolbars
      err = WriteResLoop(file, hinst, resName, resType, RT_BITMAP);
      break;
   }

   // ecrit la (ou les) resource(s) demande(es)
   BOOL loop = (resType == NULL);
   HTREEITEM hit = NULL;
   do {
      if (err) break;
      if (loop && !(resType = m_param.NextResourceType(hit)) ) break;
      err = WriteResLoop(file, hinst, resName, resType, resType);
   } while (hit);

   if (err == 0) {
      switch (DWORD(resType)) {
      case RT_DIALOG :
         SearchCDiagAddon(hinst, resName, list);
         WriteAddonIcon(file, hinst, list);
         WriteResLoop(file, hinst, resName, RT_DLGINIT, RT_DLGINIT);
         break;
      case RT_BITMAP :
         if (m_toolbar.cnt > 0 && resName) {
            // on ecrit la resource toolbar cree par l'outil
            err = WriteBmToolbarRes(file, &m_toolbar, resName);
         }
         break;
      }
   }

   return err;
}

// Ecrire la resource designee ou toutes les resources
//                      d'un type defini (dans la table)

UINT CResDoc::WriteResLoop(CFile &file,
                           HINSTANCE hinst,
                           LPCTSTR resName,
                           LPCTSTR resType,  // Type utilise pour la recherche
                           LPCTSTR resWType) // Type ecrit dans le fichier
{
   HTREEITEM hit = NULL;
   UINT err = 0;
   bool loop = (resName == NULL);
   do {
      if (loop && !(resName = m_param.NextResource(hit, resType)) ) break;

      err = WriteResData(file, hinst, resName, resWType);
   } while (hit && err == 0);
   return err;
}

// Ecrire les RT_ICON ou les RT_CURSOR correspondant
//                                  aux RT_GROUP_... correspondants

UINT CResDoc::WriteResIconLoop(CFile &file,
                               HINSTANCE hinst,
                               LPCTSTR resName,
                               LPCTSTR resType)
{
   HTREEITEM hit = NULL;
   BOOL loop = (resName == NULL);
   UINT err = 0;

   do {
      if (loop && !(resName = m_param.NextResource(hit, resType)) ) break;

      // Recherche et ecrit les RT_ICON ou _CURSOR correspondant au RT_GROUP_
      UINT resId, no = 0;
      while ((resId = GetIconCursId(hinst, resName, resType, no++)) && err == 0)
         err = WriteResData(file, hinst, LPCSTR(resId), resType - DIFFERENCE );

   } while (hit && err == 0);
   return err;
}
/////////////////////////////////////////////////////////////////////////////
// Static BITMAP and ICON in CDialog

int CResList::Compare(POSITION pos, t_RESDEF &res)
{
   t_RESDEF &cur = GetAt(pos);
   return (cur.resType == res.resType)? cur.resName - res.resName:
                                        cur.resType - res.resType;
}

void CResList::Add(LPCTSTR resName, LPCTSTR resType)
{
   t_RESDEF res = {resName, resType};
   POSITION pos = GetHeadPosition();

   while (pos) {
      int dif = Compare(pos, res);
      if (dif >= 0) {
         if (dif != 0) InsertBefore(pos, res);
         return;
      }
      GetNext(pos);
   }
   AddTail(res);
}

CResList::~CResList()
{
/*   POSITION pos = GetHeadPosition();
   while (pos) {
      t_RESDEF &cur = GetNext(pos);
      TRACE("CResList %04x %04x\n", cur.resName, cur.resType);
   }*/
}

UINT CResDoc::WriteAddonIcon(CFile &file, HINSTANCE hinst, const CResList &list)
{
   POSITION pos = list.GetHeadPosition();

   // attention les resources designees dans cette liste n'existent peut-etre pas
   while (pos) {
      const t_RESDEF &res = list.GetNext(pos);
      WriteRes(file, hinst, res.resName, res.resType);
   }
   return 0;
}
/////////////////////////////////////////////////////////////////////////////
// Search ICONs or BITMAPs used in Static Windows class

UINT CResDoc::SearchCDiagAddon(HINSTANCE hinst, LPCTSTR resName,
                               CResList &list)                 // liste de sortie
{
   bool loop = (resName == NULL);
   HTREEITEM hit = NULL;

   do {
      if (loop && !(resName = m_param.NextResource(hit, RT_DIALOG)) ) break;

      DWORD rsize;
      PWORD pWord = (PWORD)RawData(hinst, resName, RT_DIALOG, &rsize);
      PWORD pEnd = PWORD( PBYTE(pWord) + rsize );
      if (pWord == NULL) return IDS_DATAERROR;

      int cnt;
      DWORD  style;
      bool  exTmpl;
      CString clsTxt;
      if ( (exTmpl =((DLGTEMPLATEEX*)pWord)->signature == 0xFFFF) ) {
         cnt    = ((DLGTEMPLATEEX*)pWord)->cDlgItems;
         style  = ((DLGTEMPLATEEX*)pWord)->style;
         pWord  = (PWORD)((DLGTEMPLATEEX*)pWord + 1);
      } else {
         cnt    = ((DLGTEMPLATE*)pWord)->cdit;
         style  = ((DLGTEMPLATE*)pWord)->style;
         pWord  = (PWORD)((DLGTEMPLATE*)pWord + 1);
      }
      if (pWord >= pEnd) return IDS_DATAERROR;
      SkipResId(pWord);                                  // menu
      if (*pWord == 0xFFFF) pWord++;
      else SkipResId(pWord);                             // class

      SkipResId(pWord);                                  // title
      if (style & DS_SETFONT) {                          // font
         if (exTmpl) pWord += 3; else pWord++;
         SkipResId(pWord);
      }

      while (--cnt >= 0) {
         // Lecture composants du CDialog
         pWord = ALIGN_DWORD( PWORD, pWord );
         if (pWord >= pEnd) return IDS_DATAERROR;
         if (exTmpl) {
            style  =((DLGITEMTEMPLATEEX*)pWord)->style;
            pWord  = (PWORD)((DLGITEMTEMPLATEEX*)pWord + 1);
         } else {
            style  = ((DLGITEMTEMPLATE*)pWord)->style;
            pWord  = (PWORD)((DLGITEMTEMPLATE*)pWord + 1);
         }
         LPCTSTR classId  = ReadResId(pWord);            // class
         t_ClassStyle *pClass = ClassStyle(classId);
         clsTxt.Format("%s:%s", ClassName(pClass),
                             ClassTypeName(pClass, style, TRUE));

         LPCTSTR nameId = ReadResId(pWord);              // title
         if (nameId) {
            if (clsTxt == "Static:ICON") list.Add(nameId, RT_GROUP_ICON);
            else if (clsTxt.Compare("Static:BITMAP") == 0) list.Add(nameId, RT_BITMAP);
         }

         WORD dataLen = *pWord++;                        // data
         pWord = (PWORD)((BYTE*)pWord + dataLen);
         if (pWord > pEnd) return IDS_DATAERROR;
      }
   } while (hit);
   return 0;
}
/////////////////////////////////////////////////////////////////////////////
// Search a symbol in the resource library

void CResDoc::SearchSymbol(const CString &symbol)
{
   CTreeCtrl *pTree = m_param.pTree;
   if (pTree == NULL) return;
   LPCTSTR resType, resName;
   HTREEITEM hit = pTree->GetNextItem(TVI_ROOT, TVGN_CHILD);
   int sep = symbol.Find(TCHAR('/'));
   TRACE("CResDoc::SearchSymbol %s\n", symbol);

   hit = pTree->GetNextItem(hit, TVGN_CHILD);
   if (sep > 0) {
      // resType and resName are define (type/name)
      CString tname = symbol.Left(sep), rname = symbol.Mid(sep+1);
      m_param.symbol = rname;
      while (hit) {
         if (tname == pTree->GetItemText(hit)) {
            resType = (LPCTSTR)pTree->GetItemData(hit);
            HTREEITEM hii = pTree->GetNextItem(hit, TVGN_CHILD);
            while (hii) {
               if (rname == pTree->GetItemText(hii)) {
                  resName = (LPCTSTR)pTree->GetItemData(hii);
                  SetCurrent( NULL, resType, resName,
                                      RES_VIEW|LIST_VIEW|LEFT_VIEW_SELECT );
                  return;
               }
               hii = pTree->GetNextItem(hii, TVGN_NEXT);
            }
         }
         hit = pTree->GetNextItem(hit, TVGN_NEXT);
      }

   } else {
      // only a symbol
      m_param.symbol = symbol;
      while (hit) {
         resType = (LPCTSTR)pTree->GetItemData(hit);
         if (symbol == pTree->GetItemText(hit)) {
            SetCurrent( NULL, resType, NULL, LIST_VIEW|LEFT_VIEW_SELECT );
            return;
         }
         hit = pTree->GetNextItem(hit, TVGN_NEXT);
      }
/*      if (symbol.Left(4) == "IDS_") {
//         SetCurrent( NULL, RT_STRING, NULL, LIST_VIEW|LIST_VIEW_SYMBOL|LEFT_VIEW_SELECT );
//      } else {
         while (hit) {
            resType = (LPCTSTR)pTree->GetItemData(hit);
            if (symbol == pTree->GetItemText(hit)) {
               SetCurrent( NULL, resType, NULL, LIST_VIEW|LEFT_VIEW_SELECT );
               return;
            }
            HTREEITEM hii = pTree->GetNextItem(hit, TVGN_CHILD);
            while (hii) {
               if (symbol == pTree->GetItemText(hii)) {
                  resName = (LPCTSTR)pTree->GetItemData(hii);
                  SetCurrent( NULL, resType, resName,
                                              RES_VIEW|LIST_VIEW|LEFT_VIEW_SELECT );
                  return;
               }
               hii = pTree->GetNextItem(hii, TVGN_NEXT);
            }
            hit = pTree->GetNextItem(hit, TVGN_NEXT);
         }
      }*/
   }
   UpdateAllViews(NULL, LIST_VIEW_SYMBOL, &m_param);
}
/////////////////////////////////////////////////////////////////////////////
// Generate a symbol table with all symbols of all resources

void CResDoc::SymbolUpdate()
{
   HINSTANCE hinst = m_param.hLib;
   if (hinst) {
      SymbolStringUpd(hinst);
      SymbolAccelUpd(hinst);
      SymbolMenuUpd(hinst);
      SymbolDialogUpd(hinst);
      SymbolResTypeUpd();
   }
}
/////////////////////////////////////////////////////////////////////////////
// Generate a symbol table with the resource types

void CResDoc::SymbolResTypeUpd()
{
   DWORD type;
   LPCTSTR resType, resName;
   CTreeCtrl *pTree = m_param.pTree;
   HTREEITEM hit = pTree->GetNextItem(TVI_ROOT, TVGN_CHILD);

   hit = pTree->GetNextItem(hit, TVGN_CHILD);
   while (hit) {
      resType = (LPCTSTR)pTree->GetItemData(hit);
      type = (resType)? GetResEntryEx( resType )->type: 0;
      if (type & TYP_USR) SymbolGen(resType, TICON(type)|TYP_RTX);

      HTREEITEM hii = pTree->GetNextItem(hit, TVGN_CHILD);
      while (hii) {
         if (resName = (LPCTSTR)pTree->GetItemData(hii)) {
            SymbolGen(resName, type);
         }
         hii = pTree->GetNextItem(hii, TVGN_NEXT);
      }
      hit = pTree->GetNextItem(hit, TVGN_NEXT);
   }
}
/////////////////////////////////////////////////////////////////////////////
// Symbols counters

UINT CResDoc::SymbolLocate(CSymbolList &counts, bool fast)
{
   UINT tmp;
   t_SYMBOL fnd;
   LPCTSTR resType, resName;
   CTreeCtrl *pTree = m_param.pTree;
   if (m_param.hLib == NULL) return 0;
   t_SYMBOL &symbol = counts.GetTail();
   CString &name = symbol.name, typeText;
   HTREEITEM hit = pTree->GetNextItem(TVI_ROOT, TVGN_CHILD);
//   TRACE("CResDoc::SymbolLocate %s\n", name);

   fnd.type = TYP_NUM|TYP_DEC;
   hit = pTree->GetNextItem(hit, TVGN_CHILD);
   while (hit) {
      resType = (LPCTSTR)pTree->GetItemData(hit);
      typeText = pTree->GetItemText(hit);

      if (resType == RT_STRING) {
         if (tmp = IsStringValid(symbol)) {
            if (fast) return 1;
            fnd.name.Format("%s/%s", typeText, FormatId(tmp, TYP_NUM));
            fnd.value = 1;
            counts.AddHead(fnd);
         }
      } else {

         HTREEITEM hii = pTree->GetNextItem(hit, TVGN_CHILD);
         while (hii) {
            resName = (LPCTSTR)pTree->GetItemData(hii);

            if (name == pTree->GetItemText(hii)) fnd.value = 1;
            else fnd.value = 0;

            switch (DWORD(resType)) {
            case RT_MENU :      fnd.value += MenuCount(resName, symbol); break;
            case RT_DIALOG :    fnd.value += DialogCount(resName, symbol); break;
            case RT_ACCELERATOR:fnd.value += AccelCount(resName, symbol); break;
            }
            if (fnd.value > 0) {
               if (fast) return 1;
               fnd.name.Format("%s/%s", typeText, pTree->GetItemText(hii));
               counts.AddHead(fnd);
            }
            hii = pTree->GetNextItem(hii, TVGN_NEXT);
         }
      }
      if (name == typeText) {
         if (fast) return 1;
         fnd.name = typeText;
         fnd.value = 1;
         counts.AddHead(fnd);
      }
      hit = pTree->GetNextItem(hit, TVGN_NEXT);
   }
   return 0;
}
//// String list symbol validity check

UINT CResDoc::IsStringValid(t_SYMBOL &symbol)
{
   if (symbol.name == FormatId(symbol.value, TYP_IDS)) {
      char buffer[2];
      if (::LoadString(m_param.hLib, symbol.value, buffer, 2) > 0)
         return symbol.value / 16 + 1;
   }
   return 0;
}
//// in Menu resource Symbols counters

int CResDoc::MenuCount(LPCTSTR resName, t_SYMBOL &symbol)
{
   int count = 0;
   HMENU hMenu = ::LoadMenu(m_param.hLib, resName);
   if (hMenu) {
      count = SubmenuCount(hMenu, symbol);
      DestroyMenu(hMenu);
   }
   return count;
}

int CResDoc::SubmenuCount(HMENU hMenu, t_SYMBOL &symbol)
{
   CMenu *pMenu = CMenu::FromHandle(hMenu);
   int count = 0;

   if (SCompare(symbol, pMenu->GetMenuContextHelpId(), TYP_HIC)) count++;
   int n = pMenu->GetMenuItemCount();
   for (int i = 0; i < n; i++) {
      WORD id = pMenu->GetMenuItemID(i);
      if (SCompare(symbol, id, TYP_ID_|TYP_SEP)) count++;
      HMENU hsm = ::GetSubMenu(hMenu, i);
      if (hsm) count += SubmenuCount(hsm, symbol);
   }
   return count;
}
//// in Dialog box Resource Symbols counters

int CResDoc::DialogCount(LPCTSTR resName, t_SYMBOL &symbol)
{
   DWORD helpID, rsize, style;
   int count = 0, cnt;
   UINT id;
   bool  exTmpl;
   LPCTSTR nameId;
   CString clsTxt;
   PWORD pWord = (PWORD)RawData(m_param.hLib, resName, RT_DIALOG, &rsize);
   PWORD pEnd = PWORD( PBYTE(pWord) + rsize );
   if (pWord == NULL) return 0;

   if ( (exTmpl =((DLGTEMPLATEEX*)pWord)->signature == 0xFFFF) ) {
      cnt    = ((DLGTEMPLATEEX*)pWord)->cDlgItems;
      style  = ((DLGTEMPLATEEX*)pWord)->style;
      helpID = ((DLGTEMPLATEEX*)pWord)->helpID;
      pWord  = (PWORD)((DLGTEMPLATEEX*)pWord + 1);
      if (helpID) if (SCompare(symbol, helpID, TYP_HIR)) count++;
   } else {
      cnt    = ((DLGTEMPLATE*)pWord)->cdit;
      style  = ((DLGTEMPLATE*)pWord)->style;
      pWord  = (PWORD)((DLGTEMPLATE*)pWord + 1);
   }
   if (pWord >= pEnd) return 0;
   SkipResId(pWord);                                  // menu
   if (*pWord == 0xFFFF) pWord++;
   else SkipResId(pWord);                             // class

   SkipResId(pWord);                                  // title
   if (style & DS_SETFONT) {                          // font
      if (exTmpl) pWord += 3; else pWord++;
      SkipResId(pWord);
   }

   while (--cnt >= 0) {
      // Lecture composants du CDialog
      pWord = ALIGN_DWORD( PWORD, pWord );
      if (pWord >= pEnd) return 0;
      if (exTmpl) {
         id     = ((DLGITEMTEMPLATEEX*)pWord)->id;
         helpID = ((DLGITEMTEMPLATEEX*)pWord)->helpID;
         pWord  = (PWORD)((DLGITEMTEMPLATEEX*)pWord + 1);
         if (helpID) if (SCompare(symbol, helpID, TYP_HIC)) count++;
      } else {
         id     = ((DLGITEMTEMPLATE*)pWord)->id;
         pWord  = (PWORD)((DLGITEMTEMPLATE*)pWord + 1);
      }
      if (SCompare(symbol, id, TYP_IDC)) count++;

      LPCTSTR classId  = ReadResId(pWord);            // class
      t_ClassStyle *pClass = ClassStyle(classId);
      clsTxt.Format("%s:%s", ClassName(pClass),
                             ClassTypeName(pClass, style, TRUE));
      if (nameId = ReadResId(pWord)) {                // title
         if (clsTxt == "Static:ICON") {
            if (SCompare(symbol, nameId, TYP_IDI)) count++;
         } else if (clsTxt == "Static:BITMAP") {
            if (SCompare(symbol, nameId, TYP_IDB)) count++;
         }
      }

      WORD dataLen = *pWord++;                        // data
      pWord = (PWORD)((BYTE*)pWord + dataLen);
      if (pWord > pEnd) return 0;
   }
   return count;
}
//// In Accelerator resource Symbols counters

int CResDoc::AccelCount(LPCTSTR resName, t_SYMBOL &symbol)
{
   DWORD rsize;
   RACCEL *pAc = (RACCEL*)RawData(m_param.hLib, resName, RT_ACCELERATOR, &rsize);
   if (pAc == NULL) return 0;
   int count = 0;
   for (int nb = rsize / sizeof(RACCEL); --nb >= 0; pAc++) {
      if (SCompare(symbol, pAc->cmd, TYP_ID_)) count++;
   }
   return count;
}
/////////////////////////////////////////////////////////////////////////////
// Generate the symbol table with the components of RT_DIALOG resources

UINT CResDoc::SymbolDialogUpd(HINSTANCE hinst)
{
   LPCTSTR resName = NULL;
   HTREEITEM hit = NULL;
   DWORD helpID;
   DWORD rsize;
   UINT id;

   do {
      if ( (resName = m_param.NextResource(hit, RT_DIALOG)) == NULL ) break;

      PWORD pWord = (PWORD)RawData(hinst, resName, RT_DIALOG, &rsize);
      PWORD pEnd = PWORD( PBYTE(pWord) + rsize );
      if (pWord == NULL) return IDS_DATAERROR;

      int cnt;
      DWORD  style;
      bool  exTmpl;
      CString clsTxt;
      if ( (exTmpl =((DLGTEMPLATEEX*)pWord)->signature == 0xFFFF) ) {
         cnt    = ((DLGTEMPLATEEX*)pWord)->cDlgItems;
         style  = ((DLGTEMPLATEEX*)pWord)->style;
         helpID = ((DLGTEMPLATEEX*)pWord)->helpID;
         pWord  = (PWORD)((DLGTEMPLATEEX*)pWord + 1);
         if (helpID) SymbolGen(helpID, TYP_HIR | RI_HELP);
      } else {
         cnt    = ((DLGTEMPLATE*)pWord)->cdit;
         style  = ((DLGTEMPLATE*)pWord)->style;
         pWord  = (PWORD)((DLGTEMPLATE*)pWord + 1);
      }
      if (pWord >= pEnd) return IDS_DATAERROR;
      SkipResId(pWord);                                  // menu
      if (*pWord == 0xFFFF) pWord++;
      else SkipResId(pWord);                             // class

      SkipResId(pWord);                                  // title
      if (style & DS_SETFONT) {                          // font
         if (exTmpl) pWord += 3; else pWord++;
         SkipResId(pWord);
      }

      while (--cnt >= 0) {
         // Lecture composants du CDialog
         pWord = ALIGN_DWORD( PWORD, pWord );
         if (pWord >= pEnd) return IDS_DATAERROR;
         if (exTmpl) {
            id     = ((DLGITEMTEMPLATEEX*)pWord)->id;
            helpID = ((DLGITEMTEMPLATEEX*)pWord)->helpID;
            pWord  = (PWORD)((DLGITEMTEMPLATEEX*)pWord + 1);
            if (helpID) SymbolGen(helpID, TYP_HIC | RI_HELP);
         } else {
            id     = ((DLGITEMTEMPLATE*)pWord)->id;
            pWord  = (PWORD)((DLGITEMTEMPLATE*)pWord + 1);
         }

         LPCTSTR classId  = ReadResId(pWord);            // class
         t_ClassStyle *pClass = ClassStyle(classId);
         ClassTypeName(pClass, style, TRUE);
         SkipResId(pWord);                               // title

         SymbolGen(id, TYP_IDC | pClass->image);

         WORD dataLen = *pWord++;                        // data
         pWord = (PWORD)((BYTE*)pWord + dataLen);
         if (pWord > pEnd) return IDS_DATAERROR;
      }
   } while (hit);
   return 0;
}
/////////////////////////////////////////////////////////////////////////////
// Generate the symbol table with the components of RT_STRING resources

void CResDoc::SymbolStringUpd(HINSTANCE hinst)
{
   LPCTSTR resName = NULL;
   HTREEITEM hit = NULL;
   DWORD rsize;

   do {
      if ( (resName = m_param.NextResource(hit, RT_STRING)) == NULL ) break;

      PWORD pWord = (PWORD)RawData(hinst, resName, RT_STRING, &rsize);
      if (pWord) {
         DWORD id = DWORD(resName) * 16 - 16;
         int size = rsize / 2;
         while (size > 0) {
            if ( !ExtractString(pWord, size).IsEmpty() )
               SymbolGen(id, TYP_IDS | RI_STRING);

            id++;
         }
      }
   } while (hit);
}
/////////////////////////////////////////////////////////////////////////////
// Generate the symbol table with the Menu Resource

void CResDoc::SymbolMenuUpd(HINSTANCE hinst)
{
   LPCTSTR resName;
   HTREEITEM hit = NULL;

   do {
      if (resName = m_param.NextResource(hit, RT_MENU)) {
         HMENU hMenu = ::LoadMenu(hinst, resName);
         if (hMenu) {
            SymbolSubmenuUpd(hinst, hMenu);
            DestroyMenu(hMenu);
         }
      }
   } while (hit);
}

void CResDoc::SymbolSubmenuUpd(HINSTANCE hinst, HMENU hMenu)
{
   CMenu *pMenu = CMenu::FromHandle(hMenu);
   DWORD helpId = pMenu->GetMenuContextHelpId();

   if (helpId) SymbolGen(helpId, TYP_HIC | RI_HELP);
   int n = pMenu->GetMenuItemCount();
   for (int i = 0; i < n; i++) {
      WORD id = pMenu->GetMenuItemID(i);
      if (id != 0 && id != 0xFFFF) SymbolGen(id, TYP_ID_ | RI_MENUCMD);

      HMENU hsm = ::GetSubMenu(hMenu, i);
      if (hsm) SymbolSubmenuUpd(hinst, hsm);
   }
}
/////////////////////////////////////////////////////////////////////////////
// Generate the symbol table for the RT_ACCELERATOR

void CResDoc::SymbolAccelUpd(HINSTANCE hinst)
{
   HTREEITEM hit = NULL;
   LPCTSTR resName;
   DWORD rsize;

   do {
      if ( (resName = m_param.NextResource(hit, RT_ACCELERATOR)) == NULL) break; 
      RACCEL *pAc = (RACCEL*)RawData(hinst, resName, RT_ACCELERATOR, &rsize);
      if (pAc) {
         for (int nb = rsize / sizeof(RACCEL); --nb >= 0; pAc++) {
            SymbolGen(pAc->cmd, TYP_ID_ | RI_MENUCMD);
         }
      }
   } while (hit);
}
/////////////////////////////////////////////////////////////////////////////
// Copie la resource dans le presse papier

void CResDoc::CopyOnClipbord(UINT uFormat, HANDLE pData, DWORD size)
{
   // Make a buffer to send to clipboard
   HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE|GMEM_DDESHARE, size );
   if (hGlobal) {
      LPVOID pBits = ::GlobalLock( hGlobal );
      // Copy the bits to the buffer
      memcpy( pBits, pData, size );
      ::GlobalUnlock( hGlobal );
      // Send it to the clipboard
      CopyOnClipbord( uFormat, hGlobal );
   }
}

void CResDoc::CopyOnClipbord(UINT uFormat, HGLOBAL hGlobal)
{
   // Open the clipboard
   if ( hGlobal && AfxGetMainWnd()->OpenClipboard() ) {
      // Empty it
      if ( ::EmptyClipboard() ) {
         // Send it to the clipboard
         ::SetClipboardData( uFormat, hGlobal );
         ::CloseClipboard();
         GlobalFree( hGlobal );
      }
   }
}
/////////////////////////////////////////////////////////////////////////////
/*
BOOL CResDoc::CreateToolFile(const CString &path1,
                                     const CString &path2, PPERsrcInst RsrcInst)
{
	DlgData dlgData(path1, TRUE);
   if (!path2.IsEmpty()) {
      PPERsrcInst RsrcBmp = GetResourceInst((WORD)RT_BITMAP, RsrcInst->Name());
      if (RsrcBmp == NULL) return FALSE;
      CreateBMPFile(path2, RsrcBmp);
      delete RsrcBmp;
   }
   dlgData.DumpToolbar(RsrcInst->GetData(), RsrcInst->Name(), path2, FALSE);
	return dlgData.DumpWrite();
}

BOOL CResDoc::CreateToolBmpFile(const CString &path, PPERsrcInst RsrcInst)
{
	DlgData dlgData(path, TRUE);
   CreateBMPFile(path, RsrcInst);
   dlgData.DumpToolbar((PVOID)&m_toolbar, RsrcInst->Name(),
                                                PathRenameExt(path, "dlg"), TRUE);
	return dlgData.DumpWrite();
}

BOOL CResDoc::CreateMenuFile(const CString &path, PPERsrcInst RsrcInst)
{
	DlgData dlgData(path, TRUE);
   dlgData.DumpMenu(RsrcInst->GetData(), RsrcInst->Name());
	return dlgData.DumpWrite();
}

BOOL CResDoc::CreateStringFile(const CString &path, PPERsrcInst RsrcInst)
{
	DlgData dlgData(path, TRUE);
   dlgData.DumpString(RsrcInst->GetData(), RsrcInst->Name());
	return dlgData.DumpWrite();
}
*/
/////////////////////////////////////////////////////////////////////////////
//
void CResDoc::ResourceVerify(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType)
{
   HRSRC hsrc = ::FindResource(hinst, resName, resType);
   if (hsrc) {
      HGLOBAL hg = ::LoadResource(hinst, hsrc);
      PVOID pt = ::LockResource(hg);
      DWORD rs = ::SizeofResource(hinst, hsrc);

//      for (t_ResID* p = m_ResSign; *p->text; p++) ;
   }
}
/////////////////////////////////////////////////////////////////////////////
// Class PARAM

LPCTSTR CParams::NextResource(HTREEITEM& pos, LPCTSTR lpType)
{
   LPCTSTR lpName = NULL;
   HTREEITEM hit = pos;

   if (lpType == NULL) lpType = resType;
   if (pTree) {
      if (hit == NULL) {
         hit = pTree->GetNextItem(TVI_ROOT, TVGN_CHILD);
         hit = pTree->GetNextItem(hit, TVGN_CHILD);
         while (hit) {
//            if (ResIdCompare(lpType, (LPCTSTR)pTree->GetItemData(hit))) {
            if (lpType == LPCTSTR(pTree->GetItemData(hit))) {
               hit = pTree->GetNextItem(hit, TVGN_CHILD);
               break;
            }
            hit = pTree->GetNextItem(hit, TVGN_NEXT);
         }
      }
      if (hit) {
         lpName = (LPCTSTR)pTree->GetItemData(hit);
         hit = pTree->GetNextItem(hit, TVGN_NEXT);
      }
   } else {
      pos = NULL;
   }
   pos = hit;
   return lpName;
}

LPCTSTR CParams::NextResourceType(HTREEITEM& pos)
{
   LPCTSTR lpType = NULL;
   HTREEITEM hit = pos;
   if (pTree) {
      if (hit == NULL) {
         hit = pTree->GetNextItem(TVI_ROOT, TVGN_CHILD);
         hit = pTree->GetNextItem(hit, TVGN_CHILD);
      }
      if (hit) {
         lpType = (LPCTSTR)pTree->GetItemData(hit);
         hit = pTree->GetNextItem(hit, TVGN_NEXT);
      }
   } else {
      pos = NULL;
   }
   pos = hit;
//   resType = lpType;
//   resName = NULL;
   return lpType;
}

WORD CParams::GetView()
{
   WORD ret = 0;
   if (pRID) {
      ret = resName? pRID->vInst: pRID->vType;
   } else if (hLib) ret = WRT;
   return ret; 
}

CString CParams::GetFilter()
{
   CString ext;
   if (pRID && resType) {
      if (resName && pRID->filter) ext = CString(pRID->filter) + '|';
   }
   ext += "Resource (*.res)|*.res|Data (*.*)|*.*|";
   return ext;
}

CString CParams::GetDefName()
{
   CString name;
   if (resName)
      name.Format("%s_%s", libTitle, GetResStr(resName, FALSE));
   else if (pRID)
      name.Format("%s %s", libTitle, pRID->fname);
   else
      name.Format("%s All", libTitle);
   return name;
}

BOOL CParams::NewLibrary(LPCTSTR lpszPathName)
{
   TCHAR tmp[MAX_PATH];
   CString filnam;

   FreeLibrary();
   IPictureRelease();
   UniqueResIdInitialise();

   strcpy(tmp, ::PathFindFileName(lpszPathName));

   // Warning : it's desallocation problem with this dll
   limited = (CString(tmp).CompareNoCase(_T("user32.dll")) == 0);
   
   // Read a new library
   if ( !(freeEn = (hLib = ::GetModuleHandle(lpszPathName)) == NULL) ||
       (!limited && !(limited = (hLib = ::LoadLibrary(lpszPathName)) == NULL)) ||
        (hLib = ::LoadLibraryEx(lpszPathName, NULL, LOAD_LIBRARY_AS_DATAFILE)) ){

      ::PathRemoveExtension(tmp);
      libTitle= CString(tmp);
      libName = lpszPathName;
      resName = NULL;   // dosnt clear retType to keep the same res. type display
      item    = 0;
      pRID    = NULL;

//    TRACE("CParams::NewLibrary l=%d f=%d %s\n", limited, freeEn, lpszPathName);
      return TRUE;

   } else {
      Reset();
      limited = FALSE;
      libTitle = libName = "";
      return FALSE;
   }
}

void CParams::FreeLibrary()
{
   if (hLib && freeEn) ::FreeLibrary(hLib);

   hLib = NULL;
   FreeAni();
   SEmpty();
}

VOID CParams::Save(LPCTSTR rName, LPCTSTR rType)
{
   if (pRID && resType && (resName != rName || resType != rType)) {
      TRACE("CParams::Save t=%08x n=%08x\n", resType, resName);
      sv[2] = sv[1], sv[1] = sv[0];
      sv[0].item = item;
      sv[0].pRID = pRID;
      sv[0].resName = resName;
      sv[0].resType = resType;
   }
   resName = rName, resType = rType;
}

BOOL CParams::Restore()
{
   if ( IsUndo() ) {
      item = sv[0].item;
      pRID = sv[0].pRID;
      resName = sv[0].resName;
      resType = sv[0].resType;
      sv[0] = sv[1]; sv[1] = sv[2], sv[2].pRID = NULL;
      return TRUE;
   }
   return FALSE;
}
