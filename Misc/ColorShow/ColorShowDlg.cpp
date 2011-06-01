// ColorShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ColorShow.h"
#include "ColorShowDlg.h"

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
// CColorShowDlg dialog

CColorShowDlg::CColorShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorShowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorShowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_color = RGB(0, 0, 0);
}

void CColorShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorShowDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorShowDlg, CDialog)
	//{{AFX_MSG_MAP(CColorShowDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorShowDlg message handlers

BOOL CColorShowDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColorShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CColorShowDlg::OnPaint() 
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

		CWnd* pWnd = GetDlgItem(IDC_COLOR_VALUE);
		CRect rcWindow;
		pWnd->GetWindowRect(&rcWindow);
		ScreenToClient(&rcWindow);

		CRect rcColorView;
		rcColorView.left = rcWindow.left;
		rcColorView.right = rcColorView.left + rcWindow.Width();

		rcColorView.top = rcWindow.bottom + 50, 
		rcColorView.bottom = rcColorView.top + 100;		

		CClientDC dc(this);
		dc.FillSolidRect(rcColorView.left, rcColorView.top, rcColorView.Width(), rcColorView.Height(), m_color);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CColorShowDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CColorShowDlg::OnOK() 
{
	CString szColorStr;
	GetDlgItemText(IDC_COLOR_VALUE, szColorStr);

	szColorStr.TrimLeft();
	szColorStr.TrimRight();
	szColorStr.MakeLower();
	int nLength = szColorStr.GetLength();
	if(nLength > 6)
	{
		AfxMessageBox("The color length is longer than 6.", MB_OK | MB_ICONERROR);
		return;
	}
	int i;
	BOOL bValid = TRUE;
	for(i = 0; i < nLength; i++)
	{
		TCHAR c = szColorStr.GetAt(i);
		if( (c >= _T('0') && c <= _T('9')) || (c >= _T('a') && c <= _T('f')) )
		{

		}
		else
		{
			bValid = FALSE;
			break;
		}
	}
	if(!bValid)
	{
		AfxMessageBox("The color string has invalid character(s).", MB_OK | MB_ICONERROR);
		return;
	}
	CString szZeros = "000000";
	int diff = 6 - nLength;
	szColorStr = szZeros.Left(diff) + szColorStr;

	CString sR = szColorStr.Left(2);
	CString sG = szColorStr.Mid(2, 2);
	CString sB = szColorStr.Right(2);

	m_color = RGB(HexToDec(sR), HexToDec(sG), HexToDec(sB));

	Invalidate();
}

int CColorShowDlg::HexToDec(const CString &str)
{
	int nResult = 0;
	int nLength = str.GetLength();
	int nDecValue;
	for(int i = 0; i < nLength; i++)
	{
		TCHAR c = str.GetAt(i);
		nDecValue = 0;
		if( c >= _T('0') && c <= _T('9') )
		{
			nDecValue = (int)(c - _T('0'));
		}
		else if(c >= _T('a') && c <= _T('f'))
		{
			nDecValue = (int)(c - _T('a')) + 10;
		}
		nResult = nResult * 16 + nDecValue;
	}
	return nResult;
}
