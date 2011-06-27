// PPgGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "PPgGeneral.h"
#include "Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPgGeneral dialog

CPPgGeneral::CPPgGeneral() : CPropertyPage(IDD_PROPPAGE_GENERAL)
{
	//{{AFX_DATA_INIT(CPPgGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
	ON_EN_CHANGE(IDC_MAX_HISTORY_ITEMS, OnSettingsChange)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE_SUB_FOLDER, OnSettingsChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPgGeneral message handlers

BOOL CPPgGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	LoadSettings();

	RepositionComponents();
	
	return TRUE;
}

BOOL CPPgGeneral::OnApply() 
{
	UINT nChecked = IsDlgButtonChecked(IDC_CHECK_INCLUDE_SUB_FOLDER);
	SYS_PREF()->m_bSearchIncludeSubFolders = ((nChecked & BST_CHECKED) != 0);

	CString str;
	GetDlgItemText(IDC_MAX_HISTORY_ITEMS, str);
	if(!str.IsEmpty())
	{
		SYS_PREF()->m_nMaxItemsInFilterComboBox = _ttoi(str);
	}

	SetModified(FALSE);
	return CPropertyPage::OnApply();
}

void CPPgGeneral::LoadSettings()
{
	CheckDlgButton(IDC_CHECK_INCLUDE_SUB_FOLDER, 
		(SYS_PREF()->m_bSearchIncludeSubFolders ? BST_CHECKED : BST_UNCHECKED));

	SetDlgItemInt(IDC_MAX_HISTORY_ITEMS, SYS_PREF()->m_nMaxItemsInFilterComboBox);
}

void CPPgGeneral::RepositionComponents()
{
	CWnd* pWnd = GetDlgItem(IDC_TXT_MAX_HISTORY_ITEMS);
	ASSERT(pWnd);
	CString szText;
	pWnd->GetWindowText(szText);

	CDC* pDC = GetDC();
	CSize sz = pDC->GetTextExtent(szText);
	ReleaseDC(pDC);
	sz.cx += 2;

	CRect rcLabelMaxHisItems;
	pWnd->GetWindowRect(&rcLabelMaxHisItems);
	ScreenToClient(&rcLabelMaxHisItems);
	rcLabelMaxHisItems.right = rcLabelMaxHisItems.left + sz.cx;
	pWnd->MoveWindow(&rcLabelMaxHisItems);


	pWnd = GetDlgItem(IDC_MAX_HISTORY_ITEMS);
	ASSERT(pWnd);

	CRect rcMaxHisItems;
	pWnd->GetWindowRect(&rcMaxHisItems);
	ScreenToClient(&rcMaxHisItems);
	int nWidth = rcMaxHisItems.Width();
	rcMaxHisItems.left = rcLabelMaxHisItems.right;
	rcMaxHisItems.right = rcMaxHisItems.left + nWidth;
	pWnd->MoveWindow(&rcMaxHisItems);
}
