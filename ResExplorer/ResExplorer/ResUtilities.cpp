// ResUtilities.cpp: implementation of the CResUtilities class.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResUtilities.h"
#include "SymbolDlg.h"
#include "ResInfo.h"
#include <MMREG.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CImageList m_image;
CImageList m_sIcon;
/////////////////////////////////////////////////////////////////////////////
// Create a unique resource ID
// two resources with the same name must have the same id
static CStringList symList;

LPCTSTR UniqueResId(LPCTSTR resId)
{
   if (DWORD(resId) & 0xFFFF0000) {
      const CString name = CString(resId);
      POSITION pos;

      if (name.Find(" \r\n\t,;") >= 0) return NULL;

      for (pos = symList.GetHeadPosition(); pos; symList.GetNext(pos)) {
         int dif = name.Compare(symList.GetAt(pos));
         if (dif > 0) continue;
         if (dif != 0) pos = symList.InsertBefore(pos, name);
         return symList.GetAt(pos);
      }

      return symList.GetAt(symList.AddTail(name));
   } else {
      return resId;
   }
}

BOOL IsUniqueResId(LPCTSTR resId)
{
   if (DWORD(resId) & 0xFFFF0000) {
      POSITION pos = symList.GetHeadPosition();
      // only the pointer compare
      while (pos) if (resId == LPCTSTR(symList.GetNext(pos))) return TRUE;
   }
   return FALSE;
}

void UniqueResIdInitialise()
{
   symList.RemoveAll();
}
///////////////////////////////////////////////////////////////////////////////
// Lecture et codage des ResId dans le corps de la resource
//    deux formats : 1)ORDINAL 0xFFFF, idWord          => 0x0000idwr
//                   2)NAME    0x00as, ... , 0x0000    => pt to str (LPSTR)

LPCTSTR ReadResId(PWORD &pWord)
{
   WORD w = *pWord++;

   if (w == 0xFFFF) {
      // ResId en format 1
      return LPCTSTR(*pWord++);     // ordinal
   } else if (w) {
      CString text;
      // ResId en format 2
      while (w) text += TCHAR(w), w = *pWord++;
      // il faut etre raisonnable on elimine le symboles de plus de 32 chars
      return (text.GetLength() <= 32)? UniqueResId(text): NULL; 
   } else {
      // ResId nulle
      return NULL;
   }
}

void WriteResId(PWORD& pWord, LPCTSTR lpId)
{
   if ( HIWORD(lpId) ) {
      while ( *pWord++ = WORD(*lpId++) ) ;       // NAME
   } else {
      *pWord++ = 0xFFFF, *pWord++ = WORD(lpId);  // ORDINAL
   }
}

DWORD LenResId(LPCTSTR lpId)
{
   if ( HIWORD(lpId) ) return 2*(strlen( lpId ) + 1);
   else return 4;
}

CString GetResStr(UINT resId)
{
   CString msg;

   msg.Format( HIWORD(resId)? "%08x": "%04x", resId);
   return msg;
}

CString GetResStr(LPCTSTR lpId, BOOL quote)
{
   CString msg, fmt = quote? "\"%s\"": "%s";

   msg.Format( HIWORD(lpId)? fmt:"%04x", lpId );
   return msg;
}
/////////////////////////////////////////////////////////////////////////////
//

CString ReadString(PWORD &pWord)
{
   CString text;
   WORD w = *pWord++, id;
   if (w == 0xFFFF) {
      id = *pWord++;
      text.Format("%04x", id);
   } else {
      while (w) text += TCHAR(w), w = *pWord++;
   }
   return text;
}

void SkipResId(PWORD &pWord)
{
   if (*pWord == 0xFFFF) pWord += 2;
   else while (*pWord++);
}

CString FormatId(UINT id, DWORD type)
{
   return CSymbolDlg::GetString(id, type);
}

CString FormatId(LPCTSTR resId, DWORD type)
{
   if (DWORD(resId) & 0xFFFF0000) return CString(resId);
   else return CSymbolDlg::GetString(DWORD(resId), type);
}

void SymbolGen(UINT id, DWORD type)
{
   CSymbolDlg::SymbolGen(id, type);
}
   
void SymbolGen(LPCTSTR resId, DWORD type)
{
   if ((DWORD(resId) & 0xFFFF0000) == 0)
      CSymbolDlg::SymbolGen(DWORD(resId), type);
}

bool SCompare(t_SYMBOL &symbol, UINT id, DWORD type)
{
   return (id == symbol.value && symbol.name == FormatId(id, type));
}

bool SCompare(t_SYMBOL &symbol, LPCTSTR resId, DWORD type)
{
   return (DWORD(resId) == symbol.value && symbol.name == FormatId(resId, type));
}
/////////////////////////////////////////////////////////////////////////////
// CResTools
/////////////////////////////////////////////////////////////////////////////
// Creation d'un buffer de donnes brutes a partir d'une resource

PVOID RawData(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType, PDWORD pSize)
{
   HRSRC hsrc = ::FindResource(hinst, resName, resType);
   if (hsrc) {
      DWORD size = ::SizeofResource(hinst, hsrc);
      HGLOBAL hg = ::LoadResource(hinst, hsrc);
      if (hg) {
         PVOID pt = ::LockResource(hg);
         try {   // check a memory reading
            BYTE x = PBYTE(pt)[0] + PBYTE(pt)[size];
         } catch (...) { pt = NULL; }
         if (pSize) *pSize = size;
         return pt;
      }
   }
   return NULL;
}
/////////////////////////////////////////////////////////////////////////////

DWORD GetFileNameExt(LPCTSTR fileName)
{
   DWORD dw = 0, t = '.abc';
   int c;
   LPSTR pCar = strrchr(fileName, '.');
   if (pCar) {
      while (c = *pCar++) dw = (dw << 8) | tolower(c & 0x7F); 
   }
   return dw;
}

CString FormatDeci(DWORD dw, LPCTSTR lpMsg)
{
   CString msg;
   msg.Format(lpMsg? "%d %s": "%d", dw, lpMsg);
   return msg;
}

CString FormatHexa(DWORD dw, LPCTSTR lpMsg)
{
   CString msg;
   msg.Format(lpMsg? "%08x %s": "%08x", dw, lpMsg);
   return msg;
}

CString FormatFloat(double fl, LPCTSTR lpMsg)
{
   CString msg;
   msg.Format(lpMsg? "%.3f %s": "%.3f", fl, lpMsg);
   return msg;
}

BOOL ResIdCompare(LPCTSTR lpRes, LPCTSTR lpId)
{
   if ( lpRes == lpId ) return TRUE;
   if ( (DWORD(lpRes) & 0xFFFF0000) == 0 ) return FALSE;
   if ( (DWORD(lpId)  & 0xFFFF0000) == 0 ) return FALSE;
   while (*lpRes) if (toupper(*lpRes++) != toupper(*lpId++)) return FALSE;
   return (*lpId == 0);
}

BOOL CompareSign(PBYTE pRes, LPCTSTR pMod)
{
   int len = strlen(pMod);
   LPCTSTR pMsk = pMod + len + 1;
   if (*pMsk) {
      for (int i = 0; i < len; i++) {
         TCHAR c = (TCHAR)pRes[i];
         switch (pMsk[i]) {
         case '.' : if (c != pMod[i]) return FALSE; break;
         case '*' : break;
         case '0' : if (c != 0) return FALSE; break; 
         case 'F' : if (c != 0xFF) return FALSE; break;
         }
      }
      return TRUE;
   } else {
      return strcmp((LPCTSTR)pRes, pMod) == 0;
   }
}

CString FourCCtoStr(DWORD fcc)
{
   CString str;
   TCHAR c;
   for ( DWORD w = fcc; w; w >>= 8 ) {
      c = TCHAR(w & 0x7F);
      str += isprint(c)? c: '.';
   }
   return str;
}
/////////////////////////////////////////////////////////////////////////////
// Table d'identification des resources
//    raisonnable resource type names
static TCHAR aviMsg[] = {"AVI\0"};
static TCHAR midMsg[] = {"MID\0MIDI\0"};
static TCHAR regMsg[] = {"REG\0REGISTER\0REGISTRY\0"};
static TCHAR rtfMsg[] = {"RTF\0"};
static TCHAR wavMsg[] = {"WAV\0WAVE\0"};
static TCHAR bmpMsg[] = {"BMP\0BITMAP\0"};
static TCHAR jpgMsg[] = {"JPG\0JPEG\0"};
static TCHAR gifMsg[] = {"GIF\0"};
static TCHAR wmfMsg[] = {"WMF\0HME\0META\0METAFILE\0"};
static TCHAR emfMsg[] = {"EMF\0HEM\0ENH\0ENHMETAFILE\0"};
static TCHAR txtMsg[] = {"TEXT\0STRING\0MESSAGE\0"};
const DWORD typ_idd = TYP_IDD|TYP_XSD;

