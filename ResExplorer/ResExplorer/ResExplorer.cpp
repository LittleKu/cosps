// ResExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "MsgDecode.h"
#include "TooltipEx.h"
#include "MainFrm.h"
#include "ResDoc.h"
#include "UpperView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResExplorer

BEGIN_MESSAGE_MAP(CResExplorer, CWinApp)
	//{{AFX_MSG_MAP(CResExplorer)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResExplorer construction

CResExplorer::CResExplorer()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CResExplorer object

CResExplorer theApp;

/////////////////////////////////////////////////////////////////////////////
// CResExplorer initialization

BOOL CResExplorer::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit()) {
		AfxMessageBox("Init OLE error");
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Free Tools"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

   m_image.Create(IDB_RESICONES, 16, 0, RGB(255, 0, 255));
   m_sIcon.Create(IDB_SORTICONS, 12, 0, RGB(255, 0, 255));

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CResDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CUpperView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
//	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

BOOL CResExplorer::WaitEndOfWork()
{
   // Wait a end of update
   MSG msg;
   while (::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE )) {
      if (!AfxGetApp()->PumpMessage()) {
         ::PostQuitMessage(0);
         return FALSE;
      }
   }
   return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnBtnProgram();
	afx_msg void OnResexicon();
	//}}AFX_MSG
   afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

// App command to run the dialog
void CResExplorer::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
   EnableToolTips();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_BTN_PROGRAM, OnBtnProgram)
	ON_BN_CLICKED(IDC_RESEXICON, OnResexicon)
	//}}AFX_MSG_MAP
   ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
   *pResult = (LRESULT)ToolTipNotify(pNMH);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

void CAboutDlg::OnBtnProgram() 
{
   DWORD id = GetDlgItem(IDC_BTN_PROGRAM)->GetWindowContextHelpId();
   WinHelp(id, HELP_CONTEXTPOPUP);
}

void CAboutDlg::OnResexicon() 
{
   DWORD id = GetDlgItem(IDC_RESEXICON)->GetWindowContextHelpId();
   WinHelp(id, HELP_CONTEXTPOPUP);
   TRACE("CAboutDlg::OnResexicon %x\n", id);	
}
