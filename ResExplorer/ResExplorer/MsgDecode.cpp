
#include "stdafx.h"
#include <AFXPRIV.H>
#include "MsgDecode.h"

#ifdef _DEBUG       // entire file for debugging

// Module de test et de debug
// Decode les messages et les resources pour la TRACE par exemple

#ifndef WM_SYSTIMER
#define WM_SYSTIMER 0x0118 // (caret blink)
#endif

//   report very frequently sent messages

static UINT veryFreq[] = {
// Add in this table a message to skip
   WM_MOUSEMOVE,
   WM_NCMOUSEMOVE,
   WM_NCHITTEST,
   WM_SETCURSOR,
   WM_CTLCOLORBTN,
   WM_CTLCOLORDLG,
   WM_CTLCOLOREDIT,
   WM_CTLCOLORLISTBOX,
   WM_CTLCOLORMSGBOX,
   WM_CTLCOLORSCROLLBAR,
   WM_CTLCOLORSTATIC,
   WM_ENTERIDLE,
   WM_CANCELMODE,
//   WM_TIMER,
   WM_SYSTIMER,   // 0x0118 (caret blink)
   0                     };

BOOL IsVeryFrequentlySent(UINT msg)
{
   UINT *pMsg = veryFreq, m;
   while (m = *pMsg++) if (m == msg) return TRUE;
   return FALSE;
}

// Messages decode table

typedef struct { UINT   id; LPCTSTR text; } t_MESS;
#define TEXID(id) { id, #id }

