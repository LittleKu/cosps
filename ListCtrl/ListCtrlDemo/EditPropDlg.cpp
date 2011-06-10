// FilterTreeModifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "EditPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterTreeModifyDlg dialog


CEditPropDlg::CEditPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EDIT_PROP_DLG, pParent)
{
	//{{AFX_DATA_INIT(CFilterTreeModifyDlg)
	//}}AFX_DATA_INIT
}


void CEditPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterTreeModifyDlg)
	DDX_Text(pDX, IDC_NEW_NAME, m_NewNameStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditPropDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterTreeModifyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterTreeModifyDlg message handlers

void CEditPropDlg::OnOK() 
{	
	CDialog::OnOK();
}

BOOL CEditPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!m_sTitle.IsEmpty())
	{
		SetWindowText(m_sTitle);
	}
	if(!m_LabelText.IsEmpty())
	{
		SetDlgItemText(IDC_TXT_NEW_NAME, m_LabelText);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
