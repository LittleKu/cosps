// BCGHelloWorldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGHelloWorld.h"
#include "BCGHelloWorldDlg.h"

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
// CBCGHelloWorldDlg dialog

CBCGHelloWorldDlg::CBCGHelloWorldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBCGHelloWorldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBCGHelloWorldDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBCGHelloWorldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCGHelloWorldDlg)
	DDX_Control(pDX, IDC_PROPLIST_LOCATION, m_wndPropListLocation);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBCGHelloWorldDlg, CDialog)
	//{{AFX_MSG_MAP(CBCGHelloWorldDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXPAND_ALL, OnBtnExpandAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGHelloWorldDlg message handlers

BOOL CBCGHelloWorldDlg::OnInitDialog()
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
	CRect rectPropList;
	m_wndPropListLocation.GetClientRect (&rectPropList);
	m_wndPropListLocation.MapWindowPoints (this, &rectPropList);

	m_wndPropList.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectPropList, this, (UINT)-1);
	
	m_wndPropList.EnableHeaderCtrl ();
	m_wndPropList.EnableDesciptionArea ();

	m_wndPropList.SetCustomColors (
		RGB (250, 250, 250),
		RGB (0, 0, 0),
		RGB (240, 240, 240),
		RGB (40, 76, 165),
		RGB (250, 250, 250),
		RGB (0, 85, 136),
			RGB (168, 189, 216));

	CBCGPProp* pGroup1 = new CBCGPProp (_T("Appearance"));
	
	pGroup1->AddSubItem (new CBCGPProp (_T("3D Look"), (_variant_t) false,
		_T("Specifies the dialog's font will be nonbold and controls will have a 3D border")));
	
	CBCGPProp* pProp = new CBCGPProp (_T("Border"), _T("Dialog Frame"),
		_T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption (_T("None"));
	pProp->AddOption (_T("Thin"));
	pProp->AddOption (_T("Resizable"));
	pProp->AddOption (_T("Dialog Frame"));
	pProp->AllowEdit (FALSE);
	
	pGroup1->AddSubItem (pProp);
	pGroup1->AddSubItem (new CBCGPProp (_T("Caption"), (_variant_t) _T("About BCGProTest"),
		_T("Specifies the text that will be displayed in the dialog's title bar")));
	
	m_wndPropList.AddProperty (pGroup1);
	
	CBCGPProp* pSize = new CBCGPProp (_T("Window Size"), 0, TRUE);
	
	pProp = new CBCGPProp (_T("Height"), (_variant_t) 250l,
		_T("Specifies the dialog's height"));
	pProp->EnableSpinControl (TRUE, 0, 1000);
	pSize->AddSubItem (pProp);
	
	pProp = new CBCGPProp (	_T("Width"), (_variant_t) 150l,
		_T("Specifies the dialog's width"));
	pProp->EnableSpinControl ();
	pSize->AddSubItem (pProp);
	
	m_wndPropList.AddProperty (pSize);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBCGHelloWorldDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBCGHelloWorldDlg::OnPaint() 
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
HCURSOR CBCGHelloWorldDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBCGHelloWorldDlg::PreSubclassWindow() 
{
	
	CDialog::PreSubclassWindow();
}

BOOL CBCGHelloWorldDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

void CBCGHelloWorldDlg::OnBtnExpandAll() 
{
	static BOOL bExpanded = FALSE;
	
	m_wndPropList.ExpandAll (!bExpanded);
	
	SetDlgItemText(IDC_BTN_EXPAND_ALL, bExpanded ?
		_T("Expand &All") : _T("Collapse &All"));
	
	bExpanded = !bExpanded;
}
