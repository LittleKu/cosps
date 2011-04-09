// DlgDataRW.cpp : implementation file
//

#include "stdafx.h"
#include "ResUtilities.h"
#include "Resource.h"
#include "ResInfo.h"
#include "DlgDataRW.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// dlgdata.cpp   
// class for parsing dialog templates
//
static CString m_szRCType;
static CString m_szDCStyles;
static CString m_szDCExStyle;
static CString m_szDataBuffer;
static BOOL    m_symbol = TRUE;
static BOOL    m_warningMsg = FALSE;

/////////////////////////////////////////////////////////////////////////////
static UINT GetSymbolNumber(CString symbol);
static void DumpDlgTemp(LPVOID lpDlg);
static void DumpDlgTempEX( LPVOID lpDlg );
static PWORD DumpDlgItemSbr(PWORD pWord, WORD id, DWORD style, DWORD exStyle,
                            short *pDlgSize, short *pDim, DWORD helpID = 0);
static void DumpSubMenu(CMenu *pMenu, const CString &ident = "");
/////////////////////////////////////////////////////////////////////////////
// Place dans un fichier la definition des symboles utilises

typedef struct { CString name; DWORD value; } t_SYMSAV;
static CList<t_SYMSAV, t_SYMSAV&> symTable;
static DWORD m_symValue = 100;

void BufferEmpty()
{
   while (!symTable.IsEmpty()) symTable.RemoveHead();
   m_szDataBuffer.Empty();
   m_symValue = 100;
}

static void DumpSymbolTable(CFile &file, bool include)
{ 
   POSITION pos = symTable.GetHeadPosition();
   if (pos && m_symbol) {
      // Define table
      CString szSymbols, tmp;

      if (include) szSymbols = "#include <afxres.h>\r\n\r\n";

      while (pos) {
         t_SYMSAV &sym = symTable.GetNext(pos);
         tmp.Format("#define %-20s 0x%04X\r\n", sym.name, sym.value);
         szSymbols += tmp;
      }

      szSymbols += "\r\n";
      file.Write(szSymbols.GetBuffer(0), szSymbols.GetLength());
   }
}

static void SaveSymbol(CString &name, DWORD value)
{
   t_SYMSAV sym;
   POSITION pos;

   sym.name = name;
   sym.value = value;
   for (pos = symTable.GetHeadPosition(); pos; symTable.GetNext(pos)) {
      int dif = name.Compare(symTable.GetAt(pos).name);
      if (dif < 0) continue;
      if (dif != 0) symTable.InsertBefore(pos, sym);
      return;
   }
   symTable.AddTail(sym);
}

static CString Symbol( LPCTSTR id, DWORD type, CString fmt, bool save=true )
{
   CString name = FormatId(id, type | TYP_SYM), out;

   if (name.IsEmpty()) return "";
   if (save) SaveSymbol(name, DWORD(id));
   out.Format(fmt, name);

   return out;
}

static CString Symbol( UINT id, DWORD type, CString fmt, bool save=true )
{
   CString name = FormatId(id, type | TYP_SYM), out;

   if (name.IsEmpty()) return "";
   if (save) SaveSymbol(name, id);
   out.Format(fmt, name);

   return out;
}
/////////////////////////////////////////////////////////////////////////////
// Write on a file a CDialog string buffer

UINT WriteDlg(CString fnam, HINSTANCE hinst, LPCTSTR resName)
{
   if ( DumpDialog( hinst, resName ) ) {

      CFile file(fnam, CFile::modeCreate|CFile::modeWrite);
      
      DumpSymbolTable(file, true);

      file.Write(m_szDataBuffer.GetBuffer(0), m_szDataBuffer.GetLength());

      file.Close();
      BufferEmpty();
      return 0;
   }
   return IDS_READERROR;
}

// Helper function that reads a unicode string out of a dialog template,
// and stores the results in an ANSI (8 bit) buffer.  Returns a pointer to
// the byte immediately following the string.
CString GetDlgString( PWORD &pWord, LPCTSTR format )
{
//   PWORD pW = pWord;
   WORD w = *pWord++;
   if (w != 0 && w != 0xFFFF) {
      CString out, str;
      // Loop through all chars in the unicode string
      do out += (char)w; while ( w = *pWord++);
      
      if (format) str.Format(format, out); else return out;
      return str;
   }
   return "";
}
/////////////////////////////////////////////////////////////////////////////
// Put on a string buffer all fields of a Dialog template