static t_MESS messages[] = {
   TEXID(WM_CREATE),                // 0x0001
   TEXID(WM_DESTROY),               // 0x0002
   TEXID(WM_MOVE),                  // 0x0003
   TEXID(0x0004),
   TEXID(WM_SIZE),                  // 0x0005
   TEXID(WM_ACTIVATE),              // 0x0006
   TEXID(WM_SETFOCUS),              // 0x0007
   TEXID(WM_KILLFOCUS),             // 0x0008
   TEXID(0x0009),
   TEXID(WM_ENABLE),                // 0x000A
   TEXID(WM_SETREDRAW),             // 0x000B
   TEXID(WM_SETTEXT),               // 0x000C
   TEXID(WM_GETTEXT),               // 0x000D
   TEXID(WM_GETTEXTLENGTH),         // 0x000E
   TEXID(WM_PAINT),                 // 0x000F
   TEXID(WM_CLOSE),                 // 0x0010
   TEXID(WM_QUERYENDSESSION),       // 0x0011
   TEXID(WM_QUIT),                  // 0x0012
   TEXID(WM_QUERYOPEN),             // 0x0013
   TEXID(WM_ERASEBKGND),            // 0x0014
   TEXID(WM_SYSCOLORCHANGE),        // 0x0015
   TEXID(WM_ENDSESSION),            // 0x0016
   TEXID(0x0017),
   TEXID(WM_SHOWWINDOW),            // 0x0018
   TEXID(0x0019),
   TEXID(WM_WININICHANGE),          // 0x001A
   TEXID(WM_DEVMODECHANGE),         // 0x001B
   TEXID(WM_ACTIVATEAPP),           // 0x001C
   TEXID(WM_FONTCHANGE),            // 0x001D
   TEXID(WM_TIMECHANGE),            // 0x001E
   TEXID(WM_CANCELMODE),            // 0x001F
   TEXID(WM_SETCURSOR),             // 0x0020
   TEXID(WM_MOUSEACTIVATE),         // 0x0021
   TEXID(WM_CHILDACTIVATE),         // 0x0022
   TEXID(WM_QUEUESYNC),             // 0x0023
   TEXID(WM_GETMINMAXINFO),         // 0x0024
   TEXID(0x0025),
   TEXID(WM_PAINTICON),             // 0x0026
   TEXID(WM_ICONERASEBKGND),        // 0x0027
   TEXID(WM_NEXTDLGCTL),            // 0x0028
   TEXID(0x0029),
   TEXID(WM_SPOOLERSTATUS),         // 0x002A
   TEXID(WM_DRAWITEM),              // 0x002B
   TEXID(WM_MEASUREITEM),           // 0x002C
   TEXID(WM_DELETEITEM),            // 0x002D
   TEXID(WM_VKEYTOITEM),            // 0x002E
   TEXID(WM_CHARTOITEM),            // 0x002F
   TEXID(WM_SETFONT),               // 0x0030
   TEXID(WM_GETFONT),               // 0x0031
   TEXID(WM_SETHOTKEY),             // 0x0032
   TEXID(WM_GETHOTKEY),             // 0x0033
   TEXID(0x0034),
   TEXID(0x0035),
   TEXID(0x0036),
   TEXID(WM_QUERYDRAGICON),         // 0x0037
   TEXID(0x0038),
   TEXID(WM_COMPAREITEM),           // 0x0039
   TEXID(0x003A),
   TEXID(0x003B),
   TEXID(0x003C),
#ifndef WM_GETOBJECT // WINVER >= 0x0500
#define WM_GETOBJECT 0x003D
#endif
   TEXID(WM_GETOBJECT),             // 0x003D
   TEXID(0x003E),
   TEXID(0x003F),
   TEXID(0x0040),
   TEXID(WM_COMPACTING),            // 0x0041
   TEXID(0x0042),
   TEXID(0x0043),
   TEXID(WM_COMMNOTIFY),            // 0x0044  /* no longer suported */
   TEXID(0x0045),
   TEXID(WM_WINDOWPOSCHANGING),     // 0x0046
   TEXID(WM_WINDOWPOSCHANGED),      // 0x0047
   TEXID(WM_POWER),                 // 0x0048
   TEXID(WM_COPYDATA),
   TEXID(WM_CANCELJOURNAL),
#if(WINVER >= 0x0400)
   TEXID(WM_NOTIFY),
   TEXID(WM_INPUTLANGCHANGEREQUEST),
   TEXID(WM_INPUTLANGCHANGE),
   TEXID(WM_TCARD),
   TEXID(WM_HELP),
   TEXID(WM_USERCHANGED),
   TEXID(WM_NOTIFYFORMAT),
   TEXID(WM_CONTEXTMENU),
   TEXID(WM_STYLECHANGING),
   TEXID(WM_STYLECHANGED),
   TEXID(WM_DISPLAYCHANGE),
   TEXID(WM_GETICON),
   TEXID(WM_SETICON),
#endif /* WINVER >= 0x0400 */
   TEXID(WM_NCCREATE),
   TEXID(WM_NCDESTROY),
   TEXID(WM_NCCALCSIZE),
   TEXID(WM_NCHITTEST),
   TEXID(WM_NCPAINT),
   TEXID(WM_NCACTIVATE),
   TEXID(WM_GETDLGCODE),
   TEXID(WM_SYNCPAINT),
   TEXID(WM_NCMOUSEMOVE),
   TEXID(WM_NCLBUTTONDOWN),
   TEXID(WM_NCLBUTTONUP),
   TEXID(WM_NCLBUTTONDBLCLK),
   TEXID(WM_NCRBUTTONDOWN),
   TEXID(WM_NCRBUTTONUP),
   TEXID(WM_NCRBUTTONDBLCLK),
   TEXID(WM_NCMBUTTONDOWN),
   TEXID(WM_NCMBUTTONUP),
   TEXID(WM_NCMBUTTONDBLCLK),       // 0x00A9

   TEXID(SBM_SETPOS),               // 0x00E0 /*not in win3.1 */
   TEXID(SBM_GETPOS),               // 0x00E1 /*not in win3.1 */
   TEXID(SBM_SETRANGE),             // 0x00E2 /*not in win3.1 */
   TEXID(SBM_SETRANGEREDRAW),       // 0x00E6 /*not in win3.1 */
   TEXID(SBM_GETRANGE),             // 0x00E3 /*not in win3.1 */
   TEXID(SBM_ENABLE_ARROWS),        // 0x00E4 /*not in win3.1 */
#if(WINVER >= 0x0400)
   TEXID(SBM_SETSCROLLINFO),        // 0x00E9
   TEXID(SBM_GETSCROLLINFO),        // 0x00EA
#endif
/*
 * Button Control Messages
 */
   TEXID(BM_GETCHECK),              // 0x00F0
   TEXID(BM_SETCHECK),              // 0x00F1
   TEXID(BM_GETSTATE),              // 0x00F2
   TEXID(BM_SETSTATE),              // 0x00F3
   TEXID(BM_SETSTYLE),              // 0x00F4
   TEXID(BM_CLICK),                 // 0x00F5
   TEXID(BM_GETIMAGE),              // 0x00F6
   TEXID(BM_SETIMAGE),              // 0x00F7

   TEXID(WM_KEYFIRST),              // 0x0100
   TEXID(WM_KEYDOWN),
   TEXID(WM_KEYUP),
   TEXID(WM_CHAR),
   TEXID(WM_DEADCHAR),
   TEXID(WM_SYSKEYDOWN),
   TEXID(WM_SYSKEYUP),
   TEXID(WM_SYSCHAR),
   TEXID(WM_SYSDEADCHAR),
   TEXID(WM_KEYLAST),               // 0x0108
#if(WINVER >= 0x0400)
   TEXID(WM_IME_STARTCOMPOSITION),
   TEXID(WM_IME_ENDCOMPOSITION),
   TEXID(WM_IME_COMPOSITION),
   TEXID(WM_IME_KEYLAST),
#endif /* WINVER >= 0x0400 */
   TEXID(WM_INITDIALOG),            // 0x0110
   TEXID(WM_COMMAND),               // 0x0111
   TEXID(WM_SYSCOMMAND),            // 0x0112
   TEXID(WM_TIMER),                 // 0x0113
   TEXID(WM_HSCROLL),               // 0x0114
   TEXID(WM_VSCROLL),               // 0x0115
   TEXID(WM_INITMENU),              // 0x0116
   TEXID(WM_INITMENUPOPUP),         // 0x0117
   TEXID(WM_SYSTIMER),              // 0x0118 (caret blink)

   TEXID(WM_MENUSELECT),            // 0x011F
   TEXID(WM_MENUCHAR),              // 0x0120
   TEXID(WM_ENTERIDLE),             // 0x0121
#if(WINVER >= 0x0500)
   TEXID(WM_MENURBUTTONUP),
   TEXID(WM_MENUDRAG),
   TEXID(WM_MENUGETOBJECT),
   TEXID(WM_UNINITMENUPOPUP),
   TEXID(WM_MENUCOMMAND),
#endif /* WINVER >= 0x0500 */
   TEXID(WM_CTLCOLORMSGBOX),        // 0x0132
   TEXID(WM_CTLCOLOREDIT),
   TEXID(WM_CTLCOLORLISTBOX),
   TEXID(WM_CTLCOLORBTN),
   TEXID(WM_CTLCOLORDLG),
   TEXID(WM_CTLCOLORSCROLLBAR),
   TEXID(WM_CTLCOLORSTATIC),        // 0x0138
/*
 * Combo Box messages
 */
   TEXID(CB_GETEDITSEL),            // 0x0140
   TEXID(CB_LIMITTEXT),             // 0x0141
   TEXID(CB_SETEDITSEL),            // 0x0142
   TEXID(CB_ADDSTRING),             // 0x0143
   TEXID(CB_DELETESTRING),          // 0x0144
   TEXID(CB_DIR),                   // 0x0145
   TEXID(CB_GETCOUNT),              // 0x0146
   TEXID(CB_GETCURSEL),             // 0x0147
   TEXID(CB_GETLBTEXT),             // 0x0148
   TEXID(CB_GETLBTEXTLEN),          // 0x0149
   TEXID(CB_INSERTSTRING),          // 0x014A
   TEXID(CB_RESETCONTENT),          // 0x014B
   TEXID(CB_FINDSTRING),            // 0x014C
   TEXID(CB_SELECTSTRING),          // 0x014D
   TEXID(CB_SETCURSEL),             // 0x014E
   TEXID(CB_SHOWDROPDOWN),          // 0x014F
   TEXID(CB_GETITEMDATA),           // 0x0150
   TEXID(CB_SETITEMDATA),           // 0x0151
   TEXID(CB_GETDROPPEDCONTROLRECT), // 0x0152
   TEXID(CB_SETITEMHEIGHT),         // 0x0153
   TEXID(CB_GETITEMHEIGHT),         // 0x0154
   TEXID(CB_SETEXTENDEDUI),         // 0x0155
   TEXID(CB_GETEXTENDEDUI),         // 0x0156
   TEXID(CB_GETDROPPEDSTATE),       // 0x0157
   TEXID(CB_FINDSTRINGEXACT),       // 0x0158
   TEXID(CB_SETLOCALE),             // 0x0159
   TEXID(CB_GETLOCALE),             // 0x015A
#if(WINVER >= 0x0400)
   TEXID(CB_GETTOPINDEX),           // 0x015b
   TEXID(CB_SETTOPINDEX),           // 0x015c
   TEXID(CB_GETHORIZONTALEXTENT),   // 0x015d
   TEXID(CB_SETHORIZONTALEXTENT),   // 0x015e
   TEXID(CB_GETDROPPEDWIDTH),       // 0x015f
   TEXID(CB_SETDROPPEDWIDTH),       // 0x0160
   TEXID(CB_INITSTORAGE),           // 0x0161
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0400)
   TEXID(CB_MSGMAX),                // 0x0162
