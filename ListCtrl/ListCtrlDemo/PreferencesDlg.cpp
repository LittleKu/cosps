#include "stdafx.h"
#include "PreferencesDlg.h"
#include "Preferences.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CPreferencesDlg, CTreePropSheet)

BEGIN_MESSAGE_MAP(CPreferencesDlg, CTreePropSheet)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CPreferencesDlg::CPreferencesDlg() : m_bSaveIniFile(FALSE)
{
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_wndGeneral.m_psp.dwFlags &= ~PSP_HASHELP;	
	m_wndStatistic.m_psp.dwFlags &= ~PSP_HASHELP;

	TreePropSheet::CTreePropSheet::SetPageIcon(&m_wndGeneral, IDI_OPTIONS_GENERAL);

	AddPage(&m_wndGeneral);
	AddPage(&m_wndStatistic);

	//Enable Tree View Mode when there are more than 3 pages.
	SetTreeViewMode((GetPageCount() >= 3), TRUE, TRUE);
}

CPreferencesDlg::~CPreferencesDlg()
{
}


BOOL CPreferencesDlg::OnInitDialog()
{
	BOOL bResult = CTreePropSheet::OnInitDialog();

	SetTitle(_T("Options"));

	return bResult;
}

void CPreferencesDlg::OnDestroy()
{
	CTreePropSheet::OnDestroy();
	if (m_bSaveIniFile)
	{
		SYS_PREF()->Save();
		m_bSaveIniFile = FALSE;
	}
}

BOOL CPreferencesDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDOK || wParam == ID_APPLY_NOW)
	{
		m_bSaveIniFile = TRUE;
	}
	return CTreePropSheet::OnCommand(wParam, lParam);
}