// ListCtrlSortTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlSortTest.h"
#include "ListCtrlSortTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_MYMSG	(WM_USER + 19)

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
// CListCtrlSortTestDlg dialog

CListCtrlSortTestDlg::CListCtrlSortTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrlSortTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListCtrlSortTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInit = FALSE;
}

void CListCtrlSortTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlSortTestDlg)
	DDX_Control(pDX, IDC_LIST_MAIN, m_myListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CListCtrlSortTestDlg, CDialog)
	//{{AFX_MSG_MAP(CListCtrlSortTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_MESSAGE(WM_MYMSG, OnMyMsg)
	ON_BN_CLICKED(IDC_BTN_INIT, OnBtnInit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlSortTestDlg message handlers

BOOL CListCtrlSortTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	m_myListCtrl.Init();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListCtrlSortTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListCtrlSortTestDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListCtrlSortTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

static int count = 0;
static BOOL bRunning = FALSE;
static UINT MyThreadProc( LPVOID pParam )
{
	HWND hWnd = (HWND)pParam;

	bRunning = TRUE;
	while(bRunning)
	{
		//::Sleep(1);
		count++;
		::SendMessage(hWnd, WM_MYMSG, count, 0);
	}

	return 0;
}

LRESULT CListCtrlSortTestDlg::OnMyMsg(WPARAM wParam, LPARAM lParam)
{
	int n = (int)wParam;
	//AfxTrace(_T("OnMyMsg: %d\n"), n);
	CString szText;
	for(int i = 0; i < 10; i++)
	{
		m_myListCtrl.SetItemProgress(i, 0, (n + i) % 101, 100);
		szText.Format(_T("%.2f%c"), ((((double)((n + i) % 101)) * 100) / 100), _T('%'));
		m_myListCtrl.SetItemText(i, 0, szText);
		m_myListCtrl.InvalidateSubItem(i, 0);
	}

	//m_myListCtrl.Invalidate();

	return 0;
}

void CListCtrlSortTestDlg::OnBtnStart() 
{
	if(bRunning)
	{
		return;
	}
	AfxBeginThread(MyThreadProc, GetSafeHwnd());
}

void CListCtrlSortTestDlg::OnBtnStop() 
{
	bRunning = FALSE;
}

void CListCtrlSortTestDlg::OnBtnInit() 
{
	if(m_bInit)
	{
		return;
	}
	m_myListCtrl.InitData();
	m_bInit = TRUE;
}
