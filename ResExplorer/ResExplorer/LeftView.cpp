// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "ResUtilities.h"
#include <Shlwapi.h>

#include "ResDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginrdrag)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
//	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
   m_pParam = NULL;
}

CLeftView::~CLeftView()
{
   if (m_pParam) m_pParam->FreeLibrary();
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1) return -1;

	GetTreeCtrl().SetImageList(&m_image, TVSIL_NORMAL);

	return 0;
}

void CLeftView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
   SetWindowContextHelpId(0x20000+IDR_LEFTVIEW);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CResDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResDoc)));
	return (CResDoc*)m_pDocument;
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   if (pHint) m_pParam = (CParams*)pHint;
   if (m_pParam) {
      if (lHint & LEFT_VIEW) {
         m_pParam->pTree = NewTree();

         GetDocument()->SetCurrent(this, m_pParam->resType, m_pParam->resName,
                                                  LIST_VIEW|RES_VIEW);
      }
      if (lHint & LEFT_VIEW_SELECT) {
         TreeSelect(m_pParam->pTree, m_pParam->resType, m_pParam->resName, TRUE);
         TRACE("CLeftView::OnUpdate restype=%08x resNam=%08x\n",
            m_pParam->resType, m_pParam->resName);
      }
      if (lHint & LEFT_VIEW_UPDAT) TreeUpdate();
   }
}

void CLeftView::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
   // Ouvre les folders lors de l'expansion de l'arbre correspondant
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
   CTreeCtrl& tree = GetTreeCtrl();
   int im, sim;

   // les images correspondent a selecte ou non
   tree.GetItemImage(hItem, im, sim);
   switch(pNMTreeView->action) {
   case TVE_EXPAND  :
      if (im == RI_FOLDER)  im = sim = RI_OPENFLDR;
      if (im == RI_XFOLDER) im = sim = RI_XOPENFLDR;
      break;
   case TVE_COLLAPSE:
      if (im == RI_OPENFLDR)  im = sim = RI_FOLDER;
      if (im == RI_XOPENFLDR) im = sim = RI_XFOLDER;
      break;
   }
   tree.SetItemImage(hItem, im, sim);

	*pResult = 0;
}

void CLeftView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
   CTreeCtrl& tree = GetTreeCtrl();
   if (pNMHDR->hwndFrom == tree.m_hWnd && pNMTreeView->action) {
      int n = 1;
      LPCTSTR tbl[5] = {NULL,NULL,};
      HTREEITEM hi = tree.GetSelectedItem();
      if (hi) m_pParam->symbol = tree.GetItemText(hi);
      while (hi) {
         tbl[++n] = (LPCTSTR)tree.GetItemData(hi);
         hi = tree.GetParentItem(hi);
      }

      if (n > 1) GetDocument()->SetCurrent( this, tbl[n-1], tbl[n-2] );
   }	*/
	*pResult = 0;
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
   CTreeCtrl& tree = GetTreeCtrl();
   if (pNMHDR->hwndFrom == tree.m_hWnd && pNMTreeView->action) {
      int n = 1;
      LPCTSTR tbl[5] = {NULL,NULL,};
      HTREEITEM hi = tree.GetSelectedItem();
      if (hi) m_pParam->symbol = tree.GetItemText(hi);
      while (hi) {
         tbl[++n] = (LPCTSTR)tree.GetItemData(hi);
         hi = tree.GetParentItem(hi);
      }

      if (n > 1) GetDocument()->SetCurrent( this, tbl[n-1], tbl[n-2] );
   }
	*pResult = 0;
}

void CLeftView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TRACE("CLeftView::OnBegindrag act=%08x %s %08x\n",
            pNMTreeView->action, pNMTreeView->itemOld.pszText,
               pNMTreeView->itemOld.lParam);
	*pResult = 0;
}

void CLeftView::OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TRACE("CLeftView::OnBeginrdrag act=%08x %s %08x\n",
            pNMTreeView->action, pNMTreeView->itemOld.pszText,
               pNMTreeView->itemOld.lParam);
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
// Selectionne l'objet designe et expand sa racine

