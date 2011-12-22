// AddTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "AddTaskDlg.h"
#include "YTGetDlg.h"
#include "CommonUtils.h"

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
	//}}AFX_DATA_INIT
}


void CAddTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddTaskDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddTaskDlg, CDialog)
	//{{AFX_MSG_MAP(CAddTaskDlg)
	ON_EN_CHANGE(IDC_NEW_ADDRESS, OnChangeNewAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddTaskDlg message handlers

void CAddTaskDlg::OnOK() 
{
	CString szAddress;
	GetDlgItemText(IDC_NEW_ADDRESS, szAddress);

	if(szAddress.IsEmpty())
	{
		AfxMessageBox("Empty URL");
		return;
	}

	CString szFileName;
	GetDlgItemText(IDC_EDIT_SAVE_TO_FILE_NAME, szFileName);
	if(szFileName.IsEmpty())
	{
		CCommonUtils::ExtractFileName(szAddress, szFileName, TRUE);
	}

	int nIDCheckButton = GetCheckedRadioButton(IDC_RADIO_START_AUTO, IDC_RADIO_START_MANUALLY);
	ASSERT(nIDCheckButton != 0);
	StartModeEnum nStartMode = ((nIDCheckButton == IDC_RADIO_START_AUTO) ? SME_AUTO : SME_MANUAL);

	CYTGetDlg* pParent = (CYTGetDlg*)m_pParentWnd;
	ASSERT(pParent != NULL);

	CDialog::OnOK();

	pParent->AddTask(szAddress, szFileName, nStartMode);
}

void CAddTaskDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CAddTaskDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckRadioButton(IDC_RADIO_START_AUTO, IDC_RADIO_START_MANUALLY, IDC_RADIO_START_AUTO);
	
	return TRUE;
}

void CAddTaskDlg::OnChangeNewAddress() 
{
	CString szURL;
	GetDlgItemText(IDC_NEW_ADDRESS, szURL);

	CString szFileName;
	CCommonUtils::ExtractFileName(szURL, szFileName, TRUE);

	SetDlgItemText(IDC_EDIT_SAVE_TO_FILE_NAME, szFileName);
}
