// ResUtilities.h: interface for the CResUtilities class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESUTILITIES_H__B7E56224_5496_44FA_BD99_F9DFFA9C3649__INCLUDED_)
#define AFX_RESUTILITIES_H__B7E56224_5496_44FA_BD99_F9DFFA9C3649__INCLUDED_

#include <afxtempl.h>
#include <mmsystem.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct {
   LPSTR   lpId;     // resource id RT_XXX (code dans resource)
   WORD    wId;      // resource id RT_XXX (code reduit utilse dans ce programme)
//   WORD    image;    // no d'images
   DWORD   type;     // type de symboles a afficher et icon
   WORD    vType;
   WORD    vInst;
   LPCTSTR text;     // titre affiche correspondant
   LPCTSTR fname;    // nom du fichier par defaut
   LPCTSTR filter;   // extensions possibles pour ce nom
} t_ResID;

#define RID1(id, image, type, vType, vInst, text) \
          {id, (WORD)id, image|type, vType|VLS, vInst|VLS, text, NULL, NULL}
#define RID2(id, image, type, vType, vInst, text, ext, filter) \
          {id, (WORD)id, image|type, vType|VLS|WRT, vInst|VLS, text, ext, filter}
#define RID3(id, wid, image, type, vType, vInst, text, ext, filter) \
               {id, wid, image|type, vType|VLS|WRT, vInst|VLS, text, ext, filter}

#define RT_UNDEF (LPTSTR)0
#define RT_XML   (LPTSTR)24
#define RT_USERDEFINE (LPTSTR)0x0000FFFF
const WORD rt_txt    = 0xFFF0;
const WORD rt_avi    = 0xFFF1;
const WORD rt_wav    = 0xFFF2;
const WORD rt_mid    = 0xFFF3;
const WORD rt_rtf    = 0xFFF4;
const WORD rt_reg    = 0xFFF5;
const WORD rt_bmp    = 0xFFF6;
const WORD rt_bmx    = 0xFFF7;   // undefine image, IPicture compatible
const WORD rt_icon   = 0xFFF8;
const WORD rt_jpg    = 0xFFF9;
const WORD rt_gif    = 0xFFFA;
const WORD rt_wmf    = 0xFFFB;   // METAFILE
const WORD rt_emf    = 0xFFFC;   // ENHMETAFILE

typedef enum { 
   VRS = 0x001,   // Affichage de la fenetre CDrawView
   VDT = 0x002,   // Affichage de la fenetre CEditView a la place CDrawView
   VLS = 0x004,   // Affichage de la fenetre CReportView
   ICO = 0x080,   // Affiche icon ou cursor
   EDT = 0x010,   // Copier/coller Edition (Clipboard)
   WRT = 0x020,   // Sortie sur fichier
   PRN = 0x040,   // Imprimer la page de droite
   DAT = 0x080,   // Recherche le type d'apres les donnees
   NC  = 0
};
///////////////////////////////////////////////////////////////////////////////
//
class CNodTbl;

typedef struct {
   DWORD id;
   LPCTSTR title;
} t_nameID;

#define NAMESTR(id)     {WORD(id),  #id}
#define BITMASK(id)     {id,  (LPCTSTR)1}
#define TABLE_LINK(tbl) {DWORD(tbl), NULL}
#define END_OF_TABLE    { 0,  NULL }

typedef enum { DFF_REST=0x01, DFF_DNULL=0x02, DFF_SORT=0x04, DFF_ALFA=0x08, DFF_NLINK=0x10 };

extern void BreakLine(CString &text, int lineLen, int pos = 0, int tab = 0);
extern CString DecodFlags(DWORD dwAttr, const t_nameID *pTbl,
                          const CString &sep = "\r\n", UINT flgs = 0);

typedef struct {
   DWORD id;
   int  image;
   LPCTSTR title;
} t_namImID;

typedef struct {
   CNodTbl  *pStyle;
   LPCTSTR  name;
   DWORD    std;
   WORD     image;
   WORD     ne;
   LPCTSTR  *RCType;
} t_ClassStyle;

typedef enum { NOD_ITEM=0x01, NOD_SUBITEM=0x00, NOD_SUITE=0x02 };
typedef enum { NOD_STYLE=0, NOD_EXSTYLE=1 };

class CNodTbl
{
public:
   CNodTbl(t_nameID namID[],BYTE first, BYTE nb, BYTE flg =NOD_ITEM);
   CNodTbl(t_nameID*namID[],BYTE first, BYTE nb, BYTE flg =NOD_ITEM);
   CNodTbl(CNodTbl node[],  BYTE first, BYTE nb, BYTE flg =NOD_ITEM);
   CNodTbl(CNodTbl *node[], BYTE first, BYTE nb, BYTE flg =NOD_ITEM);

   ~CNodTbl() {}

   void SetFirst(BYTE frst) { m_frst = frst; }
   void EntryDelete();
   void SetEntry(t_nameID *pName, int idx);
   void SetEntry(CNodTbl *pNod, int idx);

