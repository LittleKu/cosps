// OptionDlg.cpp: implementation of the COptionDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YTGet.h"
#include "OptionDlg.h"
#include "Options.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(COptionDlg, CTreePropSheet)

BEGIN_MESSAGE_MAP(COptionDlg, CTreePropSheet)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

COptionDlg::COptionDlg() : m_bSaveIniFile(FALSE)
{
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_wndGeneral.m_psp.dwFlags &= ~PSP_HASHELP;	
	m_wndConnection.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndProxy.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_wndGeneral);
	AddPage(&m_wndConnection);
	AddPage(&m_wndProxy);
	
	//Enable Tree View Mode when there are more than 3 pages.
	SetTreeViewMode(/*(GetPageCount() >= 3)*/TRUE, TRUE, TRUE);
}

COptionDlg::~COptionDlg()
{

}

BOOL COptionDlg::OnInitDialog()
{
	BOOL bResult = CTreePropSheet::OnInitDialog();
	
	SetTitle(_T("Options"));
	
	return bResult;
}

void COptionDlg::OnDestroy()
{
	CTreePropSheet::OnDestroy();
	if (m_bSaveIniFile)
	{
		SYS_OPTIONS()->Save();
		m_bSaveIniFile = FALSE;
	}
}

BOOL COptionDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDOK || wParam == ID_APPLY_NOW)
	{
		m_bSaveIniFile = TRUE;
	}
	return CTreePropSheet::OnCommand(wParam, lParam);
}
