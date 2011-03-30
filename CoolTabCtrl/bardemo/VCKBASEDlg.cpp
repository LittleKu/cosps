// VCKBASEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BarDemo.h"
#include "VCKBASEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVCKBASEDlg dialog

IMPLEMENT_DYNCREATE(CVCKBASEDlg, CDialog)

CVCKBASEDlg::CVCKBASEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCKBASEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCKBASEDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CVCKBASEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVCKBASEDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVCKBASEDlg, CDialog)
	//{{AFX_MSG_MAP(CVCKBASEDlg)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCKBASEDlg message handlers

void CVCKBASEDlg::OnCheck1() 
{
	AfxMessageBox("ÄúºÃ°¡£¡ÅóÓÑ£¡");	
}


BOOL CVCKBASEDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}