   CString GetSring(CListCtrl *pList, int item, int subItem);
   void NodTRACE(CString str);

   CString Decod(CString &attr, t_nameID *pTbl);

   inline t_nameID *GetFlagTbl() { return m_pName; }
   inline t_nameID *GetFlagTbl(UINT n) { return m_TName[n]; }

protected:
   BYTE m_tag, m_next, m_frst, m_cnt;
   union {
      t_nameID *m_pName;
      CNodTbl *m_pNod;
      t_nameID **m_TName;
      CNodTbl **m_TNod;
   };
};
//
extern CNodTbl DlgStyle, MenuStyle;
extern t_nameID DlgStyleFlags[];
extern t_nameID WndStyleExFlags[];
//
extern LPCTSTR cntrl;
extern t_ClassStyle *ClassStyle(LPCTSTR name);
extern LPCTSTR ClassName(t_ClassStyle *pClass);
extern LPCTSTR ClassTypeName(t_ClassStyle *pClass, DWORD dwStyle, BOOL list);
extern CString GetStyleFlags(DWORD dwAttr, t_ClassStyle *pClass, UINT ex, DWORD dwDef=0);
///////////////////////////////////////////////////////////////////////////////

extern t_nameID waveFormatTagID[];
extern t_nameID biCompression[];

extern PVOID RawData(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType, PDWORD size = NULL);

extern CString FormatDeci( DWORD dw, LPCTSTR lpMsg = NULL );
extern CString FormatHexa( DWORD dw, LPCTSTR lpMsg = NULL );
extern CString FormatFloat( double df, LPCTSTR lpMsg = NULL );
extern CString GetType( DWORD dwStyle, const t_namImID* flgtbl, int& image );
extern CString ExtractString( PWORD& pWord, int& size );
extern CString ExtractStr( LPTSTR lpStr );
extern BOOL ExtractTrueStr(CString &str, BOOL True, BOOL Quote);
extern BOOL CompareSign( PBYTE pRes, LPCTSTR pMod );
extern DWORD GetFileNameExt( LPCTSTR fileName );
//
extern CString GetBitfieldsAsString( DWORD dwStyle, const t_nameID* flgtbl );
extern CString StrFilter( const CString& str, BOOL quote );
extern CString PathReduce( const CString& path, const CString& pathRef );
extern void Tab( CString& text, int tab );
extern CString FourCCtoStr( DWORD fcc );
extern CString ReadBfText( LPSTR& ppStr, LPCTSTR pEnd );


///////////////////////////////////////////////////////////////////////////////
// CResTools

extern void UniqueResIdInitialise();
extern LPCTSTR UniqueResId(LPCTSTR resId);
extern BOOL IsUniqueResId(LPCTSTR resId);

extern LPCTSTR ReadResId(PWORD &pWord);
extern void WriteResId(PWORD& pWord, LPCTSTR lpId);
extern DWORD LenResId(LPCTSTR lpId);
extern CString GetResStr(UINT resId);
extern CString GetResStr(LPCTSTR lpId, BOOL quote = TRUE);

extern CString ReadString(PWORD &pWord);
extern void SkipResId(PWORD &pWord);

extern t_ResID *GetResEntry(LPCTSTR resType);
extern t_ResID *GetResEntryEx(LPCTSTR resType);
extern BOOL ResIdCompare(LPCTSTR lpRes, LPCTSTR lpId);
extern CString LoadString(UINT uID, HINSTANCE hIns = NULL);
extern inline CString FormatId(UINT id, DWORD type = 0);
extern inline CString FormatId(LPCTSTR resId, DWORD type = 0);
extern inline void SymbolGen(UINT id, DWORD type);
extern inline void SymbolGen(LPCTSTR resId, DWORD type);
/////////////////////////////////////////////////////////////////////////////
// intern ICONs definition

typedef struct {              // Symbols definition
   CString name;
   DWORD   value;
   DWORD   type;              // definition and icon
} t_SYMBOL;

typedef CList<t_SYMBOL,t_SYMBOL&> CSymbolList;

extern inline bool SCompare(t_SYMBOL &symbol, UINT id, DWORD type);
extern inline bool SCompare(t_SYMBOL &symbol, LPCTSTR resId, DWORD type);

extern CImageList m_image;    // bank of resource icons
extern CImageList m_sIcon;    // bank of sort icons