CString* DumpDialog( HINSTANCE hinst, LPCTSTR resName )
{

   PVOID pDlg = RawData(hinst, resName, RT_DIALOG);
   if (pDlg) {

      m_szDataBuffer = Symbol(resName, TYP_IDD, "\r\n%s  ");

      // Is it a regular DLGTEMPLATE or DLGTEMPLATEEX
      if ( HIWORD(((LPDLGTEMPLATE)pDlg)->style) != 0xFFFF )
         DumpDlgTemp(pDlg);
      else
         DumpDlgTempEX(pDlg);

      return &m_szDataBuffer;
   }
   return NULL;
}
//
// Displays all the fields of a dialog template,
// and then calls DumpDialogItem to display all the controls.
//
void DumpDlgTemp(LPVOID lpDlg)
{
   LPDLGTEMPLATE pDlgTemp = (LPDLGTEMPLATE)lpDlg;
   PWORD pWord = (PWORD)(pDlgTemp + 1);  // Apres la structure
   short *pDlgSize = &pDlgTemp->cx;
   CString szDlgStyles;
   CString szNumbers;
   CString szString;
   int pos;
   //
   // Start out by printing all the fixed fields in the DLGTEMPLATE
   //          

   // dialog header
	szNumbers.Format("DIALOG DISCARDABLE %i,%i,%i,%i\r\n",
                     pDlgTemp->x, pDlgTemp->y, pDlgTemp->cx, pDlgTemp->cy);
   m_szDataBuffer += szNumbers;

	// windows styles
	szDlgStyles = GetBitfieldsAsString( pDlgTemp->style & ~DS_SETFONT, DlgStyleFlags );
   if (!szDlgStyles.IsEmpty()) {
      pos = m_szDataBuffer.GetLength();
	   m_szDataBuffer += "STYLE " + szDlgStyles + "\r\n";
      BreakLine(m_szDataBuffer, 75, pos, 4);
   }

	// extended windows styles
	szDlgStyles = GetBitfieldsAsString( pDlgTemp->dwExtendedStyle, WndStyleExFlags );
   if (!szDlgStyles.IsEmpty()) {
      pos = m_szDataBuffer.GetLength();
		m_szDataBuffer += "EXSTYLE " + szDlgStyles + "\r\n";
      BreakLine(m_szDataBuffer, 75, pos, 4);
   }

   // Following the fixed DLGTEMPLATE, first up is the menu
   if ( *pWord == 0xFFFF ) {
      pWord++;
      szString.Format("MENU %hu\r\n",*pWord++);
      m_szDataBuffer += szString;
   } else m_szDataBuffer += GetDlgString( pWord, "MENU \"%s\"\r\n" );
    
   // Following the menu array is the "window class" array
   m_szDataBuffer += GetDlgString( pWord, "CLASS \"%s\"\r\n" );
          
   // Following the window class array is the title array
   m_szDataBuffer += GetDlgString( pWord, "CAPTION \"%s\"\r\n" );
 
   // Following the title array is the point size and font (if DS_SETFONT
   // is set in pDlgTemp->Style
   if ( pDlgTemp->style & DS_SETFONT ) {       
      szString.Format("FONT %hu,\"%s\"\r\n",*pWord++, "%s");
		m_szDataBuffer += GetDlgString( pWord, szString );
	} 

	// start of controls
	m_szDataBuffer += "BEGIN\r\n";

   // Now iterate through each of the controls
   for (UINT i = 0; i < pDlgTemp->cdit; i++ ) {
      // align pointer to item template
      pWord = ALIGN_DWORD(PWORD, pWord);

      // Displays all the fields of a dialog control.
      // Returns the next address following the control's data.
      pWord = DumpDlgItemSbr( PWORD(LPDLGITEMTEMPLATE(pWord) + 1),
                              LPDLGITEMTEMPLATE(pWord)->id,
                              LPDLGITEMTEMPLATE(pWord)->style,
                              LPDLGITEMTEMPLATE(pWord)->dwExtendedStyle,
                              pDlgSize,
                              &LPDLGITEMTEMPLATE(pWord)->x,
                              0 );
   }

   // end of controls
   m_szDataBuffer += "END\r\n";
}
//
// Displays all the fields of a dialog templateEx,
// and then calls DumpDialogItem to display all the controls.
//
void DumpDlgTempEX( LPVOID lpDlg )
{
   LPDLGTEMPLATEEX pDlgTempEX = (LPDLGTEMPLATEEX)lpDlg;
   PWORD pWord = (PWORD)(pDlgTempEX + 1);
   CString szDlgStyles, szNumbers, szString;
   short *pDlgSize = &pDlgTempEX->cx;
   int pos;
   LPDLGFONT pFont;
   // DLGTEMPLATEX 
   // Start out by printing all the fixed fields in the DLGTEMPLATE
   //          
   // dialog header
   // 
	szNumbers.Format(" DIALOGEX %i,%i,%i,%i",
            pDlgTempEX->x, pDlgTempEX->y, pDlgTempEX->cx, pDlgTempEX->cy);
	m_szDataBuffer += szNumbers;

   // DlgWindows Help id
   m_szDataBuffer += Symbol(pDlgTempEX->helpID, TYP_HIR, ",%s");
   m_szDataBuffer += "\r\n";

   // DlgWindows styles
   pos = m_szDataBuffer.GetLength();
	szDlgStyles = GetBitfieldsAsString( pDlgTempEX->style & ~DS_SETFONT, DlgStyleFlags );
	m_szDataBuffer += "STYLE " + szDlgStyles + "\r\n";
   BreakLine(m_szDataBuffer, 75, pos, 4);

	// DlgWindows extended styles
	szDlgStyles = GetBitfieldsAsString( pDlgTempEX->exStyle, WndStyleExFlags );
   if (!szDlgStyles.IsEmpty()) {
      pos = m_szDataBuffer.GetLength();
		m_szDataBuffer += "EXSTYLE " + szDlgStyles + "\r\n";
      BreakLine(m_szDataBuffer, 75, pos, 4);
   }

   // Menu
   if ( 0xFFFF == *pWord ) {
      pWord++;
      szString.Format("MENU %hu\r\n", *pWord++);
      m_szDataBuffer += szString;
   } else m_szDataBuffer += GetDlgString( pWord, "MENU \"%s\"\r\n" );
    
   // Following the menu array is the "window class" array
   m_szDataBuffer += GetDlgString( pWord, "CLASS \"%s\"\r\n" );
                
   // Following the window class array is the title array
   m_szDataBuffer += GetDlgString( pWord, "CAPTION \"%s\"\r\n" );

   // Following the title array is the point size, weight,
   //  italic and fontname (if DS_SETFONT is set in pDlgTemp->Style)
   // FONT pointsize, typeface, weight, italic, charset
   if ( pDlgTempEX->style & DS_SETFONT ) { 
      pFont = (LPDLGFONT)pWord;
      pWord = pFont->typeFace;
      szString.Format("FONT %hu,\"%s\",%d,%d\r\n", pFont->pointSize, "%s",
              pFont->weight? 1: 0, pFont->italic? 1: 0);
      m_szDataBuffer += GetDlgString( pWord, szString );
   }

   // start of controls
   m_szDataBuffer += "BEGIN\r\n";

   // Now iterate through each of the control items
   for (UINT i = 0; i < pDlgTempEX->cDlgItems; i++ ) {
      // align pointer to item template
      pWord =  ALIGN_DWORD(PWORD, pWord);

      // Displays all the fields of a dialog control.
      // Returns the next address following the control's data.
      pWord = DumpDlgItemSbr( PWORD(LPDLGITEMTEMPLATEEX(pWord) + 1),
                              LPDLGITEMTEMPLATEEX(pWord)->id,
                              LPDLGITEMTEMPLATEEX(pWord)->style,
                              LPDLGITEMTEMPLATEEX(pWord)->exStyle,
                              pDlgSize,
                              &LPDLGITEMTEMPLATEEX(pWord)->x,
                              LPDLGITEMTEMPLATEEX(pWord)->helpID );
   } // end loop for(...

   // end of controls
   m_szDataBuffer += "END\r\n";
}
//
// DumpDialogItem to display all the controls (for template and templateEx)
//
PWORD DumpDlgItemSbr(PWORD pWord,      // apres la structure DLGITEMTEMPLATE(EX)
                     WORD id,
                     DWORD style,
                     DWORD exStyle,
                     short *pDlgSize,  // size of Dialog
                     short *pDim,      // dim of item
                     DWORD helpID)
{
   const CString quote("\""), quotev("\",");
   CString szDim, szClassName, szTitle, szStylesEx, szStyles;
   WORD wData;

   szDim.Format("%d,%d,%d,%d", pDim[0], pDim[1], pDim[2], pDim[3]);

   // gather the class info
   LPCTSTR classId  = ReadResId(pWord); // class
   t_ClassStyle *pClass = ClassStyle(classId);
   LPCTSTR pRCType = ClassTypeName(pClass, style, FALSE);

   // comput a styles string
   szStyles = GetStyleFlags(style, pClass, NOD_STYLE, WS_CHILD|WS_VISIBLE);

   // Following the window class array is the title array gather title info
   LPCTSTR title = ReadResId(pWord);

   // Following the title array is the (optional) creation data
   wData = *pWord++;

   if (m_warningMsg) {
      // Emit a warning message if the dialog item is not visible
      if ( 0 == (style & WS_VISIBLE) )
         m_szDataBuffer += "//*** Dialog item not visible ***\r\n";

      short cy = (pClass->image == RI_COMBOX && (style & CBS_DROPDOWN))? 16: pDim[3];
      // Check that the dialog item is within the bounds of the dialog.
      // If not, emit a warning message
      if ((pDim[0] + pDim[2]) > pDlgSize[0] || (pDim[1] + cy) > pDlgSize[1])
         m_szDataBuffer += (pDim[0] > pDlgSize[0] || pDim[1] > pDlgSize[1])?
            "//*** Item not within dialog ***\r\n":
               "//*** Item overload dialog ***\r\n";

      if (wData)
         m_szDataBuffer += "//*** Dialog Contains Creation Data ***\r\n";
   }

   // PUT the DlgItem string : KEYWORD [text,]id, ...
   int pos = m_szDataBuffer.GetLength();
   m_szDataBuffer += CString(TCHAR(' '), 4) + pRCType;
   Tab(m_szDataBuffer, 18);

   // no title for COMBOBOX LISTBOX SCROLLBAR or EDITTEXT
   if (title)
      m_szDataBuffer += (HIWORD(title))? StrFilter(title, TRUE) + ",":
                                       Symbol(title, TYP_IDR, ",");
	// control id of this item
	m_szDataBuffer += Symbol(id, TYP_IDC, "%s,");

   if ( pRCType == cntrl ) {
   // Syntax :
   //   CONTROL text,id,class,style,x,y,width,height[[,exStyle],helpId]

		// class name
		m_szDataBuffer += quote + ClassName(pClass) + quotev;

		// styles
      m_szDataBuffer += (szStyles.IsEmpty())? "0,": szStyles + ",";

		// coordinates
		m_szDataBuffer += szDim;

   } else {
   // Syntax :
   //   KEYWORD [text,]id, x,y,width,height[[,style[,exStyle]],helpId]
   //       with KEWORD != CONTROL

      // coordinates
      m_szDataBuffer += szDim;

		// styles
      if ( !szStyles.IsEmpty() ) m_szDataBuffer += "," + szStyles;
      else if ( exStyle|helpID ) m_szDataBuffer += ",0";

   }
   // Syntax :    ... ,exStyle]],helpId]
   if ( exStyle|helpID ) {
      // stylesEx
      szStylesEx = GetStyleFlags( exStyle, pClass, NOD_EXSTYLE );
      m_szDataBuffer += szStylesEx.IsEmpty()? ",0": "," + szStylesEx;

      // optional HelpID
      if (helpID) m_szDataBuffer += Symbol(helpID, TYP_HIC, ",%s");
   }

   // Following the item control parameters is the (optional) creation data.
   if ( wData ) {
   // Syntax : [{data-element-1 [,data-element-2 [, ... ]] }]

      LPCTSTR pSep = " {";
      for (WORD n = 0; n < wData; n += 2) {
         CString tmp;

         // data-element-n
         tmp.Format("%s0x%x", pSep, *pWord++);
         m_szDataBuffer += tmp;
         pSep = ",";
      }
      m_szDataBuffer += "}";
   }

   // end of Dlg item
   m_szDataBuffer += "\r\n";
   BreakLine(m_szDataBuffer, 75, pos, 18);

   // Return value is next byte after the DLGITEMTEMPLATE
   //   and its variable len  fields     
   return pWord;
}
/////////////////////////////////////////////////////////////////////////////
// Put on a string buffer a DLGINIT resource