#else
   TEXID(CB_MSGMAX),                // 0x015B
#endif
/*
 * Listbox messages
 */
   TEXID(LB_ADDSTRING),             // 0x0180
   TEXID(LB_INSERTSTRING),          // 0x0181
   TEXID(LB_DELETESTRING),          // 0x0182
   TEXID(LB_SELITEMRANGEEX),        // 0x0183
   TEXID(LB_RESETCONTENT),          // 0x0184
   TEXID(LB_SETSEL),                // 0x0185
   TEXID(LB_SETCURSEL),             // 0x0186
   TEXID(LB_GETSEL),                // 0x0187
   TEXID(LB_GETCURSEL),             // 0x0188
   TEXID(LB_GETTEXT),               // 0x0189
   TEXID(LB_GETTEXTLEN),            // 0x018A
   TEXID(LB_GETCOUNT),              // 0x018B
   TEXID(LB_SELECTSTRING),          // 0x018C
   TEXID(LB_DIR),                   // 0x018D
   TEXID(LB_GETTOPINDEX),           // 0x018E
   TEXID(LB_FINDSTRING),            // 0x018F
   TEXID(LB_GETSELCOUNT),           // 0x0190
   TEXID(LB_GETSELITEMS),           // 0x0191
   TEXID(LB_SETTABSTOPS),           // 0x0192
   TEXID(LB_GETHORIZONTALEXTENT),   // 0x0193
   TEXID(LB_SETHORIZONTALEXTENT),   // 0x0194
   TEXID(LB_SETCOLUMNWIDTH),        // 0x0195
   TEXID(LB_ADDFILE),               // 0x0196
   TEXID(LB_SETTOPINDEX),           // 0x0197
   TEXID(LB_GETITEMRECT),           // 0x0198
   TEXID(LB_GETITEMDATA),           // 0x0199
   TEXID(LB_SETITEMDATA),           // 0x019A
   TEXID(LB_SELITEMRANGE),          // 0x019B
   TEXID(LB_SETANCHORINDEX),        // 0x019C
   TEXID(LB_GETANCHORINDEX),        // 0x019D
   TEXID(LB_SETCARETINDEX),         // 0x019E
   TEXID(LB_GETCARETINDEX),         // 0x019F
   TEXID(LB_SETITEMHEIGHT),         // 0x01A0
   TEXID(LB_GETITEMHEIGHT),         // 0x01A1
   TEXID(LB_FINDSTRINGEXACT),       // 0x01A2
   TEXID(LB_SETLOCALE),             // 0x01A5
   TEXID(LB_GETLOCALE),             // 0x01A6
   TEXID(LB_SETCOUNT),              // 0x01A7
