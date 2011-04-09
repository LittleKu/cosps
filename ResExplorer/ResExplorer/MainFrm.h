// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BCA3EFAE_1A5F_4F18_A165_59556FB21A68__INCLUDED_)
#define AFX_MAINFRM_H__BCA3EFAE_1A5F_4F18_A165_59556FB21A68__INCLUDED_

#include "Splitter.h"
#include "SymbolDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CDrawView;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:

// Operations
public:
	BOOL CreateView(UINT flg);

   inline BOOL SymbolSelect(LPCTSTR pszName) { return m_symDlg.SymbolSelect(pszName); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL
//	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Attributes
protected:
	CSplitterWndEx m_Splitter_1;
	CSplitterWndEx m_Splitter_2;
	CSplitterWndEx m_Splitter_3;
   UINT           m_flg;
protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
   CSymbolDlg  m_symDlg;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSymbolDlg();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BCA3EFAE_1A5F_4F18_A165_59556FB21A68__INCLUDED_)
