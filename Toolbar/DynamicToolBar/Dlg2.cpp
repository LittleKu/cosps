// Dlg2.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "Dlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg2 dialog


CDlg2::CDlg2(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlg2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg2::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg2, CResizableDialog)
	//{{AFX_MSG_MAP(CDlg2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg2 message handlers

BOOL CDlg2::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor(IDC_LIST1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(ID_CANCEL2, BOTTOM_CENTER, BOTTOM_CENTER);
	AddAnchor(ID_OK2, BOTTOM_CENTER, BOTTOM_CENTER);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
