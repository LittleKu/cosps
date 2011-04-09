// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__4105EA65_948F_4990_B5B1_D02BFFF73B3C__INCLUDED_)
#define AFX_LEFTVIEW_H__4105EA65_948F_4990_B5B1_D02BFFF73B3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

   // utilise pour la construction de l'arbre des resources
typedef struct {
   CTreeCtrl *pTree;
   HTREEITEM item;
   int       dimage;
   DWORD     type; // pour lever les anbiguites dans la conversion des symboles
} t_NODE;

class CResDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CResDoc* GetDocument();

// Operations
public:
	LPCTSTR GetNextName(LPCTSTR type, HTREEITEM& hit);
	int GetItemCount(LPCTSTR lpType);
	CTreeCtrl *NewTree();
   void TreeUpdate();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CParams *m_pParam;
   static HTREEITEM TreeSelect(CTreeCtrl *pTree, LPCTSTR lpType, LPCTSTR lpName, BOOL expand);
	static BOOL WINAPI EnumTypesFunc(HMODULE hModule, LPSTR lpType, LPARAM lParam);
	static BOOL WINAPI EnumNamesFunc(HMODULE hModule, LPCTSTR lpType, LPSTR lpName, LPARAM lParam);
	static BOOL WINAPI EnumLangsFunc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLang, LPARAM lParam);

// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CResDoc* CLeftView::GetDocument() { return (CResDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__4105EA65_948F_4990_B5B1_D02BFFF73B3C__INCLUDED_)
