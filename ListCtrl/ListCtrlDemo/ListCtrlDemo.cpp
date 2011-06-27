// ListCtrlDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "MainDlg.h"
#include "Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoApp

BEGIN_MESSAGE_MAP(CListCtrlDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CListCtrlDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoApp construction

CListCtrlDemoApp::CListCtrlDemoApp()
{
	m_pSysBkBrush  = NULL;
}

CListCtrlDemoApp::~CListCtrlDemoApp()
{
	if(m_pSysBkBrush != NULL)
	{
		m_pSysBkBrush->DeleteObject();
		delete m_pSysBkBrush;
		m_pSysBkBrush = NULL;
	}

	free((void*)m_pszAppName);
	m_pszAppName = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CListCtrlDemoApp object

CListCtrlDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoApp initialization

BOOL CListCtrlDemoApp::InitInstance()
{
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
	//Set App's name
	if(m_pszAppName != NULL)
	{
		free((void*)m_pszAppName);
		m_pszAppName = NULL;
	}
	m_pszAppName = _tcsdup(SZ_PRODUCT_NAME);

	m_szPrefIniFile.Format(_T("%s"), CommonUtils::GetConfFilePath(_T("preferences.ini"), GCFP_USER));

	SYS_PREF()->Init();

	m_pSysBkBrush = new CBrush();
	m_pSysBkBrush->CreateSolidBrush(RGB(251, 252, 249));

	CMainDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

CString& SYS_PREF_INI_FILE() 
{
	return SYS_APP()->m_szPrefIniFile;
}

CListCtrlDemoApp* SYS_APP()
{
	CListCtrlDemoApp* pApp = (CListCtrlDemoApp*)AfxGetApp();
	ASSERT(pApp);
	return pApp;
}
