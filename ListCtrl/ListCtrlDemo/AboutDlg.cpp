#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "AboutDlg.h"
#include "ThirdParty/Gradient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ABOUTBOX, pParent)
{
	//{{AFX_DATA_INIT(CAboutDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDialog)
	DDX_Control(pDX, IDC_PRODUCT_VERSION, m_ProductVersion);
	DDX_Control(pDX, IDC_REG_INFO, m_RegInfo);	
	DDX_Control(pDX, IDC_HOMEPAGE, m_HomePage);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Window Title
	CString sText;
	sText.Format(_T("%s %s"), "About", SZ_PRODUCT_NAME);
	SetWindowText(sText);

	//Bold font
	CFont *pFontTTL = m_ProductVersion.GetFont();
	LOGFONT lf;
	pFontTTL->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect(&lf);
	m_ProductVersion.SetFont(&m_fontBold);
	
	//Set product and version info
	CString szProductVersion;
	szProductVersion.Format("%s - %s", SZ_PRODUCT_NAME, SZ_VERSION_NAME);
	SetDlgItemText(IDC_PRODUCT_VERSION, szProductVersion);	

	//Set licenced info
	//TODO:
	CString szLicenceInfo;
	szLicenceInfo.Format(_T("%s\r\n\t%s\r\n\t%s\r\n"), 
		"This software is licensed to:",
		"User Tester",
		"2596-9748-0000-0000-YYYY-ZZZZ-AAAA-WXYZ");
	m_RegInfo.SetWindowText(szLicenceInfo);
	
	return TRUE;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() != IDC_REG_INFO)
    {
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	return hbr;
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	CListCtrlDemoApp* pApp = (CListCtrlDemoApp*)AfxGetApp();
	pDC->FillRect(&rect, pApp->m_pSysBkBrush);

	COLORREF colorStart = RGB(251, 252, 249);
	COLORREF colorEnd = RGB(229, 244, 248);
	CGradient::Draw(pDC, &rect, colorStart, colorEnd, FALSE);

	return TRUE;
}