#if(WINVER >= 0x0400)
   TEXID(LB_INITSTORAGE),           // 0x01A8
   TEXID(LB_ITEMFROMPOINT),         // 0x01A9
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0400)
   TEXID(LB_MSGMAX),                // 0x01B0
#else
   TEXID(LB_MSGMAX),                // 0x01A8
#endif

   TEXID(WM_MOUSEFIRST),            // 0x0200
   TEXID(WM_MOUSEMOVE),
   TEXID(WM_LBUTTONDOWN),
   TEXID(WM_LBUTTONUP),
   TEXID(WM_LBUTTONDBLCLK),
   TEXID(WM_RBUTTONDOWN),
   TEXID(WM_RBUTTONUP),
   TEXID(WM_RBUTTONDBLCLK),
   TEXID(WM_MBUTTONDOWN),
   TEXID(WM_MBUTTONUP),
   TEXID(WM_MBUTTONDBLCLK),
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
   TEXID(WM_MOUSEWHEEL),         // 0x020A
   TEXID(WM_MOUSELAST),
#else
   TEXID(WM_MOUSELAST),
#endif /* if (_WIN32_WINNT < 0x0400) */
   TEXID(WM_PARENTNOTIFY),
   TEXID(WM_ENTERMENULOOP),
   TEXID(WM_EXITMENULOOP),
#if(WINVER >= 0x0400)
   TEXID(WM_NEXTMENU),
   TEXID(WM_SIZING),
   TEXID(WM_CAPTURECHANGED),
   TEXID(WM_MOVING),
// end_r_winuser
   TEXID(WM_POWERBROADCAST),      // r_winuser pbt
// begin_r_winuser
   TEXID(WM_DEVICECHANGE),
#endif /* WINVER >= 0x0400 */
   TEXID(WM_MDICREATE),
   TEXID(WM_MDIDESTROY),
   TEXID(WM_MDIACTIVATE),
   TEXID(WM_MDIRESTORE),
   TEXID(WM_MDINEXT),
   TEXID(WM_MDIMAXIMIZE),
   TEXID(WM_MDITILE),
   TEXID(WM_MDICASCADE),
   TEXID(WM_MDIICONARRANGE),
   TEXID(WM_MDIGETACTIVE),
   TEXID(WM_MDISETMENU),
   TEXID(WM_ENTERSIZEMOVE),
   TEXID(WM_EXITSIZEMOVE),
   TEXID(WM_DROPFILES),
   TEXID(WM_MDIREFRESHMENU),
#if(WINVER >= 0x0400)
   TEXID(WM_IME_SETCONTEXT),
   TEXID(WM_IME_NOTIFY),
   TEXID(WM_IME_CONTROL),
   TEXID(WM_IME_COMPOSITIONFULL),
   TEXID(WM_IME_SELECT),
   TEXID(WM_IME_CHAR),
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
   TEXID(WM_IME_REQUEST),
#endif /* WINVER >= 0x0500 */
#if(WINVER >= 0x0400)
   TEXID(WM_IME_KEYDOWN),
   TEXID(WM_IME_KEYUP),
#endif /* WINVER >= 0x0400 */
#if(_WIN32_WINNT >= 0x0400)
   TEXID(WM_MOUSEHOVER),
   TEXID(WM_MOUSELEAVE),
#endif /* _WIN32_WINNT >= 0x0400 */
   TEXID(WM_CUT),//                          0x0300
   TEXID(WM_COPY),//                         0x0301
   TEXID(WM_PASTE),//                        0x0302
   TEXID(WM_CLEAR),//                        0x0303
   TEXID(WM_UNDO),//                         0x0304
   TEXID(WM_RENDERFORMAT),//                 0x0305
   TEXID(WM_RENDERALLFORMATS),//             0x0306
   TEXID(WM_DESTROYCLIPBOARD),//             0x0307
   TEXID(WM_DRAWCLIPBOARD),//                0x0308
   TEXID(WM_PAINTCLIPBOARD),//               0x0309
   TEXID(WM_VSCROLLCLIPBOARD),//             0x030A
   TEXID(WM_SIZECLIPBOARD),//                0x030B
   TEXID(WM_ASKCBFORMATNAME),//              0x030C
   TEXID(WM_CHANGECBCHAIN),//                0x030D
   TEXID(WM_HSCROLLCLIPBOARD),//             0x030E
   TEXID(WM_QUERYNEWPALETTE),//              0x030F
   TEXID(WM_PALETTEISCHANGING),//            0x0310
   TEXID(WM_PALETTECHANGED),//               0x0311
   TEXID(WM_HOTKEY),//                       0x0312
