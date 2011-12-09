// AddTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "AddTaskDlg.h"
#include "YTGetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddTaskDlg dialog


CAddTaskDlg::CAddTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddTaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddTaskDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddTaskDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddTaskDlg, CDialog)
	//{{AFX_MSG_MAP(CAddTaskDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddTaskDlg message handlers

void CAddTaskDlg::OnOK() 
{
	CString szAddress;
	GetDlgItemText(IDC_NEW_ADDRESS, szAddress);

	CYTGetDlg* pParent = (CYTGetDlg*)m_pParentWnd;
	ASSERT(pParent != NULL);
	pParent->AddTask(szAddress);
	
	CDialog::OnOK();
}

void CAddTaskDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
