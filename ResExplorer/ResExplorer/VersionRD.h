#if !defined(AFX_VERSIONRD_H__3EA662C8_C8E8_4C1C_BD07_8376DCB86833__INCLUDED_)
#define AFX_VERSIONRD_H__3EA662C8_C8E8_4C1C_BD07_8376DCB86833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VersionRD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVersionRD
extern UINT VersionInit(HINSTANCE hinst, LPCTSTR resName, CString& name, CString& item);
extern BOOL FixedFileInfo(UINT& n, CString& name, CString& item);
extern BOOL VersionDecod(PWORD& pW, CString& name, CString& item);
extern LPCTSTR GetLang(WORD code);

extern CString DisplayKbStr(RACCEL *pAc);
extern CString DisplayKbType(RACCEL *pAc);
extern CString* DumpAccel(HINSTANCE hinst, LPCTSTR resName);

extern CString AviHeader(PDWORD pDWord, UINT item);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERSIONRD_H__3EA662C8_C8E8_4C1C_BD07_8376DCB86833__INCLUDED_)