CString* DumpDlgInit(PVOID pDlg, const CString &path, bool sn)
{
   CString szDlgName, szTmp;
   CString space1("\r\n    "), space2("\r\n        ");
   DLGINITRES *pDlgIn = (DLGINITRES*)pDlg;
   szDlgName.Format("IDC_%s", path);
   m_szDataBuffer = "\r\n" + szDlgName;
//	if (sn) m_defineCards.Add(szDlgName);     // add to define card
   m_szDataBuffer += " DLGINIT\r\nBEGIN";

   CString item;
   WORD lastId = 0;
   while (pDlgIn->itemId) {
      if (pDlgIn->itemId != lastId) {
         lastId = pDlgIn->itemId;            // new item
         item = Symbol(lastId, TYP_IDC, "%s,", sn);
      }
      // Item header
      szTmp.Format("%s0x%04x,%d,0", item, pDlgIn->uMsg, pDlgIn->length);
	   m_szDataBuffer += space1 + szTmp + space2;

      // Item datas
      PWORD pWord = (PWORD)pDlgIn->data;
      int len = (pDlgIn->length + 1) / 2;
      int n = 0;
      while ( --len > 0 ) {
         szTmp.Format("0x%04x,", *pWord++);
	      m_szDataBuffer += szTmp;
         if (++n > 7) m_szDataBuffer += space2, n = 0;
      }
      szTmp.Format("0x%04x", (*pWord & 0x00FF));
	   m_szDataBuffer += szTmp;

      // next entry
      pDlgIn = (DLGINITRES*)(pDlgIn->data + pDlgIn->length);
   }
   m_szDataBuffer += "\r\nEND\r\n";

   return &m_szDataBuffer;
}
/////////////////////////////////////////////////////////////////////////////
// Put on a string buffer a TOOLBAR resource

