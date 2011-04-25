// ListCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "ListCtrlDemoDlg.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct ColumnInfo
{
    LPCTSTR     name;
    SORT_TYPE   sortType;
    SORT_STATE  sortState;
    int         lengthExtension;
    int         fmt;
};

static ColumnInfo columns[] =
{
    { _T("File Name"),  				TYPE_TEXT,    DESCENDING,   40 },
    { _T("Extension"),  				TYPE_TEXT,    DESCENDING,   0  },
    { _T("Path"),       				TYPE_TEXT,    DESCENDING,   25 },
    { _T("Lines"),						TYPE_NUMERIC, DESCENDING,   15, LVCFMT_RIGHT },
    { _T("Code Only"),					TYPE_NUMERIC, DESCENDING,  -10, LVCFMT_RIGHT },
    { _T("Comments Only"),				TYPE_NUMERIC, DESCENDING,  -10, LVCFMT_RIGHT },
    { _T("Both (Code /w Comments)"),	TYPE_NUMERIC, DESCENDING,  -75, LVCFMT_RIGHT },
    { _T("Blank"),      				TYPE_NUMERIC, DESCENDING,   15, LVCFMT_RIGHT }
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
// CListCtrlDemoDlg dialog

CListCtrlDemoDlg::CListCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CListCtrlDemoDlg::IDD, pParent), m_pProgressDlg(NULL)
{
	//{{AFX_DATA_INIT(CListCtrlDemoDlg)
	m_recursiveSubBtn = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlDemoDlg)
	DDX_Control(pDX, IDC_FILTER_COMBO, m_filterComboBox);
	DDX_Control(pDX, IDC_SOURCE_DIR_LIST, m_srcDirListBox);
	DDX_Control(pDX, IDC_RESULT_LIST, m_resultListCtrl);
	DDX_Check(pDX, IDC_RECURSIVE_SUB_CHECK, m_recursiveSubBtn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CListCtrlDemoDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CListCtrlDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_MESSAGE(WM_START_COUNT, OnStartCount)
	ON_MESSAGE(WM_END_COUNT, OnEndCount)
	ON_MESSAGE(WM_PROGRESS_UPDATE, OnProgressUpdate)
	ON_MESSAGE(WM_SUMMARY_UPDATE, OnSummaryUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg message handlers

BOOL CListCtrlDemoDlg::OnInitDialog()
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
	InitResultListCtrl();
	//ResizableDialog Init
	InitResizableDlgAnchor();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListCtrlDemoDlg::InitResizableDlgAnchor()
{
    AddAnchor(IDC_MENU_SEP, TOP_LEFT, TOP_RIGHT);
	
    AddAnchor(IDC_SOURCE_DIR_FRAME, TOP_LEFT, TOP_RIGHT);
	
    AddAnchor(IDC_SOURCE_DIR_LIST, TOP_LEFT,    TOP_RIGHT);
    AddAnchor(IDC_FILTER_COMBO,		TOP_LEFT,    TOP_RIGHT);
    AddAnchor(IDC_RECURSIVE_SUB_CHECK,		TOP_LEFT, TOP_LEFT);
	
    AddAnchor(IDC_BUTTON_ADD, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_BUTTON_DEL, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_BUTTON_START, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_BUTTON_STOP, TOP_RIGHT, TOP_RIGHT);
	
    AddAnchor(IDC_RESULT_FRAME, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_RESULT_LIST, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_SUMMARY_FRAME, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TXT_TOTAL, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_FILE_NUMBER, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_LINES, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_SHARP, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_NUMBER, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_LINES, BOTTOM_LEFT, BOTTOM_LEFT);
	
    EnableSaveRestore("ListCtrlDemoDlg");
}

void CListCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CListCtrlDemoDlg::OnPaint() 
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
HCURSOR CListCtrlDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CListCtrlDemoDlg::InitResultListCtrl()
{
    m_resultListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM) 
        LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT |
        m_resultListCtrl.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE));

	int         i;
    LVCOLUMN    lvc;
	
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	int size = sizeof(columns)/sizeof(columns[0]);

    for(i = 0; i < size; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = (char *)columns[i].name;
        lvc.cx = m_resultListCtrl.GetStringWidth(lvc.pszText) + 
            columns[i].lengthExtension + 15;
        if (lvc.cx < 40) lvc.cx = 40;
        lvc.fmt = columns[i].fmt;
        m_resultListCtrl.InsertColumn(i, columns[i].sortType, columns[i].sortState, 
            &lvc);
    }
	
	m_resultListCtrl.SortColumn( 2, false );
	m_resultListCtrl.SortColumn( 1, true );
	m_resultListCtrl.SortColumn( 0, true );
	
	m_resultListCtrl.SetUniqueName( "ResultsListCtrl" );
	m_resultListCtrl.LoadColumnWidths();
    m_resultListCtrl.LoadColumnOrder();
    m_resultListCtrl.LoadColumnSort();
}


