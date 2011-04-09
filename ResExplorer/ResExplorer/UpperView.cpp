// UpperView.cpp : implementation file
//

#include "DirDoc.h"
#include "DirView.h"
#include "stdafx.h"
#include "MainFrm.h"
#include "DirPickDlg.h"
#include "Utilities.h"
#include <Shlwapi.h>
#include <afxpriv.h>
#include <afxole.h>

#include "MsgDecode.h"

#include "UpperView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpperView

IMPLEMENT_DYNCREATE(CUpperView, CListView)

CUpperView::CUpperView()
{
   m_oldSelect = -1;
   m_pThread = NULL;
   m_hCursor = LoadCursor(NULL, IDC_ARROW);
   m_hCurWait= LoadCursor(NULL, IDC_WAIT);
   m_pParam = NULL;
   m_pSource = NULL;
}

CUpperView::~CUpperView()
{
   if (m_pSource) delete m_pSource;
}

BOOL CUpperView::PreCreateWindow(CREATESTRUCT& cs) 
{
   cs.style |= LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_SHAREIMAGELISTS;	
	
	return CListView::PreCreateWindow(cs);
}

BOOL CUpperView::PreTranslateMessage(MSG* pMsg) 
{
	m_toolView.RelayEvent(pMsg);
	return CListView::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CUpperView, CListView)
	//{{AFX_MSG_MAP(CUpperView)
	ON_NOTIFY_REFLECT(LVN_BEGINRDRAG, OnBeginrdrag)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_KEYUP()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
   ON_WM_SETCURSOR()
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_UVSORTCOL00, ID_UVSORTCOL12, OnSortColumn)
   ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI) // handle idle UI msg
   ON_MESSAGE(TTM_GETTEXT, OnToolTipText)
   ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpperView diagnostics

#ifdef _DEBUG
void CUpperView::AssertValid() const
{
	CListView::AssertValid();
}

void CUpperView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CResDoc* CUpperView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResDoc)));
	return (CResDoc*)m_pDocument;
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CUpperView message handlers

int CUpperView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SHFILEINFO sh;
   CListCtrl& list = GetListCtrl();

   list.SetImageList(CImageList::FromHandle((HIMAGELIST)::SHGetFileInfo("C:\\",
         0, &sh, sizeof(sh), SHGFI_SYSICONINDEX|SHGFI_SMALLICON)), LVSIL_SMALL);

   // define the columns
   int col = InitHeader(list, IDS_RER_UPPERVIEW) - 1;
   m_subItemLast = InitHeader(list, InitIndexTbl(col), col);

   list.GetHeaderCtrl()->SetWindowContextHelpId(0x20000 + IDR_UPPERVIEWHD);
   list.SetExtendedStyle(list.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
   SetWindowContextHelpId(0x20000 + IDR_UPPERVIEW);

   m_toolView.Create(this, &list);

	return 0;
}

void CUpperView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
}

void CUpperView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   if (pHint) m_pParam = (CParams*)pHint;
   if (lHint & UPPER_VIEW) {
      m_oldSelect = -1;
      // lance l'analyse des fichiers
      if (m_pThread == NULL) m_pThread = AfxBeginThread(ThreadProcNL, this);
   }
}

LRESULT CUpperView::OnToolTipText(WPARAM id, LPARAM lParam)
{
   static CString msg;
   TOOLINFO* pInfo = (TOOLINFO*)lParam;
   int iItem = HIWORD(id), iSubItem = LOWORD(id);

   msg.LoadString( (iItem == 0xFFFF)? IDS_TTPSUPPERVIEW_H: IDS_TTPSUPPERVIEW_L); 

   pInfo->lpszText = msg.GetBuffer(1);
   return 0;		// not use
}

void CUpperView::OnEditPaste() 
{
   m_oldSelect = -1;
   // lance l'analyse des fichiers
   if (m_pThread == NULL) m_pThread = AfxBeginThread(ThreadProcCB, this);
}

void CUpperView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(::IsClipboardFormatAvailable(CF_HDROP) );
}

void CUpperView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
   ReadItem( ((NM_LISTVIEW*)pNMHDR)->iItem, FALSE );
	*pResult = 0;
}

void CUpperView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
   ReadItem( ((NM_LISTVIEW*)pNMHDR)->iItem, TRUE );
	*pResult = 0;
}

