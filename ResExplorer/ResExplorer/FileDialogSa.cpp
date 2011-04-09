// FileDialogSa.cpp : implementation file
//

#include "stdafx.h"
#include "FileDialogSa.h"
#include <DLGS.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CFileDialogSa

LPOFNHOOKPROC CFileDialogSa::m_defHook = NULL;

IMPLEMENT_DYNAMIC(CFileDialogSa, CFileDialog)

static DWORD uiLBSELCHSTRING = 0;

CFileDialogSa::CFileDialogSa(LPCTSTR lpszFilter, LPCTSTR lpszFileName,
                              DWORD dwFlags, CWnd* pParentWnd) :
   CFileDialog(FALSE, m_defExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
   m_defExt[0] = 0;
   m_defHook = m_ofn.lpfnHook;
   m_ofn.lpfnHook = fnHook;

   uiLBSELCHSTRING = ::RegisterWindowMessage(LBSELCHSTRING);
}

void CFileDialogSa::SetNames(LPCTSTR lpszFilter, LPCTSTR lpszFileName)
{
   if (lpszFileName == NULL) lpszFileName = "";
   m_ofn.lpstrFilter = lpszFilter;
   lstrcpy(LPSTR(m_ofn.lpstrFile), lpszFileName);

   // Translate filter into commdlg format (lots of \0)
   if (lpszFilter) {
      m_strFilter = lpszFilter;
      LPTSTR pch = m_strFilter.GetBuffer(0); // modify the buffer in place
      // MFC delimits with '|' not '\0'
      while ((pch = _tcschr(pch, '|')) != NULL) *pch++ = '\0';
      m_ofn.lpstrFilter = m_strFilter;
      // do not call ReleaseBuffer() since the string contains '\0' characters
	}
}

CString CFileDialogSa::GetDefExtension(BOOL point)
{
   CString ext(m_defExt);
   if (ext.IsEmpty() || point == FALSE) return ext;
   else return "." + ext;
}

BEGIN_MESSAGE_MAP(CFileDialogSa, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogSa)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Ajoute l'extension au nom du fichier
void CFileDialogSa::updExtension(OPENFILENAME *pOfn, int no)
{
   LPSTR pTmp, pFilter = LPSTR(pOfn->lpstrFilter);

   // extraire l'extension selectee
   if (pFilter) {
      while ( --no >= 0 && *pFilter ) pFilter += lstrlen(pFilter) + 1;
      pFilter = strchr( pFilter, '.' );
   }
   if ( pFilter == NULL || pFilter[1] == '*' ) pFilter = "\0";
   // pour forcer l'extension sur les noms de fichiers
   if (pOfn->lpstrDefExt) lstrcpy( LPSTR(pOfn->lpstrDefExt), pFilter+1 );

   // garder le nom sans les sous-directories
   pTmp = strrchr( pOfn->lpstrFile, '\\' );
   if (pTmp == NULL) pTmp = strrchr( pOfn->lpstrFile, ':' );
   if (pTmp) lstrcpy( LPSTR(pOfn->lpstrFile), pTmp+1 );

   // modifier ou ajouter l'extension
   pTmp = strrchr( pOfn->lpstrFile, '.' );   // ancienne extension ?
   if (pTmp) lstrcpy( pTmp, pFilter );       // modifier l'extension
   else lstrcat( LPSTR(pOfn->lpstrFile), pFilter ); // ou ajouter
}

// Explorer-Style Hook Procedures
UINT CALLBACK CFileDialogSa::fnHook(HWND hdlg,// handle to dialog box window
                             UINT uiMsg,      // message identifier
                             WPARAM wParam,   // message parameter
                             LPARAM lParam)   // message parameter
{
   static OPENFILENAME *pOfn = NULL;

   UINT ret = (m_defHook)(hdlg, uiMsg, wParam, lParam);

   if (uiMsg == WM_INITDIALOG) {
      pOfn = (OPENFILENAME*)lParam;
      updExtension( pOfn, 1 );
      ::SetDlgItemText(::GetParent(hdlg), edt1, pOfn->lpstrFile); 

   } else if (uiMsg == uiLBSELCHSTRING) {
      // boite de dialogue de choix des extensions
      ::GetDlgItemText(::GetParent(hdlg), edt1, pOfn->lpstrFile, pOfn->nMaxFile);
      updExtension( pOfn, (lParam << 1) + 1 );
      ::SetDlgItemText(::GetParent(hdlg), edt1, pOfn->lpstrFile); 
   }
//   TRACE("fnHook Msg=%08x wp=%08x lp=%08x %s\n",
//                  uiMsg, wParam, lParam, pOfn ? pOfn->lpstrFile: "");
   return ret;
}
