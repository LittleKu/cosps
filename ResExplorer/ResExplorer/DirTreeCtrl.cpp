// DirTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DirTreeCtrl.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirTreeCtrl

CDirTreeCtrl::CDirTreeCtrl()
{
   m_timer = 0;
}

CDirTreeCtrl::~CDirTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CDirTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDirTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirTreeCtrl message handlers

void CDirTreeCtrl::OnTimer(UINT nIDEvent) 
{
	CheckDrive();
	CTreeCtrl::OnTimer(nIDEvent);
}

void CDirTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
   if (pNMTreeView->itemNew.lParam)
      DirectoryUpdat(pNMTreeView->itemNew.hItem);

	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
// Subroutines

/////////////////////////////////////////////////////////////////////////////
// Creation d'une nouvelle arborescence de fichiers

CString CDirTreeCtrl::NewTree(LPCTSTR fileSelPath, LPCTSTR defPath)
{
   CString path = defPath? defPath: GetItemPath();
   LPSHELLFOLDER  lpsf = NULL, lpsf2 = NULL;
   LPITEMIDLIST   lpi  = NULL, lpi2 = NULL;
   LPENUMIDLIST   lpe  = NULL;
   HRESULT        hr;
	SHFILEINFO     sh;
   LPMALLOC       lpMalloc = NULL;
   TVINSERTSTRUCT tvi;

   tvi.hParent = TVI_ROOT;
   tvi.hInsertAfter = TVI_LAST;
   tvi.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	
   DeleteAllItems();

   // Get a system image list
   if (GetImageList(TVSIL_NORMAL) == NULL)
      SetImageList( CImageList::FromHandle( HIMAGELIST(::SHGetFileInfo("C:\\",
         0, &sh, sizeof(sh), SHGFI_SYSICONINDEX|SHGFI_SMALLICON)) ), TVSIL_NORMAL );

   // Allocate a shell memory object. 
   hr = ::SHGetMalloc(&lpMalloc);

   // Get a pointer to the desktop folder and drives folder
   hr = ::SHGetDesktopFolder(&lpsf);
   hr = ::SHGetSpecialFolderLocation( NULL, CSIDL_DRIVES, &lpi );
   hr = lpsf->BindToObject(lpi, 0, IID_IShellFolder, (LPVOID*)&lpsf2);

   // Get the IEnumIDList object for the given folder.
   hr = lpsf2->EnumObjects( NULL, SHCONTF_FOLDERS, &lpe );

   // Enumerate throught the list of folder objects.
   while (S_OK == lpe->Next(1, &lpi2, NULL)) {

       CString name = GetName(lpsf2, lpi2, SHGDN_NORMAL);
       CString driv = GetDrive(name);

       // Add a drive entry to the tree
       if (!driv.IsEmpty()) SetDirItem(&tvi, driv + "\\", name, FALSE);
   }

   if (lpMalloc) {
      if (lpi2) lpMalloc->Free(lpi2);
      if (lpi) lpMalloc->Free(lpi);
      if (lpe) lpe->Release();
      if (lpsf) lpsf->Release();
      if (lpsf2) lpsf2->Release();
      lpMalloc->Release();
   }

   PathSelect(path, FALSE);
   m_hitDrv = GetChildItem(TVI_ROOT);
   m_timer = SetTimer(1, 500, 0);
   return path;
}

BOOL CDirTreeCtrl::DeleteAllItems()
{
   if (m_timer) KillTimer(m_timer);
   m_timer = 0;
   m_hitDrv = NULL;
   return CTreeCtrl::DeleteAllItems();
}
/////////////////////////////////////////////////////////////////////////////
// Mise a jour en tache de fond des folders de drives
 
