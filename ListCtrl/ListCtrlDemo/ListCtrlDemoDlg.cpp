// ListCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "ListCtrlDemoDlg.h"
#include "ProgressDlg.h"
#include "Export.h"
#include "Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg dialog

CListCtrlDemoDlg::CListCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CListCtrlDemoDlg::IDD, pParent), m_pProgressDlg(NULL)
{
	//{{AFX_DATA_INIT(CListCtrlDemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CListCtrlDemoDlg::~CListCtrlDemoDlg()
{
}

void CListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlDemoDlg)
	DDX_Control(pDX, IDC_FILTER_TREE, m_filterTree);
	DDX_Control(pDX, IDC_FILTER_COMBO, m_filterComboBox);
	DDX_Control(pDX, IDC_SOURCE_DIR_LIST, m_srcDirListCtrl);
	DDX_Control(pDX, IDC_RESULT_LIST, m_resultListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CListCtrlDemoDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CListCtrlDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_START_COUNT, OnStartCount)
	ON_MESSAGE(WM_END_COUNT, OnEndCount)
	ON_MESSAGE(WM_PROGRESS_UPDATE, OnProgressUpdate)
	ON_MESSAGE(WM_SUMMARY_UPDATE, OnSummaryUpdate)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(ID_TREE_ITEM_SELECTED_EVENT, OnTreeItemSelected)
	ON_WM_CTLCOLOR()
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
	InitSplitters();

	m_filterComboBox.Init();

	InitFilterTree();

	//ResizableDialog Init
	InitResizableDlgAnchor();
	
	//Source dir list
	InitSrcDirListCtrl();

	//Result List
	InitResultListCtrl();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListCtrlDemoDlg::InitFilterTree()
{
	m_checkBoxStateIL.Create(IDB_FILTER_TREE_CHECK_BOX_STATE, 13, 1, RGB(255,255,255));
	m_selectedStateIL.Create(IDB_FILTER_TREE_SEL_STATE, 16, 1, RGB(255,255,255));
	
//	m_filterTree.SetImageList(&m_selectedStateIL, TVSIL_NORMAL);
	m_filterTree.SetImageList(&m_checkBoxStateIL, TVSIL_STATE);

	m_filterTree.Init();
//	m_filterTree.ExpandAllItems();
	OnTreeItemSelected(0, 0);
}
void CListCtrlDemoDlg::InitResizableDlgAnchor()
{
	//Anchor for splitters
	AddAnchor(m_splitterVertical.m_hWnd, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(m_splitterHorizontal.m_hWnd, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_FILTER_TREE, TOP_LEFT, BOTTOM_LEFT);
    AddAnchor(IDC_SOURCE_DIR_FRAME, TOP_LEFT, TOP_RIGHT);
	
    AddAnchor(IDC_SOURCE_DIR_LIST, TOP_LEFT,    TOP_RIGHT);
    AddAnchor(IDC_FILTER_COMBO,		TOP_LEFT,    TOP_RIGHT);

	
    AddAnchor(IDC_RESULT_FRAME, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_RESULT_LIST, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_SUMMARY_FRAME, BOTTOM_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_TXT_TOTAL, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_FILE_NUMBER, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_LINES, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_CODE, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_COMMENT, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_MIXED, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_TXT_BLANK, BOTTOM_LEFT, BOTTOM_LEFT);

	AddAnchor(IDC_TXT_C, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_NUMBER_C, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_LINES_C, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_CODE_C, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_COMMENT_C, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_MIXED_C, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_BLANK_C, BOTTOM_LEFT, BOTTOM_LEFT);
	

	AddAnchor(IDC_TXT_P, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_NUMBER_P, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_LINES_P, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_CODE_P, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_COMMENT_P, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_MIXED_P, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_BLANK_P, BOTTOM_LEFT, BOTTOM_LEFT);

	SetSizeGripVisibility(FALSE);
	UpdateSizeGrip();
}

void CListCtrlDemoDlg::InitSplitters()
{
	m_splitterVBkBitmap.LoadBitmap(IDB_SPLITTER_V_BKG);
	m_splitterHBkBitmap.LoadBitmap(IDB_SPLITTER_H_BKG);
	CRect rc;
	CWnd* pWnd;
	
	pWnd = GetDlgItem(IDC_SPLITTER_VERTICAL);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	pWnd->DestroyWindow();
	m_splitterVertical.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER_VERTICAL);
	m_splitterVertical.m_pBkBitmap = &m_splitterVBkBitmap;
	SetSplitterVRange();
	
	pWnd = GetDlgItem(IDC_SPLITTER_HORIZONTAL);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	pWnd->DestroyWindow();
	m_splitterHorizontal.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER_HORIZONTAL);
	m_splitterHorizontal.m_pBkBitmap = &m_splitterHBkBitmap;
	SetSplitterHRange();
}

void CListCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
// 		CAboutDlg dlgAbout;
// 		dlgAbout.DoModal();
	}
	else
	{
		CResizableDialog::OnSysCommand(nID, lParam);
	}
}
BOOL CListCtrlDemoDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	pDC->FillRect(&rect, SYS_APP()->m_pSysBkBrush);

	return TRUE;
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
	m_resultListCtrl.Init();
}


void CListCtrlDemoDlg::OnDestroy() 
{
	m_sFilterArray.RemoveAll();

	DeleteAllListItems();
	CResizableDialog::OnDestroy();
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
	m_totalInfo.Reset();

	SetPair(IDC_NUMBER_C, IDC_NUMBER_P, -1);
	
	SetPair(IDC_LINES_C, IDC_LINES_P, -1);
	SetPair(IDC_CODE_C, IDC_CODE_P, -1);
	SetPair(IDC_COMMENT_C, IDC_COMMENT_P, -1);
	SetPair(IDC_MIXED_C, IDC_MIXED_P, -1);
	SetPair(IDC_BLANK_C, IDC_BLANK_P, -1);
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

	if(m_filterComboBox.AddString(sFilter) >= 0)
	{
		m_filterComboBox.SetWindowText(sFilter);
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

void CListCtrlDemoDlg::OnButtonAdd() 
{
	/*
	//Add selected directoy
	char szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath)); 

    BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = "Choose a folder";

	LPITEMIDLIST pidlSelected = SHBrowseForFolder(&bi);

	//Got a valid path
	if(pidlSelected != NULL)
	{
		if(SHGetPathFromIDList(pidlSelected, szPath))
		{
			m_srcDirListCtrl.AddSrcDir(szPath);
		}
		//Free it
		LPMALLOC pMalloc = NULL;
		if(SUCCEEDED(SHGetMalloc(&pMalloc)))
        {
            pMalloc->Free(pidlSelected);
            pMalloc->Release();
        }
	}
	*/
	TCHAR szPath[MAX_PATH] = {0};
	_sntprintf(szPath, _countof(szPath), _T("%s"), SYS_PREF()->m_szLastSelectedSrcFolder);
	szPath[_countof(szPath) - 1] = _T('\0');
	if( gtb::BrowseFolder(GetSafeHwnd(), szPath, _T("Select Source Code Folder")) )
	{
		m_srcDirListCtrl.AddSrcDir(szPath);
	}
}

void CListCtrlDemoDlg::OnButtonDel() 
{
	//Delete all selected items
	POSITION pos = m_srcDirListCtrl.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("Please select at least one source folder to delete."));
		return;
	}
	int nItem = -1;
	while (pos != NULL)
	{
		nItem = m_srcDirListCtrl.GetNextSelectedItem(pos);
		AfxTrace("Selected %d\n", nItem);
		m_srcDirListCtrl.DeleteItem(nItem);
		
		//Delete the previous one item will affect the pos
		pos = m_srcDirListCtrl.GetFirstSelectedItemPosition();
	}
}	

