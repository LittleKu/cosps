// FileDialogSa.h : header file
#if !defined(AFX_FILEDIALOGSA_H__F9923301_E2B2_4D6D_BF8E_75EF4FCCD918__INCLUDED_)
#define AFX_FILEDIALOGSA_H__F9923301_E2B2_4D6D_BF8E_75EF4FCCD918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

/////////////////////////////////////////////////////////////////////////////
// CFileDialogSa dialog

class CFileDialogSa : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogSa)

public:
	CFileDialogSa(LPCTSTR lpszFilter = NULL, LPCTSTR lpszFileName = NULL,
		   DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		   CWnd* pParentWnd = NULL);
   // definition des extensions possibles et du nom de fichier par defaut
   void SetNames(LPCTSTR lpszFilter = NULL, LPCTSTR lpszFileName = NULL);
   // lecture de l'extension par defaut choisie
   CString GetDefExtension(BOOL point = FALSE);

protected:
   static LPOFNHOOKPROC m_defHook;
   static void updExtension(OPENFILENAME *pOfn, int no);
   static UINT CALLBACK fnHook(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
   TCHAR m_defExt[10];

//	virtual void OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);

	//{{AFX_MSG(CFileDialogSa)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIALOGSA_H__F9923301_E2B2_4D6D_BF8E_75EF4FCCD918__INCLUDED_)