BOOL CDirTreeCtrl::CheckDrive()
{
   if (m_hitDrv) {
      CString path = GetItemText(m_hitDrv);
      if ((GetItemData(m_hitDrv) & 1) && PathIsDirEmpty(path))
         DirectorySetEmpty(m_hitDrv);
      m_hitDrv = GetNextSiblingItem(m_hitDrv);
      return TRUE;
   }
   if (m_timer) KillTimer(m_timer);
   m_timer = 0;
   return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// Extrait le nom de fichier du noeud

CString CDirTreeCtrl::GetItemText(HTREEITEM hItem)
{
   CString path = CTreeCtrl::GetItemText(hItem);
   int n = path.ReverseFind(':');
   // c'est le nom du drive, on l'extrait
   if (n > 0) return path.Mid(n - 1, 2);
   path.TrimLeft();  // elimine espace devant le nom de directory
   return path;
}
/////////////////////////////////////////////////////////////////////////////
// Extrait le path correspondant a un noeud (ou au noeud selectionne)

CString CDirTreeCtrl::GetItemPath(HTREEITEM hit)
{
   CString path;
   if (!hit) hit = GetSelectedItem();
   if (hit) {
      path = GetItemText(hit);
      // drive number is the last
      while (hit = GetParentItem(hit))
         path = GetItemText(hit) + "\\" + path;
   }
   return path;
}

int CDirTreeCtrl::GetItemPath(LPTSTR buffer, int maxLen, // buffer de sortie
                              HTREEITEM hit)             // item
{
   CString path = GetItemPath(hit);
   int length = path.GetLength();
   if (length > --maxLen) length = maxLen;
   for (int i = 0; i < length; i++) *buffer++ = path.GetAt(i);
   *buffer = 0;
   return length;
}
/////////////////////////////////////////////////////////////////////////////
// Selectionne le noeud correspondant au path donne

HTREEITEM CDirTreeCtrl::PathSelect(CString &path,  // path (corrige)
                                   BOOL exact)     // TRUE=> il faut que le path soit exact
{
   HTREEITEM hit = GetItem(path, exact);
   if (hit) {
      SelectItem(hit);
      path = GetItemPath(hit);
      SetFocus();
   }
   return hit;
}
/////////////////////////////////////////////////////////////////////////////
// Determine le noeud correspondant au path donne

HTREEITEM CDirTreeCtrl::GetItem(const CString &path,  // path
                                BOOL exact)        // TRUE=> il faut que le path soit exact
{
   HTREEITEM hitFnd = NULL, hit = TVI_ROOT;
   CString tmp, part, fn;
   int b, e = -1;

   if (path.IsEmpty()) {
      // un path vide correspond a la directory courante
      TCHAR buffer[MAX_PATH];
      ::GetCurrentDirectory(MAX_PATH, buffer);
      tmp = buffer;
   } else {
      tmp = path;
   }

   tmp.TrimLeft(); tmp.TrimRight('\\');
   do {
      // complete la branche
      if (hit != TVI_ROOT && (GetItemData(hit) & 1)) DirectoryUpdat(hit);

      // decoupe le path en ses sous-directories
      b = e + 1; e = tmp.Find('\\', b);
      part = e >= 0? tmp.Mid(b, e-b): tmp.Mid(b);

      // recherche la branche portant le meme nom
      hit = GetChildItem(hit);
      while (hit) {
         fn = GetItemText(hit);
         if (part.CompareNoCase(fn) == 0) {
            hitFnd = hit;
            break;
         }
         hit = GetNextSiblingItem(hit);
      }
   } while (hit && e >= 0);

   if (hit || exact) return hit;
   return hitFnd;
}
/////////////////////////////////////////////////////////////////////////////
// Mise a jour des fichiers appartenant a un noeud

void CDirTreeCtrl::DirectoryUpdat(HTREEITEM hit)
{
   CWaitCursor();
   TVINSERTSTRUCT tvi;
   tvi.hInsertAfter = TVI_SORT;
   tvi.hParent = hit;
   tvi.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;

   SetItemData(hit, GetItemData(hit) & ~1);  // supprime flag de demande mise a jour

   DeleteItem(GetChildItem(hit));            // supression du faux fichier

   CString path = GetItemPath(hit);          // path d'acces au noeud

   CFileFind finder;
   BOOL ok = finder.FindFile(path + "\\*.*");
   while (ok) {
      ok = finder.FindNextFile();
      if (!finder.IsHidden()) {
         CString path = finder.GetFilePath();
         if (!finder.IsDirectory()) {
            // fichiers
            if (::PathMatchSpec(finder.GetFileName(), m_fileSelPath))
               SetFileItem(&tvi, path, finder.GetFileName());

         } else if (!finder.IsDots()) {
            // directory
            SetDirItem(&tvi, path, finder.GetFileName(), PathIsDirEmpty(path));
         }
      }
   }
}

void CDirTreeCtrl::DirectorySetEmpty(HTREEITEM hit)
{
   SetItemData(hit, GetItemData(hit) & ~1);  // supprime flag de demande mise a jour
   DeleteItem(GetChildItem(hit));            // supression de faux fichier
}

void CDirTreeCtrl::SetFileItem(TVINSERTSTRUCT *pTv, LPCTSTR path, LPCTSTR fn)
{
   SHFILEINFO sh;

   // Get only the index of a system image list icon
   ::SHGetFileInfo(path, 0, &sh, sizeof(sh), SHGFI_SYSICONINDEX);
   pTv->item.iImage = sh.iIcon;

   ::SHGetFileInfo(path, 0, &sh, sizeof(sh), SHGFI_SYSICONINDEX|SHGFI_OPENICON);
   pTv->item.iSelectedImage = sh.iIcon;

   pTv->item.pszText = (LPTSTR)fn;
   pTv->item.lParam = 0;
   InsertItem(pTv);
}

void CDirTreeCtrl::SetDirItem(TVINSERTSTRUCT *pTv, LPCTSTR path, LPCTSTR fn, BOOL empty)
{
   CString name = " " + CString(fn);
   SHFILEINFO sh;

   // Get only the index of a system image list icon
   ::SHGetFileInfo(path, 0, &sh, sizeof(sh), SHGFI_SYSICONINDEX);
   pTv->item.iImage = sh.iIcon;

   ::SHGetFileInfo(path, 0, &sh, sizeof(sh), SHGFI_SYSICONINDEX|SHGFI_OPENICON);
   pTv->item.iSelectedImage = sh.iIcon;

   pTv->item.pszText = name.GetBuffer(MAX_PATH);

   if (empty) {
      // Create a empty folder
      pTv->item.lParam = 0;
      InsertItem(pTv);
   } else {
      TVINSERTSTRUCT tv;

      pTv->item.lParam = 1;      // flag demandant la mise a jour lorsque necessaire
      tv.hInsertAfter = TVI_LAST; 
      tv.item.mask = 0;
      tv.hParent = InsertItem(pTv);
      // par defaut une sous-directory a toujours des fichiers
      InsertItem(&tv);
   }
}

BOOL CDirTreeCtrl::PathIsDirEmpty(LPCTSTR path)
{
   CFileFind finder;
   BOOL ok = finder.FindFile(CString(path) + "\\*.*");
   while (ok) {
      ok = finder.FindNextFile();
      if (!finder.IsHidden()) {
         if (!finder.IsDirectory()) {
            // fichiers
            if (::PathMatchSpec(finder.GetFileName(), m_fileSelPath))
               return FALSE;
         } else if (!finder.IsDots()) {
            // directory
            return FALSE;
         }
      }
   }
   return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// Conversion ItemIdList vers nom

CString CDirTreeCtrl::GetDrive(LPCTSTR path)
{
   LPCTSTR pc = strrchr(path, ':');
   return (pc)? CString( pc - 1, 2): "";
}

CString CDirTreeCtrl::GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags)
{
   STRRET str;
   
   if (NOERROR == lpsf->GetDisplayNameOf(lpi, dwFlags, &str)) {
      switch (str.uType) {
      case STRRET_WSTR:   return CString( str.pOleStr );
      case STRRET_OFFSET: return CString( LPSTR(lpi) + str.uOffset );
      case STRRET_CSTR:   return CString( str.cStr );
      }
   }
   return "";
}
