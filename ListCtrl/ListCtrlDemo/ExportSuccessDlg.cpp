// ExportSuccessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "ExportSuccessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportSuccessDlg dialog


CExportSuccessDlg::CExportSuccessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportSuccessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportSuccessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExportSuccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportSuccessDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportSuccessDlg, CDialog)
	//{{AFX_MSG_MAP(CExportSuccessDlg)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DIRECTORY, OnButtonOpenDirectory)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, OnButtonOpenFile)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportSuccessDlg message handlers
void CExportSuccessDlg::SetExportedFile(LPCTSTR lpFileName)
{
	m_szExportedFile = lpFileName;
}
void CExportSuccessDlg::OnOK()
{
}
void CExportSuccessDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CExportSuccessDlg::OnButtonOpenDirectory() 
{
	ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,\"") + m_szExportedFile + _T("\""), NULL, SW_SHOW);
	OnCancel();
}

void CExportSuccessDlg::OnButtonOpenFile() 
{
	HINSTANCE hInst = ::ShellExecute(NULL, _T("open"), m_szExportedFile, NULL, NULL, SW_SHOW);
	OnCancel();
}

BOOL CExportSuccessDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	COLORREF colorStart = RGB(229, 244, 248);
	COLORREF colorEnd = RGB(251, 252, 249);
	gtb::DrawGradient(pDC, &rect, colorStart, colorEnd, FALSE);
	
	return TRUE;
}
HBRUSH CExportSuccessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	return hbr;
}

BOOL CExportSuccessDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Make the Info Label to auto wrap line
	CWnd* pWndInfoLabel = GetDlgItem(IDC_INFO_LABEL);
	ASSERT(pWndInfoLabel != NULL);
	pWndInfoLabel->ModifyStyle(0, SS_EDITCONTROL, 0);

	//Set Title
	SetWindowText(SYS_APP()->m_pszAppName);

	//Show Information
	CString szText;
	szText.Format(_T("The data was successfully exported to: %s"), m_szExportedFile);
	pWndInfoLabel->SetWindowText(szText);
	
	return TRUE;
}
