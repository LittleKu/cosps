// Dlg1.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "Dlg1.h"
#include "DynamicToolBarDlg.h"

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
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
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

void CDlg1::OnButton1() 
{
	CDynamicToolBarDlg* pParent = (CDynamicToolBarDlg*)GetParent();
	pParent->m_staticStatusBar.SetText(_T("long long long name for a text test..., to see the entire text. Please contat me."), 0);
}

void CDlg1::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
}

void CDlg1::OnButton2() 
{
	CDynamicToolBarDlg* pParent = (CDynamicToolBarDlg*)GetParent();
	pParent->m_staticStatusBar.SetText(_T("Some text."), 1);
	
}

void CDlg1::OnButton3() 
{
	CDynamicToolBarDlg* pParent = (CDynamicToolBarDlg*)GetParent();
	pParent->m_staticStatusBar.SetText(_T("Hi Jack. How Are you Today?"), 2, SBT_NOBORDERS);
}

void CDlg1::OnButton4() 
{
	CDynamicToolBarDlg* pParent = (CDynamicToolBarDlg*)GetParent();
	pParent->m_staticStatusBar.SetText(_T("I'm here just to make some space."), 2);
}

void CDlg1::OnButton5() 
{
	CDynamicToolBarDlg* pParent = (CDynamicToolBarDlg*)GetParent();
	pParent->m_staticStatusBar.SetText(_T("No borde text."), 1, SBT_NOBORDERS);
}