#if(WINVER >= 0x0400)
   TEXID(WM_PRINT),//                        0x0317
   TEXID(WM_PRINTCLIENT),//                  0x0318
   TEXID(WM_HANDHELDFIRST),//                0x0358
   TEXID(WM_HANDHELDLAST),//                 0x035F
   TEXID(WM_AFXFIRST),//                     0x0360
   TEXID(WM_QUERYAFXWNDPROC),//  0x0360  // lResult = 1 if processed by AfxWndProc
   TEXID(WM_SIZEPARENT),//       0x0361  // lParam = &AFX_SIZEPARENTPARAMS
   TEXID(WM_SETMESSAGESTRING),// 0x0362  // wParam = nIDS (or 0),
									// lParam = lpszOther (or NULL)
   TEXID(WM_IDLEUPDATECMDUI),//  0x0363  // wParam == bDisableIfNoHandler
   TEXID(WM_INITIALUPDATE),//    0x0364  // (params unused) - sent to children
   TEXID(WM_COMMANDHELP),//      0x0365  // lResult = TRUE/FALSE,
									// lParam = dwContext
   TEXID(WM_HELPHITTEST),//      0x0366  // lResult = dwContext,
									// lParam = MAKELONG(x,y)
   TEXID(WM_EXITHELPMODE),//     0x0367  // (params unused)
   TEXID(WM_RECALCPARENT),//     0x0368  // force RecalcLayout on frame window
									//  (only for inplace frame windows)
   TEXID(WM_SIZECHILD),//        0x0369  // special notify from COleResizeBar
									// wParam = ID of child window
									// lParam = lpRectNew (new position/size)
   TEXID(WM_KICKIDLE),//         0x036A  // (params unused) causes idles to kick in
   TEXID(WM_QUERYCENTERWND),//   0x036B  // lParam = HWND to use as centering parent
   TEXID(WM_DISABLEMODAL),//     0x036C  // lResult = 0, disable during modal state
									// lResult = 1, don't disable
   TEXID(WM_FLOATSTATUS),//      0x036D  // wParam combination of FS_* flags below

// WM_ACTIVATETOPLEVEL is like WM_ACTIVATEAPP but works with hierarchies
//   of mixed processes (as is the case with OLE in-place activation)
   TEXID(WM_ACTIVATETOPLEVEL),// 0x036E  // wParam = nState (like WM_ACTIVATE)
									// lParam = pointer to HWND[2]
									//  lParam[0] = hWnd getting WM_ACTIVATE
									//  lParam[1] = hWndOther

//   TEXID(WM_QUERY3DCONTROLS),//  0x036F  // lResult != 0 if 3D controls wanted

// Note: Messages 0x0370, 0x0371, and 0x372 were incorrectly used by
//  some versions of Windows.  To remain compatible, MFC does not
//  use messages in that range.
   TEXID(WM_RESERVED_0370),//    0x0370
   TEXID(WM_RESERVED_0371),//    0x0371
   TEXID(WM_RESERVED_0372),//    0x0372

// WM_SOCKET_NOTIFY and WM_SOCKET_DEAD are used internally by MFC's
// Windows sockets implementation.  For more information, see sockcore.cpp
   TEXID(WM_SOCKET_NOTIFY),//    0x0373
   TEXID(WM_SOCKET_DEAD),//      0x0374

// same as WM_SETMESSAGESTRING except not popped if IsTracking()
   TEXID(WM_POPMESSAGESTRING),// 0x0375

// WM_HELPPROMPTADDR is used internally to get the address of
//  m_dwPromptContext from the associated frame window. This is used
//  during message boxes to setup for F1 help while that msg box is
//  displayed. lResult is the address of m_dwPromptContext.
   TEXID(WM_HELPPROMPTADDR),//   0x0376

// Constants used in DLGINIT resources for OLE control containers
// NOTE: These are NOT real Windows messages they are simply tags
// used in the control resource and are never used as 'messages'
   TEXID(WM_OCC_LOADFROMSTREAM),//           0x0376
   TEXID(WM_OCC_LOADFROMSTORAGE),//          0x0377
   TEXID(WM_OCC_INITNEW),//                  0x0378
   TEXID(WM_OCC_LOADFROMSTREAM_EX),//        0x037A
   TEXID(WM_OCC_LOADFROMSTORAGE_EX),//       0x037B

// Marker used while rearranging the message queue
   TEXID(WM_QUEUE_SENTINEL),//   0x0379

// Note: Messages 0x037C - 0x37E reserved for future MFC use.
   TEXID(WM_RESERVED_037C),//    0x037C
   TEXID(WM_RESERVED_037D),//    0x037D
   TEXID(WM_RESERVED_037E),//    0x037E

// WM_FORWARDMSG - used by ATL to forward a message to another window for processing
//  WPARAM - DWORD dwUserData - defined by user
//  LPARAM - LPMSG pMsg - a pointer to the MSG structure
//  return value - 0 if the message was not processed, nonzero if it was
   TEXID(WM_FORWARDMSG),//       0x037F

   TEXID(WM_AFXLAST),//                      0x037F
#endif /* WINVER >= 0x0400 */
   TEXID(WM_PENWINFIRST),//                  0x0380
   TEXID(WM_PENWINLAST),//                   0x038F
#if(WINVER >= 0x0400)
   TEXID(WM_APP),//                          0x8000
