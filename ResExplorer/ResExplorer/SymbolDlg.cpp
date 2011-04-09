// SymbolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "Utilities.h"        // InitHeader(), CulumnSort()
#include "ToolTipEx.h"
#include "ResDoc.h"
#include "MainFrm.h"
#include "SymbolDlg.h"
#include <afxadv.h>
#include <afxole.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog
CSymbolList CSymbolDlg::m_symbols;
BOOL        CSymbolDlg::m_chkSymExt = FALSE;
BOOL        CSymbolDlg::m_chkSymStd = TRUE;
BOOL        CSymbolDlg::m_chkDefine = TRUE;
BOOL        CSymbolDlg::m_chkOutHex = FALSE;
POSITION    CSymbolDlg::m_lastFound = NULL;

CSymbolDlg::CSymbolDlg(CWnd* pParent)
	: CDialog(CSymbolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSymbolDlg)
	//}}AFX_DATA_INIT
   m_iItem = -1;
   if (m_symbols.IsEmpty()) AddStandards();
   EnableToolTips();
}

BOOL CSymbolDlg::Create(CWnd* pParent)
{
   return CDialog::Create(CSymbolDlg::IDD, pParent);
}

void CSymbolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolDlg)
	DDX_Control(pDX, IDC_LST_SYMBOL, m_list);
	DDX_Check(pDX, IDC_CHK_SYMEXT, m_chkSymExt);
	DDX_Check(pDX, IDC_CHK_SYMSTD, m_chkSymStd);
	//}}AFX_DATA_MAP
}

void CSymbolDlg::UpdateAllViews()
{
   CResDoc *pDoc = (CResDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
   pDoc->UpdateAllViews(NULL, LEFT_VIEW_UPDAT|RES_VIEW|LIST_VIEW);
}

BEGIN_MESSAGE_MAP(CSymbolDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolDlg)
	ON_BN_CLICKED(IDC_BTN_SYMFILE, OnBtnSymfile)
	ON_BN_CLICKED(IDC_BTN_GENERATE, OnBtnGenerate)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LST_SYMBOL, OnColumnclickList)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SYM_COPY, OnSymCopy)
	ON_COMMAND(ID_SYM_FORMAT, OnSymFormat)
	ON_COMMAND(ID_SYM_PAST, OnSymPaste)
	ON_COMMAND(ID_SYM_REPLACE, OnSymReplace)
	ON_COMMAND(ID_SYM_DELETE, OnSymDelete)
	ON_COMMAND(ID_SYM_DELETEALL, OnSymDeleteall)
	ON_COMMAND(ID_SYM_SELALL, OnSymSelectall)
	ON_COMMAND(ID_SYM_MODIF, OnSymModifie)
	ON_NOTIFY(NM_CLICK, IDC_LST_SYMBOL, OnClickList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LST_SYMBOL, OnBegindragList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LST_SYMBOL, OnEndlabeleditList)
	ON_COMMAND(ID_SYM_MLTDEF, OnSmultidef)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHK_SYMEXT, OnChkSymbol)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_SYMBOL, OnDblclkList)
	ON_COMMAND(ID_SYM_SEARCH, OnSymSearch)
	ON_NOTIFY(NM_RCLICK, IDC_LST_SYMBOL, OnRclickList)
	ON_COMMAND(ID_SYM_CUTFAT, OnSymCutfat)
	ON_NOTIFY(NM_RDBLCLK, IDC_LST_SYMBOL, OnRdblclkList)
	ON_BN_CLICKED(IDC_CHK_SYMSTD, OnChkSymbol)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_HSYM_SORT0, ID_HSYM_SORT4, OnHsymSort)
   ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg message handlers

BOOL CSymbolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   // Sauvegarde les anciens symboles pour le cas Cancel
   m_symSave.RemoveAll();
   POSITION pos = m_symbols.GetHeadPosition();
   while (pos) m_symSave.AddHead(m_symbols.GetNext(pos));

   // Initialise l'affichage des symboles
   m_list.SetImageList( &m_image, LVSIL_SMALL );
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
   InitHeader(m_list, IDS_RER_SYMBOLS);
   m_duplic = Qualifie(true);
   ListUpdate(false);
   UpdateBtnState();

   m_DropTarget.Register(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSymbolDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
   if (bShow) {
      // Sauvegarde les anciens symboles pour le cas Cancel
      m_symSave.RemoveAll();
      POSITION pos = m_symbols.GetHeadPosition();
      while (pos) m_symSave.AddHead(m_symbols.GetNext(pos));

      ListUpdate(false);
      m_duplic = Qualifie(true);

      UpdateBtnState();
   }
	CDialog::OnShowWindow(bShow, nStatus);
}

BOOL CSymbolDlg::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
   *pResult = (LRESULT)ToolTipNotify(pNMH);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// Buttons

