// YTGet.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "YTGet.h"
#include "MainDlg.h"
#include <log4cplus/configurator.h>
#include <curl/curl.h>
#include "Options.h"
#include "SegmentInfoMap.h"
#include "gtb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYTGetApp

BEGIN_MESSAGE_MAP(CYTGetApp, CWinApp)
	//{{AFX_MSG_MAP(CYTGetApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYTGetApp construction

CYTGetApp::CYTGetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pSysBkBrush = NULL;
}
CYTGetApp::~CYTGetApp()
{
	if(m_pSysBkBrush != NULL)
	{
		m_pSysBkBrush->DeleteObject();
		delete m_pSysBkBrush;
		m_pSysBkBrush = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CYTGetApp object

CYTGetApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CYTGetApp initialization

BOOL CYTGetApp::InitInstance()
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
	Init();

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
	DeInit();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CYTGetApp::InitLog4cplus()
{

#ifdef ENABLE_LOG4CPLUS	
	const char* pLogFileSuffixName = "LOG_FILE_SUFFIX";
	CString szLogFileSuffix = getenv(pLogFileSuffixName);
	
	const char* pAppWorkDirName = "APP_WORK_DIR";
	CString szAppWorkDir = getenv(pAppWorkDirName);
	
	//Set the env to the current time str
	CString sTime = CTime::GetCurrentTime().Format("_%Y_%m_%d_%H_%M_%S");
	
	CString szValue;
	szValue.Format("%s=%s", pLogFileSuffixName, sTime);
	_putenv(szValue);
	
	szValue.Format("%s=%s", pAppWorkDirName, m_szWorkDir);
	_putenv(szValue);
	
	CString szConfigFile;
	szConfigFile.Format("%s\\log4cplus.properties", m_szWorkDir);
	try
	{
		//This will take effect in the configure process
		log4cplus::PropertyConfigurator::doConfigure((LPCTSTR)szConfigFile);

		{
			CString szLog;
			szLog.Format("Config File=%s", szConfigFile);
			LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
		}
	}
	catch(...) 
	{
		CString sLogInfo;
		sLogInfo.Format("Exception occured when configure log4cplus, config file = %s", szConfigFile);
		AfxTrace("%s\n", sLogInfo);
		LOG4CPLUS_FATAL(ROOT_LOGGER, (LPCTSTR)sLogInfo)
	}
	
	//After configuration, restore the environment variable.
	szValue.Format("%s=%s", pLogFileSuffixName, szLogFileSuffix);
	_putenv(szValue);
	
	szValue.Format("%s=%s", pAppWorkDirName, szAppWorkDir);
	_putenv(szValue);
#endif
}

void CYTGetApp::Init()
{
	//Work Directory
	gtb::GetWorkDirectory(m_szWorkDir, _T("log"));

	//First init log4cplus
	InitLog4cplus();
	
	//Load system options
	SYS_OPTIONS()->Init();
	
	//Make sure the singleton was initialized.
	CSegmentInfoMap::GetInstance();
	
	//curl lib global init
	curl_global_init(CURL_GLOBAL_ALL);
	
	m_threadMonitor.StartMonitor();
	
	m_pSysBkBrush = new CBrush();
	m_pSysBkBrush->CreateSolidBrush(RGB(251, 252, 249)/*RGB(255, 128, 0)*/);
}
void CYTGetApp::DeInit()
{
	ASSERT(m_pMainWnd->GetSafeHwnd() == NULL);
	
	//Wait thread monitor thread ends
	SYS_THREAD_MONITOR()->StopMonitor(TRUE);
	
	curl_global_cleanup();
}

CThreadMonitor* CYTGetApp::GetThreadMonitor()
{
	return &m_threadMonitor;
}

CYTGetApp* SYS_APP()
{
	CYTGetApp* pApp = (CYTGetApp*)AfxGetApp();
	ASSERT(pApp);
	return pApp;
}