CString* DumpToolbar(PVOID pDlg,     // memoire contenant la resource
                     const CString &path1,
                     const CString &path2,
                     LPCTSTR pszFileName,
                     BOOL tool)      // TRUE=> tool cree localement
{
   TOOLBARRES *pTool = (TOOLBARRES*)pDlg;
   CString szDlgName, szTmp;
   szDlgName.Format("IDC_%s", path1 );

   // Bitmap file definition
   if (!path2.IsEmpty()) {
      m_szDataBuffer += "\r\n" + szDlgName + " BITMAP DISCARDABLE " +
                          StrFilter(PathReduce(path2, pszFileName), TRUE) + "\r\n";
   }

   // Toolbar definition
   m_szDataBuffer += "\r\n" + szDlgName;
	SaveSymbol(szDlgName, m_symValue++);     // add to define card
   szTmp.Format(" TOOLBAR DISCARDABLE %d,%d\r\n", pTool->x, pTool->y);
	m_szDataBuffer += szTmp;

   // Buttons definition
	m_szDataBuffer += "BEGIN\r\n";
   for (WORD i = 0; i < pTool->cnt; i++) {
      WORD id = tool? pTool->toolId[0]+i : pTool->toolId[i];
      if (id == 0) m_szDataBuffer += "      SEPARATOR\r\n";
      else m_szDataBuffer += Symbol(id, TYP_IDC, "      BUTTON %s\r\n");
   }
   m_szDataBuffer += "END\r\n";

   return &m_szDataBuffer;
}
/////////////////////////////////////////////////////////////////////////////
// Put on a string buffer a MENU resource

