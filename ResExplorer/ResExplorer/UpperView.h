#if !defined(AFX_UPPERVIEW_H__92E81D3F_2B9B_4DFD_9312_D77A5767B29A__INCLUDED_)
#define AFX_UPPERVIEW_H__92E81D3F_2B9B_4DFD_9312_D77A5767B29A__INCLUDED_

#include "ResDoc.h"
#include "TooltipEx.h"
#include "Utilities.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpperView.h : header file
//

typedef struct {
   LPSTR id;
   int count;
   int size;
} t_RESCNT;

typedef struct {
   LPSTR id;
   LPSTR name;
   t_RESCNT *pR;
   int col;
} t_RESINDEX;

/////////////////////////////////////////////////////////////////////////////
// CUpperView view

class CUpperView : public CListView
{
protected:
	CUpperView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CUpperView)

// Attributes
public:
	CResDoc* GetDocument();
protected: // create from serialization only
   int m_oldSelect;
   int m_subItemLast;
   int m_fileCnt;
   CTooltipList m_toolView;
   CParams *m_pParam;

// Operations
	static t_RESCNT m_resTbl[];
   static t_RESINDEX m_ixTbl[];
	static BOOL WINAPI EnumNamesFunc(HMODULE hModule, LPCTSTR lpType, LPSTR lpName, LPARAM lParam);
	static BOOL WINAPI EnumLangsFunc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLang, LPARAM lParam);
	static BOOL WINAPI EnumTypesFunc(HMODULE hModule, LPSTR lpType, LPARAM lParam);
   CString InitIndexTbl(int col);
   void ResourceCount(LPCTSTR path, WIN32_FIND_DATA *pFD);
	BOOL ReadList(LPCTSTR path, LPCTSTR selPath);
	BOOL DirectorySearch(LPCTSTR path, LPCTSTR selPath);
public:
	BOOL NewList();
   BOOL ClipBoardList();

// Operations
protected:
   void PutSortMessage();
	void ReadItem(int no, BOOL AddToRecentFileList);
	void ReadSelected(BOOL AddToRecentFileList);
   static UINT ThreadProcNL(LPVOID pParam);
   static UINT ThreadProcCB(LPVOID pParam);
   CWinThread* m_pThread;
   HCURSOR m_hCursor;
   HCURSOR m_hCurWait;
   class COleDataSource* m_pSource;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpperView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetPathName(int no);
	virtual ~CUpperView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
   //{{AFX_MSG(CUpperView)
	afx_msg void OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnEditPaste();
   afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnSortColumn(UINT id);
   afx_msg LRESULT OnToolTipText(WPARAM, LPARAM);
   afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
   afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
   DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DirView.cpp
inline CResDoc* CUpperView::GetDocument() { return (CResDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPPERVIEW_H__92E81D3F_2B9B_4DFD_9312_D77A5767B29A__INCLUDED_)
