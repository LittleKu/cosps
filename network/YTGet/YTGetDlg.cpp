// YTGetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "YTGetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYTGetDlg dialog


CYTGetDlg::CYTGetDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CYTGetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYTGetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CYTGetDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYTGetDlg)
	DDX_Control(pDX, IDC_TASK_LIST, m_taskListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYTGetDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CYTGetDlg)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYTGetDlg message handlers

void CYTGetDlg::InitTaskListCtrl()
{
	m_taskListCtrl.Init();
}

void CYTGetDlg::InitResizableDlgAnchor()
{
	AddAnchor(m_taskListCtrl.m_hWnd, TOP_LEFT, BOTTOM_RIGHT);
}

BOOL CYTGetDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitResizableDlgAnchor();

	InitTaskListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CYTGetDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	pDC->FillRect(&rect, SYS_APP()->m_pSysBkBrush);
	
	return TRUE;
}