void CListCtrlDemoDlg::OnClose() 
{
	DeleteAllListItems();
	CResizableDialog::OnClose();
}

void CListCtrlDemoDlg::DeleteAllListItems()
{
	int nCount = m_resultListCtrl.GetItemCount();
	CFileInfo* pFi = NULL;
	for(int i = 0; i < nCount; i++)
	{
		pFi = (CFileInfo*)m_resultListCtrl.GetItemData(i);
		ASSERT(pFi);
		delete pFi;
	}
	m_resultListCtrl.DeleteAllItems();
}

void CListCtrlDemoDlg::ResetResult()
{
	//1. list ctrl
	DeleteAllListItems();
	
	//2. Summray info
	SetDlgItemText(IDC_NUMBER, "");
	SetDlgItemText(IDC_LINES, "");
}

void CListCtrlDemoDlg::RefreshFilterArrays()
{
	m_sFilterArray.RemoveAll();
	
	CString sFilter;
	m_filterComboBox.GetWindowText(sFilter);
	AfxTrace("Original Filter: [%s]\n", sFilter);

	sFilter.TrimLeft();
	sFilter.TrimRight();
	if(sFilter.IsEmpty())
	{
		return;
	}

	char cSep = ';';
	int nStart = 0, nIndex;
	CString str;
	while( (nIndex = sFilter.Find(cSep, nStart)) != -1)
	{
		if(nIndex > nStart)
		{
			str = sFilter.Mid(nStart, nIndex - nStart);
			m_sFilterArray.Add(str);
		}
		nStart = nIndex + 1;
	}
	//Not the last char
	if(nStart < sFilter.GetLength())
	{
		str = sFilter.Mid(nStart);
		m_sFilterArray.Add(str);
	}
#ifdef _DEBUG
	for(int i = 0; i < m_sFilterArray.GetSize(); i++)
	{
		AfxTrace("Filer(%d): [%s]\n", i + 1, m_sFilterArray.GetAt(i));
	}
#endif
}

int CListCtrlDemoDlg::AddRow(const CFileInfo& fi)
{
	int nRes = -1;
	
    LV_ITEM lvi;
	
    lvi.mask = LVIF_TEXT | LVIF_PARAM;
    lvi.iItem = m_resultListCtrl.GetItemCount();
    lvi.lParam = (LPARAM)&fi;
	
    lvi.iSubItem = 0;
    lvi.pszText = (LPTSTR)(LPCTSTR)fi.m_sFileName;
    nRes = m_resultListCtrl.InsertItem(&lvi);
	
    int iSubItem = 0;
    m_resultListCtrl.SetItemText(lvi.iItem, ++iSubItem, fi.m_sFileExt);
    m_resultListCtrl.SetItemText(lvi.iItem, ++iSubItem, fi.m_sFilePath);

	m_resultListCtrl.SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nTotalLines));
    m_resultListCtrl.SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nCodeLines));
    m_resultListCtrl.SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nCommentLines));
    m_resultListCtrl.SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.GetMixedLines()));
    m_resultListCtrl.SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nBlankLines));

	return nRes;
}

