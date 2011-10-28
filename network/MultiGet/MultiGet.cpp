// MultiGet.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MultiGet.h"
#include "MultiGetDlg.h"
#include <log4cplus/configurator.h>
#include <curl/curl.h>
#include "Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiGetApp

BEGIN_MESSAGE_MAP(CMultiGetApp, CWinApp)
	//{{AFX_MSG_MAP(CMultiGetApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiGetApp construction

CMultiGetApp::CMultiGetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMultiGetApp object

CMultiGetApp theApp;

static void InitLog4cplus()
{
#ifdef ENABLE_LOG4CPLUS
	
	const char* pEnvName = "LOG_FILE_SUFFIX";
	CString szOldValue = getenv(pEnvName);
	
	//Set the env to the current time str
	CString sTime = CTime::GetCurrentTime().Format("_%Y_%m_%d_%H_%M_%S");
	
	CString szValue;
	szValue.Format("%s=%s", pEnvName, sTime);
	_putenv(szValue);
	
	//This will take effect in the configure process
	log4cplus::PropertyConfigurator::doConfigure("log4cplus.properties");
	
	//After configuration, restore the envrionment varialble.
	szValue.Format("%s=%s", pEnvName, szOldValue);
	_putenv(szValue);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CMultiGetApp initialization

BOOL CMultiGetApp::InitInstance()
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
	//First init log4cplus
	InitLog4cplus();

	//Load system options
	SYS_OPTIONS()->Init();

	//curl lib global init
	curl_global_init(CURL_GLOBAL_ALL);

	CMultiGetDlg dlg;
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
	curl_global_cleanup();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