HTREEITEM CLeftView::TreeSelect(CTreeCtrl *pTree,
                                LPCTSTR lpType,
                                LPCTSTR lpName,
                                BOOL expand)
{
   HTREEITEM hit = pTree->GetNextItem(TVI_ROOT, TVGN_CHILD);
   hit = pTree->GetNextItem(hit, TVGN_CHILD);
   while (hit) {
      if (lpType == (LPCTSTR)pTree->GetItemData(hit)) {
         if (expand) pTree->Expand(hit, TVE_EXPAND);
         if (lpName) {
            HTREEITEM hii = pTree->GetNextItem(hit, TVGN_CHILD);
            while (hii) {
               if (lpName == (LPCTSTR)pTree->GetItemData(hii)) {
                  if (expand) pTree->Select(hii, TVGN_CARET);
                  return hii;
               }
               hii = pTree->GetNextItem(hii, TVGN_NEXT);
            }
            break;
         } else {
            if (expand) pTree->SelectItem(hit);
            return hit;
         }
      }
      hit = pTree->GetNextItem(hit, TVGN_NEXT);
   }
   return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// Generate a new tree with a new library

CTreeCtrl *CLeftView::NewTree()
{
   t_NODE root;
   static BOOL entr = TRUE;
   root.pTree = &GetTreeCtrl();
   root.pTree->DeleteAllItems();

   if (m_pParam && m_pParam->hLib) {
      root.dimage = m_pParam->limited? 2: 0;
      // construction de l'arbre decrivant les ressources
      root.item = root.pTree->InsertItem(::PathFindFileName(m_pParam->libName),
                                       RI_FOLDER, RI_OPENFLDR);

      EnumResourceTypes(m_pParam->hLib, EnumTypesFunc, (LPARAM)&root);

      root.pTree->Expand(root.item, TVE_EXPAND);
   }
   return root.pTree;
}

/////////////////////////////////////////////////////////////////////////////
// Update the resource names (with or without) symbols translation

void CLeftView::TreeUpdate()
{
   DWORD type;
   LPCTSTR resType, resName;
   CTreeCtrl &tree = GetTreeCtrl();
   HTREEITEM hit = tree.GetNextItem(TVI_ROOT, TVGN_CHILD);

   hit = tree.GetNextItem(hit, TVGN_CHILD);
   while (hit) {
      resType = (LPCTSTR)tree.GetItemData(hit);
      type = GetResEntryEx( resType )->type;
      if (type & TYP_USR)
         tree.SetItemText(hit, FormatId(resType, TICON(type)|TYP_RTX));

      HTREEITEM hii = tree.GetNextItem(hit, TVGN_CHILD);
      while (hii) {
         if (resName = (LPCTSTR)tree.GetItemData(hii))
            tree.SetItemText(hii, FormatId(resName, type));
         hii = tree.GetNextItem(hii, TVGN_NEXT);
      }
      hit = tree.GetNextItem(hit, TVGN_NEXT);
   }
}
/////////////////////////////////////////////////////////////////////////////

//    FUNCTION: EnumTypesFunc(HANDLE, LPSTR, LONG) 
// 
//    PURPOSE:  Resource name callback 

BOOL WINAPI CLeftView::EnumTypesFunc(  // used for error checking 
                     HMODULE hModule,  // module handle 
                     LPTSTR lpType,    // address of resource type 
                     LONG lParam)      // extra parameter, could be 
{ 
   t_NODE node = *((t_NODE*)lParam);
   t_ResID *pRes = GetResEntryEx( lpType );
   // Eliminate RT_CURSOR and RT_ICON, there are already show with RT_GROUP_CURSOR, or ..ICON
   if (pRes->fname) {
      // Take a special mark on the RT_DIALOG folder if it's a limited librairy
      if (lpType == RT_DIALOG && node.dimage != 0) 
         node.item = node.pTree->InsertItem(pRes->text? pRes->text: GetResStr(lpType),
                            RI_XFOLDER, RI_XOPENFLDR, node.item);
      else
         node.item = node.pTree->InsertItem(pRes->text? pRes->text: GetResStr(lpType),
                            RI_FOLDER, RI_OPENFLDR, node.item);
      node.pTree->SetItemData(node.item, (DWORD)UniqueResId(lpType));
      node.type = pRes->type;

      // Find the names of all resources of type lpType. 
      EnumResourceNames(hModule, lpType, EnumNamesFunc, (LPARAM)&node);
   }
   return TRUE; 
} 
 
//    FUNCTION: EnumNamesFunc(HANDLE, LPSTR, LPSTR, LONG) 
// 
//    PURPOSE:  Resource name callback 
 
BOOL WINAPI CLeftView::EnumNamesFunc(  // used for error checking 
                     HMODULE hModule,  // module handle 
                     LPCTSTR lpType,   // address of resource type 
                     LPTSTR lpName,    // address of resource name 
                     LONG lParam)      // extra parameter, could be 
{ 
   t_NODE *pNode = (t_NODE*)lParam;
   CTreeCtrl *pTree = pNode->pTree;
   DWORD type = pNode->type;
   LPCTSTR lpId = UniqueResId(lpName);
   HTREEITEM hitem = pTree->InsertItem(FormatId(lpId, type), TICON(type), TICON(type),
                                       pNode->item);
   pTree->SetItemData(hitem, DWORD(lpId));
   pTree->SetCheck(hitem, TRUE);
   if (lpType == RT_TOOLBAR) {
      // Search the RT_BITMAP corresponding, take it a toolbar icon
      HTREEITEM hit = TreeSelect(pTree, RT_BITMAP, lpId, FALSE);
      if (hit) pTree->SetItemImage(hit, TICON(type), TICON(type));
//         HTREEITEM hi = pTree->GetNextItem(hit, TVGN_PARENT);
//         pTree->DeleteItem(hit);
//         if (!pTree->ItemHasChildren(hi)) pTree->DeleteItem(hi);
//      }
   }

//   EnumResourceLanguages(hModule, lpType, lpName, EnumLangsFunc, (LPARAM)hitem); 

   return TRUE; 
} 
 
//    FUNCTION: EnumLangsFunc(HANDLE, LPSTR, LPSTR, WORD, LONG) 
// 
//    PURPOSE:  Resource language callback 
 
BOOL WINAPI CLeftView::EnumLangsFunc( // used for error checking 
                     HMODULE hModule, // module handle 
                     LPCTSTR lpType,  // address of resource type 
                     LPCTSTR lpName,  // address of resource name 
                     WORD wLang,      // resource language 
                     LONG lParam)     // extra parameter, could be 
{ 
   HANDLE hResInfo = FindResourceEx(hModule, lpType, lpName, wLang); 
/* 
    // Write the resource language to the resource information file. 
    cbString = sprintf(szBuffer, "\t\tLanguage: %u\n", (USHORT)wLang); 

    WriteFile(hFile, szBuffer, (DWORD) cbString, 
        &cbWritten, NULL); 
 
    // Write the resource handle and size to buffer. 
    cbString = sprintf(szBuffer, 
        "\t\thResInfo == %lx,  Size == %lu\n\n", 
        hResInfo, 
        SizeofResource(hModule, hResInfo)); 
 
    WriteFile(hFile, szBuffer, (DWORD) cbString, 
        &cbWritten, NULL); */
 
   return TRUE; 
}
