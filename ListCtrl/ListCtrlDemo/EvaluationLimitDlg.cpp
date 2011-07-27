// EvaluationLimitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "EvaluationLimitDlg.h"
#include "RegisterDlg.h"
#include "./ThirdParty/HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvaluationLimitDlg dialog


CEvaluationLimitDlg::CEvaluationLimitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvaluationLimitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvaluationLimitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEvaluationLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvaluationLimitDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEvaluationLimitDlg, CDialog)
	//{{AFX_MSG_MAP(CEvaluationLimitDlg)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_ORDER, OnButtonOrder)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvaluationLimitDlg message handlers

BOOL CEvaluationLimitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Bold font
	CFont *pFontTTL = GetFont();
	LOGFONT lf;
	pFontTTL->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	//lf.lfHeight *= 2;
	m_fontBold.CreateFontIndirect(&lf);

	//Set Font to the Labels
	SendDlgItemMessage(IDC_STATIC_P1, WM_SETFONT, (WPARAM)m_fontBold.GetSafeHandle(), TRUE);
	SendDlgItemMessage(IDC_STATIC_P2, WM_SETFONT, (WPARAM)m_fontBold.GetSafeHandle(), TRUE);

	CString szText;

	szText.Format(_T("You are using the evaluation copy of %s. "
		"Only %d files could be counted in each count operation. "
		"To remove this limitation, please order a registration key."),
		SZ_PRODUCT_NAME, MAX_EVALUATION_FILE_COUNT);
	SetDlgItemText(IDC_STATIC_P1, szText);

	szText.Format(_T("1. Click \"Order Now\" to order a registration key."
		"\n2. Click \"Register\" to activate the full version, if you have purchased one."
		"\n3. Click \"Conitnue Trial\" to continue evaluation."));
	SetDlgItemText(IDC_STATIC_P2, szText);
	
	return TRUE;
}

void CEvaluationLimitDlg::OnButtonRegister() 
{
	CRegisterDlg regDlg;
	regDlg.DoModal();
}

void CEvaluationLimitDlg::OnButtonOrder() 
{
	CHyperLink::GotoURL(SZ_PRODUCT_WEBSITE_ORDER, SW_SHOW);
}

void CEvaluationLimitDlg::OnOK()
{
	CDialog::OnOK();
}

void CEvaluationLimitDlg::OnCancel() 
{
	CDialog::OnCancel();
}

HBRUSH CEvaluationLimitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	return hbr;
}

BOOL CEvaluationLimitDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	COLORREF colorStart = RGB(229, 244, 248);
	COLORREF colorEnd = RGB(251, 252, 249);
	gtb::DrawGradient(pDC, &rect, colorStart, colorEnd, FALSE);
	
	return TRUE;
}