void CListCtrlDemoDlg::OnButtonStart() 
{
	//check validation
	int nCount = m_srcDirListCtrl.CountCheckedItems(0);
	if(nCount <= 0)
	{
		AfxMessageBox(_T("Please select at least one directory."));
		return;
	}

	//Remove existing data
	ResetResult();
	
	//Prepare parameters
	LPCountThreadParam lpThreadParam = new CountThreadParam;
	lpThreadParam->hwndMain = GetSafeHwnd();
	
	//Dir list
	CString sDir;
	nCount = m_srcDirListCtrl.GetItemCount();
	int i;
	for(i = 0; i < nCount; i++)
	{
		if(m_srcDirListCtrl.GetItemCheckedState(i, 0) == CL_CHECKED)
		{
			sDir = m_srcDirListCtrl.GetItemText(i, 1);
			lpThreadParam->dirList.Add(sDir);
		}
	}

	//Filter
	RefreshFilterArrays();
	nCount = m_sFilterArray.GetSize();
	for(i = 0; i < nCount; i++)
	{
		lpThreadParam->filterList.Add(m_sFilterArray.GetAt(i));
	}

	//Is recursive?
	lpThreadParam->bRecursive = SYS_PREF()->m_bSearchIncludeSubFolders;
	
	CWinThread* pThread = AfxBeginThread(CCounter::CountThreadProc, lpThreadParam);
}


void CListCtrlDemoDlg::OnButtonClear() 
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
	m_pProgressDlg->SetWindowText(_T("Please Wait..."));
	m_pProgressDlg->SetStatus(_T("Start To Count..."));
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
	m_resultListCtrl.AddRow(*pFi);

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
	m_totalInfo.m_nTotalCount = nTotalCount;

//	int nTotalLines = 0, nCodeLines = 0, nCommentLines = 0, nMixedLines = 0, nBlankLines = 0;
	CFileInfo* pFi = NULL;
	for(i = 0; i < nCount; i++)
	{
		pFi = (CFileInfo*)m_resultListCtrl.GetItemData(i);
		ASSERT(pFi);
		m_totalInfo.m_nTotalLines += pFi->m_nTotalLines;
		m_totalInfo.m_nTotalCodeLines += pFi->m_nCodeLines;
		m_totalInfo.m_nTotalCommentLines += pFi->m_nCommentLines;
		m_totalInfo.m_nTotalMixedLines += pFi->GetMixedLines();
		m_totalInfo.m_nTotalBlankLines += pFi->m_nBlankLines;
	}

	SetPair(IDC_NUMBER_C, IDC_NUMBER_P, m_totalInfo.m_nTotalCount, m_totalInfo.m_nTotalCount);

	SetPair(IDC_LINES_C, IDC_LINES_P, m_totalInfo.m_nTotalLines, m_totalInfo.m_nTotalLines);
	SetPair(IDC_CODE_C, IDC_CODE_P, m_totalInfo.m_nTotalCodeLines, m_totalInfo.m_nTotalLines);
	SetPair(IDC_COMMENT_C, IDC_COMMENT_P, m_totalInfo.m_nTotalCommentLines, m_totalInfo.m_nTotalLines);
	SetPair(IDC_MIXED_C, IDC_MIXED_P, m_totalInfo.m_nTotalMixedLines, m_totalInfo.m_nTotalLines);
	SetPair(IDC_BLANK_C, IDC_BLANK_P, m_totalInfo.m_nTotalBlankLines, m_totalInfo.m_nTotalLines);

	return 0;
}


void CListCtrlDemoDlg::SetPair(int idc, int idp, int count, int total)
{
	if(count < 0)
	{
		GetDlgItem(idc)->SetWindowText("");
		GetDlgItem(idp)->SetWindowText("");
		return;
	}

	SetDlgItemInt(idc, count);

	CString sPercent = CommonUtils::GetPercentStr(count, total);
	SetDlgItemText(idp, sPercent);
}

void CListCtrlDemoDlg::InitSrcDirListCtrl()
{
	m_srcDirListCtrl.Init();
}

