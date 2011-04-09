// DlgDataRW.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DLGDATARW_H__57C14705_6C0B_4B9A_A98A_E6A0E70DB1EA__INCLUDED_)
#define AFX_DLGDATARW_H__57C14705_6C0B_4B9A_A98A_E6A0E70DB1EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

extern UINT WriteDlg(CString fnam, HINSTANCE hinst, LPCTSTR resName);
extern UINT WriteMenu(CString fnam, HINSTANCE hinst, LPCTSTR resName);
extern UINT WriteStringTable(CString fnam, HINSTANCE hinst, LPCTSTR resName);

extern VOID BufferEmpty();

extern CString* DumpDialog( HINSTANCE hinst, LPCTSTR resName );
extern CString* DumpDlgInit(PVOID pDlg, const CString &path, BOOL sn);
extern CString* DumpMenu(HINSTANCE hinst, LPCTSTR resName);
extern CString* DumpStringTable(HINSTANCE hinst, LPCTSTR resName);

#endif // !defined(AFX_DLGDATARW_H__57C14705_6C0B_4B9A_A98A_E6A0E70DB1EA__INCLUDED_)
