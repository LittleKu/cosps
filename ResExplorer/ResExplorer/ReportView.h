// ReportView.h : header file
#if !defined(AFX_REPORTVIEW_H__97E71E8C_C2A2_4F12_B3BC_053FF8C8A946__INCLUDED_)
#define AFX_REPORTVIEW_H__97E71E8C_C2A2_4F12_B3BC_053FF8C8A946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "ResUtilities.h"
#include "TooltipEx.h"

/////////////////////////////////////////////////////////////////////////////
// CReportView view
class CResDoc;

class CReportView : public CListView
{
protected:
	CReportView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CReportView)

// Attributes
public:
	CResDoc* GetDocument();

// Operations
public:
   BOOL SearchSymbol(const CString &symbol);
	void CallCopyPastMenu();
	BOOL InfoRIFF(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
   BOOL Chunk(PDWORD pDWord, DWORD dwSize, DWORD type, int image);
   static BOOL m_details;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
   class CParams *m_pParam;
   LPCTSTR     m_lpMenu;
   UINT        m_menuItem;
   ANI_ICON    *m_pAicon;
   CTooltipList m_toolView;
   DWORD       m_type;           // symbol preference display type

   BOOL        m_trueText;
   BOOL        m_quotText;
//   CString     m_copyPast;
	void CacheTextCopy(BOOL cb);

   static CNodTbl m_typeDlg[1];  // tooltips and help

   void InitListHeader();
   void DialogList(HINSTANCE hinst, LPCTSTR resName);
   CString ReadStringDec(PWORD &pt);
   CString ReadClass(PWORD &pt, DWORD &clsStyle, int &image, DWORD style);
	void AccelList(HINSTANCE hinst, LPCTSTR resName);
   void DlgInitList(HINSTANCE hinst, LPCTSTR resName);
   void StringList(HINSTANCE hinst, LPCTSTR resName);
   void ImagesList(HINSTANCE hinst, LPCTSTR resName);
   void IconesList(HINSTANCE hinst, LPCTSTR resName, LPCTSTR resType);
   void ToolbarList(HINSTANCE hinst, LPCTSTR resName);
   void MenuList(HINSTANCE hinst, LPCTSTR resName);
   int MenuCount(HMENU hMenu);
   CString GetToolHelpMessage(HINSTANCE hinst, UINT id, int m = 0);
   void HexaList(HINSTANCE hinst, LPCTSTR resType, LPCTSTR resName);
   void VersionList(HINSTANCE hinst, LPCTSTR resName);
   CString ReadText(PWORD &pt, int len);

   void SubmenuList(HINSTANCE hinst, HMENU hMenu, int image);
   static UINT ThreadProc(LPVOID pParam);
   LPVOID m_pWave;

protected:
	virtual ~CReportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CReportView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCopy();
	afx_msg void OnEdrvQuot();
	afx_msg void OnEdrvTrue();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
   afx_msg LRESULT OnToolTipText(UINT id, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CReportView.cpp
inline CResDoc* CReportView::GetDocument() { return (CResDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTVIEW_H__97E71E8C_C2A2_4F12_B3BC_053FF8C8A946__INCLUDED_)
