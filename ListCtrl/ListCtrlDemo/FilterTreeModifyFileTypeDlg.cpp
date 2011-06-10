// FilterTreeModifyFileTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "FilterTreeModifyFileTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterTreeModifyFileTypeDlg dialog


CFilterTreeModifyFileTypeDlg::CFilterTreeModifyFileTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FILTER_TREE_MODIFY_FILE_TYPE, pParent)
{
	//{{AFX_DATA_INIT(CFilterTreeModifyFileTypeDlg)
	m_NewNameStr = _T("");
	m_FilterFileTypeStr = _T("");
	//}}AFX_DATA_INIT
}


void CFilterTreeModifyFileTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterTreeModifyFileTypeDlg)
	DDX_Text(pDX, IDC_NEW_NAME, m_NewNameStr);
	DDX_Text(pDX, IDC_FILTER_FILE_TYPE, m_FilterFileTypeStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterTreeModifyFileTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterTreeModifyFileTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterTreeModifyFileTypeDlg message handlers

void CFilterTreeModifyFileTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CFilterTreeModifyFileTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!m_sTitle.IsEmpty())
	{
		SetWindowText(m_sTitle);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