#endif /* WINVER >= 0x0400 */
/*
 * NOTE: All Message Numbers below 0x0400 are RESERVED.
 *
 * Private Window Messages Start Here:
 */
   TEXID(WM_USER),//                         0x0400
   TEXID(TTM_ACTIVATE),//            (WM_USER + 1)
   TEXID(TTM_SETDELAYTIME),//        (WM_USER + 3)
   TEXID(TTM_ADDTOOLA),//            (WM_USER + 4)
   TEXID(TTM_ADDTOOLW),//            (WM_USER + 50)
   TEXID(TTM_DELTOOLA),//            (WM_USER + 5)
   TEXID(TTM_DELTOOLW),//            (WM_USER + 51)
   TEXID(TTM_NEWTOOLRECTA),//        (WM_USER + 6)
   TEXID(TTM_NEWTOOLRECTW),//        (WM_USER + 52)
   TEXID(TTM_RELAYEVENT),//          (WM_USER + 7)
   TEXID(TTM_GETTOOLINFOA),//        (WM_USER + 8)
   TEXID(TTM_GETTOOLINFOW),//        (WM_USER + 53)
   TEXID(TTM_SETTOOLINFOA),//        (WM_USER + 9)
   TEXID(TTM_SETTOOLINFOW),//        (WM_USER + 54)
   TEXID(TTM_HITTESTA),//            (WM_USER + 10)
   TEXID(TTM_HITTESTW),//            (WM_USER + 55)
   TEXID(TTM_GETTEXTA),//            (WM_USER + 11)
   TEXID(TTM_GETTEXTW),//            (WM_USER + 56)
   TEXID(TTM_UPDATETIPTEXTA),//      (WM_USER + 12)
   TEXID(TTM_UPDATETIPTEXTW),//      (WM_USER + 57)
   TEXID(TTM_GETTOOLCOUNT),//        (WM_USER + 13)
   TEXID(TTM_ENUMTOOLSA),//          (WM_USER + 14)
   TEXID(TTM_ENUMTOOLSW),//          (WM_USER + 58)
   TEXID(TTM_GETCURRENTTOOLA),//     (WM_USER + 15)
   TEXID(TTM_GETCURRENTTOOLW),//     (WM_USER + 59)
   TEXID(TTM_WINDOWFROMPOINT),//     (WM_USER + 16)
#if (_WIN32_IE >= 0x0300)
   TEXID(TTM_TRACKACTIVATE),//       (WM_USER + 17)  // wParam = TRUE/FALSE start end  lparam = LPTOOLINFO
   TEXID(TTM_TRACKPOSITION),//       (WM_USER + 18)  // lParam = dwPos
   TEXID(TTM_SETTIPBKCOLOR),//       (WM_USER + 19)
   TEXID(TTM_SETTIPTEXTCOLOR),//     (WM_USER + 20)
   TEXID(TTM_GETDELAYTIME),//        (WM_USER + 21)
   TEXID(TTM_GETTIPBKCOLOR),//       (WM_USER + 22)
   TEXID(TTM_GETTIPTEXTCOLOR),//     (WM_USER + 23)
   TEXID(TTM_SETMAXTIPWIDTH),//      (WM_USER + 24)
   TEXID(TTM_GETMAXTIPWIDTH),//      (WM_USER + 25)
   TEXID(TTM_SETMARGIN),//           (WM_USER + 26)  // lParam = lprc
   TEXID(TTM_GETMARGIN),//           (WM_USER + 27)  // lParam = lprc
   TEXID(TTM_POP),//                 (WM_USER + 28)
#endif
#if (_WIN32_IE >= 0x0400)
   TEXID(TTM_UPDATE),//              (WM_USER + 29)
