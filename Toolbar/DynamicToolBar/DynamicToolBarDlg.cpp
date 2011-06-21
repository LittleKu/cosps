// DynamicToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "DynamicToolBarDlg.h"

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
// CDynamicToolBarDlg dialog
void DialogCreateIndirect(CDialog *pWnd, UINT uID)
{
#if 0
	// This could be a nice way to change the font size of the main windows without needing
	// to re-design the dialog resources. However, that technique does not work for the
	// SearchWnd and it also introduces new glitches (which would need to get resolved)
	// in almost all of the main windows.
	CDialogTemplate dlgTempl;
	dlgTempl.Load(MAKEINTRESOURCE(uID));
	dlgTempl.SetFont(_T("MS Shell Dlg"), 8);
	pWnd->CreateIndirect(dlgTempl.m_hTemplate);
	FreeResource(dlgTempl.Detach());
#else
	pWnd->Create(uID);
#endif
}
CDynamicToolBarDlg::CDynamicToolBarDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDynamicToolBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDynamicToolBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	toolbar = new CMyToolBarCtrl();
	pDlg1 = new CDlg1();
	pDlg2 = new CDlg2();

	activewnd = NULL;
	m_bkBrush = NULL;

	m_bUseReBar = TRUE;

	m_dwGripTempState = 1;
}

CDynamicToolBarDlg::~CDynamicToolBarDlg()
{
	delete toolbar;
	delete pDlg1;
	delete pDlg2;
	if(m_bkBrush != NULL)
	{
		m_bkBrush->DeleteObject();
		delete m_bkBrush;
		m_bkBrush = NULL;
	}
	if(bitmap != NULL)
	{
		delete bitmap;
		bitmap = NULL;
	}
}

void CDynamicToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynamicToolBarDlg)
	DDX_Control(pDX, IDC_STATIC_STATUS_BAR, m_staticStatusBar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDynamicToolBarDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CDynamicToolBarDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_REGISTERED_MESSAGE(UM_TOOLBARCTRLX_REFRESH, OnToolBarRefresh)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicToolBarDlg message handlers
LRESULT CDynamicToolBarDlg::OnToolBarRefresh(WPARAM wParam, LPARAM lParam)
{
	
	if(m_ctlMainTopReBar.m_hWnd)
	{
		RemoveAnchor(m_ctlMainTopReBar.m_hWnd);
		
		REBARBANDINFO rbbi = {0};
		CSize sizeBar;
		toolbar->GetMaxSize(&sizeBar);
		ASSERT( sizeBar.cx != 0 && sizeBar.cy != 0 );
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
		rbbi.cxMinChild = sizeBar.cy;
		rbbi.cyMinChild = sizeBar.cy;
		rbbi.cxIdeal = sizeBar.cx;
		VERIFY( m_ctlMainTopReBar.SetBandInfo(0, &rbbi) );
		
		AddAnchor(m_ctlMainTopReBar.m_hWnd, TOP_LEFT, TOP_RIGHT);
	}
	else
	{
		RemoveAnchor(toolbar->m_hWnd);
		AddAnchor(toolbar->m_hWnd, TOP_LEFT, TOP_RIGHT);
	}
	CRect rToolbarRect;
	toolbar->GetWindowRect(&rToolbarRect);
	
	CRect rClientRect;
	GetClientRect(&rClientRect);
	
	CRect rStatusbarRect;
//	statusbar->GetWindowRect(&rStatusbarRect);
//	m_bar.GetWindowRect(&rStatusbarRect);
	m_staticStatusBar.GetWindowRect(&rStatusbarRect);
	
	rClientRect.top += rToolbarRect.Height();
	rClientRect.bottom -= rStatusbarRect.Height();
	
	CRect rcClient = rClientRect;
	AfxTrace("rcClient(%d, %d, %d, %d)\n", rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
	CWnd* apWnds[] =
	{
		pDlg1,
		pDlg2
	};
	int count = sizeof(apWnds) / sizeof(apWnds[0]);
	for (int i = 0; i < count; i++)
	{
		apWnds[i]->SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOZORDER);
		RemoveAnchor(apWnds[i]->m_hWnd);
		AddAnchor(apWnds[i]->m_hWnd, TOP_LEFT, BOTTOM_RIGHT);
	}

	return TRUE;
}
BOOL CDynamicToolBarDlg::OnInitDialog()
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
	m_hToolBarBkBmp = (HBITMAP)::LoadImage(NULL, _T(".\\res\\background.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bitmap = new CBitmap();
	bitmap->LoadBitmap(IDB_BKG);
	m_bkBrush = new CBrush;
	m_bkBrush->CreatePatternBrush(bitmap);
	
	
	MENUINFO MenuInfo = {0};
	MenuInfo.cbSize = sizeof(MenuInfo);
	MenuInfo.hbrBack = *m_bkBrush; // Brush you want to draw
	MenuInfo.fMask = MIM_BACKGROUND;
	MenuInfo.dwStyle = MNS_AUTODISMISS;
	
	CMenu* pMenu = this->GetMenu();
	
	if(IsMenu(pMenu->m_hMenu))
	{
		SetMenuInfo(pMenu->m_hMenu, &MenuInfo);
	}
	Init();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDynamicToolBarDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
BOOL CDynamicToolBarDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL bRet = CResizableDialog::OnEraseBkgnd(pDC);
	return bRet;
}
void CDynamicToolBarDlg::OnPaint() 
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
HCURSOR CDynamicToolBarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

static UINT BASED_CODE indicators[] =
{
	ID_INDICATOR_NISH,
		ID_INDICATOR_TIME
};

void CDynamicToolBarDlg::Init()
{
	CWnd* pwndToolbarX = toolbar;
	if (toolbar->Create(WS_VISIBLE | WS_CHILD | CCS_TOP | CCS_NODIVIDER |  0x00000010L |TBSTYLE_TOOLTIPS 
		| TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST,
		CRect(0,0,0,0), this, IDC_TOOLBAR))
	{
		if(IsUseReBar())
		{
			toolbar->ModifyStyle(0, CCS_NORESIZE);
			toolbar->SetExtendedStyle(toolbar->GetExtendedStyle() | TBSTYLE_EX_HIDECLIPPEDBUTTONS);
		}
		toolbar->Init();
		pwndToolbarX = InitReBar();
	}
	// set statusbar
	// the statusbar control is created as a custom control in the dialog resource,
	// this solves font and sizing problems when using large system fonts
// 	statusbar->SubclassWindow(GetDlgItem(IDC_STATUSBAR)->m_hWnd);
// 	statusbar->ModifyStyle(SBARS_SIZEGRIP, 0);
// 	statusbar->SetFont(GetFont(), TRUE);
// 	SetStatusBarPartsSize();
// 	statusbar->SetWindowText(_T("DynamicToolBar Status Bar Version v0.50a DEBUG"));
	m_staticStatusBar.SetFont(GetFont(), TRUE);
	SetStatusBarPartsSize();

	DialogCreateIndirect(pDlg1, IDD_DIALOG1);
	DialogCreateIndirect(pDlg2, IDD_DIALOG2);

	// if still no active window, activate server window
	if (activewnd == NULL)
		SetActiveDialog(pDlg1);

	// adjust all main window sizes for toolbar height and maximize the child windows
	CRect rcClient, rcToolbar, rcStatusbar;
	GetClientRect(&rcClient);
	pwndToolbarX->GetWindowRect(&rcToolbar);
	rcStatusbar.SetRectEmpty();;
	m_staticStatusBar.GetWindowRect(&rcStatusbar);
	rcClient.top += rcToolbar.Height();
	rcClient.bottom -= rcStatusbar.Height();
	CWnd* apWnds[] =
	{
		pDlg1,
		pDlg2
	};
	int count = sizeof(apWnds) / sizeof(apWnds[0]);
	for (int i = 0; i < count; i++)
		apWnds[i]->SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOZORDER);
	
	// anchors
	AddAnchor(*pDlg1,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*pDlg2,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*pwndToolbarX,	TOP_LEFT, TOP_RIGHT);
	AddAnchor(m_staticStatusBar.m_hWnd,	BOTTOM_LEFT, BOTTOM_RIGHT);

	HideSizeGrip(&m_dwGripTempState);
	UpdateSizeGrip();
}

void CDynamicToolBarDlg::SetActiveDialog(CWnd* dlg)
{
	if (dlg == activewnd)
		return;
	if (activewnd)
		activewnd->ShowWindow(SW_HIDE);
	dlg->ShowWindow(SW_SHOW);
	dlg->SetFocus();
	activewnd = dlg;
}

BOOL CDynamicToolBarDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
	case TBBTN_DLG1:
		SetActiveDialog(pDlg1);
		break;
	case TBBTN_DLG2:
		SetActiveDialog(pDlg2);
		break;
	case TBBTN_CHANGESTYLE:
		{
			EToolbarLabelType eLabelType = toolbar->GetLabelType();

			if(eLabelType == NoLabels)
			{
				eLabelType = LabelsBelow;
			}
			else
			{
				eLabelType = (EToolbarLabelType)(3 - eLabelType);
			}
			toolbar->ChangeTextLabelStyle(eLabelType, true);
		}
		break;
	case TBBTN_NO_LABEL:
		{
			toolbar->ChangeTextLabelStyle(NoLabels, true);
		}
		break;
	case TBBTN_CHANGE_TBBK:
		{
			REBARBANDINFO rbbi = {0};
			rbbi.cbSize = sizeof(rbbi);
			rbbi.fMask = RBBIM_BACKGROUND;

			if(m_ctlMainTopReBar.GetBandInfo(0, &rbbi))
			{
				if(rbbi.hbmBack)
				{
					toolbar->UpdateBackground(NULL);
				}
				else
				{
					toolbar->UpdateBackground(m_hToolBarBkBmp);
				}
				toolbar->Invalidate();
			}
		}
		break;
	}
	return CResizableDialog::OnCommand(wParam, lParam);
}

