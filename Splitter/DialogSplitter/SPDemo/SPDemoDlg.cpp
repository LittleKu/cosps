// SPDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SPDemo.h"
#include "SPDemoDlg.h"

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
// CSPDemoDlg dialog

CSPDemoDlg::CSPDemoDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CSPDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSPDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPDemoDlg)
	DDX_Control(pDX, IDC_EDIT, m_txtContent);
	DDX_Control(pDX, IDC_LIST, m_lstItem);
	DDX_Control(pDX, IDC_TREE, m_wndType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSPDemoDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CSPDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPDemoDlg message handlers

BOOL CSPDemoDlg::OnInitDialog()
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
	CRect rc;
	CWnd* pWnd;

	pWnd = GetDlgItem(IDC_SPLITTER1);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter1.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER1);
	SetSplitterRange();
//	m_wndSplitter1.SetRange(50, 50, -1);
	
	pWnd = GetDlgItem(IDC_SPLITTER2);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter2.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER2);
	m_wndSplitter2.SetRange(50, 50, -1);

	AddAnchor(IDC_STATIC, TOP_LEFT, TOP_RIGHT);
	AddAnchor(m_wndType.m_hWnd, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(m_wndSplitter1, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_LIST, TOP_LEFT, TOP_RIGHT);
	AddAnchor(m_wndSplitter2, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT, BOTTOM_RIGHT);

	InitSampleData();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSPDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSPDemoDlg::OnPaint() 
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
HCURSOR CSPDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CSPDemoDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITTER1)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoResize1(pHdr->delta);
		}
		else if (wParam == IDC_SPLITTER2)
		{
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoResize2(pHdr->delta);
		}
	}
	
	return CResizableDialog::DefWindowProc(message, wParam, lParam);
}

