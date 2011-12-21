// PPgGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "PPgGeneral.h"
#include "gtb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPgGeneral property page

IMPLEMENT_DYNCREATE(CPPgGeneral, CPropertyPage)

CPPgGeneral::CPPgGeneral() : CPropertyPage(CPPgGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPPgGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPPgGeneral::~CPPgGeneral()
{
}

void CPPgGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPgGeneral)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPgGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPPgGeneral)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_DIR, OnBtnOutputDir)
	ON_BN_CLICKED(IDC_BTN_TEMP_DIR, OnBtnTempDir)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_DIR, OnSettingsChange)
	ON_EN_CHANGE(IDC_EDIT_TEMP_DIR, OnSettingsChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPgGeneral message handlers

BOOL CPPgGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	LoadSettings();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPPgGeneral::OnApply() 
{
	//Output folder
	CString szPath;
	GetDlgItemText(IDC_EDIT_OUTPUT_DIR, szPath);
	//Changed
	if(SYS_OPTIONS()->m_szSaveToFolder.CompareNoCase(szPath) != 0)
	{
		if(!::PathFileExists(szPath))
		{
			CString szPrompt;
			szPrompt.Format("The specified output directory path doesn't exist.\n%s", szPath);
			AfxMessageBox(szPrompt);
			return FALSE;
		}

		SYS_OPTIONS()->m_szSaveToFolder = szPath;
	}

	//Temp folder
	GetDlgItemText(IDC_EDIT_TEMP_DIR, szPath);
	//Changed
	if(SYS_OPTIONS()->m_szTempFolder.CompareNoCase(szPath) != 0)
	{
		if(!::PathFileExists(szPath))
		{
			CString szPrompt;
			szPrompt.Format("The specified temporary directory path doesn't exist.\n%s", szPath);
			AfxMessageBox(szPrompt);
			return FALSE;
		}
		
		SYS_OPTIONS()->m_szTempFolder = szPath;
	}

	SetModified(FALSE);
	return CPropertyPage::OnApply();
}

void CPPgGeneral::LoadSettings()
{
	SetDlgItemText(IDC_EDIT_OUTPUT_DIR, SYS_OPTIONS()->m_szSaveToFolder);
	SetDlgItemText(IDC_EDIT_TEMP_DIR, SYS_OPTIONS()->m_szTempFolder);
}

void CPPgGeneral::OnBtnOutputDir() 
{
	CString szPath;
	GetDlgItemText(IDC_EDIT_OUTPUT_DIR, szPath);
	LPTSTR lpBuffer = szPath.GetBuffer(MAX_PATH);
	if( gtb::BrowseFolder(GetSafeHwnd(), lpBuffer, _T("Select Output Folder")) )
	{
		szPath.ReleaseBuffer();
		SetDlgItemText(IDC_EDIT_OUTPUT_DIR, szPath);
	}
}

void CPPgGeneral::OnBtnTempDir() 
{
	CString szPath;
	GetDlgItemText(IDC_EDIT_TEMP_DIR, szPath);
	LPTSTR lpBuffer = szPath.GetBuffer(MAX_PATH);
	if( gtb::BrowseFolder(GetSafeHwnd(), lpBuffer, _T("Select Temporary Folder")) )
	{
		szPath.ReleaseBuffer();
		SetDlgItemText(IDC_EDIT_TEMP_DIR, szPath);
	}
}
