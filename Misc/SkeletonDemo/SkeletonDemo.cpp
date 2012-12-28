// SkeletonDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "MainDlg.h"
#include "cflbase/tstring.h"
#include "cflbase/FileUtils.h"

#ifdef ENABLE_LOG4CPLUS
#include <log4cplus/configurator.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DECLARE_THE_LOGGER_NAME(_T("CSkeletonDemoApp"))

/////////////////////////////////////////////////////////////////////////////
// CSkeletonDemoApp

BEGIN_MESSAGE_MAP(CSkeletonDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CSkeletonDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkeletonDemoApp construction

CSkeletonDemoApp::CSkeletonDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pSysBkBrush = NULL;
}

CSkeletonDemoApp::~CSkeletonDemoApp()
{
	if(m_pSysBkBrush != NULL)
	{
		m_pSysBkBrush->DeleteObject();
		delete m_pSysBkBrush;
		m_pSysBkBrush = NULL;
	}

	free((void*)m_pszAppName);
	m_pszAppName = NULL;
	
	LOG4CPLUS_INFO_STR(THE_LOGGER, _T("Application Destructor called"));
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSkeletonDemoApp object

CSkeletonDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSkeletonDemoApp initialization

BOOL CSkeletonDemoApp::InitInstance()
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
	InitWorkDir();
	InitLog4cplus();
	InitAppVariables();
	InitBCG();

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

	DeInitBCG();
	LOG4CPLUS_INFO_STR(THE_LOGGER, _T("Application InitInstance Done"))
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
int CSkeletonDemoApp::ExitInstance() 
{
	LOG4CPLUS_INFO_STR(THE_LOGGER, _T("Application ExitInstance"));
	return CWinApp::ExitInstance();
}

BOOL CSkeletonDemoApp::PreTranslateMessage(MSG* pMsg) 
{
	//The Key RETURN and ESCAPE will cause the system halted, remove the response of the 2 keys.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			HWND hWndMsgParent = ::GetParent(pMsg->hwnd);
			HWND hWndMain = AfxGetMainWnd()->GetSafeHwnd();
			HWND hWndWork = ((CMainDlg*)AfxGetMainWnd())->m_pVCDlg->GetSafeHwnd();
			if(hWndWork == hWndMsgParent || hWndMain == hWndMsgParent)
			{
				return TRUE;
			}
		}
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}

CSkeletonDemoApp* SYS_APP()
{
	CSkeletonDemoApp* pApp = (CSkeletonDemoApp*)AfxGetApp();
	ASSERT(pApp);
	return pApp;
}

void CSkeletonDemoApp::InitWorkDir()
{
	TCHAR szFQPath[1024] = {0};
	::GetModuleFileName(NULL, szFQPath, 1024);

 	cfl::tstring szPath;
 	cfl::FileUtils::GetPathOnly(szPath, szFQPath);
	
	m_szWorkDir = szPath.c_str();

	AfxTrace(_T("Work Folder=%s\n"), (LPCTSTR)m_szWorkDir);
}
void CSkeletonDemoApp::InitLog4cplus()
{
#ifdef ENABLE_LOG4CPLUS
	std::locale::global(std::locale("chs"));
	LPCTSTR pLogFileSuffixName = _T("LOG_FILE_SUFFIX");
	CString szLogFileSuffix = _tgetenv(pLogFileSuffixName);
	
	LPCTSTR pAppWorkDirName = _T("APP_WORK_DIR");
	CString szAppWorkDir = _tgetenv(pAppWorkDirName);
	
	//Set the env to the current time str
	CString sTime = CTime::GetCurrentTime().Format(_T("_%Y_%m_%d_%H_%M_%S"));
	
	CString szValue;
	szValue.Format(_T("%s=%s"), pLogFileSuffixName, sTime);
	_tputenv(szValue);
	
	szValue.Format(_T("%s=%s"), pAppWorkDirName, SYS_APP()->m_szWorkDir);
	_tputenv(szValue);
	
	cfl::tstring szConfigFile;
	cfl::tformat(szConfigFile, _T("%s\\dat\\config\\log4cplus.properties"), (LPCTSTR)(SYS_APP()->m_szWorkDir));
	try
	{
		//This will take effect in the configure process
		log4cplus::PropertyConfigurator::doConfigure(szConfigFile);
		
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::INFO_LOG_LEVEL))
		{
			TIME_ZONE_INFORMATION tzi;
			GetTimeZoneInformation(&tzi);
			
			cfl::tstring szCurLocaleName = CFL_STRING_TO_TSTRING(std::locale().name());
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Application Initialized. TimeZone=%d, Locale=%s"), (-(tzi.Bias)), szCurLocaleName.c_str());
			LOG4CPLUS_INFO_STR(THE_LOGGER, szLog)
		}
	}
	catch(...) 
	{
		cfl::tstring sLogInfo;
		cfl::tformat(sLogInfo, _T("Exception occured when configure log4cplus, config file = %s"), szConfigFile.c_str());
		AfxTrace(_T("%s\n"), szConfigFile.c_str());
		LOG4CPLUS_FATAL_STR(ROOT_LOGGER, sLogInfo)
	}
	
	//After configuration, restore the environment variable.
	szValue.Format(_T("%s=%s"), pLogFileSuffixName, szLogFileSuffix);
	_tputenv(szValue);
	
	szValue.Format(_T("%s=%s"), pAppWorkDirName, szAppWorkDir);
	_tputenv(szValue);
	
#endif
}
void CSkeletonDemoApp::InitAppVariables()
{
	//Init default system background color
	m_crSysBkColor = RGB(251, 252, 249);

	//Init sys background brush
	m_pSysBkBrush = new CBrush();
	m_pSysBkBrush->CreateSolidBrush(m_crSysBkColor);

	//Set App's name
	if(m_pszAppName != NULL)
	{
		free((void*)m_pszAppName);
		m_pszAppName = NULL;
	}
	m_pszAppName = _tcsdup(_T("SkeletionDemo - TODO"));
}

void CSkeletonDemoApp::InitBCG()
{
	AfxOleInit();
	
	SetRegistryBase (_T("SkeletonDemo"));
	
	// Initialize customization managers:
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager();
	
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
	CBCGPButton::EnableWinXPTheme ();
}

void CSkeletonDemoApp::DeInitBCG()
{
	BCGCBProCleanUp();
}