// Displayed resources table
static t_ResID m_resTbl[] = {
//    Resource        icon      symbol  view:type  view:inst title         default    filter
 RID2(RT_ACCELERATOR, RI_ACCEL, TYP_IDR,   0,        WRT|EDT,"Accelerator","Acceler", NULL),
 RID2(RT_ANICURSOR,   RI_CURSOR,TYP_IDC,   0,            VRS,"ANICURSOR",  "AniCurs","Cursor (*.ani)|*.ani"),
 RID2(RT_ANIICON,     RI_ICON,  TYP_IDI,   0,            VRS,"ANIICON",    "AniIcon","Icon (*.ani)|*.ani"),
 RID2(RT_BITMAP,      RI_BITMAP,TYP_IDB, VRS,    WRT|EDT|VRS,"Bitmap",     "Image", "Bitmap (*.bmp)|*.bmp"),
 RID1(RT_CURSOR,      0,        0,         0,              0,NULL),
 RID2(RT_DIALOG,      RI_DIALOG,typ_idd, VRS,    WRT|EDT|VRS,"Dialog box", "Dialog","Dialog (*.dlg)|*.dlg"),
 RID1(RT_DLGINIT,     RI_DLGINIT,TYP_IDC,  0,            VRS,"Dlg init"),
 RID1(RT_FONT,        RI_FONT,  0,       VRS,            VRS,"Font"),
 RID1(RT_FONTDIR,     RI_FONT,  0,       VRS,            VRS,"Font directory"), 
 RID2(RT_GROUP_CURSOR,RI_CURSOR,TYP_IDC, VRS,ICO|WRT|EDT|VRS,"Cursor",     "Cursor",
                          "Cursor (*.cur)|*.cur|Icon (*.ico)|*.ico|Image (*.bmp)|*.bmp"),
 RID2(RT_GROUP_ICON,  RI_ICON,  TYP_IDI, VRS,ICO|WRT|EDT|VRS,"Icon",       "Icon",
                          "Icon (*.ico)|*.ico|Cursor (*.cur)|*.cur|Image (*.bmp)|*.bmp"),
 RID1(RT_HTML,        RI_HTML,  TYP_IDx,   0,        DAT|VRS,"HTML document"),
 RID1(RT_XML,         RI_HTML,  TYP_IDx,   0,        DAT|VRS,"XML document"), 
 RID1(RT_ICON,        0,        0,         0,              0,NULL),
 RID2(RT_MENU,        RI_MENU,  TYP_IDR,   0,    WRT|EDT|VRS,"Menu",       "Menu","Dialog (*.dlg)|*.dlg"),
 RID1(RT_MESSAGETABLE,0,        0,       VRS,        DAT|VRS,"Message-table"),
 RID1(RT_PLUGPLAY,    0,        0,         0,              0,"Plug and play"),
 RID2(RT_RCDATA,      RI_DATA,  TYP_USR,   0,        DAT|WRT,"Appli-defined","Data",  NULL), 
 RID2(RT_STRING,      RI_STRING,TYP_NUM,   0,        EDT|WRT,"String-table","String","String (*.dlg)|*.dlg"),
 RID2(RT_TOOLBAR,     RI_TOOLBAR,TYP_IDR,VRS,        WRT|VRS,"Toolbar",    "Toolbar", NULL),
 RID2(RT_USERDEFINE,  RI_CUSTOM,TYP_USR,   0,        DAT|WRT,"User define","User",    NULL), 
 RID2(RT_VERSION,     RI_VERSION,TYP_NUM,  0,            WRT,"Version",    "Version", NULL),
 RID1(RT_VXD,         0,        0,         0,              0,"VXD"),
 RID3(aviMsg, rt_avi, RI_AVI,   TYP_USR,   0,        WRT|VRS,"Avi",        "Anim", "Anim (*.avi)|*.avi"),
 RID3(midMsg, rt_mid, RI_MIDI,  TYP_USR,   0,        WRT|VRS,"Midi",       "Midi", "Midi (*.mid)|*.mid"),
 RID3(regMsg, rt_reg, RI_REG,   TYP_USR,   0,    EDT|WRT|VRS,"Registry",   "Register","Register (*.reg)|*.reg"),
 RID3(rtfMsg, rt_rtf, RI_STRING,TYP_USR,   0,    EDT|WRT|VRS,"Rtf",        "RtfTxt",
                           "Rtf (*.rtf)|*.rtf|Texte (*.txt)|*.txt"),
 RID3(wavMsg, rt_wav, RI_WAVE,  TYP_USR,   0,            WRT,"Wave",       "Wave", "Wave (*.wav)|*.wav"),
 RID3(bmpMsg, rt_bmp, RI_BITMAP,TYP_USR,   0,    EDT|WRT|VRS,"Image",      "Image","Bitmap (*.bmp)|*.bmp"),
 RID3(bmpMsg, rt_bmx, RI_BITMAP,TYP_USR,   0,    EDT|WRT|VRS,"Image",      "Image","Bitmap (*.bmp)|*.bmp"),
 RID3(jpgMsg, rt_jpg, RI_BITMAP,TYP_USR,   0,    EDT|WRT|VRS,"Image",      "Image",
                           "Image (*.jpg)|*.jpg|Bitmap (*.bmp)|*.bmp"),
 RID3(gifMsg, rt_gif, RI_BITMAP,TYP_USR,   0,    EDT|WRT|VRS,"Image",      "Image",
                           "Image (*.gif)|*.gif|Bitmap (*.bmp)|*.bmp"),
 RID3(wmfMsg, rt_wmf, RI_BITMAP,TYP_USR,   0,    EDT|WRT|VRS,"Image",      "Image","Meta (*.wmf)|*.wmf"),
 RID3(emfMsg, rt_emf, RI_BITMAP,TYP_USR,   0,    EDT|WRT|VRS,"Image",      "Image","EnhM (*.emf)|*.emf"),
 RID3(txtMsg, rt_txt, RI_STRING,TYP_USR,   0,    EDT|WRT|VDT,"Texte",      "Text","Text  (*.txt)|*.txt"),
 RID2(RT_UNDEF,       RI_UNDEF, TYP_USR,   0,        DAT|WRT, NULL,        "Data",    NULL) 
};

t_ResID *GetResEntry(LPCTSTR resType)
{
   for (t_ResID *pRid = m_resTbl; pRid->lpId; pRid++)
      if (resType == pRid->lpId || WORD(resType) == pRid->wId) break;
   return pRid;
}

t_ResID *GetResEntryEx(LPCTSTR resType)
{
   t_ResID *pRid = m_resTbl;
   if ( HIWORD(resType) ) {
      for ( ; pRid->lpId; pRid++)
         if ( HIWORD(pRid->lpId) ) {
            LPCTSTR pt = pRid->lpId, pr;
            do {
               TCHAR c = 0;
               pr = resType;
               while (toupper(c = *pt++) == toupper(*pr++))
                  if (c == 0) return pRid;
               while (c) c = *pt++;
            } while (*pt);
         }
   } else {
      for ( ; pRid->lpId; pRid++)
         if ( pRid->lpId == resType ) return pRid;
   }
   return pRid;
}
/////////////////////////////////////////////////////////////////////////////
// Coupe les lignes trop longues d'un texte

void BreakLine(CString &text, int lineLen, int pos /*=0*/, int tab /*=0*/)
{
   int brk = 0, i;
   CString endOfLine = CString("\r\n") + CString(' ', tab), quote("\"");
   BOOL seq = TRUE;  // pour ne pas ajouter de saut de ligne dans une chaine ""

   // determine le debut de la ligne
   while (pos >= 0) {
      TCHAR c = text.GetAt(pos);
      if (c == '\n' || c == '\r') break;
      pos--;
   }

   // coupe la ligne si c'est necessaire
   if ((text.GetLength() - ++pos) > lineLen) {
      // pos pointe le debut de la ligne
      for (i = pos; i < text.GetLength(); i++) {

         // si la ligne est trop longue (et la coupure possible)
         if (i >= (pos + lineLen) && brk > 0 && seq) {
//            if (seq) {
               text.Insert( i = brk + 1, endOfLine );
//            } else {
//               // nous sommes dans une chaine entre quotes
//               text.Insert( brk, quote + endOfLine + quote );
//               // i saute '"' et pointe '\r' : entraine la mise a jour de pos
//               i = brk + 1; seq = TRUE;
//            }
         }

         // detecte le point de coupure et la fin de ligne
         switch ( text.GetAt(i) ) {

         case '\"': seq = ! seq, brk = 0; break;   // debut ou fin de chaine entre quotes

         case ' ':
         case ',':
         case '|': if (i > (pos + tab)) brk = i; break;

         case '\n':  // pos est mis en debut de ligne
         case '\r': pos = i, brk = 0; break;
         }
      }
   }
}
/////////////////////////////////////////////////////////////////////////////
// Decode les Styles ou extStyles de Windows

static int scp1(const void*p, const void*q) {return int(*PDWORD(p) - *PDWORD(q));}
static int scp2(const void*p, const void*q) {return strcmp(*(LPCTSTR*)p, *(LPCTSTR*)q);}

CString DecodFlags( DWORD dwAttr,
                     const t_nameID *pTbl,
                     const CString &sep,
                     UINT flgs )
{
   CString attrMsg, restMsg;
   LPCTSTR outName[32], *pt = outName; // pointeur des textes retenus
   DWORD mask = 0, data;
   short cnt;                          // count des symboles decodes

   while ( pTbl ) {

      switch ( DWORD(pTbl->title) ) {
      case 0:                          // fin de la table
         pTbl = (flgs & DFF_NLINK)? NULL: (t_nameID*)pTbl->id;
         break;
      case 1:                          // entree masque
         dwAttr ^= (data = dwAttr & (mask = pTbl->id)), pTbl++;
         break;
      default:                         // un titre existe
         // fin du masque, des bits sont a l'exterieur
         if ( mask && (pTbl->id & ~mask) ) mask = 0;

         // extraction des messages (avec ou sans entrees nulles)
         if ( mask ) {                 // mode masque
            if ( (pTbl->id|(DFF_DNULL&~flgs)) && pTbl->id==data )
               *pt++ = pTbl->title;
         } else {                      // mode bit isole
            if ( pTbl->id && pTbl->id == (pTbl->id & dwAttr) )
               dwAttr &= ~pTbl->id, *pt++ = pTbl->title; 
         }
         pTbl++;                       // entree suivante
      }
   } // end loop while()

   if ( (cnt = (pt - outName)) > 0 ) {
      // classe les noms par ordre d'importance
      if ( flgs & (DFF_SORT|DFF_ALFA) )
         qsort(outName, cnt, sizeof(LPCTSTR), (flgs & DFF_ALFA)? scp2: scp1);

      // composition du texte de sortie
      attrMsg = CString( outName[0] );
      for (short n = 1; n < cnt; n++) attrMsg += sep + outName[n];
      restMsg = sep;
   }

   if ( dwAttr && (flgs & DFF_REST) ) {
      // Ajoute les bits non detectes en hexa
      attrMsg += restMsg + FormatHexa(dwAttr);
   }
   return attrMsg;
}
/////////////////////////////////////////////////////////////////////////////
// Style flags common to all wnds
//
//
// Extended Window Styles
//
t_nameID WndStyleExFlags[] = {
	NAMESTR( WS_EX_OVERLAPPEDWINDOW),// WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE
	NAMESTR( WS_EX_PALETTEWINDOW),   // WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST
	NAMESTR( WS_EX_DLGMODALFRAME),   // 0x00000001
	NAMESTR( WS_EX_NOPARENTNOTIFY),  // 0x00000004
	NAMESTR( WS_EX_TOPMOST),         // 0x00000008
	NAMESTR( WS_EX_ACCEPTFILES),     // 0x00000010  
	NAMESTR( WS_EX_TRANSPARENT),     // 0x00000020   
	NAMESTR( WS_EX_MDICHILD),        // 0x00000040
	NAMESTR( WS_EX_TOOLWINDOW),      // 0x00000080       
	NAMESTR( WS_EX_WINDOWEDGE),      // 0x00000100
	NAMESTR( WS_EX_CLIENTEDGE),      // 0x00000200
	NAMESTR( WS_EX_CONTEXTHELP),     // 0x00000400
	NAMESTR( WS_EX_RIGHT),           // 0x00001000
	NAMESTR( WS_EX_RTLREADING),      // 0x00002000
	NAMESTR( WS_EX_LEFTSCROLLBAR),   // 0x00004000 
	NAMESTR( WS_EX_CONTROLPARENT),   // 0x00010000   
	NAMESTR( WS_EX_STATICEDGE ),     // 0x00020000 
	NAMESTR( WS_EX_APPWINDOW),       // 0x00040000
   END_OF_TABLE              };