void CUpperView::OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
   CString path = GetPathName( ((NM_LISTVIEW*)pNMHDR)->iItem );
   // Plus two blanks : one for end of string, one for end of list
   int nSize = sizeof(DROPFILES) + path.GetLength() + 2;
   HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE, nSize );
   DROPFILES* pDf = (DROPFILES*)::GlobalLock( hGlobal );

   memset( pDf, 0, nSize );
   // Offset of the file list from the beginning of this structure
   pDf->pFiles = sizeof(DROPFILES);
   // The file list contains ANSI characters
   strcpy( LPSTR(pDf + 1), path.GetBuffer(0) );
   ::GlobalUnlock( hGlobal );

   if (m_pSource) delete m_pSource;
   m_pSource = new COleDataSource();
   m_pSource->CacheGlobalData( CF_HDROP, hGlobal );
   // Performs a drag-and-drop operation with the selected file
   m_pSource->DoDragDrop();

	*pResult = 0;
}

void CUpperView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
   int iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;

   BeginWaitCursor();
   ColumnSort(GetListCtrl(), iSubItem, CS_REVERSE);
   PutSortMessage();
   EndWaitCursor();

	*pResult = 0;
}

void CUpperView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListView::OnKeyUp(nChar, nRepCnt, nFlags);
   if (nChar == VK_UP || nChar == VK_DOWN) ReadSelected(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// Context Menu (Column sort command)

void CUpperView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   CMenu menu;
   CRect headRect;
   CListCtrl &list = GetListCtrl();
   list.GetHeaderCtrl()->GetWindowRect(headRect);
   if (list.GetItemCount() > 1 && headRect.PtInRect(point)) {
      menu.LoadMenu(IDR_UVSORTHD);
      InsertMenu(list, &menu, ID_UVSORTCOL00, &m_sIcon);
      menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
                                          point.x, point.y, this);
   }
}

void CUpperView::OnSortColumn(UINT id) 
{
   BeginWaitCursor();
	ColumnSort(GetListCtrl(), id - ID_UVSORTCOL00, CS_REVERSE);
   PutSortMessage();
   EndWaitCursor();
}
/////////////////////////////////////////////////////////////////////////////
// Taches et mecanisme associe permettant l'analyse des fichiers selectes

UINT CUpperView::ThreadProcNL(LPVOID pParam)
{
   CUpperView *pView = (CUpperView*)pParam;
   // analyse des fichiers
   pView->NewList();
   // message indiquant la fin de l'analyse
   pView->PostMessage(WM_ACTIVATEAPP, FALSE, (LPARAM)AfxGetThread());
   return TRUE;
}

UINT CUpperView::ThreadProcCB(LPVOID pParam)
{
   CUpperView *pView = (CUpperView*)pParam;
   // analyse des fichiers designes par le presse papier
   pView->ClipBoardList();
   // message indiquant la fin de l'analyse
   pView->PostMessage(WM_ACTIVATEAPP, FALSE, (LPARAM)AfxGetThread());
   return TRUE;
}

void CUpperView::OnActivateApp( BOOL bActive, DWORD hTask )
{
   // traitement de la fin de l'analyse
   if ( (CWinThread*)hTask == m_pThread && ! bActive ) {
      m_pThread = NULL;
	   ((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);
      SetCursor(m_hCursor);

      // si un seul fichier est analyse, il sera aussi lu
      if (m_fileCnt == 1) ReadItem( 0, FALSE );
   }
}

BOOL CUpperView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_pThread == NULL) return CListView::OnSetCursor(pWnd, nHitTest, message);

   SetCursor(m_hCurWait);
   return TRUE;
}

LRESULT CUpperView::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
   if (m_pThread) {
      CString cntMsg;
      cntMsg.Format(IDS_ANALISEDFILES,  m_fileCnt);
      ((CMainFrame*)AfxGetMainWnd())->SetMessageText(cntMsg);
   }
   return 0L;
}
/////////////////////////////////////////////////////////////////////////////
// Subroutines

void CUpperView::ReadSelected(BOOL AddToRecentFileList)
{
   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   if (pos) {
      // note l'item selecte
      ReadItem(list.GetNextSelectedItem(pos), AddToRecentFileList);
   }
}

void CUpperView::ReadItem(int no, BOOL AddToRecentFileList)
{
   CListCtrl& list = GetListCtrl();
   CString path = GetPathName(no);
   TRACE("CUpperView::ReadItem %s\n", path);
   if (no >= 0) {
      if (no != m_oldSelect) ((CResDoc*)GetDocument())->ReadDocument(path);
      if (AddToRecentFileList) AfxGetApp()->AddToRecentFileList(path);
      m_oldSelect = no;
   }
}

BOOL CUpperView::NewList()
{
   CListCtrl& list = GetListCtrl();
   if (m_pParam == NULL) return FALSE;
   list.DeleteAllItems();

   ReadList(m_pParam->filesPath, m_pParam->selPath);

   return TRUE;
}