// IDB_RESFOLDER icones number
typedef enum { RI_NULL, RI_DISK, RI_FLOPPY, RI_CD_DVD, RI_FOLDER, // RI_FOLDER=4
   RI_OPENFLDR, RI_XFOLDER, RI_XOPENFLDR, RI_DIALOG, RI_BITMAP,   // RI_BITMAP=9
   RI_RECT, RI_STATIC, RI_EDIT, RI_ACCEL, RI_CURSOR, RI_ICON,     // RI_ICON=15
   RI_MENU, RI_MENU_1, RI_MENU_2, RI_MENU_3, RI_DLGINIT, RI_REG,  // RI_REG=21
   RI_HTML, RI_VERSION, RI_FONT, RI_AVI, RI_GROUP, RI_BUTTON,     // RI_BUTTON=27
   RI_CHECK, RI_RADIO, RI_COMBOX, RI_LISTBOX, RI_HSCROLL,         // RI_HSCROLL=32
   RI_VSCROLL, RI_SPIN, RI_PROGRESS, RI_SLIDER, RI_REPORT,        // RI_REPORT=37
   RI_TREE, RI_TABCTRL, RI_ANI, RI_STRING, RI_CUSTOM, RI_TOOLBAR, // RI_TOOLBAR=43
   RI_TOOLBTN, RI_SEPARATOR, RI_WAVE, RI_MIDI, RI_INDEX_1,        // RI_INDEX_1=48
   RI_INDEX_2, RI_DATA, RI_UNDEF, RI_ERROR, RI_SYERROR,
   RI_MENUCMD=64, RI_MULT, RI_HELP  };

// type of symbols
typedef enum {
   TYP_UND=0x00000000,
   // data qualifier types
               //
   TYP_USR=0x00000100, TYP_ID_=0x00000200, TYP_IDB=0x00000400,
   TYP_IDC=0x00000800, TYP_IDD=0x00001000, TYP_IDI=0x00002000,
   TYP_IDM=0x00004000, TYP_IDP=0x00008000, TYP_IDR=0x00010000,
   TYP_IDS=0x00020000, TYP_IDW=0x00040000, TYP_IDx=0x00080000,
               //
   TYP_HIR=0x00100000,
   TYP_HIC=0x00200000,
   TYP_RTX=0x00400000,     // resource type user define
   TYP_xxx=0x00800000,
   // general display types
   TYP_STD=0x01000000,     // standard symbols
   TYP_MDF=0x02000000,     // multidef symbols
   TYP_XSD=0x04000000,
   TYP_SYM=0x08000000,
   TYP_NUM=0x10000000,
   TYP_SEP=0x20000000,
   TYP_BLK=0x40000000,
   TYP_DEC=0x80000000,     // value is decimal coded else hexa
   // acces mask
   TYP_TMSK=0x00FFFF00,
   TYP_IMSK=0x000000FF     // mask to extract icon number
};
#define TICON(data) ((data) & TYP_IMSK)
/////////////////////////////////////////////////////////////////////////////
// Class CPosID

class CPosObj {

class CPosID {
public:
   LPCTSTR id;
   CRect pos;
   inline CPosID(LPCTSTR ID = "", const CRect &P = CRect()) { id = ID, pos = P; }
   inline CPosID(const CRect &p) { id = "", pos = p; }
};

   CList <CPosID, CPosID&> drawObj;
   UINT item;  // 0=> res, 1=> item, 2=> subItem, 3=> position
   LPCTSTR lpSel;

public:
   inline CPosObj() { lpSel = NULL; }
   inline void SetSelect(LPCTSTR id) { lpSel = id; }
   inline UINT Type() { return item; }
   inline void RemoveAll() { drawObj.RemoveAll(); }
   inline BOOL Add(const CRect& pos, LPCTSTR lpName, UINT i = 0)
      { item = i; drawObj.AddTail(CPosID(lpName, pos)); return (lpName==lpSel); }
   inline BOOL IsEmpty() { return drawObj.IsEmpty(); }
   inline CPoint GetOffset()
      { return drawObj.IsEmpty()? CPoint(0,0): drawObj.GetHead().pos.TopLeft(); }

   BOOL Select(LPCTSTR id);
   LPCTSTR GetObj(const CPoint& pos);
   LPCTSTR GetSelect(BOOL frst = TRUE);
};

typedef struct {
      PBYTE pData;
      DWORD size;
      HICON hIcon;
      BYTE  w, h;
      BYTE  step, seq;
} AI_ENT;

typedef struct {
   WORD   count;
   WORD   type;
   AI_ENT icon[1];
} ANI_ICON;

#pragma pack( push )
#pragma pack( 2 )
typedef struct {
   FOURCC ckId;
   DWORD  ckSize;
   DWORD  ckData[1];
} t_CHUNK;
#pragma pack( pop )
typedef DWORD JIF, *PJIF;

typedef struct {     // anih
    DWORD cbSizeof;
    DWORD cFrames;
    DWORD cSteps;
    DWORD cx, cy;
    DWORD cBitCount, cPlanes;
    JIF   jifRate;
    DWORD fl;
} ANIHEADER;
//
// If the AF_ICON flag is specified the fields cx, cy, cBitCount, and
// cPlanes are all unused.  Each frame will be of type ICON and will
// contain its own dimensional information.
//
#define AF_ICON     0x0001L     /* Windows format icon/cursor animation */
#define AF_SEQUENCE 0x0002L     /* Animation is sequenced */

#endif // !defined(AFX_RESUTILITIES_H__B7E56224_5496_44FA_BD99_F9DFFA9C3649__INCLUDED_)