//
// Class styles
//
t_nameID ClassStyleFlags[] = {
   NAMESTR( CS_VREDRAW ),           // 0x00000001
   NAMESTR( CS_HREDRAW ),           // 0x00000002
   NAMESTR( CS_DBLCLKS ),           // 0x00000008
   NAMESTR( CS_OWNDC ),             // 0x00000020
   NAMESTR( CS_CLASSDC ),           // 0x00000040
   NAMESTR( CS_PARENTDC ),          // 0x00000080
   NAMESTR( CS_NOCLOSE ),           // 0x00000200
   NAMESTR( CS_SAVEBITS ),          // 0x00000800
   NAMESTR( CS_BYTEALIGNCLIENT ),   // 0x00001000
   NAMESTR( CS_BYTEALIGNWINDOW ),   // 0x00002000
   NAMESTR( CS_GLOBALCLASS ),       // 0x00004000
   NAMESTR( CS_IME ),               // 0x00010000
   END_OF_TABLE              };
//
static t_nameID DefCntrlStyleFlags[] = {
   NAMESTR( DS_ABSALIGN ),          // 0x0001
   NAMESTR( DS_SYSMODAL ),          // 0x0002
   NAMESTR( DS_3DLOOK ),            // 0x0004
   NAMESTR( DS_FIXEDSYS ),          // 0x0008
   NAMESTR( DS_NOFAILCREATE ),      // 0x0010
   NAMESTR( DS_LOCALEDIT ),         // 0x0020  Edit items get Local storage.
   NAMESTR( DS_SETFONT ),           // 0x0040  User specified font for Dlg controls
   NAMESTR( DS_MODALFRAME ),        // 0x0080  Can be combined with WS_CAPTION
   NAMESTR( DS_NOIDLEMSG ),         // 0x0100  WM_ENTERIDLE message will not be sent
   NAMESTR( DS_SETFOREGROUND ),     // 0x0200  not in win3.1
   NAMESTR( DS_CONTROL ),           // 0x0400
   NAMESTR( DS_CENTER ),            // 0x0800
   NAMESTR( DS_CENTERMOUSE ),       // 0x1000
   NAMESTR( DS_CONTEXTHELP ),       // 0x2000

	NAMESTR( BS_NOTIFY ),            // 0x4000           
	NAMESTR( BS_FLAT ),              // 0x8000
   END_OF_TABLE            };
/////////////////////////////////////////////////////////////////////////////
//
// Window Styles
//
t_nameID WndStyleFlags[] = {
   BITMASK(WS_POPUP|WS_CHILD),
	NAMESTR(WS_OVERLAPPED),          // 0x00000000
	NAMESTR(WS_POPUP),               // 0x80000000
	NAMESTR(WS_CHILD),               // 0x40000000

	NAMESTR(WS_MINIMIZE),            // 0x20000000
	NAMESTR(WS_VISIBLE),             // 0x10000000
	NAMESTR(WS_DISABLED),            // 0x08000000
	NAMESTR(WS_CLIPSIBLINGS),        // 0x04000000
	NAMESTR(WS_CLIPCHILDREN),        // 0x02000000
	NAMESTR(WS_MAXIMIZE),            // 0x01000000
	NAMESTR(WS_CAPTION),             // 0x00C00000   WS_BORDER | WS_DLGFRAME
	NAMESTR(WS_BORDER),              // 0x00800000
	NAMESTR(WS_DLGFRAME),            // 0x00400000
	NAMESTR(WS_VSCROLL),             // 0x00200000
	NAMESTR(WS_HSCROLL),             // 0x00100000
	NAMESTR(WS_SYSMENU),             // 0x00080000
	NAMESTR(WS_THICKFRAME),          // 0x00040000
	NAMESTR(WS_GROUP),               // 0x00020000
	NAMESTR(WS_TABSTOP),             // 0x00010000
   TABLE_LINK(DefCntrlStyleFlags)   };
//
// Dialog Styles
// All style flags used by dialogs
//
t_nameID DlgStyleFlags[] = {
   NAMESTR( DS_ABSALIGN ),          // 0x0001
   NAMESTR( DS_SYSMODAL ),          // 0x0002
   NAMESTR( DS_3DLOOK ),            // 0x0004
   NAMESTR( DS_FIXEDSYS ),          // 0x0008
   NAMESTR( DS_NOFAILCREATE ),      // 0x0010
   NAMESTR( DS_LOCALEDIT ),         // 0x0020  Edit items get Local storage.
   NAMESTR( DS_SETFONT ),           // 0x0040  User specified font for Dlg controls
   NAMESTR( DS_MODALFRAME ),        // 0x0080  Can be combined with WS_CAPTION
   NAMESTR( DS_NOIDLEMSG ),         // 0x0100  WM_ENTERIDLE message will not be sent
   NAMESTR( DS_SETFOREGROUND ),     // 0x0200  not in win3.1
   NAMESTR( DS_CONTROL ),           // 0x0400
   NAMESTR( DS_CENTER ),            // 0x0800
   NAMESTR( DS_CENTERMOUSE ),       // 0x1000
   NAMESTR( DS_CONTEXTHELP ),       // 0x2000
// includes all wnd styles
   TABLE_LINK(WndStyleFlags)   };
//
static t_nameID CommonDlgStyleFlags[] = {
   // overlapped window est elimine
	NAMESTR(WS_POPUP),               // 0x80000000
	NAMESTR(WS_CHILD),               // 0x40000000
	NAMESTR(WS_MINIMIZE),            // 0x20000000
	NAMESTR(WS_VISIBLE),             // 0x10000000
	NAMESTR(WS_DISABLED),            // 0x08000000
	NAMESTR(WS_CLIPSIBLINGS),        // 0x04000000
	NAMESTR(WS_CLIPCHILDREN),        // 0x02000000
	NAMESTR(WS_MAXIMIZE),            // 0x01000000
	NAMESTR(WS_CAPTION),             // 0x00C00000   WS_BORDER | WS_DLGFRAME
	NAMESTR(WS_BORDER),              // 0x00800000
	NAMESTR(WS_DLGFRAME),            // 0x00400000
	NAMESTR(WS_VSCROLL),             // 0x00200000
	NAMESTR(WS_HSCROLL),             // 0x00100000
	NAMESTR(WS_SYSMENU),             // 0x00080000
	NAMESTR(WS_THICKFRAME),          // 0x00040000
	NAMESTR(WS_GROUP),               // 0x00020000  implicit
	NAMESTR(WS_TABSTOP),             // 0x00010000  not for Button and Static
   TABLE_LINK(DefCntrlStyleFlags) };// for add default spec.
//
// Button Control Styles class 0x0080
//
static t_nameID ButtonStyleFlags[] = {
	BITMASK( SS_TYPEMASK ),          // 0x001F         
	NAMESTR( BS_PUSHBUTTON ),        // 0x0000       
	NAMESTR( BS_DEFPUSHBUTTON ),     // 0x0001    
	NAMESTR( BS_CHECKBOX ),          // 0x0002         
	NAMESTR( BS_AUTOCHECKBOX ),      // 0x0003
	NAMESTR( BS_RADIOBUTTON ),       // 0x0004      
	NAMESTR( BS_3STATE ),            // 0x0005           
	NAMESTR( BS_AUTO3STATE ),        // 0x0006       
	NAMESTR( BS_GROUPBOX ),          // 0x0007         
	NAMESTR( BS_USERBUTTON ),        // 0x0008       
	NAMESTR( BS_AUTORADIOBUTTON ),   // 0x0009  
	NAMESTR( BS_OWNERDRAW ),         // 0x000B
   
	NAMESTR( BS_LEFTTEXT ),          // 0x0020         
	NAMESTR( BS_ICON ),              // 0x0040             
	NAMESTR( BS_BITMAP ),            // 0x0080
   
	NAMESTR( BS_CENTER ),            // 0x0300          
	NAMESTR( BS_LEFT ),              // 0x0100             
	NAMESTR( BS_RIGHT ),             // 0x0200
   
	NAMESTR( BS_VCENTER ),           // 0x0C00          
	NAMESTR( BS_TOP ),               // 0x0400              
	NAMESTR( BS_BOTTOM ),            // 0x0800
   
	NAMESTR( BS_PUSHLIKE ),          // 0x1000         
	NAMESTR( BS_MULTILINE ),         // 0x2000        
	NAMESTR( BS_NOTIFY ),            // 0x4000           
	NAMESTR( BS_FLAT ),              // 0x8000
   TABLE_LINK(CommonDlgStyleFlags) };
//
// Edit Control Styles class 0x0081
//
static t_nameID EditStyleFlags[] = {
	BITMASK( ES_LEFT|ES_CENTER|ES_RIGHT ),         
	NAMESTR( ES_LEFT ),              // 0x0000
	NAMESTR( ES_CENTER ),            // 0x0001
	NAMESTR( ES_RIGHT ),             // 0x0002
   
	NAMESTR( ES_MULTILINE),          // 0x0004
	NAMESTR( ES_UPPERCASE),          // 0x0008
	NAMESTR( ES_LOWERCASE),          // 0x0010
	NAMESTR( ES_PASSWORD),           // 0x0020
	NAMESTR( ES_AUTOVSCROLL),        // 0x0040
	NAMESTR( ES_AUTOHSCROLL),        // 0x0080   
	NAMESTR( ES_NOHIDESEL),          // 0x0100
	NAMESTR( ES_OEMCONVERT),         // 0x0400
	NAMESTR( ES_READONLY),           // 0x0800
	NAMESTR( ES_WANTRETURN),         // 0x1000
	NAMESTR( ES_NUMBER),             // 0x2000
   TABLE_LINK(CommonDlgStyleFlags) };