void CSymbolDlg::OnBtnSymfile() 
{
   CFileDialog fd(TRUE, NULL, TEXT("Resource.h"), OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,
                  TEXT("Resource (*.h)|*.h;*.hm|All (*.*)|*.*|") );

   if ( fd.DoModal() == IDOK) {
      BeginWaitCursor();
	   ((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_SYMREADWAIT);
      if (Read(fd.GetPathName()) > 0 ) {
         m_duplic = Qualifie(true);
         if (ListUpdate(false)) m_chkSymExt = TRUE;
         UpdateAllViews();
      }
      EndWaitCursor();
	   ((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);
   }
   UpdateData(FALSE);
}

void CSymbolDlg::OnBtnGenerate() 
{
   UpdateData();
   BeginWaitCursor();
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_SYMGENWAIT);
   CResDoc *pDoc = (CResDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
   pDoc->SymbolUpdate();
   m_duplic = Qualifie(false);
   if (ListUpdate(false)) m_chkSymExt = TRUE;
   UpdateAllViews();
   EndWaitCursor();
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);
   UpdateData(FALSE);
}

void CSymbolDlg::OnOK()
{
   UpdateData();
   m_symSave.RemoveAll();
   UpdateAllViews();
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);

	CDialog::OnOK();
}

void CSymbolDlg::OnCancel() 
{
   POSITION pos = m_symSave.GetHeadPosition();
   // restore the old symbol list
   m_symbols.RemoveAll();
   while (pos) m_symbols.AddHead(m_symSave.GetNext(pos));
   m_symSave.RemoveAll();

   UpdateAllViews();
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);
	
	CDialog::OnCancel();
}
/////////////////////////////////////////////////////////////////////////////
// Check Standard Symbols, Extern Symbols

void CSymbolDlg::OnChkSymbol() 
{
   UpdateData();
   UpdateAllViews();
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);
}
/////////////////////////////////////////////////////////////////////////////
// Mouse click left/right, single/double, column/header

void CSymbolDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   m_iItem = ((NM_LISTVIEW*)pNMHDR)->iItem;
   m_iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
   CResDoc *pDoc = (CResDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
   pDoc->SearchSymbol(m_list.GetItemText(m_iItem, 1));
	*pResult = 0;
}

void CSymbolDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   m_iItem = ((NM_LISTVIEW*)pNMHDR)->iItem;
   m_iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
   SearchSymbol(m_iItem);
	*pResult = 0;
}

void CSymbolDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   m_iItem = ((NM_LISTVIEW*)pNMHDR)->iItem;
   m_iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
	*pResult = 0;
}

void CSymbolDlg::OnRdblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   m_iItem = ((NM_LISTVIEW*)pNMHDR)->iItem;
   m_iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
   SearchSymbol(m_iItem);
	*pResult = 0;
}

void CSymbolDlg::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   m_iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
   BeginWaitCursor();
   if (!m_loc.IsEmpty()) ListUpdate(false);
   else ColumnSort(m_list, m_iSubItem, CS_REVERSE);
   PutSortMessage();
   EndWaitCursor();
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
// In place edit (Value, Label)

void CSymbolDlg::OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   UINT err = SymbolUpdate(((LV_DISPINFO*)pNMHDR)->item);
   if (err) AfxMessageBox(err);
   m_duplic = Qualifie(true);
   *pResult = 0;     // the list is yet updated
}
/////////////////////////////////////////////////////////////////////////////
// Drag/drop writing functions