void CListCtrlDemoDlg::OnButtonAdd() 
{
	//Add selected directoy
	char szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath)); 

	LPITEMIDLIST pidlSelected = NULL;
    BROWSEINFO bi = {0};

	bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = "Choose a folder";
    bi.ulFlags = 0;
    bi.lpfn = NULL;
    bi.lParam = 0;

	pidlSelected = SHBrowseForFolder(&bi);
	//Got a valid path
	if(pidlSelected && SHGetPathFromIDList(pidlSelected, szPath))
	{
		AfxTrace("%s\n", szPath);
		m_srcDirListBox.AddString(szPath);
	}
}

void CListCtrlDemoDlg::OnButtonDel() 
{
	int nIndex = m_srcDirListBox.GetCurSel();
	if(nIndex != LB_ERR)
	{
		m_srcDirListBox.DeleteString(nIndex);
	}
}	

void CListCtrlDemoDlg::OnButtonStart() 
{
	//check validation
	int nCount = m_srcDirListBox.GetCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("Please select at least one directory."));
		return;
	}
	
	RefreshFilterArrays();
	//Remove existing data
	ResetResult();

	//Prepare parameters
	LPCountThreadParam lpThreadParam = new CountThreadParam;
	lpThreadParam->hwndMain = GetSafeHwnd();
	
	CString sDir;
	for(int i = 0; i < nCount; i++)
	{
		m_srcDirListBox.GetText(i, sDir);
		lpThreadParam->dirList.Add(sDir);
	}
	CWinThread* pThread = AfxBeginThread(CCounter::CountThreadProc, lpThreadParam);
}


void CListCtrlDemoDlg::OnButtonStop() 
{
	ResetResult();
}

LRESULT CListCtrlDemoDlg::OnStartCount(WPARAM wParam, LPARAM lParam)
{
	//Show ProgressDlg
	if(m_pProgressDlg != NULL)
	{
		ASSERT_VALID (m_pProgressDlg);		
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	}
	m_pProgressDlg = new CProgressDlg();
	m_pProgressDlg->Create(this);
	m_pProgressDlg->SetWindowText("Changed Caption");
	m_pProgressDlg->SetStatus("Empty Status");
	m_pProgressDlg->UpdateWindow();
	m_pProgressDlg->ShowWindow(SW_NORMAL);
	return (LRESULT)m_pProgressDlg->GetSafeHwnd();
}

LRESULT CListCtrlDemoDlg::OnEndCount(WPARAM wParam, LPARAM lParam)
{
	if(m_pProgressDlg != NULL)
	{
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	}
	return (LRESULT)TRUE;
}

LRESULT CListCtrlDemoDlg::OnProgressUpdate(WPARAM wParam, LPARAM lParam)
{
	CFileInfo* pFi = (CFileInfo*)wParam;
	AddRow(*pFi);

	return 0;
}

LRESULT CListCtrlDemoDlg::OnSummaryUpdate(WPARAM wParam, LPARAM lParam)
{
	int nTotalCount = (int)wParam;

	int i, nCount = m_resultListCtrl.GetItemCount();
	if(nTotalCount <= 0)
	{
		nTotalCount = nCount;
	}

	int nTotalLines = 0, nCodeLines = 0, nCommentLines = 0, nMixedLines = 0, nBlankLines = 0;
	CFileInfo* pFi = NULL;
	for(i = 0; i < nCount; i++)
	{
		pFi = (CFileInfo*)m_resultListCtrl.GetItemData(i);
		ASSERT(pFi);
		nTotalLines += pFi->m_nTotalLines;
		nCodeLines += pFi->m_nCodeLines;
		nCommentLines += pFi->m_nCommentLines;
		nMixedLines += pFi->GetMixedLines();
		nBlankLines += pFi->m_nBlankLines;
	}

	SetDlgItemInt(IDC_NUMBER, nTotalCount);
	SetDlgItemInt(IDC_LINES, nTotalLines);

	return 0;
}