//
// Static Control Constants class 0x0082
//
static t_nameID StaticStyleFlags[] = {
	BITMASK( SS_TYPEMASK ),          // 0x001F
	NAMESTR( SS_LEFT ),              // 0x0000
	NAMESTR( SS_CENTER ),            // 0x0001
	NAMESTR( SS_RIGHT ),             // 0x0002
	NAMESTR( SS_ICON ),              // 0x0003
	NAMESTR( SS_BLACKRECT ),         // 0x0004
	NAMESTR( SS_GRAYRECT ),          // 0x0005
	NAMESTR( SS_WHITERECT ),         // 0x0006
	NAMESTR( SS_BLACKFRAME ),        // 0x0007
	NAMESTR( SS_GRAYFRAME ),         // 0x0008
	NAMESTR( SS_WHITEFRAME ),        // 0x0009
	NAMESTR( SS_USERITEM ),          // 0x000A
	NAMESTR( SS_SIMPLE ),            // 0x000B
	NAMESTR( SS_LEFTNOWORDWRAP ),    // 0x000C
	NAMESTR( SS_OWNERDRAW ),         // 0x000D
	NAMESTR( SS_BITMAP ),            // 0x000E
	NAMESTR( SS_ENHMETAFILE ),       // 0x000F
	NAMESTR( SS_ETCHEDHORZ ),        // 0x0010
	NAMESTR( SS_ETCHEDVERT ),        // 0x0011
	NAMESTR( SS_ETCHEDFRAME ),       // 0x0012
   
	NAMESTR( SS_NOPREFIX ),          // 0x0080  Don't do "&" character translation
	NAMESTR( SS_NOTIFY ),            // 0x0100
	NAMESTR( SS_CENTERIMAGE ),       // 0x0200
	NAMESTR( SS_RIGHTJUST ),         // 0x0400
	NAMESTR( SS_REALSIZEIMAGE ),     // 0x0800
	NAMESTR( SS_SUNKEN ),            // 0x1000
   
   NAMESTR( SS_WORDELLIPSIS ),      // 0xC000
	NAMESTR( SS_ENDELLIPSIS ),       // 0x4000
	NAMESTR( SS_PATHELLIPSIS ),      // 0x8000
   TABLE_LINK(CommonDlgStyleFlags) };
//
// Listbox Styles class 0x0083
//
static t_nameID ListBoxStyleFlags[] = {
   NAMESTR( LBS_STANDARD ),         // LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER
	NAMESTR( LBS_NOTIFY ),           // 0x0001
	NAMESTR( LBS_SORT ),             // 0x0002
	NAMESTR( LBS_NOREDRAW ),         // 0x0004     
	NAMESTR( LBS_MULTIPLESEL ),      // 0x0008
	NAMESTR( LBS_OWNERDRAWFIXED ),   // 0x0010
	NAMESTR( LBS_OWNERDRAWVARIABLE ),// 0x0020
	NAMESTR( LBS_HASSTRINGS ),       // 0x0040
	NAMESTR( LBS_USETABSTOPS ),      // 0x0080
	NAMESTR( LBS_NOINTEGRALHEIGHT ), // 0x0100
	NAMESTR( LBS_MULTICOLUMN ),      // 0x0200 
	NAMESTR( LBS_WANTKEYBOARDINPUT ),// 0x0400
	NAMESTR( LBS_EXTENDEDSEL ),      // 0x0800
	NAMESTR( LBS_DISABLENOSCROLL ),  // 0x1000
	NAMESTR( LBS_NODATA ),           // 0x2000
	NAMESTR( LBS_NOSEL ),            // 0x4000 
   TABLE_LINK(CommonDlgStyleFlags) };
//
// Scroll Bar Styles class 0x0084
//
static t_nameID ScrollBarStyleFlags[] = {
   BITMASK( SBS_HORZ|SBS_VERT ),
	NAMESTR( SBS_HORZ ),             // 0x0000
	NAMESTR( SBS_VERT ),             // 0x0001

	NAMESTR( SBS_TOPALIGN ),                
	NAMESTR( SBS_LEFTALIGN ),               
	NAMESTR( SBS_BOTTOMALIGN ),             
	NAMESTR( SBS_RIGHTALIGN ),              
	NAMESTR( SBS_SIZEBOXTOPLEFTALIGN ),     
	NAMESTR( SBS_SIZEBOXBOTTOMRIGHTALIGN ), 
	NAMESTR( SBS_SIZEBOX ),                 
	NAMESTR( SBS_SIZEGRIP ),
   TABLE_LINK(CommonDlgStyleFlags) };