void CSymbolDlg::OnBegindragList(NMHDR*, LRESULT* pResult) 
{
   CacheTextCopy(FALSE);
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
// Drag/drop reading functions

DROPEFFECT CSymbolDlg::OnDragEnter(COleDataObject* pDataObject)
{
   if ( GetData(pDataObject, false) > 0 ) {
      m_list.GetWindowRect(m_dragOver);
      ScreenToClient(m_dragOver);
   } else {
      m_dragOver.SetRectEmpty();
   }
   return DROPEFFECT_NONE;
}

DROPEFFECT CSymbolDlg::OnDragOver(COleDataObject* pDataObject, CPoint point)
{
   return (m_dragOver.PtInRect(point))? DROPEFFECT_COPY: DROPEFFECT_NONE;
}

BOOL CSymbolDlg::OnDrop(COleDataObject* pDataObject)
{
   if ( !m_dragOver.IsRectEmpty() && GetData(pDataObject, true) > 0 ) {
      m_duplic = Qualifie(false), ListUpdate(false);
      UpdateAllViews();
      return TRUE;
   } else return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// Context menu

void CSymbolDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   CRect rectHead;
   CMenu menu, *pSbm;
   // Load float edit menu
   menu.LoadMenu( IDR_RVSYMBOLS );
   if ( !m_loc.IsEmpty() ) {
      ListUpdate(false);
      m_list.SetItemState(m_iItem = m_iItemOld, LVIS_SELECTED, LVIS_SELECTED);
   }

   m_list.GetHeaderCtrl()->GetWindowRect(rectHead);
   if (m_list.GetItemCount() > 1 && rectHead.PtInRect(point)) {
      UINT checkId = SortStatus(m_list) + ID_HSYM_SORT1;

      pSbm = menu.GetSubMenu(0);
      pSbm->CheckMenuItem(checkId,              MF_BYCOMMAND|MF_CHECKED);

   } else {

      pSbm = menu.GetSubMenu(1);

      // Update check marks and active commands
      if (m_chkDefine) 
         pSbm->CheckMenuItem(ID_SYM_FORMAT,     MF_BYCOMMAND|MF_CHECKED);
      if (m_chkMltDef)
         pSbm->CheckMenuItem(ID_SYM_MLTDEF,     MF_BYCOMMAND|MF_CHECKED);
      if (m_duplic > 0)
         pSbm->EnableMenuItem(ID_SYM_MLTDEF,    MF_BYCOMMAND|MF_ENABLED);
      if (GetClipboardData(false) == 0)
         pSbm->EnableMenuItem(ID_SYM_PAST,      MF_BYCOMMAND|MF_GRAYED);
      if (m_list.GetItemCount() > 0) {
         pSbm->EnableMenuItem(ID_SYM_SELALL,    MF_BYCOMMAND|MF_ENABLED);
         pSbm->EnableMenuItem(ID_SYM_CUTFAT,    MF_BYCOMMAND|MF_ENABLED);
         pSbm->EnableMenuItem(ID_SYM_DELETEALL, MF_BYCOMMAND|MF_ENABLED);
      }
      if (m_list.GetSelectedCount() == 1) {
         pSbm->EnableMenuItem(ID_SYM_MODIF,     MF_BYCOMMAND|MF_ENABLED);
         if (CResDoc::GetResLib() != NULL)
            pSbm->EnableMenuItem(ID_SYM_SEARCH, MF_BYCOMMAND|MF_ENABLED);
      }
      if (m_list.GetSelectedCount() > 0) {
         pSbm->EnableMenuItem(ID_SYM_COPY,      MF_BYCOMMAND|MF_ENABLED);
         pSbm->EnableMenuItem(ID_SYM_REPLACE,   MF_BYCOMMAND|MF_ENABLED);
         pSbm->EnableMenuItem(ID_SYM_DELETE,    MF_BYCOMMAND|MF_ENABLED);
      }
   }
   // Display a menu at the cursor position
   pSbm->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CSymbolDlg::OnHsymSort(UINT id){ ListSort(id - ID_HSYM_SORT1); }
void CSymbolDlg::OnSymCopy()        { CacheTextCopy(TRUE); }
void CSymbolDlg::OnSymFormat()      { m_chkDefine ^= TRUE; }
void CSymbolDlg::OnSymModifie()     { m_list.SubItemEdit(m_iItem, m_iSubItem); }
void CSymbolDlg::OnSymSearch()      { SymbolLocate(m_iItem); }
void CSymbolDlg::OnSymReplace()     { Delete(); OnSymPaste(); }
void CSymbolDlg::OnSymDelete()      { Delete(); UpdateAllViews(); }
void CSymbolDlg::OnSymDeleteall()   { SelectAll(); Delete(); UpdateAllViews(); }
void CSymbolDlg::OnSmultidef()      { ListUpdate(!m_chkMltDef); }
void CSymbolDlg::OnSymSelectall()   { SelectAll(); }
void CSymbolDlg::OnSymCutfat()      { SymbolCutFat(false); }
void CSymbolDlg::OnSymPaste()
{
   if (GetClipboardData(true) > 0) {
      m_duplic = Qualifie(false), ListUpdate(false), UpdateAllViews();
   }
}
/////////////////////////////////////////////////////////////////////////////
// Translate a value on the corresponding symbol name

CString CSymbolDlg::GetString(DWORD value, DWORD type)
{
// TYP_XSD : standards exclude
// TYP_NUM : disable a symbol translation
// TYP_SEP : 0 and 0xFFFF values are not symbols (menu, toolbar)
// TYP_BLK : 0 value => no symbol
// TYP_DEC : value in decimal (else in hexadecimal)

   m_lastFound = NULL;
   if (value == 0) {
      if (type & TYP_BLK)  return TEXT("");
      if (type & TYP_SEP) return TEXT("----");
      if (type & (TYP_HIR|TYP_HIC)) return TEXT("0");
   } else if (value == 0xFFFF && (type & TYP_SEP)) return TEXT("");

   if ((type & TYP_NUM) == 0) {
      DWORD filter = 0, xor = 0;
      BOOL chkSymStd = (m_chkSymStd && (type & TYP_XSD) == 0);

      if (chkSymStd || m_chkSymExt) {
         POSITION pos = m_symbols.GetHeadPosition();

         if (!chkSymStd || !m_chkSymExt) filter = TYP_STD;
         if (!m_chkSymExt) xor = TYP_STD;

         while (m_lastFound = pos) {
            t_SYMBOL &sym = m_symbols.GetNext(pos);
            if (sym.value != value || ((xor ^ sym.type) & filter)) continue;
            if ( (sym.type & TYP_MDF) == 0               // unique value
                  || (sym.type & type & TYP_TMSK) != 0 ) // or type ok
               return sym.name;
         }
      }
   }

   CString name;
   name.Format((type & TYP_DEC)? TEXT("%u"): TEXT("%04x"), value);
   return name;
}
/////////////////////////////////////////////////////////////////////////////
// Create a new Symbol

void CSymbolDlg::SymbolGen(DWORD value, DWORD type)
{
   if ((type & TYP_NUM) || (value == 0 && (type &(TYP_HIR|TYP_HIC))) )
      return;

   POSITION pos = m_symbols.GetHeadPosition();
   while (pos) {
      t_SYMBOL & sym = m_symbols.GetNext(pos);
      if ( sym.value == value && ((sym.type ^ type) & TYP_TMSK) == 0 )
         return;                                // this symbol exist yet
   }
   if (IsUniqueResId((LPCTSTR)value)) return;   // its yet a symbol

   const t_nameID typTbl[] = {
      {TYP_TMSK & TYP_ID_, "ID_" }, {TYP_TMSK & TYP_IDB, "IDB_"},
      {TYP_TMSK & TYP_IDC, "IDC_"}, {TYP_TMSK & TYP_IDD, "IDD_"},
      {TYP_TMSK & TYP_IDI, "IDI_"}, {TYP_TMSK & TYP_IDM, "IDM_"},
      {TYP_TMSK & TYP_IDP, "IDP_"}, {TYP_TMSK & TYP_IDR, "IDR_"},
      {TYP_TMSK & TYP_IDS, "IDS_"}, {TYP_TMSK & TYP_IDW, "IDW_"},
      {TYP_TMSK & TYP_HIR, "HIDR_"},{TYP_TMSK & TYP_HIC, "HIDC_"},
      {TYP_TMSK & TYP_RTX, "RT_X"}, {TYP_TMSK & TYP_USR, "IDUSR_"},
      {0,                  "IDX"} };

   for (const t_nameID *pT = typTbl; pT->id; pT++)
      if (pT->id & type) break;

   t_SYMBOL sym;
   sym.name.Format((type & TYP_DEC)? TEXT("%sL%u"): TEXT("%sL%04X"), pT->title, value);
   sym.type = type;
   sym.value = value;
   SymbolAdd(sym);
}
/////////////////////////////////////////////////////////////////////////////
// In place Edit

UINT CSymbolDlg::SymbolUpdate(LVITEM &item)
{
   LPCTSTR pszText = item.pszText;
   if (pszText == NULL || *pszText == 0) return 0; // null operation
   POSITION pos = POSITION(m_list.GetItemData(item.iItem));
   t_SYMBOL sym = m_symbols.GetAt(pos);   // a copie of the symbol

   if (item.iSubItem == 0) {
      // value update
      if (!Convert(pszText, sym.value)) return IDS_INVALIDNBR; // bad value
   } else if (item.iSubItem == 1) {
      // symbol update
      sym.name = pszText;
      POSITION df = GetPosition(sym.name);
      if (df == NULL) SearchType(sym);             // new name
      else if (df != pos) return IDS_SYMDEFMLT;    // double definition
   } else return 0;

   // take the modified symbol at the first recent position
   SortStatus(m_list, SS_REMOVE);
   m_symbols.RemoveAt(pos);
   ItemUpdate(item.iItem, SymbolAdd(sym));
   m_duplic = Qualifie(true);
   UpdateAllViews();
   return 0;
}
/////////////////////////////////////////////////////////////////////////////
// Select a table entry

BOOL CSymbolDlg::SymbolSelect(LPCTSTR pszName)
{
   if (pszName && *pszName && m_list.m_hWnd) {
      CString name(pszName);
      TRACE("CSymbolDlg::SymbolSelect %s\n", name);
      int cnt = m_list.GetItemCount(), ix;
      for (ix = 0; ix < cnt; ix++) m_list.SetItemState(ix, 0, LVIS_SELECTED);

      for (ix = 0; ix < cnt; ix++) {
         if ( name.Compare(m_list.GetItemText(ix, 1)) != 0 ) continue;
         m_list.EnsureVisible(ix, FALSE);
         m_list.SetItemState(ix, LVIS_SELECTED, LVIS_SELECTED);
         m_iItem = ix, m_iSubItem = 0;
         return TRUE;
      }
   } else UpdateBtnState();
   return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//
void CSymbolDlg::UpdateBtnState()
{
   if (m_hWnd) {
      GetDlgItem(IDC_BTN_GENERATE)->EnableWindow(CResDoc::GetResLib() != NULL);
      GetDlgItem(IDC_CHK_SYMEXT)->EnableWindow(m_list.GetItemCount() > 0);
   }
}
/////////////////////////////////////////////////////////////////////////////
// Subroutines to read or write the symbols table

void CSymbolDlg::AddStandards()
{  
   const t_nameID idTbl[] = {
      NAMESTR( IDOK     ),       // 0x0001
      NAMESTR( IDCANCEL ),       // 0x0002
      NAMESTR( IDABORT  ),       // 0x0003
      NAMESTR( IDRETRY  ),       // 0x0004
      NAMESTR( IDIGNORE ),       // 0x0005
      NAMESTR( IDYES    ),       // 0x0006
      NAMESTR( IDNO     ),       // 0x0007
      NAMESTR( IDCLOSE  ),       // 0x0008
      NAMESTR( IDHELP   ),       // 0x0009
      NAMESTR( ID_SEPARATOR ),   // 0x0000
      NAMESTR( IDC_STATIC ),     // 0xFFFF
      0,       NULL         };

   t_SYMBOL sym;
   sym.type = TYP_STD | TYP_IDC;
   for (const t_nameID *pTb = idTbl; pTb->title; pTb++) {
      sym.name = pTb->title;
      sym.value = pTb->id;
      SymbolAdd(sym);
   }
}

int CSymbolDlg::Read(CString path, bool erase, bool upd)
{
   CStdioFile file;
   CString line;
   int cnt = 0;

   if (erase) {
      m_symbols.RemoveAll();
      AddStandards();
   }

   if (!file.Open(path, CFile::modeRead|CFile::typeText))
      return FALSE;

   while (file.ReadString(line)) cnt += ParseLine(line, upd);

   return cnt;
}

void CSymbolDlg::CacheTextCopy(BOOL clipboard)
{
   // It is used either during Drag/Drop operation
   // or during a Copy/Paste operation for data out
   POSITION pos = m_list.GetFirstSelectedItemPosition();
   CString text, line, number;

   while (pos) {
      DWORD data = m_list.GetItemData(m_list.GetNextSelectedItem(pos));
      if (data == 0) continue;
      t_SYMBOL &sym = m_symbols.GetAt(POSITION(data));

      number.Format(m_chkOutHex? TEXT("0x%04x"): TEXT("%6u"), sym.value);
      if (m_chkDefine) {
         line.Format(TEXT("#define %-20s %s\r\n"), sym.name, number);
      } else {
         line.Format(TEXT("%-6s\t%s\r\n"), number, sym.name);
      }
      text += line;
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

int CSymbolDlg::GetClipboardData(bool upd)
{
	COleDataObject	obj;

   return (obj.AttachClipboard())? GetData(&obj, upd): 0; 
}

int CSymbolDlg::GetData(COleDataObject *pObj, bool upd)
{
   // It is used either during Drag/Drop operation
   // or during a Copy/Paste operation for data in
   int cnt = 0;
	if (pObj->IsDataAvailable(CF_TEXT)) {
      // text input
		HGLOBAL hGlobal = pObj->GetGlobalData(CF_TEXT);
      char *pText = (char*)GlobalLock(hGlobal), c;
      char *pEnd = pText + ::GlobalSize(hGlobal);

      while (pText < pEnd) {
         CString line;           // extract a complete line
         while (pText < pEnd)
            if ((c = *pText++) == '\n') break; else if (c != '\r') line += c;

         cnt += ParseLine(line, upd);
      }
		::GlobalUnlock(hGlobal);
   } else if (pObj->IsDataAvailable(CF_HDROP)) {
      // read files
      HGLOBAL hGlobal = pObj->GetGlobalData(CF_HDROP);
      DROPFILES* pDf = (DROPFILES*)GlobalLock(hGlobal);
      TCHAR c;

      if (pDf->fWide) {
         // The file contains Unicode characters
         PWORD pW = PWORD(PBYTE(pDf) + pDf->pFiles);

         // Extract a file names and read there
         while (*pW) {
            CString path;
            while (c = (TCHAR)*pW++) path += c;
            cnt += Read(path, false, upd);
         }
      } else {
         // The file contains ANSI characters
         PBYTE pB = PBYTE(PBYTE(pDf) + pDf->pFiles);

         // Extract a file names and read there
         while (*pB) {
            CString path;
            while (c = (TCHAR)*pB++) path += c;
            cnt += Read(path, false, upd);
         }
      }
      ::GlobalUnlock(hGlobal);
   }
   return cnt;
}

int CSymbolDlg::ParseLine(const CString &line, bool upd)
{
   int last = line.GetLength(), first = 0, i;
   CString word[3];
   t_SYMBOL sym;
   int cnt = 0;

   for (i = 0; i < 3; i++) {
      if (first == last) break;
      word[i] = Word(line, first, last);
   }

   sym.name = word[1];
   if (m_chkDefine) {
      if ( i == 3 && word[0] == TEXT("#define")
                  && word[1].Left(5) != TEXT("_APS_")
                  && Convert(word[2], sym.value) ) {
         cnt++;
         if (upd) {
            SearchType(sym);
            SymbolAdd(sym);
         }
      }
   } else {
      if (i >= 2 && Convert(word[0], sym.value)) {
         cnt++;
         if (upd) {
            SearchType(sym);
            SymbolAdd(sym);
         }
      }
   }
   return cnt;
}

POSITION CSymbolDlg::SymbolAdd(t_SYMBOL &symbol)
{
   // search name in double definition
   if (GetPosition(symbol.name)) return NULL;

   return m_symbols.AddHead(symbol);
}

POSITION CSymbolDlg::GetPosition(const CString &name)
{
   POSITION pos = m_symbols.GetHeadPosition();
   while (pos)
      if (name.Compare(m_symbols.GetNext(pos).name) == 0)
         return pos;
   return NULL;
}

CString CSymbolDlg::Word(const CString &line, int &first, int last)
{
   const CString sep(TEXT(" \t,")), asep(TEXT(" \t;\n\r"));
   int n, m;

   for (n = first; n < last; n++) {
      if (sep.Find(line.GetAt(n)) >= 0) continue;
      for (m = n; m < last; m++) {
         if (asep.Find(line.GetAt(m)) >= 0) break;
      }

      first = m;
      return line.Mid(n, m-n);
   }
   first = last;
   return "";
}

BOOL CSymbolDlg::Convert(const CString &text, DWORD &out)
{
   LPSTR pT;

   out = strtoul(text, &pT, 0);
   return (*pT == 0);
}
/////////////////////////////////////////////////////////////////////////////
// Symbol list display

void CSymbolDlg::ItemUpdate(int iItem, POSITION pos)
{
   t_SYMBOL &sym = m_symbols.GetAt(pos);

   m_list.SetItem(iItem, 0, LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM, FormatValue(sym),
                                sym.type & TYP_IMSK, 0, 0, DWORD(pos));
   m_list.SetItemText(iItem, 1, sym.name);
   m_list.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
   m_list.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
}

bool CSymbolDlg::ListUpdate(bool mltOnly)
{
   int no;
   POSITION pos = m_symbols.GetHeadPosition(), old;
   DWORD mask = TYP_STD, equ = 0;

   m_list.DeleteAllItems();
   m_loc.RemoveAll();
   m_list.ModifyStyle(0, LVS_EDITLABELS);

   if (mltOnly) {
      mask = TYP_MDF|TYP_STD, equ = TYP_MDF;
      m_list.SetTextColor(0x0000C0);
   } else {
      m_list.SetTextColor(0x000000);
   }

   while (old = pos) {
      t_SYMBOL &sym = m_symbols.GetNext(pos);
      if ((sym.type & mask) != equ) continue;

      no = m_list.InsertItem(0x7FFF, FormatValue(sym), sym.type & TYP_IMSK);
      m_list.SetItemText(no, 1, sym.name);
      m_list.SetItemData(no, DWORD(old));
   }
   m_list.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
   m_list.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

   ColumnSort(m_list);

   bool enable = (m_list.GetItemCount() > 0);
   GetDlgItem(IDC_CHK_SYMEXT)->EnableWindow(enable);
   UpdateData(FALSE);
   m_chkMltDef = mltOnly;
   return enable;
}

void CSymbolDlg::ListSort(int state)
{
   SortStatus(m_list, state);
   ListUpdate(m_chkMltDef);
   PutSortMessage();
}

void CSymbolDlg::PutSortMessage()
{
   int status = SortStatus(m_list);
   CString msg, order;

   if (status < 0) {
      msg.LoadString(IDS_SYMPRIORITY);
   } else {
      order.LoadString((status & 1)? IDS_SORTORDERFOR: IDS_SORTORDERBACK);
      msg.Format(IDS_SYMSORTMSG, order, GetColumnTitle(m_list, status / 2));
   }
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(msg);
}

CString CSymbolDlg::FormatValue(t_SYMBOL &symbol)
{
   CString text;
   text.Format((symbol.type & TYP_DEC)? TEXT("%u"): TEXT("0x%04x"), symbol.value);
   return text;
}
/////////////////////////////////////////////////////////////////////////////
//

void CSymbolDlg::SelectAll()
{
   int item = m_list.GetItemCount();
   while (--item >= 0) m_list.SetItemState(item, LVIS_SELECTED, LVIS_SELECTED);
}

void CSymbolDlg::Delete()
{
   POSITION pos = m_list.GetFirstSelectedItemPosition();
   while (pos) {
      int item = m_list.GetNextSelectedItem(pos);
      m_symbols.RemoveAt(POSITION(m_list.GetItemData(item)));
   }
   m_duplic = Qualifie(true);
   ListUpdate(false);
}

// Prefix        | Resource Type           | Valid Range 
//---------------+-------------------------+-----------------
// IDR_          | multiple                | 1     -> 0x6FFF 
// IDD_          | dialog templates        | 1     -> 0x6FFF 
// IDC_,IDI_,IDB_| cursors, icons, bitmaps | 1     -> 0x6FFF 
// IDS_,IDP_     | general strings         | 1     -> 0x7FFF 
// ID_           | commands                | 0x8000-> 0xDFFF 
// IDW_,IDC_     | controls                | 8     -> 0xDFFF 

int CSymbolDlg::SearchType(t_SYMBOL &symbol)
{
   CString &name = symbol.name;
   DWORD type = 0;
   TCHAR car;
   int ix;

   if ( name.Left(ix = 2) == TEXT("ID") || name.Left(ix = 6) == TEXT("AFX_ID") ) {
      if (name.GetLength() >= (ix + 2) && ( (car = name.GetAt(ix)) == TCHAR('_')
                                         || name.GetAt(ix + 1) == TCHAR('_') ) ) {
         switch (car) {
            // For menu item commands using the MFC command architecture.
         case TCHAR('_'): type = TYP_ID_ | RI_MENUCMD; break;
            // For Bitmap resources.
         case TCHAR('B'): type = TYP_IDB | RI_BITMAP; break;
            // For Cursor resources.
         case TCHAR('C'): type = TYP_IDC | RI_CURSOR; break;
            // For dialog template resources (for example, IDD_DIALOG1).
         case TCHAR('D'): type = TYP_IDD | RI_DIALOG; break;
            // For Icon resources.
         case TCHAR('I'): type = TYP_IDI | RI_ICON; break;
            // For menu items not using the MFC command architecture.
         case TCHAR('M'): type = TYP_IDM | RI_MENU; break;
            //"IDP_" strings usually have help topics
         case TCHAR('P'): type = TYP_IDP | RI_STRING; break;
            // Multiple resource types (Used for Menus, Accelerators primarily).
         case TCHAR('R'): type = TYP_IDR | RI_MULT; break;
            // For String resources.
         case TCHAR('S'): type = TYP_IDS | RI_STRING; break;
            // as a specialized form of control IDs
         case TCHAR('W'): type = TYP_IDW | RI_MENUCMD; break;
            // others
         default :        type = TYP_IDx | RI_DATA;
         }
      } else              type = TYP_xxx;

   } else if ( name.Left(5) == TEXT("HIDR_") ) {
                          type = TYP_HIR;
   } else if ( name.Left(5) == TEXT("HIDC_") ) {
                          type = TYP_HIC;
   }
   symbol.type = type;
   return type;
}
/////////////////////////////////////////////////////////////////////////////
// Search a multi definition

int CSymbolDlg::Qualifie(bool all)
{
   POSITION pos0, pos1;
   int cnt = 0, old;

   if (all) {
      pos0 = m_symbols.GetHeadPosition();
      while (pos0) m_symbols.GetNext(pos0).type &= ~TYP_MDF;
   }

   pos0 = m_symbols.GetTailPosition();
   while (pos0) {
      t_SYMBOL &sym0 = m_symbols.GetPrev(pos0);
      if (sym0.type & TYP_MDF) continue;
      old = cnt;
      pos1 = pos0;
      while (pos1) {
         t_SYMBOL &sym1 = m_symbols.GetPrev(pos1);
         if (sym0.value != sym1.value) continue;

         cnt ++;
         if (((sym0.type ^ sym1.type) & TYP_TMSK) == 0)
            cnt += 0x00010000;
         sym1.type |= TYP_MDF;
      }
      if (old != cnt) sym0.type |= TYP_MDF;
   }

   return cnt;
}
/////////////////////////////////////////////////////////////////////////////
// Search a symbol and locate it

void CSymbolDlg::SearchSymbol(int iItem)
{
   CResDoc *pDoc = (CResDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
   CString definition, name = m_list.GetItemText(m_iItem, 1);

   if (name.Find(TCHAR('/')) > 0) {

      pDoc->SearchSymbol(name);
	   ((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);

   } else {
      POSITION pos = (POSITION)m_list.GetItemData(iItem);
      CSymbolList loc;

      if (pos == NULL) return;
      
      // search the use of this symbol
      BeginWaitCursor();
      loc.AddHead(m_symbols.GetAt(pos));
      pDoc->SymbolLocate(loc);
      EndWaitCursor();

      if (loc.GetCount() < 2) {

	      ((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_SYMNOTFOUND);

      } else if (loc.GetCount() == 2) {

         pDoc->SearchSymbol(loc.GetHead().name);
         pDoc->SearchSymbol(name);
         ((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);

      } else {
         CListBoxDlg dlg(loc, this);

         // multiple usage : select the good
         dlg.DoModal();

         if ( pos = loc.FindIndex(dlg.selected) ) {
            pDoc->SearchSymbol( loc.GetAt(pos).name );
            pDoc->SearchSymbol(name);
            ((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);
         }
      }
      m_list.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
   }
}
/////////////////////////////////////////////////////////////////////////////
// Search a symbol in the resource

void CSymbolDlg::SymbolLocate(int iItem)
{
   POSITION pos = (POSITION)m_list.GetItemData(iItem), old;
   if (pos == NULL) return;
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_SYMSEARCHWAIT);
   BeginWaitCursor();

   m_loc.RemoveAll();
   m_loc.AddHead(m_symbols.GetAt(pos));
   m_iItemOld = iItem;

   m_list.ModifyStyle(LVS_EDITLABELS, 0);

   CResDoc *pDoc = (CResDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
   UINT err = pDoc->SymbolLocate(m_loc);

   m_list.DeleteAllItems();
   m_list.SetTextColor(0xC00000);
   pos = m_loc.GetTailPosition();
   t_SYMBOL sym = m_loc.GetPrev(pos);
   int no = m_list.InsertItem(0x7FFF, FormatValue(sym), TICON(sym.type));
   m_list.SetItemText(no, 1, sym.name);
   m_list.SetItemData(no, 0);
   if (pos) {
      while (old = pos) {
         t_SYMBOL &count = m_loc.GetPrev(pos);
         no = m_list.InsertItem(0x7FFF, FormatValue(count), 0);
         m_list.SetItemText(no, 1, count.name);
         m_list.SetItemData(no, DWORD(old));
      }
   } else if (err == 0) err = IDS_SYMNOTFOUND;
   if (err) {
      CString msg;
      msg.LoadString(err);
      no = m_list.InsertItem(0x7FFF, "", RI_SYERROR);
      m_list.SetItemText(no, 1, msg);
   }
   m_list.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
   m_list.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
   EndWaitCursor();
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_READY);
}
/////////////////////////////////////////////////////////////////////////////
// Cut the fat from the symbol table

void CSymbolDlg::SymbolCutFat(bool add)
{
   int removed = 0, analysed = 0, total = m_symbols.GetCount();
   CSymbolList loc;
   CString msg;
   CResDoc *pDoc = (CResDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
   POSITION pos = m_symbols.GetHeadPosition(), old;

   BeginWaitCursor();
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(IDS_SYMCUTFAT);
   while (old = pos) {
      t_SYMBOL &sym = m_symbols.GetNext(pos);
      analysed++;
      if (sym.type & TYP_STD) continue;

      if (total > 100 && (analysed & 0x0F) == 0) {
//         if (!((CResExplorer*)AfxGetApp())->WaitEndOfWork()) break;
         msg.Format(IDS_SYMCUTFATCUR, (analysed * 100)/ total);
	      ((CMainFrame*)AfxGetMainWnd())->SetMessageText(msg);
      }

      loc.RemoveAll();
      loc.AddHead(sym);
      if (pDoc->SymbolLocate(loc, true) == 1) continue;

      // this symbol is unused : its remove of the table
      m_symbols.RemoveAt(old);
      removed++;
   }
   if (removed > 0) ListUpdate(false);
   msg.Format(IDS_SYMCUTFATOK, removed);
	((CMainFrame*)AfxGetMainWnd())->SetMessageText(msg);
   EndWaitCursor();
}
/////////////////////////////////////////////////////////////////////////////
// CODropTarget

DROPEFFECT CODropTarget::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject,
                                    DWORD, CPoint point )
{
   return ((CSymbolDlg*)pWnd)->OnDragOver(pDataObject, point);
}

BOOL CODropTarget::OnDrop( CWnd* pWnd, COleDataObject* pDataObject,
                          DROPEFFECT, CPoint )
{
   return ((CSymbolDlg*)pWnd)->OnDrop(pDataObject);
}

DROPEFFECT CODropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject,
                                     DWORD dwKeyState, CPoint point )
{
   return ((CSymbolDlg*)pWnd)->OnDragEnter(pDataObject);
}
/////////////////////////////////////////////////////////////////////////////
// CListBoxDlg dialog

CListBoxDlg::CListBoxDlg(CSymbolList &loc, CWnd* pParent /*=NULL*/)
   : CDialog(CListBoxDlg::IDD, pParent)
{
	POSITION pos = loc.GetTailPosition();

   loc.GetPrev(pos);                   // skip the first
   count = loc.GetCount() - 1;
   while (pos) list += loc.GetPrev(pos).name + TCHAR('\n');
   list.TrimRight(TCHAR('\n'));
   pFont = (pParent)? pParent->GetFont(): NULL;
   ::GetCursorPos(&point);
   selected = -1;
}

BEGIN_MESSAGE_MAP(CListBoxDlg, CDialog)
	//{{AFX_MSG_MAP(CListBoxDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CListBoxDlg message handlers

void CListBoxDlg::OnPaint() 
{
	CPaintDC dc(this);                  // device context for painting
   CRect r(6,3,3,3);
   CFont *pFold;

   if (pFont) pFold = dc.SelectObject(pFont);
   dc.DrawText(list, r, DT_CALCRECT);
   MoveWindow(point.x, point.y, r.right += 10, bottom = r.bottom += 5);
   dc.FillSolidRect(0, 0, r.right, r.bottom, COLORREF(0xD0FFD0)); 
   dc.DrawText(list, r, 0);
   if (pFont) dc.SelectObject(pFold);
}

void CListBoxDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
   selected = count - (point.y - 3) / ((bottom - 8) / count) - 1;
   CDialog::OnOK();
}
