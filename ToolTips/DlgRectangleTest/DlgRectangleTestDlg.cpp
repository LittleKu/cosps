// DlgRectangleTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRectangleTest.h"
#include "DlgRectangleTestDlg.h"

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
// CDlgRectangleTestDlg dialog

CDlgRectangleTestDlg::CDlgRectangleTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRectangleTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRectangleTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_rect[0].SetRect(10, 10, 100, 100);
	m_rect[1].SetRect(120, 10, 210, 100);
}

void CDlgRectangleTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRectangleTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgRectangleTestDlg, CDialog)
	//{{AFX_MSG_MAP(CDlgRectangleTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRectangleTestDlg message handlers

BOOL CDlgRectangleTestDlg::OnInitDialog()
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
	m_tooltip.Create(this);
	m_tooltip.SetNotify();

	m_tooltip.AddTool(this, _T("Red Area tooltips"), m_rect[0]);
	m_tooltip.AddTool(this, _T("Green Area tooltips"), m_rect[1]);

	// initialize the controls
    m_link.ConvertStaticToHyperlink(GetSafeHwnd(), IDC_URL, _T("http://www.google.com"));

	this->EnableToolTips(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgRectangleTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDlgRectangleTestDlg::OnPaint() 
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

		CClientDC dc(this);

		CDC* pDC = &dc;

		pDC->FillSolidRect(m_rect[0], RGB(255, 0, 0));
		pDC->FillSolidRect(m_rect[1], RGB(0, 255, 0));
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgRectangleTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CDlgRectangleTestDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

// int CDlgRectangleTestDlg::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
// {
// 	CWnd* pWnd = this->ChildWindowFromPoint(point);
// 	if(pWnd == NULL)
// 	{
// 		return -1;
// 	}
// 	CWnd* pDlgItem = this->GetDlgItem(IDC_URL);
// 	if(pWnd == pDlgItem)
// 	{
// 		CString tip;
// 		pTI->cbSize = sizeof(TOOLINFO);
// 		pTI->uFlags = 0;
// 		pTI->hwnd = GetSafeHwnd();
// 		pTI->uId = IDC_URL;
// 		CRect rect;
// 		pWnd->GetClientRect(&rect);
// 		pTI->rect = rect;
// 		tip.Format("tips for %d", IDC_URL);
// 		pTI->lpszText = (LPTSTR)malloc(sizeof(TCHAR)*(tip.GetLength()+1));
// 		_tcscpy(pTI->lpszText, tip);
// 
// 		return IDC_URL;
// 	}
// 	return -1;
// }

BOOL CDlgRectangleTestDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}
	
	if (nID != 0) // will be zero on a separator
		strTipText.Format("Control ID = %d", nID);
	
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
	*pResult = 0;
	
	return TRUE;    // message was handled
}
