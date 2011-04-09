// ResDoc.h : interface of the CResDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESDOC_H__E26C8094_091D_427A_9060_025DF5DDE7EF__INCLUDED_)
#define AFX_RESDOC_H__E26C8094_091D_427A_9060_025DF5DDE7EF__INCLUDED_

#include "ResUtilities.h"
#include "ResInfo.h"
#include "ToolbarDlg.h"
#include "BitmapDlg.h"
#include "FileDialogSa.h"
#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum {
   UPPER_VIEW=0x0001, UPPER_VIEW_DIR  =0x0002,
   LEFT_VIEW =0x0010, LEFT_VIEW_SELECT=0x0020, LEFT_VIEW_UPDAT =0x0040,
   LIST_VIEW =0x0100, LIST_VIEW_ITEM  =0x0200, LIST_VIEW_SYMBOL=0x0400,
   RES_VIEW  =0x1000, RES_VIEW_ITEM   =0x2000
};
typedef struct { LPCTSTR resName, resType; t_ResID *pRID; DWORD item;} t_PSAVE;
typedef struct { LPCTSTR resName, resType; } t_RESDEF;

class CResList : public CList<t_RESDEF, t_RESDEF&>
{
   int Compare(POSITION cur, t_RESDEF &res);
public:
   CResList() {}
   ~CResList();// {};
   void Add(LPCTSTR resName, LPCTSTR resType);
   void RemoveAll() { while (!IsEmpty()) RemoveHead(); }
};

class CParams : public CObject  
{
protected:
   t_PSAVE     sv[3];
public:
   void SEmpty() { sv[0].pRID = NULL; }
   void Reset() { resType = resName = NULL, hLib = NULL, pAni = NULL,
                                 pRID = NULL, pTree = NULL, item = 0, SEmpty(); }
   void Save(LPCTSTR resName, LPCTSTR resType);
   BOOL Restore();
   BOOL IsUndo() { return (sv[0].pRID && (sv[0].resName != resName ||
                                                sv[0].resType != resType)); }

   CParams(BOOL fr = TRUE) { freeEn = fr; Reset(); }
   virtual ~CParams() { CParams::FreeLibrary(); }
   void FreeAni(ANI_ICON *pA = NULL) { if (pAni) free(pAni); pAni = pA; }
   void FreeLibrary();
   BOOL NewLibrary(LPCTSTR lpszPathName);
   WORD GetView();
   CString GetFilter();
   CString GetDefName();

   LPCTSTR NextResource(HTREEITEM& pos, LPCTSTR lpType = NULL);
   LPCTSTR NextResourceType(HTREEITEM& pos);

   CTreeCtrl  *pTree;
   t_ResID    *pRID;

   BOOL        limited;
   BOOL        freeEn;
   HINSTANCE   hLib;
   LPCTSTR     resType;
   LPCTSTR     resName;
   DWORD       item;

   CString     filesPath;  // path de recherche des fichiers
   CString     selPath;    // selection des fichiers
   CString     libName;    // path du fichier en cours
   CString     libTitle;   // titre de la librairie
   CString     symbol;     // last symbol used in reference

   ANI_ICON    *pAni;
};

class CResDoc : public CDocument
{
protected: // create from serialization only
	CResDoc();
	DECLARE_DYNCREATE(CResDoc)

// Attributes
protected:
   static CParams m_param;
   CFileDialogSa* m_pOutFile;

   UINT WriteRes(CString fnam, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
   UINT WriteRes(CFile &file, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
   UINT WriteResLoop(CFile &file, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType, LPCTSTR resType2);
   UINT WriteResIconLoop(CFile &file, HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
   void CopyOnClipbord(UINT uFormat, HANDLE pData, DWORD size);
   void CopyOnClipbord(UINT uFormat, HGLOBAL hGlobal);
   UINT FileSave(CString fnam);
	void ResourceVerify(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
   void TypeModifie();

   void SymbolResTypeUpd();
   UINT SymbolDialogUpd(HINSTANCE hinst);
   void SymbolStringUpd(HINSTANCE hinst);
   void SymbolMenuUpd(HINSTANCE hinst);
   void SymbolSubmenuUpd(HINSTANCE hinst, HMENU hMenu);
   void SymbolAccelUpd(HINSTANCE hinst);

   UINT IsStringValid(t_SYMBOL &symbol);
   int MenuCount(LPCTSTR resName, t_SYMBOL &symbol);
   int SubmenuCount(HMENU hMenu, t_SYMBOL &symbol);
   int DialogCount(LPCTSTR resName, t_SYMBOL &symbol);
   int AccelCount(LPCTSTR resName, t_SYMBOL &symbol);

// Operations
public:
   TOOLBARRES m_toolbar;
   static inline HINSTANCE GetResLib() { return m_param.hLib; }
   static inline LPCTSTR GetResName() { return m_param.resName; }
   BOOL m_largeIcon;

   UINT WriteAddonIcon(CFile &file, HINSTANCE hinst, const CResList &list);
   UINT SearchCDiagAddon(HINSTANCE hinst, LPCTSTR resName, CResList &list);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
   void SymbolUpdate();
   void SearchSymbol(const CString &symbol);
   UINT SymbolLocate(CSymbolList &counts, bool fast = false);

	void SetPath(CView* pSender, LPCTSTR filesPath, LPCTSTR selPath);
	void SelectItem(CView* pSender, UINT item, LPARAM view = RES_VIEW_ITEM|LIST_VIEW_ITEM);
	void SetCurrent(CView* pSender, LPCTSTR resType, LPCTSTR resName, LPARAM view = RES_VIEW|LIST_VIEW|LIST_VIEW_ITEM|RES_VIEW_ITEM);
   void SymbolOnly(DWORD id, CString symbol, DWORD type);
	void ReadDocument(LPCTSTR lpszPathName);
	virtual ~CResDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CToolbarDlg *m_pToolbar;

// Generated message map functions
protected:
	//{{AFX_MSG(CResDoc)
	afx_msg void OnMakeToolbar();
	afx_msg void OnEditCopy();
	afx_msg void OnFileSaveAs();
	afx_msg void OnPaneBack();
	afx_msg void OnUpdateMakeToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePaneBack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewLargeicon(CCmdUI* pCmdUI);
	afx_msg void OnViewLargeicon();
	afx_msg void OnBitmapCvrt();
	afx_msg void OnUpdateBitmapCvrt(CCmdUI* pCmdUI);
	afx_msg void OnDsplBkcolor();
	afx_msg void OnUpdateDsplBkcolor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESDOC_H__E26C8094_091D_427A_9060_025DF5DDE7EF__INCLUDED_)
