
#ifndef __ResInfo_h__
#define __ResInfo_h__

//*****************************************************************************
// Structs

// These first two structs represent how the icon information is stored
// when it is bound into a EXE or DLL file. Structure members are WORD
// aligned and the last member of the structure is the ID instead of
// the imageoffset.
#pragma pack( push )
#pragma pack( 2 )

typedef struct {         // icon or cursor in Resource data
	BYTE	bWidth;              // Width of the image
	BYTE	bHeight;             // Height of the image (times 2)
	BYTE	bColorCount;         // Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;           // Reserved
	WORD	wPlanes;             // Color Planes
	WORD	wBitCount;           // Bits per pixel
	DWORD	dwBytesInRes;        // how many bytes in this resource?
	WORD	nID;                 // the ID
} MEMICONDIRENTRY, *LPMEMICONDIRENTRY;

typedef struct {
	WORD			idReserved;    // Reserved
	WORD			idType;        // resource type (1 for icons, 2 for cursors)
	WORD			idCount;       // how many images?
	MEMICONDIRENTRY	idEntries[1]; // the entries for each image
} MEMICONDIR, *LPMEMICONDIR;

//*****************************************************************************
// These next two structs represent how the icon/curs information is stored
// in an ICO/CUR file.

typedef struct {
	BYTE	bWidth;              // Width of the image
	BYTE	bHeight;             // Height of the image (times 2)
	BYTE	bColorCount;         // Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;           // Reserved
	WORD	wCursHotX;           // Cursor Hotspot x, 0=> icon
	WORD	wCursHotY;           // Cursor Hotspot y, 0=> icon
	DWORD	dwBytesInRes;        // how many bytes in this resource?
	DWORD	dwImageOffset;       // where in the file is this image
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct {
	WORD         idReserved;   // Reserved
	WORD         idType;       // resource type (1 for icons 2 for cursor)
	WORD         idCount;      // how many images?
	ICONDIRENTRY idEntries[1]; // the entries for each image
} ICONDIR, *LPICONDIR;

#define ICONDIRSIZE (sizeof(ICONDIR) - sizeof(ICONDIRENTRY))
//*****************************************************************************
// Version Resource

typedef struct { 
    DWORD dwSignature;        // value 0xFEEFO4BD
    DWORD dwStrucVersion;     // binary version number of this structure 
    DWORD dwFileVersionMS;    // most significant 32 bits of the file's binary version number
    DWORD dwFileVersionLS;    // least significant 32 bits of the file's binary version number
    DWORD dwProductVersionMS; // most significant 32 bits of the binary version number of the product
    DWORD dwProductVersionLS; // least significant 32 bits of the binary version number of the product
    DWORD dwFileFlagsMask;    // bitmask that specifies the valid bits in dwFileFlags
    DWORD dwFileFlags;        // bitmask that specifies the Boolean attributes of the file
    DWORD dwFileOS;           // operating system for which this file was designed
    DWORD dwFileType;         // general type of file
    DWORD dwFileSubtype;      // function of the file
    DWORD dwFileDateMS;       // most significant 32 bits of the file's 64-bit binary creation date
    DWORD dwFileDateLS;       // least significant 32 bits of the file's 64-bit binary creation date
} FIXEDFILEINFO;

typedef struct {
   WORD w_Length;             // length, in bytes, of the VERSIONINFO structure
   WORD wValueLength;         // length, in bytes, of the Value member
   WORD uType;                // type of data in the version resource 0=> binary, 1=> text data
   WCHAR szKey[16];           // Unicode string "VS_VERSION_INFO"
   WORD pad1;
   // FIXEDFILEINFO Value;    
   // Children
} VERSIONINFO;

//*****************************************************************************

#define UBUTTON 0x0080
#define UEDIT   0x0081
#define USTATIC 0x0082
#define ULB     0x0083
#define USCB    0x0084
#define UCBB    0x0085

typedef struct {  
   WORD   dlgVer;
   WORD   signature; 
   DWORD  helpID;
   DWORD  exStyle;
   DWORD  style; 
   WORD   cDlgItems;
   short  x;
   short  y;
   short  cx; 
   short  cy; 
} DLGTEMPLATEEX, *LPDLGTEMPLATEEX; 

// Resource toolbar
typedef struct {
   WORD  type;
   short x;
   short y;
   short cnt;
   WORD  toolId[1];
} TOOLBARRES;

typedef struct {
   WORD itemId;
   WORD uMsg;
   DWORD length;
   BYTE data[1];
} DLGINITRES;

typedef struct {
   WORD  fVirt;
   WORD  key;
   DWORD cmd;
} RACCEL;

#pragma pack( pop )
//
// item templateEX must be DWORD aligned
//
typedef struct {  
     
   DWORD  helpID;
   DWORD  exStyle;
   DWORD  style; 
   short  x;
   short  y;
   short  cx; 
   short  cy; 
   WORD   id;
} DLGITEMTEMPLATEEX, *LPDLGITEMTEMPLATEEX;

typedef struct {

   WORD pointSize;
   WORD weight;
   WORD italic;
   WORD typeFace[1];
} DLGFONT, *LPDLGFONT;
//
// icon without entries used to get true size w/o entries
//
typedef struct tagMEMICONFILEHDR 
{
	WORD			idReserved;   // Reserved
	WORD			idType;       // resource type (1 for icons)
	WORD			idCount;      // how many images?
} MEMICONFILEHDR, *LPMEMICONFILEHDR;
//
//
//
typedef struct tagRESINFO
{
        DWORD      riType;
        DWORD      riIDNum;
		CHAR       riIDName[255];
}RESINFO, FAR *LPRESINFO, *PRESINFO;
//
//
//
typedef struct tagRESICONINFO
{
	BITMAPINFOHEADER ihHeader;
	RGBQUAD          ihCOlors[1];
	BYTE             ihXOR[1];
	BYTE             ihAND[1];
}RESICONINFO, *LPRESICONINFO;


typedef struct
{
    DWORD   flag;   // specifies the button type i.e CHECHBOX etc..
    LPTSTR  name;   // .RC name
	
} DWORD_RC_DESCRIPTIONS, *LPDWORD_RC_DESCRIPTIONS;

typedef struct
{
    DWORD   flag;
    LPTSTR  name;
} DWORD_FLAG_DESCRIPTIONS, *LPDWORD_FLAG_DESCRIPTIONS;
//
// input a flag output numeric flag,string of numeric flag 
// seperated by comma
#define FLAG_AS_STRING( f ) f,#f

// Input: a ptr.  Output: the ptr aligned to a DWORD or a WORD.
#define ALIGN_DWORD( type, p ) ( (type)((DWORD(p) + 3) & ~3) )
#define ALIGN_WORD(  type, p ) ( (type)((DWORD(p) + 1) & ~1) )
//
// The aligned ordinal structure.  Ordinals start with a word that is
// always 0xffff, followed by a word that contains the ordinal id.
//
#pragma pack( push )
#pragma pack( 2 )

typedef struct {
    WORD wReserved;                 // 0xffff
    WORD wOrdID;
} ORDINAL, *PORDINAL;


typedef struct {
    DWORD DataSize;                 // Size of data.
    DWORD HeaderSize;               // Size of the resource header.
} RES, *PRES;

typedef struct {
    DWORD DataVersion;              // Predefined resource data version.
    WORD MemoryFlags;               // Resource memory flags.
    WORD LanguageId;                // UNICODE support for NLS.
    DWORD Version;                  // Version of the resource data.
    DWORD Characteristics;          // Characteristics of the data.
} RES2, *PRES2;

typedef struct {
    DWORD lStyle;                   // Style for the dialog.
    DWORD lExtendedStyle;           // The extended style.
    WORD NumberOfItems;             // Number of controls.
    WORD x;                         // Starting x location.
    WORD y;                         // Starting y location.
    WORD cx;                        // Dialog width.
    WORD cy;                        // Dialog height.
} DIALOGBOXHEADER, *PDIALOGBOXHEADER;

#pragma pack( pop )

#define SIZEOF_DIALOGBOXHEADER  (                               \
    sizeof(DWORD) +                 /* lStyle           */      \
    sizeof(DWORD) +                 /* lExtendedStyle   */      \
    sizeof(WORD) +                  /* NumberOfItems    */      \
    sizeof(WORD) +                  /* x                */      \
    sizeof(WORD) +                  /* y                */      \
    sizeof(WORD) +                  /* cx               */      \
    sizeof(WORD)                    /* cy               */      \
    )
//
// Resource memory management flags.
//
#define MMF_MOVEABLE            0x0010
#define MMF_PURE                0x0020
#define MMF_PRELOAD             0x0040
#define MMF_DISCARDABLE         0x1000

#define DEFDLGMEMFLAGS          (MMF_MOVEABLE | MMF_PURE | MMF_DISCARDABLE)

#endif