void CSPDemoDlg::DoResize1(int delta)
{
	CSplitterControl::ChangeWidth(&m_wndType, delta);
	CSplitterControl::ChangeWidth(&m_lstItem, -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(&m_txtContent, -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(&m_wndSplitter2, -delta, CW_RIGHTALIGN);

//	RemoveAnchor(IDC_STATIC);
	
	RemoveAnchor(m_wndType.m_hWnd);
	
	RemoveAnchor(m_wndSplitter1);
	
	RemoveAnchor(IDC_LIST);
	
	RemoveAnchor(m_wndSplitter2);
	
	RemoveAnchor(IDC_EDIT);
	
	RemoveAnchor(IDCANCEL);
	
	RemoveAnchor(IDOK);
	

//	AddAnchor(IDC_STATIC, TOP_LEFT, TOP_RIGHT);
	AddAnchor(m_wndType.m_hWnd, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(m_wndSplitter1, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_LIST, TOP_LEFT, TOP_RIGHT);
	AddAnchor(m_wndSplitter2, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT, BOTTOM_RIGHT);

	SetSplitterRange();

	Invalidate();
	UpdateWindow();
}

void CSPDemoDlg::DoResize2(int delta)
{
	CSplitterControl::ChangeHeight(&m_lstItem, delta);
	CSplitterControl::ChangeHeight(&m_txtContent, -delta, CW_BOTTOMALIGN);


	RemoveAnchor(IDC_LIST);
	AddAnchor(IDC_LIST, TOP_LEFT, TOP_RIGHT);
	RemoveAnchor(m_wndSplitter2);
	AddAnchor(m_wndSplitter2, TOP_LEFT, TOP_RIGHT);
	RemoveAnchor(IDC_EDIT);
	AddAnchor(IDC_EDIT, TOP_LEFT, BOTTOM_RIGHT);
// 	RemoveAnchor(IDCANCEL);
// 	AddAnchor(IDCANCEL, BOTTOM_RIGHT, BOTTOM_RIGHT);
// 	RemoveAnchor(IDOK);
// 	AddAnchor(IDOK, BOTTOM_RIGHT, BOTTOM_RIGHT);

	Invalidate();
	UpdateWindow();
}


void CSPDemoDlg::InitSampleData()
{
	m_wndType.ModifyStyle(0, TVS_LINESATROOT | TVS_HASBUTTONS
		| TVS_SHOWSELALWAYS | TVS_HASLINES );
	HTREEITEM hRoot = m_wndType.InsertItem("Local folder");
	HTREEITEM h1 = m_wndType.InsertItem("Inbox", hRoot);
	HTREEITEM h2 = m_wndType.InsertItem("Outbox", hRoot);
	HTREEITEM h3 = m_wndType.InsertItem("Send Items", hRoot);
	m_wndType.SetItemData(hRoot, 0);
	m_wndType.SetItemData(h1, 1);
	m_wndType.SetItemData(h2, 2);
	m_wndType.SetItemData(h3, 3);

	m_lstItem.ModifyStyle(0, LVS_REPORT);
	m_lstItem.InsertColumn(0, "From", LVCFMT_LEFT, 100);
	m_lstItem.InsertColumn(1, "Subject", LVCFMT_LEFT, 100);
	
}

void CSPDemoDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	HTREEITEM h = m_wndType.GetSelectedItem();
	DWORD nID = m_wndType.GetItemData(h);

	m_lstItem.DeleteAllItems();
	m_lstItem.DeleteColumn(1);
	m_lstItem.DeleteColumn(0);
	
	switch(nID)
	{
		case 0:break;
		case 1:
			m_lstItem.InsertColumn(0, "From", LVCFMT_LEFT, 100);
			m_lstItem.InsertColumn(1, "Subject", LVCFMT_LEFT, 200);
			m_lstItem.InsertItem(0, "Dad");
			m_lstItem.SetItemText(0, 1, "Dad's letter");
			m_lstItem.SetItemData(0, 0);

			m_lstItem.InsertItem(1, "AnhPhong");
			m_lstItem.SetItemText(1, 1, "Hi, how are you ?");
			m_lstItem.SetItemData(1, 1);

			m_lstItem.InsertItem(2, "TrungHau");
			m_lstItem.SetItemText(2, 1, "Reply to Hi");
			m_lstItem.SetItemData(2, 2);
			break;
		case 2:
			m_lstItem.InsertColumn(0, "Subject", LVCFMT_LEFT, 200);
			m_lstItem.InsertColumn(1, "Recipcent", LVCFMT_LEFT, 100);
			m_lstItem.InsertItem(0, "Reply to Dad's letter");
			m_lstItem.SetItemData(0, 100);

			m_lstItem.SetItemText(0, 1, "Dad");
			m_lstItem.InsertItem(1, "I'm fine, and you !");
			m_lstItem.SetItemText(1, 1, "AnhPhong");
			m_lstItem.SetItemData(1, 101);
			break;
		case 3:
			m_lstItem.InsertColumn(0, "From", LVCFMT_LEFT, 100);
			m_lstItem.InsertColumn(1, "Subject", LVCFMT_LEFT, 200);
			m_lstItem.InsertItem(0, "TrungHau");
			m_lstItem.SetItemText(0, 1, "Hi");
			m_lstItem.SetItemData(0, 200);
			break;
	}
}

void CSPDemoDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	
	CString strContent = "";
	POSITION pos = m_lstItem.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nCurSel = m_lstItem.GetNextSelectedItem(pos);
		int n = m_lstItem.GetItemData(nCurSel);
		switch(n)
		{
			case 0: strContent =	"content of Dad's letter";
				break;
			case 1: strContent = "content of Hi, how are you ?";
				break;
			case 2: strContent = "content of Reply to Hi ";
				break;
			
			case 100 : strContent = "content of Reply to Dad's letter";
				break;
			case 101: strContent = "content of I'm fine, and you !";
				break;

			case 200: strContent = "content of Hi";
				break;

		}
	}
	m_txtContent.SetWindowText(strContent);
}

void CSPDemoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_hWnd != NULL && ::IsWindow(m_hWnd))
	{
		SetSplitterRange();
	}	
}

void CSPDemoDlg::SetSplitterRange()
{
	if(m_wndType.m_hWnd != NULL && ::IsWindow(m_wndType.m_hWnd) && m_lstItem.m_hWnd != NULL && ::IsWindow(m_lstItem.m_hWnd))
	{
		CRect rectLeft;
		m_wndType.GetWindowRect(&rectLeft);
		ScreenToClient(&rectLeft);
		
		CRect rectRight;
		m_lstItem.GetWindowRect(&rectRight);
		ScreenToClient(&rectRight);
		
		m_wndSplitter1.SetRange(rectLeft.left + 20, rectRight.right - 20);
	}
}
