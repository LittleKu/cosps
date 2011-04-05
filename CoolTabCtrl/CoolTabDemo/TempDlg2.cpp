// TempDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "CoolTabDemo.h"
#include "TempDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTempDlg2 dialog

CTempDlg2::CTempDlg2(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CTempDlg2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTempDlg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTempDlg2::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTempDlg2)
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTempDlg2, CResizableDialog)
	//{{AFX_MSG_MAP(CTempDlg2)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTempDlg2 message handlers

BOOL CTempDlg2::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor(IDC_STATIC_TMP1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_BUTTON1, TOP_CENTER);
	AddAnchor(IDC_BUTTON2, BOTTOM_CENTER);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTempDlg2::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CTempDlg2::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
}
