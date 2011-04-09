// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "ResExplorer.h"
#include "ResDoc.h"
#include "MsgDecode.h"

#include "DirPickDlg.h"
#include "UpperView.h"
#include "LeftView.h"
#include "DrawView.h"
#include "EdtView.h"
#include "ReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_SYMBOL_DLG, OnSymbolDlg)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
//	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
//	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

   m_symDlg.Create(this);
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	                  CCreateContext* pContext)
{
	// create splitter window
   if (!m_Splitter_1.CreateStatic(this, 2, 1) ||
	    !m_Splitter_2.CreateStatic(&m_Splitter_1, 1, 2, WS_CHILD|WS_VISIBLE,
                                                        m_Splitter_1.IdFromRowCol(1,0)) )
		return FALSE;

   CRect rect;
   GetClientRect(rect);
   CSize uvSize(0, rect.bottom / 6);
   CSize lvSize(rect.right / 5, 0);

   // create view
   if (!m_Splitter_1.CreateView(0, 0, RUNTIME_CLASS(CUpperView), uvSize, pContext) ||
       !m_Splitter_2.CreateView(0, 0, RUNTIME_CLASS(CLeftView),  lvSize, pContext) ||
       !m_Splitter_2.CreateView(0, 1, RUNTIME_CLASS(CDrawView),  lvSize, pContext) )
	   return FALSE;

   m_flg = VRS;     // CResExplorerView is created
	return TRUE;
}

BOOL CMainFrame::CreateView(UINT flg)  // flg=> VLS : Affichage de la fenetre CReportView
{                                      //       VRS : Affichage de la fenetre CDrawView a la place CEditView
   flg &= VLS|VRS|VDT;                 //       VDT : Affichage de la fenetre CEditView a la place CDrawView
   if (m_flg == flg) return TRUE;

   CCreateContext context;
   context.m_pCurrentDoc = GetActiveDocument();
   context.m_pCurrentFrame = this;
   CRect rect;
   m_Splitter_2.GetClientRect(rect);
   CSize lvSize = rect.Size();
   CSize evSize(0, rect.bottom / 2);
   CSize elSize(0, rect.bottom / 2);

   // Efface les anciennes vues
   if (m_flg == (VLS|VRS) || m_flg == (VLS|VDT)) m_Splitter_3.DestroyWindow();
   else m_Splitter_2.DeleteView(0, 1);

   // affiche les nouvelles vues
   if (flg == (VLS|VRS)) {
      m_Splitter_3.CreateStatic(&m_Splitter_2, 2, 1, WS_CHILD|WS_VISIBLE,
                                                       m_Splitter_2.IdFromRowCol(0,1));
      m_Splitter_3.CreateView(1, 0, RUNTIME_CLASS(CReportView), elSize, &context);
      m_Splitter_3.CreateView(0, 0, RUNTIME_CLASS(CDrawView),   evSize, &context);
   } else if (flg == (VLS|VDT)) {
      m_Splitter_3.CreateStatic(&m_Splitter_2, 2, 1, WS_CHILD|WS_VISIBLE,
                                                       m_Splitter_2.IdFromRowCol(0,1));
      m_Splitter_3.CreateView(1, 0, RUNTIME_CLASS(CReportView), elSize, &context);
      m_Splitter_3.CreateView(0, 0, RUNTIME_CLASS(CEdtView),    evSize, &context);
   } else if (flg == VLS) {
      m_Splitter_2.CreateView(0, 1, RUNTIME_CLASS(CReportView), lvSize, &context);
   } else if (flg == VRS) {
      m_Splitter_2.CreateView(0, 1, RUNTIME_CLASS(CDrawView),   lvSize, &context);
   } else {       // implicite flg == VRS
      m_Splitter_2.CreateView(0, 1, RUNTIME_CLASS(CDrawView),   lvSize, &context);
   }

   InitialUpdateFrame(NULL, TRUE);
   m_Splitter_2.RecalcLayout();
   
   m_flg = flg;
   return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnFileOpen() 
{
   CResDoc *pDoc = (CResDoc*)GetActiveDocument();
	CDirPickDlg dlg;
   dlg.m_filter = "Programme (*.exe;*.dll;*.ocx)|*.exe;*.dll;*.ocx|"
                  "Exe  (*.exe)|*.exe|"
                  "Dll  (*.dll)|*.dll|"
                  "ActiveX (*.ocx)|*.ocx|"
                  "All  (*.*)|*.*|";
   dlg.m_path = pDoc->GetPathName();

   if (dlg.DoModal() == IDOK) {
      pDoc->SetPath(NULL, dlg.m_path, dlg.m_curFilter);
      AfxGetApp()->OpenDocumentFile(dlg.m_path);
   }
}

void CMainFrame::OnSymbolDlg() 
{
   m_symDlg.CenterWindow(this);
   m_symDlg.ShowWindow(m_symDlg.IsWindowVisible()? SW_HIDE: SW_SHOW);
//   m_symDlg.DoModal();
}

void CMainFrame::WinHelp(DWORD dwData, UINT nCmd) 
{
   if (nCmd == HELP_CONTEXT && dwData == (0x20000 + IDR_MAINFRAME)) {
      POINT pt;
      ::GetCursorPos(&pt);
      HWND hWnd = ::WindowFromPoint(pt);
      if (hWnd) {
         DWORD data = ::GetWindowContextHelpId(hWnd);
         if (data > 0) dwData = data;
      }
   }
	CFrameWnd::WinHelp(dwData, nCmd);
}