CString* DumpMenu(HINSTANCE hinst, LPCTSTR resName)
{
   CMenu menu;
   PVOID pDlg = RawData(hinst, resName, RT_MENU);
   if (pDlg) {
      menu.LoadMenuIndirect( (MENUTEMPLATE*)pDlg );

      m_szDataBuffer += Symbol(resName, TYP_IDC, "\r\n%s MENU DISCARDABLE\r\n");

      DumpSubMenu(&menu);

      m_szDataBuffer += "\r\n";
   }
   return &m_szDataBuffer;
}

void DumpSubMenu(CMenu *pMenu, const CString &ident)
{
   CString szTmp, name;
   CString spaces = "    " + ident;
   m_szDataBuffer += ident + "BEGIN\r\n";
   int n = pMenu->GetMenuItemCount();
   for (int i = 0; i < n; i++) {
      pMenu->GetMenuString(i, name, MF_BYPOSITION);
      name = StrFilter(name, TRUE);
      CMenu *pSm = pMenu->GetSubMenu(i);
      m_szDataBuffer += spaces;
      if (pSm) {
         m_szDataBuffer += "POPUP " + name + "\r\n";
         DumpSubMenu(pSm, spaces);
      } else {
         WORD id = pMenu->GetMenuItemID(i);
         if (id) {
            m_szDataBuffer += "MENUITEM " + name + ",";
            Tab(m_szDataBuffer, 50);
            m_szDataBuffer += Symbol(id, TYP_IDM, "%s\r\n");
         } else m_szDataBuffer += "SEPARATOR\r\n";
      }
   }
   m_szDataBuffer += ident + "END\r\n";
}
/////////////////////////////////////////////////////////////////////////////
// Write on a file a Menu buffer

