// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "MainDlg.h"
#include "Constants.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pYTGetDlg = new CYTGetDlg();
	m_pMainToolBarCtrl = new CMainToolBarCtrl();
}

CMainDlg::~CMainDlg()
{
	delete m_pMainToolBarCtrl;
	delete m_pYTGetDlg;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetMenuBarBkg();
	InitGUI();

	EnableSaveRestore("MainDlg");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CResizableDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CResizableDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMainDlg::InitGUI()
{
	CWnd* pWndToolBar = m_pMainToolBarCtrl;
	if (m_pMainToolBarCtrl->Create(WS_VISIBLE | WS_CHILD | CCS_TOP | CCS_NODIVIDER | TBSTYLE_TOOLTIPS 
		| TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST, CRect(0,0,0,0), this, IDC_TOOLBAR))
	{
		m_pMainToolBarCtrl->ModifyStyle(0, CCS_NORESIZE);
		m_pMainToolBarCtrl->SetExtendedStyle(m_pMainToolBarCtrl->GetExtendedStyle() | TBSTYLE_EX_HIDECLIPPEDBUTTONS);

		m_pMainToolBarCtrl->Init();
		pWndToolBar = InitReBar();
	}
	m_pYTGetDlg->Create(IDD_YTGET_DIALOG);
	
	// adjust all main window sizes for toolbar height and maximize the child windows
	CRect rcClient, rcToolbar;
	GetClientRect(&rcClient);
	pWndToolBar->GetWindowRect(&rcToolbar);
	rcClient.top += rcToolbar.Height();
	
	m_pYTGetDlg->SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOZORDER);
	
	// anchors
	AddAnchor(pWndToolBar->m_hWnd, TOP_LEFT, TOP_RIGHT);
	AddAnchor(m_pYTGetDlg->m_hWnd, TOP_LEFT, BOTTOM_RIGHT);
	
	SetSizeGripVisibility(FALSE);
	UpdateSizeGrip();
}

CWnd* CMainDlg::InitReBar()
{
	CMainToolBarCtrl* pToolBarCtrl = m_pMainToolBarCtrl;
	if (m_MainReBarCtrl.Create(WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | RBS_BANDBORDERS 
		| RBS_AUTOSIZE | CCS_NODIVIDER, CRect(0, 0, 0, 0), this, AFX_IDW_REBAR))
	{
		CSize sizeBar;
		VERIFY( pToolBarCtrl->GetMaxSize(&sizeBar) );
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_STYLE | RBBIM_SIZE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_ID;
		rbbi.fStyle = RBBS_NOGRIPPER | RBBS_BREAK | RBBS_USECHEVRON;
		rbbi.hwndChild = pToolBarCtrl->m_hWnd;
		rbbi.cxMinChild = sizeBar.cy;
		rbbi.cyMinChild = sizeBar.cy;
		rbbi.cxIdeal = sizeBar.cx;
		rbbi.cx = rbbi.cxIdeal;
		rbbi.wID = 0;
		VERIFY( m_MainReBarCtrl.InsertBand((UINT)-1, &rbbi) );
		
		pToolBarCtrl->SetParentReBarWnd(m_MainReBarCtrl.m_hWnd);
		
		return &m_MainReBarCtrl;
	}
	return pToolBarCtrl;
}

void CMainDlg::SetMenuBarBkg()
{
	MENUINFO MenuInfo = {0};
	MenuInfo.cbSize = sizeof(MenuInfo);
	MenuInfo.hbrBack = *(SYS_APP()->m_pSysBkBrush); // Brush you want to draw
	MenuInfo.fMask = MIM_BACKGROUND;
	MenuInfo.dwStyle = MNS_AUTODISMISS;
	
	CMenu* pMenu = GetMenu();
	
	if(IsMenu(pMenu->m_hMenu))
	{
		SetMenuInfo(pMenu->m_hMenu, &MenuInfo);
	}
}

BOOL CMainDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	BOOL bProcessed = TRUE;

	switch(wParam)
	{
	case ID_TBBTN_ADD_TASK:
		{
			m_pYTGetDlg->AddTask();
		}
		break;
	case ID_TBBTN_ADD_BATCH_TASKS:
		{
			m_pYTGetDlg->AddBatchTask();
		}
		break;
	case ID_TBBTN_START:
		{
			m_pYTGetDlg->Start();
		}
		break;
	case ID_TBBTN_PAUSE:
		{
			m_pYTGetDlg->Pause();
		}
		break;
	case ID_TBBTN_REMOVE:
		{
			m_pYTGetDlg->Remove();
		}
		break;
	case ID_TBBTN_RESTART:
		{
			m_pYTGetDlg->Restart();
		}
		break;
	case IDM_TOOLS_OPTIONS:
		{
			COptionDlg optionDlg;
			optionDlg.DoModal();
		}
		break;
	default:
		{
			bProcessed = FALSE;
		}
		break;
	}

	if(bProcessed)
	{
		return TRUE;
	}
	return CResizableDialog::OnCommand(wParam, lParam);
}