#endif
//       LVM_FIRST                     0x1000      // ListView messages
   TEXID(LVM_GETBKCOLOR),//          (LVM_FIRST + 0)
   TEXID(LVM_SETBKCOLOR),//          (LVM_FIRST + 1)
   TEXID(LVM_GETIMAGELIST),//        (LVM_FIRST + 2)
   TEXID(LVM_SETIMAGELIST),//        (LVM_FIRST + 3)
   TEXID(LVM_GETITEMCOUNT),//        (LVM_FIRST + 4)
   TEXID(LVM_GETITEMA),//            (LVM_FIRST + 5)
   TEXID(LVM_GETITEMW),//            (LVM_FIRST + 75)
   TEXID(LVM_SETITEMA),//            (LVM_FIRST + 6)
   TEXID(LVM_SETITEMW),//            (LVM_FIRST + 76)
   TEXID(LVM_INSERTITEMA),//         (LVM_FIRST + 7)
   TEXID(LVM_INSERTITEMW),//         (LVM_FIRST + 77)
   TEXID(LVM_DELETEITEM),//          (LVM_FIRST + 8)
   TEXID(LVM_DELETEALLITEMS),//      (LVM_FIRST + 9)
   TEXID(LVM_GETCALLBACKMASK),//     (LVM_FIRST + 10)
   TEXID(LVM_SETCALLBACKMASK),//     (LVM_FIRST + 11)
   TEXID(LVM_GETNEXTITEM),//         (LVM_FIRST + 12)
   TEXID(LVM_FINDITEMA),//           (LVM_FIRST + 13)
   TEXID(LVM_FINDITEMW),//           (LVM_FIRST + 83)
   TEXID(LVM_GETITEMRECT),//         (LVM_FIRST + 14)
   TEXID(LVM_SETITEMPOSITION),//     (LVM_FIRST + 15)
   TEXID(LVM_GETITEMPOSITION),//     (LVM_FIRST + 16)
   TEXID(LVM_GETSTRINGWIDTHA),//     (LVM_FIRST + 17)
   TEXID(LVM_GETSTRINGWIDTHW),//     (LVM_FIRST + 87)
   TEXID(LVM_HITTEST),//             (LVM_FIRST + 18)
   TEXID(LVM_ENSUREVISIBLE),//       (LVM_FIRST + 19)
   TEXID(LVM_SCROLL),//              (LVM_FIRST + 20)
   TEXID(LVM_REDRAWITEMS),//         (LVM_FIRST + 21)
   TEXID(LVM_ARRANGE),//             (LVM_FIRST + 22)
   TEXID(LVM_EDITLABELA),//          (LVM_FIRST + 23)
   TEXID(LVM_EDITLABELW),//          (LVM_FIRST + 118)
   TEXID(LVM_GETEDITCONTROL),//      (LVM_FIRST + 24)
   TEXID(LVM_GETCOLUMNA),//          (LVM_FIRST + 25)
   TEXID(LVM_GETCOLUMNW),//          (LVM_FIRST + 95)
   TEXID(LVM_SETCOLUMNA),//          (LVM_FIRST + 26)
   TEXID(LVM_SETCOLUMNW),//          (LVM_FIRST + 96)
   TEXID(LVM_INSERTCOLUMNA),//       (LVM_FIRST + 27)
   TEXID(LVM_INSERTCOLUMNW),//       (LVM_FIRST + 97)
   TEXID(LVM_DELETECOLUMN),//        (LVM_FIRST + 28)
   TEXID(LVM_GETCOLUMNWIDTH),//      (LVM_FIRST + 29)
   TEXID(LVM_SETCOLUMNWIDTH),//      (LVM_FIRST + 30)
   TEXID(LVM_GETHEADER),//           (LVM_FIRST + 31)
   TEXID(LVM_CREATEDRAGIMAGE),//     (LVM_FIRST + 33)
   TEXID(LVM_GETVIEWRECT),//         (LVM_FIRST + 34)
   TEXID(LVM_GETTEXTCOLOR),//        (LVM_FIRST + 35)
   TEXID(LVM_SETTEXTCOLOR),//        (LVM_FIRST + 36)
   TEXID(LVM_GETTEXTBKCOLOR),//      (LVM_FIRST + 37)
   TEXID(LVM_SETTEXTBKCOLOR),//      (LVM_FIRST + 38)
   TEXID(LVM_GETTOPINDEX),//         (LVM_FIRST + 39)
   TEXID(LVM_GETCOUNTPERPAGE),//     (LVM_FIRST + 40)
   TEXID(LVM_GETORIGIN),//           (LVM_FIRST + 41)
   TEXID(LVM_UPDATE),//              (LVM_FIRST + 42)
   TEXID(LVM_SETITEMSTATE),//        (LVM_FIRST + 43)
   TEXID(LVM_GETITEMSTATE),//        (LVM_FIRST + 44)
   TEXID(LVM_GETITEMTEXTA),//        (LVM_FIRST + 45)
   TEXID(LVM_GETITEMTEXTW),//        (LVM_FIRST + 115)
   TEXID(LVM_SETITEMTEXTA),//        (LVM_FIRST + 46)
   TEXID(LVM_SETITEMTEXTW),//        (LVM_FIRST + 116)
   TEXID(LVM_SETITEMCOUNT),//        (LVM_FIRST + 47)
   TEXID(LVM_SORTITEMS),//           (LVM_FIRST + 48)
   TEXID(LVM_SETITEMPOSITION32),//   (LVM_FIRST + 49)
   TEXID(LVM_GETSELECTEDCOUNT),//    (LVM_FIRST + 50)
   TEXID(LVM_GETITEMSPACING),//      (LVM_FIRST + 51)
   TEXID(LVM_GETISEARCHSTRINGA),//   (LVM_FIRST + 52)
   TEXID(LVM_GETISEARCHSTRINGW),//   (LVM_FIRST + 117)
   TEXID(LVM_SETICONSPACING),//      (LVM_FIRST + 53)
   TEXID(LVM_SETEXTENDEDLISTVIEWSTYLE),//(LVM_FIRST + 54)   // optional wParam == mask
   TEXID(LVM_GETEXTENDEDLISTVIEWSTYLE),//(LVM_FIRST + 55)
   TEXID(LVM_GETSUBITEMRECT),//      (LVM_FIRST + 56)
   TEXID(LVM_SUBITEMHITTEST),//      (LVM_FIRST + 57)
   TEXID(LVM_SETCOLUMNORDERARRAY),// (LVM_FIRST + 58)
   TEXID(LVM_GETCOLUMNORDERARRAY),// (LVM_FIRST + 59)
   TEXID(LVM_SETHOTITEM),//          (LVM_FIRST + 60)
   TEXID(LVM_GETHOTITEM),//          (LVM_FIRST + 61)
   TEXID(LVM_SETHOTCURSOR),//        (LVM_FIRST + 62)
   TEXID(LVM_GETHOTCURSOR),//        (LVM_FIRST + 63)
   TEXID(LVM_APPROXIMATEVIEWRECT),// (LVM_FIRST + 64)
   TEXID(LVM_SETWORKAREAS),//        (LVM_FIRST + 65)
   TEXID(LVM_GETSELECTIONMARK),//    (LVM_FIRST + 66)
   TEXID(LVM_SETSELECTIONMARK),//    (LVM_FIRST + 67)
   TEXID(LVM_SETBKIMAGEA),//         (LVM_FIRST + 68)
   TEXID(LVM_GETBKIMAGEA),//         (LVM_FIRST + 69)
   TEXID(LVM_GETWORKAREAS),//        (LVM_FIRST + 70)
   TEXID(LVM_SETHOVERTIME),//        (LVM_FIRST + 71)
   TEXID(LVM_GETHOVERTIME),//        (LVM_FIRST + 72)
   TEXID(LVM_GETNUMBEROFWORKAREAS),//(LVM_FIRST + 73)
   TEXID(LVM_SETTOOLTIPS),//         (LVM_FIRST + 74)
   TEXID(LVM_GETTOOLTIPS),//         (LVM_FIRST + 78)
   TEXID(LVM_SETBKIMAGEW),//         (LVM_FIRST + 138)
   TEXID(LVM_GETBKIMAGEW),//         (LVM_FIRST + 139)
   TEXID(LVN_ITEMCHANGING),//        (LVN_FIRST-0)
   TEXID(LVN_ITEMCHANGED),//         (LVN_FIRST-1)
   TEXID(LVN_INSERTITEM),//          (LVN_FIRST-2)
   TEXID(LVN_DELETEITEM),//          (LVN_FIRST-3)
   TEXID(LVN_DELETEALLITEMS),//      (LVN_FIRST-4)
   TEXID(LVN_BEGINLABELEDITA),//     (LVN_FIRST-5)
   TEXID(LVN_BEGINLABELEDITW),//     (LVN_FIRST-75)
   TEXID(LVN_ENDLABELEDITA),//       (LVN_FIRST-6)
   TEXID(LVN_ENDLABELEDITW),//       (LVN_FIRST-76)
   TEXID(LVN_COLUMNCLICK),//         (LVN_FIRST-8)
   TEXID(LVN_BEGINDRAG),//           (LVN_FIRST-9)
   TEXID(LVN_BEGINRDRAG),//          (LVN_FIRST-11)
   TEXID(LVN_ODCACHEHINT),//         (LVN_FIRST-13)
   TEXID(LVN_ODFINDITEMA),//         (LVN_FIRST-52)
   TEXID(LVN_ODFINDITEMW),//         (LVN_FIRST-79)
   TEXID(LVN_ITEMACTIVATE),//        (LVN_FIRST-14)
   TEXID(LVN_ODSTATECHANGED),//      (LVN_FIRST-15)
   TEXID(LVN_HOTTRACK),//            (LVN_FIRST-21)
   TEXID(LVN_GETDISPINFOA),//        (LVN_FIRST-50)
   TEXID(LVN_GETDISPINFOW),//        (LVN_FIRST-77)
   TEXID(LVN_SETDISPINFOA),//        (LVN_FIRST-51)
   TEXID(LVN_SETDISPINFOW),//        (LVN_FIRST-78)
   TEXID(LVN_MARQUEEBEGIN),//        (LVN_FIRST-56)
   TEXID(LVN_GETINFOTIPA),//         (LVN_FIRST-57)
   TEXID(LVN_GETINFOTIPW),//         (LVN_FIRST-58)
   {0, NULL} };

