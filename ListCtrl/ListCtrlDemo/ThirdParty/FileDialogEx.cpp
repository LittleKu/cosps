#include "stdafx.h"
#include <afxpriv.h>
#include "FileDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL IsWin2000();

///////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog,
   LPCTSTR lpszDefExt,
   LPCTSTR lpszFileName,
   DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
   CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
      dwFlags, lpszFilter, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
   //{{AFX_MSG_MAP(CFileDialogEx)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL IsWin2000() 
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure,
   // which is supported on Windows 2000.
   //
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
      case VER_PLATFORM_WIN32_NT:

         if ( osvi.dwMajorVersion >= 5 )
            return TRUE;

         break;
   }
   return FALSE; 
}

int CFileDialogEx::DoModal()
{
   ASSERT_VALID(this);
   ASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
   ASSERT(m_ofn.lpfnHook != NULL); // can still be a user hook

   // zero out the file buffer for consistent parsing later
   ASSERT(AfxIsValidAddress(m_ofn.lpstrFile, m_ofn.nMaxFile));
   DWORD nOffset = lstrlen(m_ofn.lpstrFile)+1;
   ASSERT(nOffset <= m_ofn.nMaxFile);
   memset(m_ofn.lpstrFile+nOffset, 0, 
         (m_ofn.nMaxFile-nOffset)*sizeof(TCHAR));

   // WINBUG: This is a special case for the file open/save dialog,
   //  which sometimes pumps while it is coming up but before it has
   //  disabled the main window.
   HWND hWndFocus = ::GetFocus();
   BOOL bEnableParent = FALSE;
   m_ofn.hwndOwner = PreModal();
   AfxUnhookWindowCreate();
   if (m_ofn.hwndOwner != NULL && ::IsWindowEnabled(m_ofn.hwndOwner))
   {
      bEnableParent = TRUE;
      ::EnableWindow(m_ofn.hwndOwner, FALSE);
   }

   _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
   ASSERT(pThreadState->m_pAlternateWndInit == NULL);

   if (m_ofn.Flags & OFN_EXPLORER)
      pThreadState->m_pAlternateWndInit = this;
   else
      AfxHookWindowCreate(this);

   memset(&m_ofnEx, 0, sizeof(m_ofnEx));
   memcpy(&m_ofnEx, &m_ofn, sizeof(m_ofn));
   if (IsWin2000())
      m_ofnEx.lStructSize = sizeof(m_ofnEx);

   int nResult;
   if (m_bOpenFileDialog)
      nResult = ::GetOpenFileName((OPENFILENAME*)&m_ofnEx);
   else
      nResult = ::GetSaveFileName((OPENFILENAME*)&m_ofnEx);

   memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
   m_ofn.lStructSize = sizeof(m_ofn);

   if (nResult)
      ASSERT(pThreadState->m_pAlternateWndInit == NULL);
   pThreadState->m_pAlternateWndInit = NULL;

   // WINBUG: Second part of special case for file open/save dialog.
   if (bEnableParent)
      ::EnableWindow(m_ofnEx.hwndOwner, TRUE);
   if (::IsWindow(hWndFocus))
      ::SetFocus(hWndFocus);

   PostModal();
   return nResult ? nResult : IDCANCEL;
}

BOOL CFileDialogEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
   memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
   m_ofn.lStructSize = sizeof(m_ofn);

   return CFileDialog::OnNotify( wParam, lParam, pResult);
}

////////////////////////////////////////////////////////////////
// The following functions are provided just to demonstrate that they
// get called; ie, that MFC's internal dialog proc is in fact hooked
// up. You can delete them if you like.
//
/*
BOOL CFileDialogEx::OnFileNameOK()
{
   TRACE(_T("CFileDialogEx::OnFileNameOK\n"));
   return CFileDialog::OnFileNameOK();
}

void CFileDialogEx::OnInitDone()
{
   TRACE(_T("CFileDialogEx::OnInitDone\n"));
   CFileDialog::OnInitDone();
}

void CFileDialogEx::OnFileNameChange()
{
   TRACE(_T("CFileDialogEx::OnFileNameChange\n"));
   CFileDialog::OnFileNameChange();
}

void CFileDialogEx::OnFolderChange()
{
   TRACE(_T("CFileDialogEx::OnFolderChange\n"));
   CFileDialog::OnFolderChange();
}

void CFileDialogEx::OnTypeChange()
{
   TRACE(_T("OnTypeChange(), index = %d\n"), m_ofn.nFilterIndex);
   CFileDialog::OnTypeChange();
}
*/