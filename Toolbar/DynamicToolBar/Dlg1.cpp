// Dlg1.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "Dlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg1 dialog


CDlg1::CDlg1(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlg1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg1::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg1, CResizableDialog)
	//{{AFX_MSG_MAP(CDlg1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg1 message handlers

BOOL CDlg1::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor(IDC_FRAME1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON1, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_BUTTON2, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_BUTTON3, BOTTOM_CENTER, BOTTOM_CENTER);

	AddAnchor(IDC_BUTTON4, BOTTOM_CENTER, BOTTOM_CENTER);
	AddAnchor(IDC_BUTTON5, BOTTOM_CENTER, BOTTOM_CENTER);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
