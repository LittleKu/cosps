// MultiGetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiGet.h"
#include "MultiGetDlg.h"
#include "TestDownloader.h"
#include "HeaderParser.h"
#include "EasyDownloader.h"
#include "SegmentDownloader.h"

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
// CMultiGetDlg dialog

CMultiGetDlg::CMultiGetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiGetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiGetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiGetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiGetDlg)
	DDX_Control(pDX, IDC_EDIT_URL, m_editUrl);
	DDX_Control(pDX, IDC_LIST_TASK, m_taskListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultiGetDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiGetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DOWNLOAD_PROGRESS, OnUpdateProgress)
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE, OnEnd)
	ON_BN_CLICKED(IDC_ADD1, OnAdd1)
	ON_BN_CLICKED(IDC_BUTTON_START1, OnButtonStart1)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_RESUME, OnButtonResume)
	ON_BN_CLICKED(IDC_BUTTON_HEADER, OnButtonHeader)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiGetDlg message handlers

BOOL CMultiGetDlg::OnInitDialog()
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
	m_taskListCtrl.Init();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMultiGetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMultiGetDlg::OnPaint() 
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
HCURSOR CMultiGetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CMultiGetDlg::OnEnd(WPARAM wParam, LPARAM lParam)
{
	const char* pmsg = (char*)wParam;

	CString szMsg = "Transfer End";
	if(pmsg != NULL)
	{
		szMsg += "\n";
		szMsg += pmsg;
	}
	AfxMessageBox(szMsg);
	return 1L;
}
void CMultiGetDlg::OnOK() 
{
}

LRESULT CMultiGetDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	CProgressInfo* pProgressInfo = (CProgressInfo*)wParam;
	int index = pProgressInfo->index;

	ASSERT(index >= 0 && index < m_taskListCtrl.GetItemCount());

	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(index);
	ASSERT(pTaskInfo != NULL);


	CString szProgress;

	DWORD64 percent = 0;
	if(pProgressInfo->dltotal > 0)
	{
		percent = pProgressInfo->dlnow * 100 / pProgressInfo->dltotal;
		szProgress.Format("%c%I64d - %I64d of %I64d", '%', percent, pProgressInfo->dlnow, pProgressInfo->dltotal);
	}
	else
	{
		szProgress.Format("N/A - %I64d of N/A", pProgressInfo->dlnow);
	}

	CString szResult;
	if(pProgressInfo->retCode >= 0)
	{
		szResult.Format("%d - %s", pProgressInfo->retCode, pProgressInfo->szReason);
	}

	
	pTaskInfo->m_progress = szProgress;

	m_taskListCtrl.SetItemText(index, 3, szProgress);
	m_taskListCtrl.InvalidateSubItem(index, 3);

	m_taskListCtrl.SetItemText(index, 4, szResult);
	m_taskListCtrl.InvalidateSubItem(index, 4);


	return 1L;
}

void CMultiGetDlg::OnAdd1() 
{
	
	CString sUrl;
	m_editUrl.GetWindowText(sUrl);
	
	if(sUrl.IsEmpty())
	{
		AfxMessageBox("Empty Url");
		return;
	}
	CTaskInfo* pTaskInfo = new CTaskInfo();
	pTaskInfo->m_url = sUrl;
	pTaskInfo->m_progress = "Ready";
	
	m_taskListCtrl.AddRow(*pTaskInfo);
	
	m_editUrl.SetWindowText("");	
}

void CMultiGetDlg::OnButtonStart1() 
{
	ASSERT(m_taskListCtrl.GetItemCount() > 0);

	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(0);
	ASSERT(pTaskInfo != NULL);

	CDownloadParam param;
	param.m_hWnd = GetSafeHwnd();
	param.m_nTaskID = 0;
	param.m_szUrl = pTaskInfo->m_url;
	param.m_nFileSize = 0;

	if(pTaskInfo->m_lpDownloaderMgr == NULL)
	{
		pTaskInfo->m_lpDownloaderMgr = new CDownloaderMgr(param);
	}

	pTaskInfo->m_lpDownloaderMgr->Start();
}

void CMultiGetDlg::OnButtonPause() 
{
	ASSERT(m_taskListCtrl.GetItemCount() > 0);
	
	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(0);
	ASSERT(pTaskInfo != NULL);
	ASSERT(pTaskInfo->m_lpDownloaderMgr != NULL);
	
	pTaskInfo->m_lpDownloaderMgr->Pause();
}

void CMultiGetDlg::OnButtonStop() 
{	
	ASSERT(m_taskListCtrl.GetItemCount() > 0);
	
	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(0);
	ASSERT(pTaskInfo != NULL);
	ASSERT(pTaskInfo->m_lpDownloaderMgr != NULL);
	
	pTaskInfo->m_lpDownloaderMgr->Stop();
}

void CMultiGetDlg::OnButtonResume() 
{
	ASSERT(m_taskListCtrl.GetItemCount() > 0);
	
	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(0);
	ASSERT(pTaskInfo != NULL);
	ASSERT(pTaskInfo->m_lpDownloaderMgr != NULL);

	pTaskInfo->m_lpDownloaderMgr->Resume();
}

void CMultiGetDlg::OnButtonHeader() 
{
	ASSERT(m_taskListCtrl.GetItemCount() > 0);

	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(0);
	ASSERT(pTaskInfo != NULL);

	CHeaderParser headerParser(pTaskInfo->m_url);
	CHeaderInfo* pHeaderInfo = headerParser.GetHeaderInfo();

	CString szMsg;
	szMsg.Format("code=%d, size=%d, is_range_bytes=%d", 
		pHeaderInfo->httpcode, pHeaderInfo->header_size, pHeaderInfo->is_range_bytes);
	AfxMessageBox(szMsg);
}

void CMultiGetDlg::OnButtonRemove() 
{
	m_taskListCtrl.RemoveSelectedItems();
}
