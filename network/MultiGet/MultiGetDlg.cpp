// MultiGetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiGet.h"
#include "MultiGetDlg.h"
#include "Downloader.h"
#include "easy_down.h"

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
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_MESSAGE(WM_DOWNLOAD_PROGRESS, OnUpdateProgress)
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE, OnEnd)
	ON_BN_CLICKED(IDC_ADD1, OnAdd1)
	ON_BN_CLICKED(IDC_BUTTON_START1, OnButtonStart1)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_RESUME, OnButtonResume)
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

static const char *urls[] = {
	"http://www.microsoft.com",
		"http://www.opensource.org",
		"http://www.google.com",
		"http://www.yahoo.com",
		"http://www.ibm.com",
		"http://www.mysql.com",
		"http://www.oracle.com",
		"http://www.ripe.net",
		"http://www.iana.org",
		"http://www.amazon.com",
		"http://www.netcraft.com",
		"http://www.heise.de",
		"http://www.chip.de",
		"http://www.ca.com",
		"http://www.cnet.com",
		"http://www.news.com",
		"http://www.cnn.com",
		"http://www.wikipedia.org",
		"http://www.dell.com",
		"http://www.hp.com",
		"http://www.cert.org",
		"http://www.mit.edu",
		"http://www.nist.gov",
		"http://www.ebay.com",
		"http://www.playstation.com",
		"http://www.uefa.com",
		"http://www.ieee.org",
		"http://www.apple.com",
		"http://www.sony.com",
		"http://www.symantec.com",
		"http://www.zdnet.com",
		"http://www.fujitsu.com",
		"http://www.supermicro.com",
		"http://www.hotmail.com",
		"http://www.ecma.com",
		"http://www.bbc.co.uk",
		"http://news.google.com",
		"http://www.foxnews.com",
		"http://www.msn.com",
		"http://www.wired.com",
		"http://www.sky.com",
		"http://www.usatoday.com",
		"http://www.cbs.com",
		"http://www.nbc.com",
		"http://slashdot.org",
		"http://www.bloglines.com",
		"http://www.techweb.com",
		"http://www.newslink.org",
		"http://www.un.org",
};

void CMultiGetDlg::OnButtonAdd() 
{
// 	CString sUrl;
// 	m_editUrl.GetWindowText(sUrl);
// 	
// 	if(sUrl.IsEmpty())
// 	{
// 		AfxMessageBox("Empty Url");
// 		return;
// 	}
// 	CTaskInfo* pTaskInfo = new CTaskInfo();
// 	pTaskInfo->m_url = sUrl;
// 	pTaskInfo->m_progress = "Ready";
// 	
// 	m_taskListCtrl.AddRow(*pTaskInfo);
// 	
// 	m_editUrl.SetWindowText("");

	int count = sizeof(urls)/sizeof(urls[0]);

	for(int i = 0; i < count; i++)
	{
		CTaskInfo* pTaskInfo = new CTaskInfo();
		pTaskInfo->m_url = urls[i];
		pTaskInfo->m_progress = "Ready";

		m_taskListCtrl.AddRow(*pTaskInfo);
	}
}

static UINT DownloadProc(LPVOID lpvData)
{
	HWND hwnd = (HWND)lpvData;
	CDownloader downloader;
	downloader.download(hwnd);

	::SendMessage(hwnd, WM_DOWNLOAD_COMPLETE, 0, 0);

	return 0;
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
	CWinThread* pThread = AfxBeginThread(DownloadProc, (LPVOID)GetSafeHwnd());
}

LRESULT CMultiGetDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	CProgressInfo* pProgressInfo = (CProgressInfo*)wParam;
	int index = (int)lParam;

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
static UINT DownloadProc1(LPVOID lpvData)
{
	CMultiGetDlg* pThis = (CMultiGetDlg*)lpvData;

	CTaskInfo* pTaskInfo = (CTaskInfo*)pThis->m_taskListCtrl.GetItemData(0);
	if(pTaskInfo == NULL)
	{
		return 1;
	}
	CEasyDown edown;
	edown.download(pTaskInfo->m_url, pThis->GetSafeHwnd(), &pThis->m_controlInfo);

	::SendMessage(pThis->GetSafeHwnd(), WM_DOWNLOAD_COMPLETE, 0, 0);
	
	return 0;
}
void CMultiGetDlg::OnButtonStart1() 
{
	m_controlInfo.isModified = FALSE;
	m_controlInfo.isPaused = FALSE;
	m_controlInfo.isStopped = FALSE;
	AfxBeginThread(DownloadProc1, (LPVOID)this);
}

void CMultiGetDlg::OnButtonPause() 
{
	m_controlInfo.isPaused = TRUE;
	m_controlInfo.isModified = TRUE;
}

void CMultiGetDlg::OnButtonStop() 
{
	m_controlInfo.isStopped = TRUE;
	
}

void CMultiGetDlg::OnButtonResume() 
{
	m_controlInfo.isPaused = FALSE;
	m_controlInfo.isModified = TRUE;
}
