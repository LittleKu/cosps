// PPgStatistic.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "PPgStatistic.h"
#include "FileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPgStatistic property page

IMPLEMENT_DYNCREATE(CPPgStatistic, CPropertyPage)

CPPgStatistic::CPPgStatistic() : CPropertyPage(IDD_PROPPAGE_STAT)
{
	//{{AFX_DATA_INIT(CPPgStatistic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPPgStatistic::~CPPgStatistic()
{
}

void CPPgStatistic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPgStatistic)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPgStatistic, CPropertyPage)
	//{{AFX_MSG_MAP(CPPgStatistic)
	ON_BN_CLICKED(IDC_CHECK_BLANK_IN_COMMENT_BLOCK_COMMENT, OnOnSettingsChange)
	ON_BN_CLICKED(IDC_CHECK_MIXED_LINE_CODE, OnOnSettingsChange)
	ON_BN_CLICKED(IDC_CHECK_MIXED_LINE_COMMENT, OnOnSettingsChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPgStatistic message handlers

BOOL CPPgStatistic::OnApply() 
{
	//Blank line in block comment
	UINT nChecked = IsDlgButtonChecked(IDC_CHECK_BLANK_IN_COMMENT_BLOCK_COMMENT);
	if(nChecked & BST_CHECKED)
	{
		SYS_PREF()->m_nStatMode |= FP_MODE_BLANK_IN_COMMENT_BLOCK_COMMENT;
	}
	else
	{
		SYS_PREF()->m_nStatMode &= ~FP_MODE_BLANK_IN_COMMENT_BLOCK_COMMENT;
	}

	//Mixed line as code
	nChecked = IsDlgButtonChecked(IDC_CHECK_MIXED_LINE_CODE);
	if(nChecked & BST_CHECKED)
	{
		SYS_PREF()->m_nStatMode |= FP_MODE_MIXED_LINE_CODE;
	}
	else
	{
		SYS_PREF()->m_nStatMode &= ~FP_MODE_MIXED_LINE_CODE;
	}

	//Mixed line as comment
	nChecked = IsDlgButtonChecked(IDC_CHECK_MIXED_LINE_COMMENT);
	if(nChecked & BST_CHECKED)
	{
		SYS_PREF()->m_nStatMode |= FP_MODE_MIXED_LINE_COMMENT;
	}
	else
	{
		SYS_PREF()->m_nStatMode &= ~FP_MODE_MIXED_LINE_COMMENT;
	}

	SetModified(FALSE);
	return CPropertyPage::OnApply();
}

BOOL CPPgStatistic::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	LoadSettings();
	
	return TRUE;
}

void CPPgStatistic::OnOnSettingsChange() 
{
	SetModified();
}

void CPPgStatistic::LoadSettings()
{
	UINT nCheck;
	nCheck = (SYS_PREF()->m_nStatMode & FP_MODE_BLANK_IN_COMMENT_BLOCK_COMMENT) ? BST_CHECKED : BST_UNCHECKED;
	CheckDlgButton(IDC_CHECK_BLANK_IN_COMMENT_BLOCK_COMMENT, nCheck);

	nCheck = (SYS_PREF()->m_nStatMode & FP_MODE_MIXED_LINE_CODE) ? BST_CHECKED : BST_UNCHECKED;
	CheckDlgButton(IDC_CHECK_MIXED_LINE_CODE, nCheck);

	nCheck = (SYS_PREF()->m_nStatMode & FP_MODE_MIXED_LINE_COMMENT) ? BST_CHECKED : BST_UNCHECKED;
	CheckDlgButton(IDC_CHECK_MIXED_LINE_COMMENT, nCheck);
}