LRESULT CListCtrlDemoDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITTER_VERTICAL)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoSizeVertical(pHdr->delta);
		}
		else if (wParam == IDC_SPLITTER_HORIZONTAL)
		{
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoSizeHorizontal(pHdr->delta);
		}
	}
	
	return CResizableDialog::DefWindowProc(message, wParam, lParam);
}

void CListCtrlDemoDlg::DoSizeVertical(int delta)
{
	CSplitterControl::ChangeWidth(GetDlgItem(IDC_FILTER_TREE), delta, CW_LEFTALIGN);

	CSplitterControl::ChangeWidth(GetDlgItem(IDC_SOURCE_DIR_FRAME), -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(&m_srcDirListCtrl, -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(&m_filterComboBox, -delta, CW_RIGHTALIGN);

	CSplitterControl::ChangeWidth(&m_splitterHorizontal, -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(GetDlgItem(IDC_RESULT_FRAME), -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(&m_resultListCtrl, -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(GetDlgItem(IDC_SUMMARY_FRAME), -delta, CW_RIGHTALIGN);

	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_TOTAL), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_FILE_NUMBER), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_LINES), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_CODE), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_COMMENT), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_MIXED), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_BLANK), -delta, 0);

	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_C), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_NUMBER_C), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_LINES_C), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_CODE_C), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_COMMENT_C), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_MIXED_C), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_BLANK_C), -delta, 0);

	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_P), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_NUMBER_P), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_LINES_P), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_CODE_P), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_COMMENT_P), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_MIXED_P), -delta, 0);
	CSplitterControl::ChangePos(GetDlgItem(IDC_BLANK_P), -delta, 0);
	
	SetSplitterVRange();

	UpdateResizableDlgAnchor();
	
	Invalidate();
	UpdateWindow();
}

void CListCtrlDemoDlg::DoSizeHorizontal(int delta)
{
	CSplitterControl::ChangeHeight(GetDlgItem(IDC_SOURCE_DIR_FRAME), delta, CW_TOPALIGN);
	CSplitterControl::ChangeHeight(&m_srcDirListCtrl, delta, CW_TOPALIGN);
	CSplitterControl::ChangePos(&m_filterComboBox, 0, delta);

	CSplitterControl::ChangeHeight(GetDlgItem(IDC_RESULT_FRAME), -delta, CW_BOTTOMALIGN);
	CSplitterControl::ChangeHeight(&m_resultListCtrl, -delta, CW_BOTTOMALIGN);

// 	CSplitterControl::ChangePos(GetDlgItem(IDC_SUMMARY_FRAME), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_TOTAL), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_FILE_NUMBER), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_LINES), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_CODE), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_COMMENT), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_MIXED), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_BLANK), 0, delta);
// 	
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_C), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_NUMBER_C), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_LINES_C), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_CODE_C), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_COMMENT_C), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_MIXED_C), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_BLANK_C), 0, delta);
// 	
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_TXT_P), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_NUMBER_P), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_LINES_P), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_CODE_P), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_COMMENT_P), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_MIXED_P), 0, delta);
// 	CSplitterControl::ChangePos(GetDlgItem(IDC_BLANK_P), 0, delta);

	SetSplitterHRange();

	UpdateResizableDlgAnchor();

	Invalidate();
	UpdateWindow();
}

void CListCtrlDemoDlg::UpdateResizableDlgAnchor()
{
	RefreshAllAnchors();
}

void CListCtrlDemoDlg::SetSplitterVRange()
{	
	if(!::IsWindow(m_splitterVertical.GetSafeHwnd()))
	{
		return;
	}
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_FILTER_TREE);
	CRect rcTree;
	pWnd->GetWindowRect(&rcTree);
	ScreenToClient(&rcTree);

	CRect rcDlg;
	GetClientRect(&rcDlg);

	m_splitterVertical.SetRange(rcTree.left, rcTree.left + 320);	
}

void CListCtrlDemoDlg::SetSplitterHRange()
{
	if(!::IsWindow(m_splitterHorizontal.GetSafeHwnd()))
	{
		return;
	}
	CRect rcDlg;
	GetClientRect(&rcDlg);
	
	m_splitterHorizontal.SetRange(rcDlg.top + 150, rcDlg.top + 380);
}

void CListCtrlDemoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	
	SetSplitterVRange();
	SetSplitterHRange();
}

LRESULT CListCtrlDemoDlg::OnTreeItemSelected(WPARAM wParam, LPARAM lParam) 
{
	CList<HTREEITEM, HTREEITEM> htiSelectedList;
	m_filterTree.GetSelectedItems(htiSelectedList);

	CString sFilterStr;
	POSITION pos = htiSelectedList.GetHeadPosition();
	HTREEITEM hti;
	CMultiSelTreeCtrl::TVITEMDATA* pTVIData = NULL;
	while(pos != NULL)
	{
		hti = htiSelectedList.GetNext(pos);
		
		pTVIData = (CMultiSelTreeCtrl::TVITEMDATA*)m_filterTree.GetItemData(hti);
		if(pTVIData != NULL && !pTVIData->szType.IsEmpty())
		{
			sFilterStr += pTVIData->szType;
			sFilterStr += _T(";");
		}
	}

	m_filterComboBox.SetWindowText(sFilterStr);

	return TRUE;
}

HBRUSH CListCtrlDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CResizableDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//	int nCtrlID = pWnd->GetDlgCtrlID();  
	if(nCtlColor == CTLCOLOR_STATIC 
		/*|| nCtrlID == IDC_SOURCE_DIR_FRAME || nCtrlID == IDC_RESULT_FRAME || nCtrlID == IDC_SUMMARY_FRAME*/)
    {
		pDC->SetBkMode(TRANSPARENT);

		return (HBRUSH)(SYS_APP()->m_pSysBkBrush->GetSafeHandle());
    }
	return hbr;
}

void CListCtrlDemoDlg::OnExport(DWORD nTypeIndex)
{
	static const TCHAR *FILTERS =
        _T("Comma Separated Values (*.csv)|*.csv|")
		_T("Excel Workbook (*.xls)|*.xls|")
        _T("XML Data (*.xml)|*.xml|")
		_T("HTML files (*.html)|*.html|")
        _T("|");
	CFileDialog dlg(FALSE, "", NULL, 
        OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, FILTERS);
    dlg.m_ofn.nFilterIndex = nTypeIndex;
	
	BOOL bSuccess = FALSE;
	IExporter* pExporter = NULL;
	if (dlg.DoModal() == IDOK  &&  !dlg.GetPathName().IsEmpty())
	{
		if(dlg.m_ofn.nFilterIndex == EXPORT_TYPE_CSV)
		{
			pExporter = new CCSVExporter(&m_resultListCtrl, &m_totalInfo);
		}
		else if(dlg.m_ofn.nFilterIndex == EXPORT_TYPE_EXCEL)
		{
			pExporter = new CXLSExporter(&m_resultListCtrl, &m_totalInfo);
		}
		else if(dlg.m_ofn.nFilterIndex == EXPORT_TYPE_XML)
		{
			pExporter = new CXMLExporter(&m_resultListCtrl, &m_totalInfo);
		}
		else if(dlg.m_ofn.nFilterIndex == EXPORT_TYPE_HTML)
		{
			pExporter = new CHTMLExporter(&m_resultListCtrl, &m_totalInfo);
		}
		else
		{
			AfxMessageBox(_T("Unsupported file format"), MB_OK | MB_ICONERROR);
		}
	}
	if(pExporter)
	{
		bSuccess = pExporter->DoExport(dlg.GetPathName());
		
		delete pExporter;
		pExporter = NULL;

		if(bSuccess)
		{
			CString sPrompt;
			sPrompt.Format(_T("The data was successfully exported to: %s"), dlg.GetPathName());
			AfxMessageBox(sPrompt, MB_OK | MB_ICONINFORMATION);
		}
	}
}
CTotalInfo* CListCtrlDemoDlg::GetTotalInfo()
{
	return &m_totalInfo;
}