CString CUpperView::GetPathName(int no)
{
   CListCtrl& list = GetListCtrl();
   return list.GetItemText(no, m_subItemLast) + "\\" + list.GetItemText(no, 0);
}

void CUpperView::PutSortMessage()
{
   int status = SortStatus(GetListCtrl());
   CString msg, sort;

   sort.LoadString((status & 1)? IDS_SORTORDERFOR: IDS_SORTORDERBACK);
   msg.Format(IDS_SORTHDMSG, sort, GetColumnTitle(GetListCtrl(), status / 2));
   ((CMainFrame*)AfxGetMainWnd())->SetMessageText(msg);
}
/////////////////////////////////////////////////////////////////////////////
// La liste des fichiers a analyser peut etre fournie par Windows Explorer
//    par l'intermediaire du presse papier

BOOL CUpperView::ClipBoardList()
{
   CListCtrl& list = GetListCtrl();
   list.DeleteAllItems();
   BOOL ret = FALSE;
   if (OpenClipboard()) {
      HGLOBAL hglb = ::GetClipboardData(CF_HDROP); 
      if (hglb) {
         DROPFILES* pDf = (DROPFILES*)GlobalLock(hglb);
         TCHAR c;
         if (pDf->fWide) {
            PWORD pW = PWORD(PBYTE(pDf)+pDf->pFiles);

            while (*pW) {
               CString path;
               while (c = (TCHAR)*pW++) path += c;
               ret = ReadList(path, "*.*");
            }
         } else {
            PBYTE pB = PBYTE(PBYTE(pDf)+pDf->pFiles);

            while (*pB) {
               CString path;
               while (c = (TCHAR)*pB++) path += c;
               ret = ReadList(path, "*.*");
            }
         }
         GlobalUnlock(hglb);
      }
      CloseClipboard();
   }
   return ret;
}
/////////////////////////////////////////////////////////////////////////////
// Lecture et analyse du fichier (ou des fichiers) designe(s)
//    si path designe une directory et selPath indique les extensions
//         des fichiers contenus qui seront analyses

BOOL CUpperView::ReadList(LPCTSTR path, LPCTSTR selPath)
{
   CListCtrl &list = GetListCtrl();
   TCHAR tmp[MAX_PATH];
   WIN32_FIND_DATA fd;
   strcpy(tmp, path);
   m_fileCnt = 0;
   int len = strlen(tmp) - 1;          // dernier char du path
   if (len >= 0 && tmp[len] == '\\') tmp[len] = 0;

	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_FILEEXPLORE);
   SortStatus(list, -1);               // no sorted line

   HANDLE hs = ::FindFirstFile(tmp, &fd);

   if (hs == INVALID_HANDLE_VALUE
      || fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      // Drive or directory
      DirectorySearch(path, selPath);
   } else {
      // File name
      ::PathRemoveFileSpec(tmp);
      ResourceCount( tmp, &fd );
   }
   ::FindClose(hs);
   ColumnWidthAdjust(list);

   return TRUE;
}

