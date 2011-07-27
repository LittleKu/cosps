#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "AboutDlg.h"
#include "gtb.h"
#include "LicenseMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CBkgEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CBkgEdit::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	pDC->FillRect(&rect, SYS_APP()->m_pSysBkBrush);
	
	COLORREF colorStart = RGB(229, 244, 248);
	COLORREF colorEnd = RGB(251, 252, 249);
	gtb::DrawGradient(pDC, &rect, colorStart, colorEnd, FALSE);
	
	return TRUE;
}
void CBkgEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetSel(-1, -1);
	Invalidate();
	CEdit::OnLButtonDown(nFlags, point);
}
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
	lf.lfHeight *= 2;
	m_fontBold.CreateFontIndirect(&lf);
	m_ProductVersion.SetFont(&m_fontBold);
	
	//Set product and version info
	CString szProductVersion;
	szProductVersion.Format("%s - %s", SZ_PRODUCT_NAME, SZ_VERSION_NAME);
	SetDlgItemText(IDC_PRODUCT_VERSION, szProductVersion);	

	CString szCopyRight;
	szCopyRight.Format(_T("Copyright (c) 2011-2016 by %s"), SZ_ORGNIZATION_NAME);
	SetDlgItemText(IDC_COPYRIGHT, szCopyRight);

	//Set license info
	CString szLicenceInfo;
	if(CLicenseMgr::GetInstance()->IsRegistered(TRUE))
	{
		CString szName, szCode;
		CLicenseMgr::GetInstance()->GetLicenseInfo(szName, szCode);

		szLicenceInfo.Format(_T("%s\r\n%s\r\n"), "This software is licensed to:", szName);
	}
	else
	{
		szLicenceInfo.Format(_T("Evaluation Version\r\n"
			"You are using the evaluation copy of %s. "
			"Only %d files could be counted in each count operation. "
			"To remove this limitation, please order a registration key."),
			SZ_PRODUCT_NAME, MAX_EVALUATION_FILE_COUNT);
	}
	m_RegInfo.SetWindowText(szLicenceInfo);

	m_HomePage.SetWindowText(SZ_PRODUCT_WEBSITE);
	m_HomePage.SetURL(SZ_PRODUCT_WEBSITE);
	
	return TRUE;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC /*&& pWnd->GetDlgCtrlID() != IDC_REG_INFO*/)
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
	
	pDC->FillRect(&rect, SYS_APP()->m_pSysBkBrush);

	COLORREF colorStart = RGB(251, 252, 249);
	COLORREF colorEnd = RGB(229, 244, 248);
	gtb::DrawGradient(pDC, &rect, colorStart, colorEnd, FALSE);

	return TRUE;
}
