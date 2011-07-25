// RegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "RegisterDlg.h"
#include "LicenseMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg dialog


CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRegisterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDlg)
	DDX_Control(pDX, IDC_REG_HELP, m_regHelpLink);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialog)
//{{AFX_MSG_MAP(CRegisterDlg)
ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg message handlers
BOOL CRegisterDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	COLORREF colorStart = RGB(229, 244, 248);
	COLORREF colorEnd = RGB(251, 252, 249);
	gtb::DrawGradient(pDC, &rect, colorStart, colorEnd, FALSE);
	
	return TRUE;
}
BOOL CRegisterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Change the OK button text to "Register"
	CWnd* pOkButton = GetDlgItem(IDOK);
	ASSERT(pOkButton);
	pOkButton->SetWindowText(_T("Register"));
	
	CString szText;
	if(CLicenseMgr::GetInstance()->IsRegistered())
	{
		szText.Format(_T("You have REGISTERED this software already. Thanks for your supporting!"));
		SetDlgItemText(IDC_TXT_REG_INFO_ABOVE, szText);
		
		CString szName, szCode;
		CLicenseMgr::GetInstance()->GetLicenseInfo(szName, szCode);
		SetDlgItemText(IDC_REG_NAME, szName);
		SetDlgItemText(IDC_REG_CODE, szCode);
		
		//Registered already, no need to register again.
		pOkButton->EnableWindow(FALSE);
	}
	else
	{
		szText.Format(_T("To activate %s, please enter the registration information provided to you."), 
			SZ_PRODUCT_NAME);
		SetDlgItemText(IDC_TXT_REG_INFO_ABOVE, szText);
	}
	
	szText.Format(_T("Your name and registration key should appear EXACTLY as emailed to you. "
		"For further help, please click the registration help link below."));
	SetDlgItemText(IDC_TXT_REG_INFO_BELOW, szText);
	
	m_regHelpLink.SetURL(SZ_PRODUCT_WEBSITE);
	
	return TRUE;
}

HBRUSH CRegisterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	return hbr;
}

void CRegisterDlg::OnOK() 
{
	CString szName, szCode;
	GetDlgItemText(IDC_REG_NAME, szName);
	GetDlgItemText(IDC_REG_CODE, szCode);
	
	BOOL bResult = CLicenseMgr::GetInstance()->Register(szName, szCode);
	CString szPromptText;
	if(bResult)
	{
		szPromptText.Format(_T("Register successfully. Thanks for your supporting."));
		AfxMessageBox(szPromptText, MB_OK | MB_ICONINFORMATION);

		CDialog::OnOK();
	}
	else
	{
		szPromptText.Format(_T("Sorry, the register code is INVALID. Please try copy and paster instead of "
			"manually input. If the code is still invalid, please contact "
			"%s with your order ID to get a new code."), SZ_PRODUCT_SUPPORT_EMAIL);
		AfxMessageBox(szPromptText, MB_OK | MB_ICONINFORMATION);
	}
}
