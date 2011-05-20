// CheckLCDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckLCDemo.h"
#include "CheckLCDemoDlg.h"

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
// CCheckLCDemoDlg dialog

CCheckLCDemoDlg::CCheckLCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckLCDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckLCDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckLCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckLCDemoDlg)
	DDX_Control(pDX, IDC_GROUPBOX1, m_gb1);
	DDX_Control(pDX, IDC_COMBO1, m_combobox);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCheckLCDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckLCDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckLCDemoDlg message handlers

BOOL CCheckLCDemoDlg::OnInitDialog()
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
	CRect rect;
	m_listCtrl.GetClientRect(rect);
	m_listCtrl.InsertColumn(0, _T("No."), LVCFMT_LEFT, 80);
	m_listCtrl.InsertColumn(1, _T("Progress"), LVCFMT_LEFT, 150);
	m_listCtrl.InsertColumn(2, _T("Description"), LVCFMT_LEFT, rect.Width() - 230);

	CString cs(_T(""));
	for(int i = 0; i < 5; i++)
	{
		cs.Format(_T("%d"), i+1);
		m_listCtrl.InsertItem(i, cs, (i % 2) + 1);
		cs.Format(_T("It's the %d item"), i+1);
		m_listCtrl.SetItemText(i, 2, cs);
	}

//	m_listCtrl.SetExtendedStyle( m_listCtrl.GetExtendedStyle() | LVS_EX_CHECKBOXES);
	m_listCtrl.Init();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCheckLCDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

BOOL CCheckLCDemoDlg::OnEraseBkgnd(CDC* pDC)
{
	CBrush brush(RGB(255, 128, 0));
	
	CRect rect;
	GetClientRect(&rect);

	pDC->FillRect(&rect, &brush);

	return TRUE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCheckLCDemoDlg::OnPaint() 
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
		CPaintDC dc(this);

		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
		CPen* pOldPen = dc.SelectObject(&pen);

		CBrush brush(RGB(255, 0, 0));
		CBrush* pOldBrush = dc.SelectObject(&brush);

		CRect rect1(10, 10, 30, 30);
		

		dc.DrawFrameControl(&rect1, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED);
		dc.FrameRect(&rect1, &brush);

// 		CRect rect2(50, 10, 110, 70);
// 		
// 		
// 		dc.DrawFrameControl(&rect2, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED);
// 		dc.FrameRect(&rect2, &brush);

		CRect rect3(130, 10, 190, 70);
		
		CMyDC myDC(&dc);
		myDC.DoDrawCheckMark(rect3.left, rect3.top, rect3.Width(), rect3.Height());
// 		dc.DrawFrameControl(&rect3, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED);
// 		dc.FrameRect(&rect3, &brush);

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);


	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCheckLCDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
