// CheckListCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckListCtrlDemo.h"
#include "CheckListCtrlDemoDlg.h"

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
// CCheckListCtrlDemoDlg dialog
enum COLUMN_TYPE
{ 
	TYPE_TEXT,
	TYPE_CHECKBOX
};
struct ColumnInfo
{
    LPCTSTR     name;
	COLUMN_TYPE type;
    int         fmt;
	int         width;
};

static ColumnInfo columns[] =
{
    { _T(""),  				TYPE_CHECKBOX,		LVCFMT_CENTER,	50},
    { _T("Description"),    TYPE_TEXT,			LVCFMT_LEFT,	200},
    { _T("Enable"),			TYPE_CHECKBOX,		LVCFMT_LEFT,	80},
	{ _T("Last"),			TYPE_TEXT,			LVCFMT_LEFT,	150}
};
CCheckListCtrlDemoDlg::CCheckListCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckListCtrlDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckListCtrlDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckListCtrlDemoDlg)
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCheckListCtrlDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckListCtrlDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK , IDC_LIST1, OnRClick)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED, OnCheckbox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrlDemoDlg message handlers

BOOL CCheckListCtrlDemoDlg::OnInitDialog()
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

void CCheckListCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckListCtrlDemoDlg::OnPaint() 
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
HCURSOR CCheckListCtrlDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCheckListCtrlDemoDlg::InitListCtrl()
{
	DWORD dwExtendedStyle = m_listCtrl.GetExtendedStyle();
	dwExtendedStyle = ((dwExtendedStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT) & (~LVS_EX_TRACKSELECT));
	m_listCtrl.SetExtendedStyle(dwExtendedStyle);
	
	int         i;
    LVCOLUMN    lvc;
	
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	int size = sizeof(columns)/sizeof(columns[0]);
	
    for(i = 0; i < size; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = (LPTSTR)columns[i].name;
		lvc.cx = columns[i].width;
        lvc.fmt = columns[i].fmt;
        m_listCtrl.InsertColumn(i, &lvc);
    }
	
	
	HDITEM hditem;
	CCheckListCtrl* pList = &m_listCtrl;
	for (i = 0; i < pList->m_HeaderCtrl.GetItemCount(); i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		pList->m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		if (i == 0 || i == 2)
			hditem.iImage = CL_CHECKED;
		else if(i == 1)
			hditem.iImage = CL_UNCHECKED;
		else
			hditem.iImage = CL_NONE_CHECK_BOX;
		
		pList->m_HeaderCtrl.SetItem(i, &hditem);
	}
	
	m_listCtrl.SetRedraw(FALSE);
	CString cs(_T(""));
	int iSubItem = 0;
	int nItem;
	for(i = 0; i < 8; i++)
	{
		cs = "";
		nItem = m_listCtrl.InsertItem(i, cs);
		if(nItem < 0)
		{
			continue;
		}
		iSubItem = 0;
		m_listCtrl.SetItemCheckedState(nItem, iSubItem, CL_UNCHECKED, FALSE);

		cs.Format(_T("Second column name. To make this column longer. %d"), i+1);
//		cs = "";
		m_listCtrl.SetItemText(i, ++iSubItem, cs);
		m_listCtrl.SetItemCheckedState(nItem, iSubItem, CL_CHECKED, FALSE);
		
		cs.Format(_T("Enable %d"), i+1);
		m_listCtrl.SetItemText(i, ++iSubItem, cs);
		m_listCtrl.SetItemCheckedState(nItem, iSubItem, CL_CHECKED, FALSE);
		
		cs.Format(_T("Last %d"), i+1);
		m_listCtrl.SetItemText(i, ++iSubItem, cs);
//		m_listCtrl.SetItemCheckedState(nItem, iSubItem, CL_CHECKED, FALSE);
	}
	m_listCtrl.SetRedraw(TRUE);
	m_listCtrl.ValidateCheck();
}


void CCheckListCtrlDemoDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE lpNMActivate = (LPNMITEMACTIVATE)pNMHDR;
	*pResult = 0;

	DWORD pVirginData = m_listCtrl.CListCtrl::GetItemData(lpNMActivate->iItem);

	CCheckListItemData* pData = (CCheckListItemData*)pVirginData;

	DWORD pAppData = m_listCtrl.GetItemData(lpNMActivate->iItem);
	AfxTrace("OnRClick: %d, %d, %d, %d, %d, %d\n", lpNMActivate->iItem, lpNMActivate->iSubItem, 
		lpNMActivate->ptAction.x, lpNMActivate->ptAction.y, lpNMActivate->lParam, lpNMActivate->uNewState);
}

LRESULT CCheckListCtrlDemoDlg::OnCheckbox(WPARAM nItem, LPARAM nSubItem)
{
	AfxTrace("OnCheckbox: %d, %d\n", nItem, nSubItem);
	return 0;
}