CString decode(UINT msg)
{
   for (t_MESS *pt = messages; pt->text; pt++)
      if (pt->id == msg) return pt->text;
   CString text;
   text.Format("WS_%08X", msg);
   return text;
}

typedef struct { LPCTSTR res; LPCTSTR text; } t_RES;

static t_RES resource[] = {
   TEXID(RT_ACCELERATOR),
   TEXID(RT_ANICURSOR),
   TEXID(RT_ANIICON),
   TEXID(RT_BITMAP),
   TEXID(RT_CURSOR),
   TEXID(RT_DIALOG),
   TEXID(RT_DLGINIT),
   TEXID(RT_FONT),
   TEXID(RT_FONTDIR), 
   TEXID(RT_GROUP_CURSOR),
   TEXID(RT_GROUP_ICON),
   TEXID(RT_HTML),
   TEXID(RT_ICON),
   TEXID(RT_MENU),
   TEXID(RT_MESSAGETABLE),
   TEXID(RT_PLUGPLAY),
   TEXID(RT_RCDATA), 
   TEXID(RT_STRING),
   TEXID(RT_TOOLBAR),
   TEXID(RT_VERSION),
   TEXID(RT_VXD),
   {NULL, NULL} };

CString decodeRes(LPCTSTR resType)
{
   for (t_RES *pt = resource; pt->text; pt++)
      if (pt->res == resType) return pt->text;
   CString text;
   text.Format("RT_%08X", resType);
   return text;
}
#endif