BOOL CUpperView::DirectorySearch(LPCTSTR path, LPCTSTR selPath)
{
   TCHAR tmp[MAX_PATH];
   WIN32_FIND_DATA fd;

   ::PathCombine(tmp, path, "*.*");
   HANDLE hs = ::FindFirstFile(tmp, &fd);
   if (hs != INVALID_HANDLE_VALUE) {
      do {
         if ((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0) {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
               // fichiers
               if (::PathMatchSpec(fd.cFileName, selPath))
                  ResourceCount( path, &fd );

            } else if (fd.cFileName[0] != '.') {
               // directory
               ::PathCombine(tmp, path, fd.cFileName);
               DirectorySearch(tmp, selPath);
            }
         }
      } while (::FindNextFile(hs, &fd));
      ::FindClose(hs);
   }
   return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// Assignation des Colonnes 

t_RESCNT CUpperView::m_resTbl[] = {
   {RT_DIALOG,    }, 
   {RT_BITMAP,    }, 
   {RT_GROUP_ICON,}, 
   {RT_ANIICON,   }, 
   {RT_GROUP_CURSOR,}, 
   {RT_ANICURSOR, }, 
   {RT_CURSOR,    },
   {RT_FONT,      }, 
   {RT_FONTDIR,   }, 
   {RT_HTML,      }, 
   {RT_ICON,      }, 
   {RT_ACCELERATOR,}, 
   {RT_MENU,      }, 
   {RT_MESSAGETABLE,}, 
   {RT_PLUGPLAY,  }, 
   {RT_RCDATA,    }, 
   {RT_STRING,    }, 
   {RT_VERSION,   },
   {RT_VXD,       },
   {NULL,         }   };

t_RESINDEX CUpperView::m_ixTbl[] = {
   {RT_DIALOG,       "Dlg", },
   {RT_BITMAP,       "Bmp", },
   {RT_GROUP_CURSOR, "Curs",},
   {RT_GROUP_ICON,   "Icon",},
   {RT_MENU,         "Menu",},
   {RT_ANICURSOR,    NULL,  },
   {RT_ANIICON,      NULL,  },
   {RT_STRING,       NULL,  },
   {RT_VERSION,      NULL,  },
   {RT_ACCELERATOR,  NULL,  },
   {NULL,            "User",}  };

CString CUpperView::InitIndexTbl(int col)
{
   CString index, tmp;

   for (t_RESINDEX *pI = m_ixTbl; ;pI++) {
      for (t_RESCNT *pR = m_resTbl; pR->id != pI->id; pR++);
      pI->pR = pR;
      if (pI->name) {
         tmp.Format("NR%s\n", pI->name);
         index += tmp;
         pI->col = col++;
      }
      if (pI->id == NULL) break;
   }
   return index;
}
/////////////////////////////////////////////////////////////////////////////
// Comptage des resources contenues dans un fichier (exe, dll ou autre)

void CUpperView::ResourceCount(LPCTSTR path, WIN32_FIND_DATA *pFD)
{
   TCHAR full[MAX_PATH];
   LARGE_INTEGER li;
   CListCtrl &list = GetListCtrl();

   li.LowPart = pFD->nFileSizeLow;
   li.HighPart= pFD->nFileSizeHigh;
   ::PathCombine(full, path, pFD->cFileName);
   HINSTANCE hLib = ::LoadLibraryEx(full, NULL, LOAD_LIBRARY_AS_DATAFILE);

   if (hLib) {
      SHFILEINFO sh;
      ::SHGetFileInfo(full, 0, &sh, sizeof(sh), SHGFI_SYSICONINDEX);
      int no = list.InsertItem(0x7FFF, pFD->cFileName, sh.iIcon);

      CString lenStr;
      lenStr.Format("%.2f Ko", double(li.QuadPart) / 1000.);
      list.SetItemText(no, 1, lenStr);

      // set to null all counters
      for (t_RESCNT *pt = m_resTbl; ; pt++) {
         pt->count = pt->size = 0;
         if (pt->id == 0) break;
      }
      // Count all resoures in the file 
      ::EnumResourceTypes(hLib, EnumTypesFunc, (LPARAM)m_resTbl);

      ::FreeLibrary(hLib);

      // Update a list
      CString sizTxt, cntTxt, tmpTxt="*";
      int sz = 9, count = 0, col;
      for (t_RESINDEX *pI = m_ixTbl; ; pI++) {
         t_RESCNT *pR = pI->pR;
         sz  += pR->size;
         count += pR->count;
         if (pI->name) {
            tmpTxt.Format("%d", pI->pR->count);
            list.SetItemText(no, col = pI->col, tmpTxt);
         }
         if (pI->id == NULL) break;
      }
      list.SetItemText(no, col+1, path);
      sizTxt.Format("%.2f Ko", double(sz)/1000.);
      cntTxt.Format("%d", count);
      list.SetItemText(no, 2, sizTxt);
      list.SetItemText(no, 3, cntTxt);

      if ((++m_fileCnt & 0x000F) == 0) ColumnWidthAdjust(list);
   }
}

BOOL WINAPI CUpperView::EnumTypesFunc(HMODULE hModule, LPSTR lpType, LPARAM lParam)
{
   for (t_RESCNT *pt = (t_RESCNT*)lParam; pt->id; pt++) if (lpType == pt->id) break;

   ::EnumResourceNames(hModule, lpType, EnumNamesFunc, (LPARAM)pt);
   return TRUE;
}

BOOL WINAPI CUpperView::EnumNamesFunc(HMODULE hModule, LPCTSTR lpType,
                                      LPSTR lpName, LPARAM lParam)
{
   HRSRC hResInfo = FindResource(hModule, lpType, lpName);
   int size = SizeofResource(hModule, hResInfo);
   ((t_RESCNT*)lParam)->count += 1;
   ((t_RESCNT*)lParam)->size += size;
//   ::EnumResourceLanguages(hModule, lpType, lpName, EnumLangsFunc, lParam);
   return TRUE;
}

BOOL WINAPI CUpperView::EnumLangsFunc(HMODULE hModule, LPCTSTR lpType,
                                      LPCTSTR lpName, WORD wLang, LPARAM lParam)
{

   return TRUE;
}
