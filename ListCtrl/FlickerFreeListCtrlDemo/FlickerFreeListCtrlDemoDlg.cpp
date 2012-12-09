// FlickerFreeListCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FlickerFreeListCtrlDemo.h"
#include "FlickerFreeListCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MY_TEST_MSG	(WM_USER + 1234)

struct ColumnInfo
{
	TCHAR		lpszName[32];
	int         nType;
	int         nWidth;
	int			nAlign;
};

static ColumnInfo columns[] =
{
	{ _T("Name"),		0,	170,	HDF_LEFT	},
    { _T("Format"),  	0,	70,		HDF_LEFT	},
    { _T("Duration"),  	0,	80,		HDF_RIGHT	},
	{ _T("Status"),  	0,	170,	HDF_LEFT	}
};

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
// CFlickerFreeListCtrlDemoDlg dialog

CFlickerFreeListCtrlDemoDlg::CFlickerFreeListCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlickerFreeListCtrlDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlickerFreeListCtrlDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlickerFreeListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlickerFreeListCtrlDemoDlg)
	DDX_Control(pDX, IDC_DATA_LISTCTRL, m_dataListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlickerFreeListCtrlDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CFlickerFreeListCtrlDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_MESSAGE(MY_TEST_MSG, OnMyTestMsg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlickerFreeListCtrlDemoDlg message handlers

BOOL CFlickerFreeListCtrlDemoDlg::OnInitDialog()
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
	InitListCtrl();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlickerFreeListCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFlickerFreeListCtrlDemoDlg::OnPaint() 
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
HCURSOR CFlickerFreeListCtrlDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

static bool stop = false;
static int count = 0;

UINT MyControllingFunction( LPVOID pParam )
{
	HWND hWnd = (HWND)pParam;
	count = 0;
	stop = false;
	while(!stop)
	{
		count++;
		::SendMessage(hWnd, MY_TEST_MSG, (WPARAM)count, 0);
		Sleep(1);
	}
	return 0;
}

LRESULT CFlickerFreeListCtrlDemoDlg::OnMyTestMsg(WPARAM wParam, LPARAM lParam)
{
	CString szText;
	szText.Format(_T("msg-%d"), (int)wParam);
	int nItem = 0;
	m_dataListCtrl.SetItemText(0, 0, szText);
	m_dataListCtrl.SetItemText(1, 1, szText);
	CTaskInfo* pTaskInfo = (CTaskInfo*)m_dataListCtrl.GetItemData(0);
	pTaskInfo->m_nProgress++;
	m_dataListCtrl.Update(0);

	return 0;
}

void CFlickerFreeListCtrlDemoDlg::OnBtnStart() 
{
	AfxBeginThread(MyControllingFunction, GetSafeHwnd());
}

void CFlickerFreeListCtrlDemoDlg::OnBtnStop() 
{
	stop = true;
}

void CFlickerFreeListCtrlDemoDlg::InitListCtrl()
{
	//1. Set Extended Style
	DWORD dwExtendedStyle = m_dataListCtrl.GetExtendedStyle();
	dwExtendedStyle = (dwExtendedStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_dataListCtrl.SetExtendedStyle(dwExtendedStyle);
	
	//2. Insert Columns
    LVCOLUMN    lvc;
    lvc.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	
	int i, size = sizeof(columns)/sizeof(columns[0]);
    for(i = 0; i < size; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = columns[i].lpszName;
		lvc.cx = columns[i].nWidth;
		
        m_dataListCtrl.InsertColumn(i, &lvc);
    }
	
	//3. Init Header Ctrl
	HDITEM hditem;
	for(i = 0; i < size; i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		m_dataListCtrl.GetHeaderCtrl()->GetItem(i, &hditem);
		
		hditem.fmt |= columns[i].nAlign;
		
		m_dataListCtrl.GetHeaderCtrl()->SetItem(i, &hditem);
	}

	m_dataListCtrl.SendMeasureItemMsg();

	CTaskInfo* pTaskInfo = new CTaskInfo();
	pTaskInfo->m_szFileName = _T("C:\\temp\\myfile1.txt");
	m_dataListCtrl.AddRow(pTaskInfo);

	pTaskInfo = new CTaskInfo();
	pTaskInfo->m_szFileName = _T("C:\\temp\\myfile2.txt");
	m_dataListCtrl.AddRow(pTaskInfo);
}

void CFlickerFreeListCtrlDemoDlg::OnDestroy() 
{
	m_dataListCtrl.DeleteAllItems();
	CDialog::OnDestroy();
}
