// ShowLangRuleDlg.cpp: implementation of the CShowLangRuleDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShowLangRuleDlg.h"
#include "DlgTemplate.h"
#include "LangGrammarDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShowLangRuleDlg::CShowLangRuleDlg()
{
	DWORD style = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 600, 35, _T("MS Sans Serif"), 8);

	m_pLangGrammarDlg = new CLangGrammarDlg();
	m_pLangListBox = new CListBox();

	m_pOkButton = new CButton();
	m_pCancelButton = new CButton();

	InitModalIndirect(*m_dlgTemplate);
}

CShowLangRuleDlg::~CShowLangRuleDlg()
{
	//Remove dialog template
	if(m_dlgTemplate != NULL)
	{
		delete m_dlgTemplate;
		m_dlgTemplate = NULL;
	}

	delete m_pLangGrammarDlg;
	delete m_pLangListBox;

	delete m_pOkButton;
	delete m_pCancelButton;
}

BOOL CShowLangRuleDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
	{
		return FALSE;
	}
	SetWindowText("Language Rules");

	InitGUI();
	
	return TRUE;
}

void CShowLangRuleDlg::OnOK()
{
	CDialog::OnOK();
}

void CShowLangRuleDlg::InitGUI()
{
	m_pLangGrammarDlg->CreateModeless(this);
	
	CRect rcGrammarDlg;
	m_pLangGrammarDlg->GetWindowRect(&rcGrammarDlg);


	CRect rcListBox;
	rcListBox.left = 7;
	rcListBox.top = 7;
	rcListBox.right = rcListBox.left + 150;
	rcListBox.bottom = rcListBox.top + rcGrammarDlg.Height();

	m_pLangListBox->Create(WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_HSCROLL, rcListBox, this, 48297);

	m_pLangGrammarDlg->SetWindowPos(NULL, rcListBox.right + 5, rcListBox.top, rcGrammarDlg.Width(), rcGrammarDlg.Height(), SWP_NOZORDER);

	int nRequiredWidth = 7 + rcListBox.Width() + 5 + rcGrammarDlg.Width() + 7;
	CRect rcWindow, rcClient;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);

	int nDlgHeight = rcListBox.bottom + 10;
	nDlgHeight += rcWindow.Height() - rcClient.Height();
	if(nDlgHeight < 376)
	{
		nDlgHeight = 376;
	}
	SetWindowPos(NULL, 0, 0, nRequiredWidth, nDlgHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