UINT WriteMenu(CString fnam, HINSTANCE hinst, LPCTSTR resName)
{
   if ( DumpMenu( hinst, resName ) ) {

      CFile file(fnam, CFile::modeCreate|CFile::modeWrite);

      DumpSymbolTable(file, false);

      file.Write(m_szDataBuffer.GetBuffer(0), m_szDataBuffer.GetLength());

      file.Close();
      BufferEmpty();
      return 0;
   }
   return IDS_READERROR;
}
/////////////////////////////////////////////////////////////////////////////
// Put a string buffer for a string Table resource

CString *DumpStringTable(HINSTANCE hinst, LPCTSTR resName)
{
   CString space1("\r\n    ");
   DWORD rsize;

   m_szDataBuffer.Empty();
   PWORD pWord = (PWORD)RawData(hinst, resName, RT_STRING, &rsize);
   if (pWord) {
      WORD id = WORD(resName) * 16 - 16;
      m_szDataBuffer += "\r\nSTRINGTABLE DISCARDABLE\r\nBEGIN";
      int size = rsize / 2;
      while (size > 0) {
         CString buffer = ExtractString(pWord, size);
         if (!buffer.IsEmpty()) {
            m_szDataBuffer += Symbol(id, TYP_IDS, " %s");
            Tab(m_szDataBuffer, 20);
            m_szDataBuffer += "\"" + buffer + "\"";
         }
         id++;
      }
      m_szDataBuffer += "\r\nEND\r\n";
   }

   return &m_szDataBuffer;
}
/////////////////////////////////////////////////////////////////////////////
// Write on a file a string Table buffer

UINT WriteStringTable(CString fnam, HINSTANCE hinst, LPCTSTR resName)
{
   if ( DumpStringTable( hinst, resName ) ) {

      CFile file(fnam, CFile::modeCreate|CFile::modeWrite);

      DumpSymbolTable(file, false);

      file.Write(m_szDataBuffer.GetBuffer(0), m_szDataBuffer.GetLength());

      file.Close();
      BufferEmpty();
      return 0;
   }
   return IDS_READERROR;
}