//
// Combo Box styles class 0x0085
//
static t_nameID ComboBoxStyleFlags[] = {
	NAMESTR( CBS_DROPDOWNLIST ),     // 0x0003  
	NAMESTR( CBS_SIMPLE ),           // 0x0001 
	NAMESTR( CBS_DROPDOWN ),         // 0x0002

	NAMESTR( CBS_OWNERDRAWFIXED ),   // 0x0010 
	NAMESTR( CBS_OWNERDRAWVARIABLE ),// 0x0020 
	NAMESTR( CBS_AUTOHSCROLL ),      // 0x0040  
	NAMESTR( CBS_OEMCONVERT ),       // 0x0080  
	NAMESTR( CBS_SORT ),             // 0x0100
	NAMESTR( CBS_HASSTRINGS ),       // 0x0200
	NAMESTR( CBS_NOINTEGRALHEIGHT ), // 0x0400
	NAMESTR( CBS_DISABLENOSCROLL ),  // 0x0800
	NAMESTR( CBS_UPPERCASE ),        // 0x2000
	NAMESTR( CBS_LOWERCASE ),        // 0x4000
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID TreeViewStyleFlags[] = {
   NAMESTR( TVS_HASBUTTONS ),       // 0x0001
   NAMESTR( TVS_HASLINES ),         // 0x0002
   NAMESTR( TVS_LINESATROOT ),      // 0x0004
   NAMESTR( TVS_EDITLABELS ),       // 0x0008
   NAMESTR( TVS_DISABLEDRAGDROP ),  // 0x0010
   NAMESTR( TVS_SHOWSELALWAYS ),    // 0x0020
   NAMESTR( TVS_RTLREADING ),       // 0x0040
   NAMESTR( TVS_NOTOOLTIPS ),       // 0x0080
   NAMESTR( TVS_CHECKBOXES ),       // 0x0100
   NAMESTR( TVS_TRACKSELECT ),      // 0x0200
   NAMESTR( TVS_SINGLEEXPAND ),     // 0x0400
   NAMESTR( TVS_INFOTIP ),          // 0x0800
   NAMESTR( TVS_FULLROWSELECT ),    // 0x1000
   NAMESTR( TVS_NOSCROLL ),         // 0x2000
   NAMESTR( TVS_NONEVENHEIGHT ),    // 0x4000
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID BarStyleFlags[] = {
   BITMASK( TBSTYLE_BUTTON|TBSTYLE_SEP ),
   NAMESTR( TBSTYLE_BUTTON ),       // 0x0000
   NAMESTR( TBSTYLE_SEP ),          // 0x0001

   NAMESTR( TBSTYLE_CHECKGROUP ),   // TBSTYLE_GROUP | TBSTYLE_CHECK
   NAMESTR( TBSTYLE_CHECK ),        // 0x0002
   NAMESTR( TBSTYLE_GROUP ),        // 0x0004
   NAMESTR( TBSTYLE_DROPDOWN ),     // 0x0008
   NAMESTR( TBSTYLE_AUTOSIZE ),     // 0x0010  automatically calculate the cx of the button
   NAMESTR( TBSTYLE_NOPREFIX ),     // 0x0020  if this button should not have accel prefix
   NAMESTR( TBSTYLE_TOOLTIPS ),     // 0x0100
   NAMESTR( TBSTYLE_WRAPABLE ),     // 0x0200
   NAMESTR( TBSTYLE_ALTDRAG ),      // 0x0400
   NAMESTR( TBSTYLE_FLAT ),         // 0x0800
   NAMESTR( TBSTYLE_LIST ),         // 0x1000
   NAMESTR( TBSTYLE_CUSTOMERASE ),  // 0x2000
   NAMESTR( TBSTYLE_REGISTERDROP ), // 0x4000
   NAMESTR( TBSTYLE_TRANSPARENT ),  // 0x8000
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID BarStyleExFlags[] = {
   NAMESTR( TBSTYLE_EX_DRAWDDARROWS),// 0x00000001
   TABLE_LINK(WndStyleExFlags) };
//
static t_nameID RebarStyleFlags[] = {
   NAMESTR( RBS_TOOLTIPS ),         // 0x0100
   NAMESTR( RBS_VARHEIGHT ),        // 0x0200
   NAMESTR( RBS_BANDBORDERS ),      // 0x0400
   NAMESTR( RBS_FIXEDORDER ),       // 0x0800
   NAMESTR( RBS_REGISTERDROP ),     // 0x1000
   NAMESTR( RBS_AUTOSIZE ),         // 0x2000
   NAMESTR( RBS_VERTICALGRIPPER),   // 0x4000 this always has the vertical gripper (default for horizontal mode)
   NAMESTR( RBS_DBLCLKTOGGLE ),     // 0x8000
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID TooltipStyleFlags[] = {
   NAMESTR( TTS_ALWAYSTIP ),        // 0x0001
   NAMESTR( TTS_NOPREFIX ),         // 0x0002
   NAMESTR( TTF_RTLREADING ),       // 0x0004
   NAMESTR( TTF_SUBCLASS ),         // 0x0010
   NAMESTR( TTF_TRACK ),            // 0x0020
   NAMESTR( TTF_ABSOLUTE ),         // 0x0080
   NAMESTR( TTF_TRANSPARENT ),      // 0x0100
   NAMESTR( TTF_DI_SETITEM ),       // 0x8000   valid only on the TTN_NEEDTEXT callback
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID StatusStyleFlags[] = {
   NAMESTR( SBT_OWNERDRAW ),        // 0x1000
   NAMESTR( SBT_NOBORDERS ),        // 0x0100
   NAMESTR( SBT_POPOUT ),           // 0x0200
   NAMESTR( SBT_RTLREADING ),       // 0x0400
   NAMESTR( SBT_TOOLTIPS ),         // 0x0800
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID UpdownStyleFlags[] = {
   NAMESTR( UDS_WRAP ),             // 0x0001
   NAMESTR( UDS_SETBUDDYINT ),      // 0x0002
   NAMESTR( UDS_ALIGNRIGHT ),       // 0x0004
   NAMESTR( UDS_ALIGNLEFT ),        // 0x0008
   NAMESTR( UDS_AUTOBUDDY ),        // 0x0010
   NAMESTR( UDS_ARROWKEYS ),        // 0x0020
   NAMESTR( UDS_HORZ ),             // 0x0040
   NAMESTR( UDS_NOTHOUSANDS ),      // 0x0080
   NAMESTR( UDS_HOTTRACK ),         // 0x0100
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID ProgresStyleFlags[] = {
   NAMESTR( PBS_SMOOTH ),           // 0x0001
   NAMESTR( PBS_VERTICAL ),         // 0x0004
   TABLE_LINK(CommonDlgStyleFlags) };
// begin_r_commctrl
//
static t_nameID SliderStyleFlags[] = { 
   NAMESTR( TBS_AUTOTICKS ),        // 0x0001
   NAMESTR( TBS_VERT ),             // 0x0002
   NAMESTR( TBS_TOP ),              // 0x0004
   NAMESTR( TBS_BOTH ),             // 0x0008
   NAMESTR( TBS_NOTICKS ),          // 0x0010
   NAMESTR( TBS_ENABLESELRANGE ),   // 0x0020
   NAMESTR( TBS_FIXEDLENGTH ),      // 0x0040
   NAMESTR( TBS_NOTHUMB ),          // 0x0080
   NAMESTR( TBS_TOOLTIPS ),         // 0x0100
   TABLE_LINK(CommonDlgStyleFlags) };
/*
   NAMESTR( CCS_LEFT ),             // CCS_VERT | CCS_TOP
   NAMESTR( CCS_RIGHT ),            // CCS_VERT | CCS_BOTTOM
   NAMESTR( CCS_NOMOVEX ),          // CCS_VERT | CCS_NOMOVEY
   NAMESTR( CCS_TOP ),              // 0x0001
   NAMESTR( CCS_NOMOVEY ),          // 0x0002
   NAMESTR( CCS_BOTTOM ),           // 0x0003
   NAMESTR( CCS_NORESIZE ),         // 0x0004
   NAMESTR( CCS_NOPARENTALIGN ),    // 0x0008
   NAMESTR( CCS_ADJUSTABLE ),       // 0x0020
   NAMESTR( CCS_NODIVIDER ),        // 0x0040
   NAMESTR( CCS_VERT ),             // 0x0080
*/
//
static t_nameID ListViewStyleFlags[] = {
   BITMASK( LVS_TYPEMASK ),         // 0x0003
   NAMESTR( LVS_ICON ),             // 0x0000
   NAMESTR( LVS_REPORT ),           // 0x0001
   NAMESTR( LVS_SMALLICON ),        // 0x0002
   NAMESTR( LVS_LIST ),             // 0x0003

   NAMESTR( LVS_SINGLESEL ),        // 0x0004
   NAMESTR( LVS_SHOWSELALWAYS ),    // 0x0008
   NAMESTR( LVS_SORTASCENDING ),    // 0x0010
   NAMESTR( LVS_SORTDESCENDING ),   // 0x0020
   NAMESTR( LVS_SHAREIMAGELISTS ),  // 0x0040
   NAMESTR( LVS_NOLABELWRAP ),      // 0x0080
   NAMESTR( LVS_AUTOARRANGE ),      // 0x0100
   NAMESTR( LVS_EDITLABELS ),       // 0x0200
   NAMESTR( LVS_OWNERDRAWFIXED ),   // 0x0400
   NAMESTR( LVS_ALIGNLEFT ),        // 0x0800
   NAMESTR( LVS_OWNERDATA ),        // 0x1000
   NAMESTR( LVS_NOSCROLL ),         // 0x2000
   NAMESTR( LVS_NOCOLUMNHEADER ),   // 0x4000
   NAMESTR( LVS_NOSORTHEADER ),     // 0x8000
   TABLE_LINK(CommonDlgStyleFlags) };
//
//
static t_nameID ListExStyleFlags[] = {
   NAMESTR( LVS_EX_GRIDLINES  ),    // 0x0001
   NAMESTR( LVS_EX_SUBITEMIMAGES ), // 0x0002
   NAMESTR( LVS_EX_CHECKBOXES ),    // 0x0004
   NAMESTR( LVS_EX_TRACKSELECT ),   // 0x0008
   NAMESTR( LVS_EX_HEADERDRAGDROP ),// 0x0010
   NAMESTR( LVS_EX_FULLROWSELECT ), // 0x0020  applies to report mode only
   NAMESTR( LVS_EX_ONECLICKACTIVATE),//0x0040
   NAMESTR( LVS_EX_TWOCLICKACTIVATE),//0x0080
   NAMESTR( LVS_EX_FLATSB ),        // 0x0100
   NAMESTR( LVS_EX_REGIONAL ),      // 0x0200
   NAMESTR( LVS_EX_INFOTIP ),       // 0x0400  listview does InfoTips for you
   NAMESTR( LVS_EX_UNDERLINEHOT ),  // 0x0800
   NAMESTR( LVS_EX_UNDERLINECOLD ), // 0x1000
   NAMESTR( LVS_EX_MULTIWORKAREAS ),// 0x2000
   TABLE_LINK(CommonDlgStyleFlags) };
//
/////////// tab
//
static t_nameID TabCtrlStyleFlags[] = {
   NAMESTR( TCS_SCROLLOPPOSITE ),   // 0x0001  assumes multiline tab
   NAMESTR( TCS_BOTTOM ),           // 0x0002
   NAMESTR( TCS_RIGHT ),            // 0x0002
   NAMESTR( TCS_MULTISELECT ),      // 0x0004  allow multi-select in button mode
   NAMESTR( TCS_FLATBUTTONS ),      // 0x0008
   NAMESTR( TCS_FORCEICONLEFT ),    // 0x0010
   NAMESTR( TCS_FORCELABELLEFT ),   // 0x0020
   NAMESTR( TCS_HOTTRACK ),         // 0x0040
   NAMESTR( TCS_VERTICAL ),         // 0x0080
   NAMESTR( TCS_TABS ),             // 0x0000
   NAMESTR( TCS_BUTTONS ),          // 0x0100
   NAMESTR( TCS_SINGLELINE ),       // 0x0000
   NAMESTR( TCS_MULTILINE ),        // 0x0200
   NAMESTR( TCS_RIGHTJUSTIFY ),     // 0x0000
   NAMESTR( TCS_FIXEDWIDTH ),       // 0x0400
   NAMESTR( TCS_RAGGEDRIGHT ),      // 0x0800
   NAMESTR( TCS_FOCUSONBUTTONDOWN), // 0x1000
   NAMESTR( TCS_OWNERDRAWFIXED ),   // 0x2000
   NAMESTR( TCS_TOOLTIPS ),         // 0x4000
   NAMESTR( TCS_FOCUSNEVER ),       // 0x8000
   TABLE_LINK(CommonDlgStyleFlags) };
//
static t_nameID TabCtrlStyleExFlags[] = {
// EX styles for use with TCM_SETEXTENDEDSTYLE
   NAMESTR( TCS_EX_FLATSEPARATORS ),// 0x00000001
   NAMESTR( TCS_EX_REGISTERDROP ),  // 0x00000002
   TABLE_LINK(WndStyleExFlags) };
//
/////Animate
//
static t_nameID AnimateStyleFlags[] = {
   NAMESTR( ACS_CENTER ),           // 0x0001
   NAMESTR( ACS_TRANSPARENT ),      // 0x0002
   NAMESTR( ACS_AUTOPLAY ),         // 0x0004
   NAMESTR( ACS_TIMER ),            // 0x0008 don't use threads... use timers
   TABLE_LINK(CommonDlgStyleFlags) };

#if (WINVER < 0x0500)
// define only in Windows NT 5.0
#define MNS_NOCHECK        0x80000000
#define MNS_MODELESS       0x40000000
#define MNS_DRAGDROP       0x20000000
#define MNS_AUTODISMISS    0x10000000
#define MNS_NOTIFYBYPOS    0x08000000
#define MNS_CHECKORBMP     0x04000000
#endif

static t_nameID MenuStyleFlags[] = {
   NAMESTR( MNS_NOCHECK ),          // 0x80000000
   NAMESTR( MNS_MODELESS ),         // 0x40000000
   NAMESTR( MNS_DRAGDROP ),         // 0x20000000
   NAMESTR( MNS_AUTODISMISS ),      // 0x10000000
   NAMESTR( MNS_NOTIFYBYPOS ),      // 0x08000000
   NAMESTR( MNS_CHECKORBMP ),       // 0x04000000

   BITMASK( 0x0000003 ),
   NAMESTR( MF_ENABLED ),           // 0x00000000
   NAMESTR( MF_GRAYED ),            // 0x00000001
   NAMESTR( MF_DISABLED ),          // 0x00000002

   BITMASK( MF_BITMAP ),            // 0x00000004
   NAMESTR( MF_STRING ),            // 0x00000000
   NAMESTR( MF_BITMAP ),            // 0x00000004

   NAMESTR( MF_CHECKED ),           // 0x00000008
   NAMESTR( MF_POPUP ),             // 0x00000010
   NAMESTR( MF_MENUBARBREAK ),      // 0x00000020
   NAMESTR( MF_MENUBREAK ),         // 0x00000040
   NAMESTR( MF_HILITE ),            // 0x00000080
   NAMESTR( MF_OWNERDRAW ),         // 0x00000100
   NAMESTR( MFT_RADIOCHECK ),       // 0x00000200
                                    // 0x00000400
   NAMESTR( MF_SEPARATOR ),         // 0x00000800
   NAMESTR( MF_DEFAULT ),           // 0x00001000
   NAMESTR( MF_SYSMENU ),           // 0x00002000
// NAMESTR( MF_HELP ),              // 0x00004000
   NAMESTR( MF_RIGHTJUSTIFY ),      // 0x00004000
   NAMESTR( MF_MOUSESELECT ),       // 0x00008000
   END_OF_TABLE            };
//
static t_nameID NullStyleFlags[] = { END_OF_TABLE };
/////////////////////////////////////////////////////////////////////////////
// Entree Style et StyleEx de DlgItem (Tooltips)

#define NOD_FRST_SUBITEM   1     // in the ReportView ListCtrl for CDialog
#define NOD_NB_SUBITEM     2     // resources  : Style and ExStyle columns
#define NOD_FLAGS(style, styleEx)               /*  subItem=> 1   subItem=> 2   */\
   static t_nameID *style##NID[NOD_NB_SUBITEM] = { style##Flags, styleEx##Flags };\
   CNodTbl style(style##NID, NOD_FRST_SUBITEM, NOD_NB_SUBITEM, NOD_SUBITEM)

NOD_FLAGS( WndStyle,       WndStyleEx );
NOD_FLAGS( DlgStyle,       WndStyleEx );
NOD_FLAGS( BarStyle,       BarStyleEx );
NOD_FLAGS( RebarStyle,     WndStyleEx );
NOD_FLAGS( TooltipStyle,   WndStyleEx );
NOD_FLAGS( StatusStyle,    WndStyleEx );
NOD_FLAGS( UpdownStyle,    WndStyleEx );
NOD_FLAGS( ProgresStyle,   WndStyleEx );
NOD_FLAGS( SliderStyle,    WndStyleEx );
NOD_FLAGS( ListViewStyle,  WndStyleEx );
NOD_FLAGS( TreeViewStyle,  WndStyleEx );
NOD_FLAGS( ComboBoxStyle,  WndStyleEx );
NOD_FLAGS( TabCtrlStyle,   TabCtrlStyleEx );
NOD_FLAGS( AnimateStyle,   WndStyleEx );
NOD_FLAGS( ButtonStyle,    WndStyleEx );
NOD_FLAGS( EditStyle,      WndStyleEx );
NOD_FLAGS( StaticStyle,    WndStyleEx );
NOD_FLAGS( ListBoxStyle,   WndStyleEx );
NOD_FLAGS( ScrollBarStyle, WndStyleEx );
NOD_FLAGS( CommonDlgStyle, WndStyleEx );
NOD_FLAGS( MenuStyle,      NullStyle  );
/////////////////////////////////////////////////////////////////////////////

LPCTSTR cntrl = "CONTROL";
static LPCTSTR RCButton[]   = {"PUSHBUTTON", "DEFPUSHBUTTON", NULL,
   NULL, NULL, "PUSHBUTTON", "PUSHBUTTON", "GROUPBOX", "UserButton", NULL};// 0x000A
static LPCTSTR RCEdit[]     = {"EDITTEXT"};
static LPCTSTR RCStatic[]   = {"LTEXT", "CTEXT", "RTEXT", "ICON"};   // 0x0004
static LPCTSTR RCListBox[]  = {"LISTBOX"};
static LPCTSTR RCScrollBar[]= {"SCROLLBAR"};
static LPCTSTR RCComboBox[] = {"COMBOBOX"};
static LPCTSTR RCCntrl[]    = { cntrl };

#define STYLECLASS(style, name, std, image) {style, name, std, image, 1, RCCntrl}
#define STYLESTDCLASS(style, name, std, image, rct) \
   {style, name, std, image, sizeof(rct)/4, rct}

static t_ClassStyle DlgStdClassStyle[] = {
   // STYLESTDCLASS(style, name, std, image, rct)
 STYLESTDCLASS(&ButtonStyle,   "Button",                 0,RI_BUTTON, RCButton),  // 0x0080
 STYLESTDCLASS(&EditStyle,     "Edit",WS_TABSTOP|WS_BORDER,RI_EDIT,   RCEdit),    // 0x0081
 STYLESTDCLASS(&StaticStyle,   "Static",          WS_GROUP,RI_STATIC, RCStatic),  // 0x0082
 STYLESTDCLASS(&ListBoxStyle,  "Listbox",                0,RI_LISTBOX,RCListBox), // 0x0083
 STYLESTDCLASS(&ScrollBarStyle,"Scrollbar",              0,RI_HSCROLL,RCScrollBar),//0x0084
 STYLESTDCLASS(&ComboBoxStyle, "Combobox",               0,RI_COMBOX, RCComboBox),// 0x0085
};

static t_ClassStyle DlgClassStyle[] = {
   // STYLECLASS(style, name, std, image)
      //    NOTOOLBAR    Customizable bitmap-button toolbar control.
   STYLECLASS(&BarStyle,       "ToolbarWindow32",   0, RI_TOOLBAR),
   STYLECLASS(&BarStyle,       "ToolbarWindow",     0, RI_TOOLBAR),

   STYLECLASS(&RebarStyle,     "ReBarWindow32",     0, 0),
   STYLECLASS(&RebarStyle,     "ReBarWindow",       0, 0),

   STYLECLASS(&TooltipStyle,   "tooltips_class32",  0, 0),
   STYLECLASS(&TooltipStyle,   "tooltips_class",    0, 0),
//    NOSTATUSBAR  Status bar control.
   STYLECLASS(&StatusStyle,    "msctls_statusbar32",0, 0),
   STYLECLASS(&StatusStyle,    "msctls_statusbar",  0, 0),
//    NOUPDOWN     Up and Down arrow increment/decrement control.
   STYLECLASS(&UpdownStyle,    "msctls_updown32",   0, RI_SPIN),
   STYLECLASS(&UpdownStyle,    "msctls_updown",     0, RI_SPIN),
//    NOPROGRESS   Progress gas gauge.
   STYLECLASS(&ProgresStyle,   "msctls_progress32", 0, RI_PROGRESS),
   STYLECLASS(&ProgresStyle,   "msctls_progress",   0, RI_PROGRESS),
//    NOTRACKBAR   Customizable column-width tracking control.
   STYLECLASS(&SliderStyle,    "msctls_trackbar32", 0, RI_SLIDER),
   STYLECLASS(&SliderStyle,    "msctls_trackbar",   0, RI_SLIDER),
//    NOLISTVIEW   ListView control.
   STYLECLASS(&ListViewStyle,  "SysListView32",     0, RI_REPORT),
   STYLECLASS(&ListViewStyle,  "SysListView",       0, RI_REPORT),
//    NOTREEVIEW   TreeView control.
   STYLECLASS(&TreeViewStyle,  "SysTreeView32",     0, RI_TREE),
   STYLECLASS(&TreeViewStyle,  "SysTreeView",       0, RI_TREE),

   STYLECLASS(&ComboBoxStyle,  "ComboBoxEx32",      0, RI_COMBOX),
   STYLECLASS(&ComboBoxStyle,  "ComboBoxEx",        0, RI_COMBOX),
//    NOTABCONTROL Tab control.
   STYLECLASS(&TabCtrlStyle,   "SysTabControl32",   0, RI_TABCTRL),
   STYLECLASS(&TabCtrlStyle,   "SysTabControl",     0, RI_TABCTRL),
//    NOANIMATE    Animate control.
   STYLECLASS(&AnimateStyle,   "SysAnimate32",      0, RI_ANI),
   STYLECLASS(&AnimateStyle,   "SysAnimate",        0, RI_ANI),

   STYLECLASS(&CommonDlgStyle, NULL,                0, 0)
};

t_ClassStyle* ClassStyle(LPCTSTR name)
{
   static t_ClassStyle out;
   DWORD ix = DWORD(name) - 0x0080;

   if (ix < 6) {
      // for name=> 0x0000 0080 .. 0x0000 0085
      out = DlgStdClassStyle[ix];
   } else {
      if (HIWORD(name) == 0) name = "??";
      for (t_ClassStyle *pCs = DlgClassStyle; pCs->name; pCs++)
         if (strcmp(name, pCs->name) == 0) break;

      out = *pCs;
      out.name = name;
   }
   return &out;
}

LPCTSTR ClassName(t_ClassStyle *pClass)
{
   return (pClass->name)? pClass->name: "";
}

LPCTSTR ClassTypeName(t_ClassStyle *pClass, DWORD dwStyle, BOOL lst)
{
   LPCTSTR pRCType = NULL;
   if (lst && pClass->ne > 1) {
      // exception des boutons pour l'affichage en liste
      static t_ClassStyle BtnClassStyle[] = {
       STYLECLASS(&ButtonStyle,"PushButton",BS_PUSHBUTTON|WS_TABSTOP,   RI_BUTTON), // 00
       STYLECLASS(&ButtonStyle,"PushButton",BS_DEFPUSHBUTTON|WS_TABSTOP,RI_BUTTON), // 01
       STYLECLASS(&ButtonStyle,"CheckBox",  BS_CHECKBOX|WS_TABSTOP,     RI_CHECK),  // 02
       STYLECLASS(&ButtonStyle,"CheckBox",  BS_AUTOCHECKBOX,            RI_CHECK),  // 03
       STYLECLASS(&ButtonStyle,"Radio",     BS_RADIOBUTTON,             RI_RADIO),  // 04
       STYLECLASS(&ButtonStyle,"PushButton",BS_3STATE,                  RI_BUTTON), // 05
       STYLECLASS(&ButtonStyle,"PushButton",BS_AUTO3STATE,              RI_BUTTON), // 06
       STYLECLASS(&ButtonStyle,"GroupeBox", BS_GROUPBOX,                RI_GROUP),  // 07
       STYLECLASS(&ButtonStyle,"UserButton",BS_USERBUTTON,              RI_BUTTON), // 08
       STYLECLASS(&ButtonStyle,"Radio",     BS_AUTORADIOBUTTON,         RI_RADIO),  // 09
       STYLECLASS(&ButtonStyle,"Button",    0,                          RI_BUTTON)
                                       };
      static t_ClassStyle StaClassStyle[] = {
       STYLECLASS(&StaticStyle,"LTEXT",     SS_LEFT|WS_GROUP,           RI_STATIC), // 00
       STYLECLASS(&StaticStyle,"CTEXT",     SS_CENTER|WS_GROUP,         RI_STATIC), // 01
       STYLECLASS(&StaticStyle,"RTEXT",     SS_RIGHT|WS_GROUP,          RI_STATIC), // 02
       STYLECLASS(&StaticStyle,"ICON",      SS_ICON,                    RI_ICON  ), // 03
       STYLECLASS(&StaticStyle,"BRECT",     SS_BLACKRECT,               RI_RECT  ), // 04
       STYLECLASS(&StaticStyle,"GRECT",     SS_GRAYRECT,                RI_RECT  ), // 05
       STYLECLASS(&StaticStyle,"WRECT",     SS_WHITERECT,               RI_RECT  ), // 06
       STYLECLASS(&StaticStyle,"BFRAME",    SS_BLACKFRAME,              RI_RECT  ), // 07
       STYLECLASS(&StaticStyle,"GFRAME",    SS_GRAYFRAME,               RI_RECT  ), // 08
       STYLECLASS(&StaticStyle,"WFRAME",    SS_WHITEFRAME,              RI_RECT  ), // 09
	    STYLECLASS(&StaticStyle,"FRAME",     SS_USERITEM,                RI_RECT  ), // 0A
	    STYLECLASS(&StaticStyle,"FRAME",     SS_SIMPLE,                  RI_RECT  ), // 0B
	    STYLECLASS(&StaticStyle,"FRAME",     SS_LEFTNOWORDWRAP,          RI_RECT  ), // 0C
	    STYLECLASS(&StaticStyle,"FRAME",     SS_OWNERDRAW,               RI_RECT  ), // 0D
	    STYLECLASS(&StaticStyle,"BITMAP",    SS_BITMAP,                  RI_BITMAP), // 0E
	    STYLECLASS(&StaticStyle,"EMETHA",    SS_ENHMETAFILE,             RI_BITMAP), // 0F
	    STYLECLASS(&StaticStyle,"FRAME",     SS_ETCHEDHORZ,              RI_RECT  ), // 10
	    STYLECLASS(&StaticStyle,"FRAME",     SS_ETCHEDVERT,              RI_RECT  ), // 11
	    STYLECLASS(&StaticStyle,"FRAME",     SS_ETCHEDFRAME,             RI_RECT  ), // 12
       STYLECLASS(&ButtonStyle,"FRAME",     0,                          RI_STATIC)
                                       };

      t_ClassStyle *pCs = (pClass->pStyle == &ButtonStyle)? BtnClassStyle: StaClassStyle;
      for ( ; pCs->std; pCs++)
         if (pCs->std == (dwStyle & (pCs->std|SS_TYPEMASK))) break;

      pClass->image = pCs->image;
      pRCType = pCs->name;
   } else {
      UINT ix = dwStyle & SS_TYPEMASK;
      if (1 == pClass->ne) {
         pRCType = pClass->RCType[0];
      } else if (ix < pClass->ne) {
         pRCType = pClass->RCType[ix];
         // supprime les styles implicites definis par la classType
         if (pRCType) pClass->std |= SS_TYPEMASK|WS_TABSTOP;
      }
   }
   if (pRCType == NULL) pRCType = cntrl;
   return pRCType;
}

CString GetStyleFlags(DWORD dwAttr,          // Flags to decod
                      t_ClassStyle*pClass,   // Out of ClassStyle
                      UINT ex,               // 0=> style, 1=> styleEx
                      DWORD dwDef)           // Not flags
{
   if (ex == NOD_STYLE) dwAttr &= ~pClass->std;
   t_nameID *pTbl= pClass->pStyle->GetFlagTbl(ex);
   const UINT df = DFF_DNULL | DFF_SORT | DFF_ALFA;
   CString str   = DecodFlags( dwAttr & ~dwDef, pTbl, " | ", df );
   CString notStr= DecodFlags( dwDef & ~dwAttr, pTbl, " ", df );

   if ( !notStr.IsEmpty() ) {
      int n = notStr.GetLength();
      while ( --n > 0 ) if (notStr.GetAt(n) == ' ') notStr.Insert(n, "| NOT ");
      str = ( str.IsEmpty() )? "NOT " + notStr: str + " | NOT " + notStr;
   }
   return str;
}
/////////////////////////////////////////////////////////////////////////////

// WAVE form wFormatTag IDs
t_nameID waveFormatTagID[] = {
   BITMASK( 0xFFFF ),
	NAMESTR(WAVE_FORMAT_UNKNOWN),//              0x0000    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_PCM),//                  0x0001
	NAMESTR(WAVE_FORMAT_ADPCM),//                0x0002    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_IEEE_FLOAT),//           0x0003    Microsoft Corporation
                                                      //  IEEE754: range (+1, -1]
                                                      //  32-bit/64-bit format as defined by
                                                      //  MSVC++ float/double type
	NAMESTR(WAVE_FORMAT_IBM_CVSD),//             0x0005    IBM Corporation
	NAMESTR(WAVE_FORMAT_ALAW),//                 0x0006    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_MULAW),//                0x0007    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_OKI_ADPCM),//            0x0010    OKI
	NAMESTR(WAVE_FORMAT_DVI_ADPCM),//            0x0011    Intel Corporation
	NAMESTR(WAVE_FORMAT_IMA_ADPCM),//  (WAVE_FORMAT_DVI_ADPCM)  Intel Corporation
	NAMESTR(WAVE_FORMAT_MEDIASPACE_ADPCM),//     0x0012    Videologic
	NAMESTR(WAVE_FORMAT_SIERRA_ADPCM),//         0x0013    Sierra Semiconductor Corp
	NAMESTR(WAVE_FORMAT_G723_ADPCM),//           0x0014    Antex Electronics Corporation
	NAMESTR(WAVE_FORMAT_DIGISTD),//              0x0015    DSP Solutions, Inc.
	NAMESTR(WAVE_FORMAT_DIGIFIX),//              0x0016    DSP Solutions, Inc.
	NAMESTR(WAVE_FORMAT_DIALOGIC_OKI_ADPCM),//   0x0017    Dialogic Corporation
	NAMESTR(WAVE_FORMAT_MEDIAVISION_ADPCM),//    0x0018    Media Vision, Inc.
	NAMESTR(WAVE_FORMAT_YAMAHA_ADPCM),//         0x0020    Yamaha Corporation of America
	NAMESTR(WAVE_FORMAT_SONARC),//               0x0021    Speech Compression
	NAMESTR(WAVE_FORMAT_DSPGROUP_TRUESPEECH),//  0x0022    DSP Group, Inc
	NAMESTR(WAVE_FORMAT_ECHOSC1),//              0x0023    Echo Speech Corporation
	NAMESTR(WAVE_FORMAT_AUDIOFILE_AF36),//       0x0024
	NAMESTR(WAVE_FORMAT_APTX),//                 0x0025    Audio Processing Technology
	NAMESTR(WAVE_FORMAT_AUDIOFILE_AF10),//       0x0026
	NAMESTR(WAVE_FORMAT_DOLBY_AC2),//            0x0030    Dolby Laboratories
	NAMESTR(WAVE_FORMAT_GSM610),//               0x0031    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_MSNAUDIO),//             0x0032    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_ANTEX_ADPCME),//         0x0033    Antex Electronics Corporation
	NAMESTR(WAVE_FORMAT_CONTROL_RES_VQLPC),//    0x0034    Control Resources Limited
	NAMESTR(WAVE_FORMAT_DIGIREAL),//             0x0035    DSP Solutions, Inc.
	NAMESTR(WAVE_FORMAT_DIGIADPCM),//            0x0036    DSP Solutions, Inc.
	NAMESTR(WAVE_FORMAT_CONTROL_RES_CR10),//     0x0037    Control Resources Limited
	NAMESTR(WAVE_FORMAT_NMS_VBXADPCM),//         0x0038    Natural MicroSystems
	NAMESTR(WAVE_FORMAT_CS_IMAADPCM),//          0x0039    Crystal Semiconductor IMA ADPCM
	NAMESTR(WAVE_FORMAT_ECHOSC3),//              0x003A    Echo Speech Corporation
	NAMESTR(WAVE_FORMAT_ROCKWELL_ADPCM),//       0x003B    Rockwell International
	NAMESTR(WAVE_FORMAT_ROCKWELL_DIGITALK),//    0x003C    Rockwell International
	NAMESTR(WAVE_FORMAT_XEBEC),//                0x003D    Xebec Multimedia Solutions Limited
	NAMESTR(WAVE_FORMAT_G721_ADPCM),//           0x0040    Antex Electronics Corporation
	NAMESTR(WAVE_FORMAT_G728_CELP),//            0x0041    Antex Electronics Corporation
	NAMESTR(WAVE_FORMAT_MPEG),//                 0x0050    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_MPEGLAYER3),//           0x0055    ISO/MPEG Layer3 Format Tag
	NAMESTR(WAVE_FORMAT_CIRRUS),//               0x0060    Cirrus Logic
	NAMESTR(WAVE_FORMAT_ESPCM),//                0x0061    ESS Technology
	NAMESTR(WAVE_FORMAT_VOXWARE),//              0x0062    Voxware Inc
//	NAMESTR(WAVEFORMAT_CANOPUS_ATRAC),//         0x0063    Canopus, co., Ltd.
	NAMESTR(WAVE_FORMAT_G726_ADPCM),//           0x0064    APICOM
	NAMESTR(WAVE_FORMAT_G722_ADPCM),//           0x0065    APICOM
//	NAMESTR(WAVE_FORMAT_DSAT),//                 0x0066    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_DSAT_DISPLAY),//         0x0067    Microsoft Corporation
	NAMESTR(WAVE_FORMAT_SOFTSOUND),//            0x0080    Softsound, Ltd.
	NAMESTR(WAVE_FORMAT_RHETOREX_ADPCM),//       0x0100    Rhetorex Inc.
	NAMESTR(WAVE_FORMAT_CREATIVE_ADPCM),//       0x0200    Creative Labs, Inc.
	NAMESTR(WAVE_FORMAT_CREATIVE_FASTSPEECH8),// 0x0202    Creative Labs, Inc.
	NAMESTR(WAVE_FORMAT_CREATIVE_FASTSPEECH10),//0x0203    Creative Labs, Inc.
	NAMESTR(WAVE_FORMAT_QUARTERDECK),//          0x0220    Quarterdeck Corporation
	NAMESTR(WAVE_FORMAT_FM_TOWNS_SND),//         0x0300    Fujitsu Corp.
	NAMESTR(WAVE_FORMAT_BTV_DIGITAL),//          0x0400    Brooktree Corporation
	NAMESTR(WAVE_FORMAT_OLIGSM),//               0x1000    Ing C. Olivetti & C., S.p.A.
	NAMESTR(WAVE_FORMAT_OLIADPCM),//             0x1001    Ing C. Olivetti & C., S.p.A.
	NAMESTR(WAVE_FORMAT_OLICELP),//              0x1002    Ing C. Olivetti & C., S.p.A.
	NAMESTR(WAVE_FORMAT_OLISBC),//               0x1003    Ing C. Olivetti & C., S.p.A.
	NAMESTR(WAVE_FORMAT_OLIOPR),//               0x1004    Ing C. Olivetti & C., S.p.A.
	NAMESTR(WAVE_FORMAT_LH_CODEC),//             0x1100    Lernout & Hauspie  */
	NAMESTR(WAVE_FORMAT_NORRIS),//               0x1400    Norris Communications, Inc.  */
   END_OF_TABLE };

/* constants for the biCompression field */
t_nameID biCompression[] = {
   BITMASK( 0xFFFF ),
	NAMESTR(BI_RGB),//        0L
	NAMESTR(BI_RLE8),//       1L
	NAMESTR(BI_RLE4),//       2L
	NAMESTR(BI_BITFIELDS),//  3L
   END_OF_TABLE };

CString GetBitfieldsAsString( DWORD dwStyle, const t_nameID* flgtbl )
{
   return DecodFlags(dwStyle, flgtbl, " | ", DFF_DNULL|DFF_SORT|DFF_ALFA);
}

CString GetType( DWORD dwStyle, const t_namImID *flgtbl, int &image )
{  
   while ( flgtbl->title ) {

      DWORD dwsave = dwStyle & flgtbl->id;
  
      if ( dwsave == flgtbl->id ) {
         image = flgtbl->image;
         return CString(flgtbl->title);
      }
      flgtbl++;
   }
   return "";
}

void Tab(CString &text, int tab)
{
   int beg = text.ReverseFind(TCHAR('\n'));
   int len = text.GetLength();
   if (beg > 0) tab += beg + 1;
   tab -= len;
   if (tab > 0) text += CString(TCHAR(' '), tab);
   else text += " ";
}

CString StrFilter(const CString &str, BOOL quote)
{
   int len = str.GetLength();
   CString buffer;
   TCHAR tchar;
   for (int i = 0; i < len; i++) {
      switch (tchar = str.GetAt(i)) {
      case '\n' : buffer += "\\n"; break;
      case '\r' : buffer += "\\r"; break;
      case '\t' : buffer += "\\t"; break;
      case '\v' : buffer += "\\v"; break;
      case '\b' : buffer += "\\b"; break;
      case '\f' : buffer += "\\f"; break;
      case '\a' : buffer += "\\a"; break;
      case '\\' : buffer += "\\" ; break;
      case '\"' : buffer += "\\\"", quote = FALSE ; break;
      case  0   : buffer += "\\0"; break;
      default: buffer += tchar;
      }
   }
   if (quote) return "\"" + buffer + "\"";
   else return buffer;
}

CString PathRemoveExt(const CString &path)
{
   int n = path.ReverseFind(TCHAR('.'));
   return (n > 0)? path.Left(n) : path;
}

CString PathRenameExt(const CString &path, const CString &ext)
{
   if (ext.GetAt(0) == '.')  return PathRemoveExt(path) + ext;
   else return PathRemoveExt(path) + "." + ext;
}

CString PathGetExt(const CString &path)
{
   int n = path.ReverseFind(TCHAR('.'));
   return (n > 0)? path.Mid(n+1) : "";
}

CString PathReduce(const CString &path, const CString &pathRef)
{
   int n, pos = 0;
   for (;;) {
      n = path.Find(TCHAR('\\'), pos);
      if (n < 0) break;
      n++;
      if ( path.Left(n).CompareNoCase(pathRef.Left(n)) != 0 ) break;
      pos = n;
   }
   return path.Mid(pos);
}

CString ExtractString(PWORD &pWord, int &size)
{
   CString buffer;
   int cnt = *pWord++;
   size -= cnt + 1;
   while ( --cnt >= 0 ) {
      TCHAR tchar = (TCHAR)*pWord++;
      switch (tchar) {
      case '\n' : buffer += "\\n"; break;
      case '\r' : buffer += "\\r"; break;
      case '\t' : buffer += "\\t"; break;
      case '\v' : buffer += "\\v"; break;
      case '\b' : buffer += "\\b"; break;
      case '\f' : buffer += "\\f"; break;
      case '\a' : buffer += "\\a"; break;
      case '\\' : buffer += "\\\\";break;
      case  0   : buffer += "\\0"; break;
      default: buffer += tchar;
      }
   }
   return buffer;
}

CString ExtractStr(LPTSTR lpStr)
{
   CString buffer;
   TCHAR tchar;
   while (tchar = *lpStr++) {
      switch (tchar) {
      case '\n' : buffer += "\\n"; break;
      case '\r' : buffer += "\\r"; break;
      case '\t' : buffer += "\\t"; break;
      case '\v' : buffer += "\\v"; break;
      case '\b' : buffer += "\\b"; break;
      case '\f' : buffer += "\\f"; break;
      case '\a' : buffer += "\\a"; break;
      case '\\' : buffer += "\\\\"; break;
      default: buffer += tchar;
      }
   }
   return buffer;
}

BOOL ExtractTrueStr(CString &str, BOOL True, BOOL quote)
{
   if (True) {
      str.Replace("\\n", "\n");
      str.Replace("\\r", "\r");
      str.Replace("\\t", "\t");
      str.Replace("\\v", "\v");
      str.Replace("\\b", "\b");
      str.Replace("\\f", "\f");
      str.Replace("\\a", "\a");
      str.Replace("\\\\","\\");
      str.Replace("\\0","\000");
   }
   if (quote) {
      str.Insert(0, '\"');
      str.Insert(0x7FFF, '\"');
   }
   return ! str.IsEmpty();
}

CString LoadString(UINT uID, HINSTANCE hIns)
{
   TCHAR buffer[128];
   if (hIns == NULL) hIns = ::AfxGetResourceHandle();
   ::LoadString(hIns, uID, buffer, 128);
   return CString(buffer);
}

static t_nameID idTbl[] = {
   NAMESTR( IDOK     ),
   NAMESTR( IDCANCEL ),
   NAMESTR( IDABORT  ),
   NAMESTR( IDRETRY  ),
   NAMESTR( IDIGNORE ),
   NAMESTR( IDYES    ),
   NAMESTR( IDNO     ),
   NAMESTR( IDCLOSE  ),
   NAMESTR( IDHELP   ),
   0xFFFF, "IDC_STATIC",
   0,      "ID_SEPARATOR",
   0, NULL
};

CString ReadBfText(LPSTR &ppStr, LPCTSTR pEnd)
{
   CString line;
   while (ppStr < pEnd) {
      int car = *ppStr++;
      if (isprint(car)) line += (TCHAR)car;
      else if (car == '\t') line += ' ';
      else if (car == '\n') break;
   }
   return line;
}
/////////////////////////////////////////////////////////////////////////////
// Class CPosObj

LPCTSTR CPosObj::GetObj(const CPoint& pos)
{
   POSITION obj = drawObj.GetHeadPosition();
   while (obj) {
      CPosID& pid = drawObj.GetNext(obj);
      if (pid.pos.PtInRect(pos)) return lpSel = pid.id;
   }
   return lpSel = NULL;
}

BOOL CPosObj::Select(LPCTSTR id)
{
   POSITION obj = drawObj.GetHeadPosition();
   while (obj) {
      CPosID& pid = drawObj.GetNext(obj);
      if (ResIdCompare(pid.id, id)) {
         lpSel = pid.id;
         return TRUE;
      }
   }
   return FALSE;
}

LPCTSTR CPosObj::GetSelect(BOOL frst)
{
   POSITION obj = drawObj.GetHeadPosition();
   while (obj) {
      CPosID& pid = drawObj.GetNext(obj);
      if (pid.id == lpSel) return lpSel;
   }
   lpSel = NULL;
   if (frst && !drawObj.IsEmpty()) return drawObj.GetHead().id;
   else return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// Class CNodTbl

CNodTbl::CNodTbl(t_nameID *namID, BYTE first, BYTE nb, BYTE flg)
{
   m_pName = namID;
   m_frst = first;
   m_cnt = nb;
   m_next = flg & 2;
   m_tag = 1 + (flg & 1);
// NodTRACE("CNodTbl::");
}

CNodTbl::CNodTbl(t_nameID **namID, BYTE first, BYTE nb, BYTE flg)
{
   m_TName = namID;
   m_frst = first;
   m_cnt = nb;
   m_next = flg & 2;
   m_tag = 3 + (flg & 1);
// NodTRACE("CNodTbl::");
}

CNodTbl::CNodTbl(CNodTbl *node, BYTE first, BYTE nb, BYTE flg)
{
   m_pNod = node;
   m_frst = first;
   m_cnt = nb;
   m_next = flg & 2;
   m_tag = 5 + (flg & 1);
// NodTRACE("CNodTbl::");
}

CNodTbl::CNodTbl(CNodTbl **node, BYTE first, BYTE nb, BYTE flg)
{
   m_TNod = node;
   m_frst = first;
   m_cnt = nb;
   m_next = flg & 2;
   m_tag = 7 + (flg & 1);
// NodTRACE("CNodTbl::");
}

void CNodTbl::EntryDelete()
{
   short cnt = m_cnt;
   switch (m_tag) {
   case 4: while (--cnt >= 0) m_TName[cnt] = NULL; break;
   case 8: while (--cnt >= 0) m_TNod[cnt]  = NULL; break;
   }
}

void CNodTbl::SetEntry(t_nameID *pName, int idx)
{
   if (m_tag == 4 && UINT(idx) < m_cnt) m_TName[idx] = pName;
}

void CNodTbl::SetEntry(CNodTbl *pNod, int idx)
{
   if (m_tag == 8 && UINT(idx) < m_cnt) m_TNod[idx] = pNod;
}

CString CNodTbl::GetSring(CListCtrl *pList, int item, int subItem)
{
   CNodTbl *pNod = this;

   while (pNod) {
      BYTE id = BYTE((pNod->m_tag & 1)? subItem: item) - pNod->m_frst;

//    TRACE("CNodTbl::GetSring tag=%d frst=%d cnt=%d id=%d\n",
//         pNod->m_tag, pNod->m_frst, pNod->m_cnt, id);

      if (id < pNod->m_cnt) {
         DWORD dwFlags;

         switch (pNod->m_tag) {

         case 1:
         case 2: return pNod->m_pName->title;

         case 3:
         case 4:
            dwFlags = strtoul(pList->GetItemText(item, subItem), NULL, 16);
            return DecodFlags(dwFlags, pNod->m_TName[id], "\r\n",
                                             DFF_DNULL|DFF_REST|DFF_SORT);

         case 5:
         case 6: pNod = pNod->m_pNod + id; break;

         case 7:
         case 8: pNod = pNod->m_TNod[id];  break;

         default: return "Table tag error";
         }
      } else if (pNod->m_next) {
         pNod++;
      } else break;
   }
   return "";
}
/*
void CNodTbl::NodTRACE(CString str)
{ TRACE("%s tag=%d frst=%d cnt=%d suit=%d\n", str, m_tag, m_frst, m_cnt, m_next); }
*/
