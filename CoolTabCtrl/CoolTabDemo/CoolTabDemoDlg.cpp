// CoolTabDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolTabDemo.h"
#include "CoolTabDemoDlg.h"
#include "TempDlg.h"
#include "CCommonDlg.h"

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
// CCoolTabDemoDlg dialog

CCoolTabDemoDlg::CCoolTabDemoDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CCoolTabDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoolTabDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_tabBtnGroup = NULL;
}

CCoolTabDemoDlg::~CCoolTabDemoDlg()
{
	if(m_tabBtnGroup != NULL)
	{
		delete m_tabBtnGroup;
		m_tabBtnGroup = NULL;
	}
}

void CCoolTabDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoolTabDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCoolTabDemoDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CCoolTabDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolTabDemoDlg message handlers

BOOL CCoolTabDemoDlg::OnInitDialog()
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
	if(!m_resMgr.Init(".\\skins\\en"))
	{
		AfxMessageBox("Failed to load skin files\n");
		EndDialog(IDCANCEL);
	}
	if(!m_resMgr.Load(".\\skins\\en.xml"))
	{
		AfxMessageBox("Failed to load skin files\n");
		EndDialog(IDCANCEL);
	}

	AddAnchor(m_mainTabCtrl.GetSafeHwnd(), TOP_LEFT, BOTTOM_RIGHT);

	// allow any size
	ResetMinTrackSize();

//	ShowWindow(SW_SHOWMAXIMIZED);

	//Bitmap Tab
	m_mainTabCtrl.SetBackgroundBitmap(&m_resMgr.m_mainBkgDps);
	m_tabBtnGroup = new TabButtonGroup(&m_resMgr.m_mainNormalDps, &m_resMgr.m_mainHighlightDps, 
		&m_resMgr.m_mainClickedDps, &m_resMgr.m_mainDisabledDps);
	TabButtonGroup* pTbg = m_tabBtnGroup;
	//button 1
	CRect rect;
	rect.left = 336;
	rect.top = 6;
	rect.right = rect.left + 152;
	rect.bottom = rect.top + 50;
	pTbg->AddTabButton(&rect);
	
	//button 2
	rect.left = 495;
	rect.top = 6;
	rect.right = rect.left + 152;
	rect.bottom = rect.top + 50;
	pTbg->AddTabButton(&rect);
	
	//button 3
	rect.left = 654;
	rect.top = 6;
	rect.right = rect.left + 152;
	rect.bottom = rect.top + 50;
	pTbg->AddTabButton(&rect);
	
	m_mainTabCtrl.m_pTabBtnGroup = pTbg;
	m_mainTabCtrl.UpdateWindow();
	this->CenterWindow();

	EnableToolTips(TRUE);   // enable tool tips for view

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCoolTabDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCoolTabDemoDlg::OnPaint() 
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
HCURSOR CCoolTabDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCoolTabDemoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CResizableDialog::PreTranslateMessage(pMsg);
}

void CCoolTabDemoDlg::OnAppExit() 
{
	// TODO: Add your command handler code here
	CResizableDialog::OnCancel();
}

int CCoolTabDemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.DeflateRect(20, 20, 20, 20);

	// TODO: Add your specialized creation code here
	m_mainTabCtrl.Create(WS_CHILD|WS_VISIBLE, clientRect, this, 8956);

	//Image Tab
//	m_mainTabCtrl.SetBackgroundBitmap(&m_resMgr.m_tiBkg);
// 	m_mainTabCtrl.m_pLeft = &m_resMgr.m_tiLeft;
// 	m_mainTabCtrl.m_pMid = &m_resMgr.m_tiMid;
// 	m_mainTabCtrl.m_pRight = &m_resMgr.m_tiRight;
// 	m_mainTabCtrl.m_pSep = &m_resMgr.m_tiSep;

	dlg1.Create(IDD_DIALOG_TEMP, &m_mainTabCtrl);
	m_mainTabCtrl.AddTab(&dlg1,"1stabcdddddddddddddddddd123",IDI_ICON2);

 	dlg2.Create(IDD_DIALOG_TEMP2, &m_mainTabCtrl);
	m_mainTabCtrl.AddTab(&dlg2,"2nddd+L????g Text Test",IDI_ICON1);

	CString str;
	for(int i = 0; i < 1; i++)
	{
		str.Format("No.%d Tab Item", i + 3);
		m_mainTabCtrl.AddTab(RUNTIME_CLASS(CTempDlg),IDD_DIALOG_TEMP,str, IDI_ICON3);
	}

	m_mainTabCtrl.SetTabReplacement(TCS_TOP);
	m_mainTabCtrl.SetMonoSpace(FALSE);
	m_mainTabCtrl.SetAutoAdjustWidth(FALSE);
	return 0;
}

