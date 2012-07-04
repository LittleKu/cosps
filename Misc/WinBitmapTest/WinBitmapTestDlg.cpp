// WinBitmapTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinBitmapTest.h"
#include "WinBitmapTestDlg.h"
#include "WinBitmap.h"
#include "RunicBMP.h"

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
// CWinBitmapTestDlg dialog
BOOL LoadBitmap(CBitmap& bmp, LPCTSTR lpszName)
{
	HBITMAP hb = (HBITMAP)::LoadImage(NULL, lpszName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(hb)
	{
		bmp.Attach(hb);
		return TRUE;
	}
	return FALSE;
}
CWinBitmapTestDlg::CWinBitmapTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinBitmapTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinBitmapTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinBitmapTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinBitmapTestDlg)
	DDX_Control(pDX, IDC_TEST_STATIC, m_testStatic);
	DDX_Control(pDX, IDC_PREIVEW_CTRL, m_previewCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinBitmapTestDlg, CDialog)
	//{{AFX_MSG_MAP(CWinBitmapTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MBITMAP, OnMbitmap)
	ON_BN_CLICKED(IDC_MBITAMP2, OnMbitamp2)
	ON_BN_CLICKED(IDC_START_PREV, OnStartPrev)
	ON_BN_CLICKED(IDC_STOP_PREV, OnStopPrev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinBitmapTestDlg message handlers

BOOL CWinBitmapTestDlg::OnInitDialog()
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
	
	LoadBitmap(bmpPrev1, _T("prev1.bmp"));
	LoadBitmap(bmpPrev2, _T("prev2.bmp"));
	m_previewCtrl.SetBitmaps(&bmpPrev1, &bmpPrev2);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinBitmapTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWinBitmapTestDlg::OnPaint() 
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
HCURSOR CWinBitmapTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWinBitmapTestDlg::OnMbitmap() 
{
	HWND hLabelWnd = ::GetDlgItem(this->m_hWnd, IDC_LABEL_DEFAUL);

	hLabelWnd = this->m_hWnd;
	ASSERT(hLabelWnd != NULL);

	CRect labelRect;
	::GetWindowRect(hLabelWnd, &labelRect);

	HDC hLabelDC = ::GetWindowDC(hLabelWnd);
	BOOL bOK = CWinBitmap::SaveBitmap(hLabelDC, labelRect.Width(), labelRect.Height(), "temp.bmp");
	::ReleaseDC(hLabelWnd, hLabelDC);

	AfxTrace("%d\n", bOK);
}

void CWinBitmapTestDlg::OnMbitamp2() 
{
	TestBMPCopy2 ("temp.bmp", "copy.bmp");
}

void CWinBitmapTestDlg::OnStartPrev() 
{
	m_previewCtrl.StartPreview();
	m_testStatic.StartP();
}

void CWinBitmapTestDlg::OnStopPrev() 
{
	m_previewCtrl.StopPreview();
	m_testStatic.StopP();
}
