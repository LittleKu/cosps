#if !defined(AFX_SYMBOLDLG_H__4B1F8235_2079_434B_98CC_5185ED289FCE__INCLUDED_)
#define AFX_SYMBOLDLG_H__4B1F8235_2079_434B_98CC_5185ED289FCE__INCLUDED_

#include "ListCtrlEx.h"
#include "Resource.h"
#include "ResUtilities.h"
#include <afxtempl.h>
#include <afxole.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymbolDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CODropTarget

class CODropTarget : public COleDropTarget
{
// Operations
public:
virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject,
                               DWORD dwKeyState, CPoint point );
virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject,
                                             DWORD dwKeyState, CPoint point );
virtual BOOL OnDrop( CWnd* pWnd, COleDataObject* pDataObject,
                                        DROPEFFECT dropEffect, CPoint point );
};
/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog

class CSymbolDlg : public CDialog
{
// Construction
public:
	CSymbolDlg(CWnd* pParent = NULL);   // standard constructor
   BOOL Create(CWnd* pParent = NULL);
   inline void Cancel() { OnCancel(); }
   inline void OK()     { OnOK(); }

   DROPEFFECT OnDragEnter(COleDataObject* pDataObject);
   DROPEFFECT OnDragOver(COleDataObject* pDataObject, CPoint point);
   BOOL OnDrop(COleDataObject* pDataObject);
   CODropTarget m_DropTarget;

// Dialog Data
	//{{AFX_DATA(CSymbolDlg)
	enum { IDD = IDD_SYMBOL_DLG };
	CListCtrlEx	m_list;
	//}}AFX_DATA
	static BOOL	m_chkSymExt;
	static BOOL	m_chkSymStd;
	static BOOL	m_chkDefine;
   static BOOL m_chkOutHex;
   bool m_chkMltDef;

public:
   static inline int Count() { return m_symbols.GetCount(); }
   static CString GetString(DWORD value, DWORD type);
   static void SymbolGen(DWORD value, DWORD type);

   BOOL SymbolSelect(LPCTSTR pszName);

   int Read(CString path, bool erase = true, bool upd = true);
   int m_duplic;

protected:
   void Delete();
   void SelectAll();
   void UpdateAllViews();
   void AddStandards();
   void CacheTextCopy(BOOL clipboard);
   int ReadFiles(COleDataObject *pObj, bool upd);
   int GetClipboardData(bool upd);
   int GetData(COleDataObject *pObj, bool upd);
   int ParseLine(const CString &line, bool upd=true);
   CString LineExtract(CMemFile &sf, int& cur);
   CString Word(const CString &line, int &first, int last);
   BOOL Convert(const CString &text, DWORD &out);
   int SearchType(t_SYMBOL &sym);
   bool ListUpdate(bool mlt);
   void ItemUpdate(int iItem, POSITION pos);
   CString FormatValue(t_SYMBOL &symbol);
   UINT SymbolUpdate(LVITEM &item);
   void SymbolLocate(int iItem);
   void SearchSymbol(int iItem);
   LPCTSTR GetLocResType();
   void ListSort(int state);
   void UpdateBtnState();
   void PutSortMessage();
   void SymbolCutFat(bool add);

	static int Qualifie(bool all);
   static POSITION SymbolAdd(t_SYMBOL &symbol);
   static POSITION GetPosition(const CString &name);
   static POSITION m_lastFound;
   static CSymbolList m_symbols;
   CSymbolList m_symSave;

   CSymbolList m_loc;
   int m_iItemOld;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   CRect m_dragOver;
   int m_iItem, m_iSubItem;

	// Generated message map functions
	//{{AFX_MSG(CSymbolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSymfile();
	afx_msg void OnBtnGenerate();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSymCopy();
	afx_msg void OnSymDelete();
	afx_msg void OnSymDeleteall();
	afx_msg void OnSymFormat();
	afx_msg void OnSymPaste();
	afx_msg void OnSymReplace();
	afx_msg void OnSymSelectall();
	afx_msg void OnSymModifie();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSmultidef();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnChkSymbol();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSymSearch();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSymCutfat();
	afx_msg void OnRdblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnHsymSort(UINT id);
   afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CListBoxDlg dialog

class CListBoxDlg : public CDialog
{
// Construction
public:
	CListBoxDlg(CSymbolList &loc, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CListBoxDlg)
	enum { IDD = IDD_LISTBOX_DLG };
	//}}AFX_DATA

   int selected;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:
   CString list;
   int count, bottom;
   CPoint point;
   CFont *pFont;

	// Generated message map functions
	//{{AFX_MSG(CListBoxDlg)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLDLG_H__4B1F8235_2079_434B_98CC_5185ED289FCE__INCLUDED_)