CWnd* CDynamicToolBarDlg::InitReBar()
{
	if(!IsUseReBar())
	{
		return toolbar;
	}

	if (m_ctlMainTopReBar.Create(WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | RBS_BANDBORDERS 
		| RBS_AUTOSIZE | CCS_NODIVIDER | 0x00000010L, CRect(0, 0, 0, 0), this, AFX_IDW_REBAR))
	{
		CSize sizeBar;
		VERIFY( toolbar->GetMaxSize(&sizeBar) );
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_STYLE | RBBIM_SIZE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_ID;
		rbbi.fStyle = RBBS_NOGRIPPER | RBBS_BREAK | RBBS_USECHEVRON;
		rbbi.hwndChild = toolbar->m_hWnd;
		rbbi.cxMinChild = sizeBar.cy;
		rbbi.cyMinChild = sizeBar.cy;
		rbbi.cxIdeal = sizeBar.cx;
		rbbi.cx = rbbi.cxIdeal;
		rbbi.wID = 0;
		VERIFY( m_ctlMainTopReBar.InsertBand((UINT)-1, &rbbi) );

		toolbar->SetParentReBarWnd(m_ctlMainTopReBar.m_hWnd);
		toolbar->UpdateBackground(m_hToolBarBkBmp);

		return &m_ctlMainTopReBar;
	}
	return toolbar;
}

BOOL CDynamicToolBarDlg::IsUseReBar()
{
	CDynamicToolBarApp* pApp = (CDynamicToolBarApp*)AfxGetApp();
	
	return m_bUseReBar && (pApp->IsCommCtrlMeetLowestReq());
}

CWnd* CDynamicToolBarDlg::GetActualToolBar()
{
	if(IsUseReBar())
	{
		return &m_ctlMainTopReBar;
	}
	else
	{
		return toolbar;
	}
}

void CDynamicToolBarDlg::OnDestroy() 
{
	CResizableDialog::OnDestroy();
	if(m_hToolBarBkBmp)
	{
		::DeleteObject(m_hToolBarBkBmp);
		m_hToolBarBkBmp = NULL;
	}
}

void CDynamicToolBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	SetStatusBarPartsSize();
// 	if(::IsWindow(m_hWnd))
// 	{
// 		Invalidate();
// 	}
}

void CDynamicToolBarDlg::SetShowSizeGrip(BOOL bShow)
{
	if(bShow)
	{
		ShowSizeGrip(&m_dwGripTempState);
	}
	else
	{
		HideSizeGrip(&m_dwGripTempState);
	}
	UpdateSizeGrip();
}

void CDynamicToolBarDlg::SetStatusBarPartsSize()
{
	if(!::IsWindow(m_staticStatusBar.m_hWnd))
	{
		return;
	}
	CRect rect;
	m_staticStatusBar.GetClientRect(&rect);
	int ussShift = 0;
	
	int aiWidths[3] =
	{ 
		rect.right - 300 - ussShift,
		rect.right - 200 - ussShift,
		-1
	};
	m_staticStatusBar.SetParts(3, aiWidths);
}
