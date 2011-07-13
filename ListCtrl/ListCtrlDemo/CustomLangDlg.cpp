// NewLangDlg.cpp: implementation of the CNewLangDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomLangDlg.h"
#include "DlgTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomLangDlg::CCustomLangDlg()
{
	DWORD style = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 600, 35, _T("MS Sans Serif"), 8);
	
	m_pLangGrammarDlg = new CLangGrammarDlg();
	
	m_pOkButton = new CButton();
	m_pCancelButton = new CButton();
	
	InitModalIndirect(*m_dlgTemplate);
}

CCustomLangDlg::~CCustomLangDlg()
{
	delete m_pLangGrammarDlg;
	delete m_pOkButton;
	delete m_pCancelButton;
	delete m_dlgTemplate;
}

BEGIN_MESSAGE_MAP(CCustomLangDlg, CDialog)
END_MESSAGE_MAP()

BOOL CCustomLangDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
	{
		return FALSE;
	}

	SetWindowText("Custom Language");

	InitGUI();

	return TRUE;
}

void CCustomLangDlg::OnOK()
{
	m_pLangGrammarDlg->UpdatePropValue();
	CLangGrammarInfo* pLangGrammarInfo = NULL;
	if(!m_pLangGrammarDlg->GetLangGrammarInfo(pLangGrammarInfo))
	{
		return;
	}
	//Add Lang Grammar into map
	CLangGrammarMap::GetInstance()->AddLangGrammarInfo(pLangGrammarInfo);
	
	CDialog::OnOK();
}
void CCustomLangDlg::InitGUI()
{
//	m_pLangGrammarDlg->SetEditWidth(100);
	m_pLangGrammarDlg->CreateModeless(this);
	
	CRect rcGrammarDlg;
	m_pLangGrammarDlg->GetWindowRect(&rcGrammarDlg);

	int nStartX = 7;
	int nStartY = 7;
	m_pLangGrammarDlg->SetWindowPos(NULL, nStartX, nStartY, rcGrammarDlg.Width(), rcGrammarDlg.Height(), SWP_NOZORDER);

	int nRequiredWidth = rcGrammarDlg.Width() + nStartX * 2;

	CRect rcClient;
	GetClientRect(&rcClient);

	int nButtonWidth = 75;
	int nButtonHeight = 23;
	int nButtonSpace = 14;

	int nButtonTotalWidth = nButtonWidth * 2 + nButtonSpace;
	CRect rect;

	rect.left = (nRequiredWidth - nButtonTotalWidth) / 2;
	rect.top = nStartY + rcGrammarDlg.Height() + nButtonSpace;
	rect.right = rect.left + nButtonWidth;
	rect.bottom = rect.top + nButtonHeight;

	CFont* pDlgFont = GetFont();
	m_pOkButton->Create("OK", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, rect, this, IDOK);
	m_pOkButton->SetFont(pDlgFont);

	rect.left = rect.right + nButtonSpace;
	rect.right = rect.left + nButtonWidth;
	m_pCancelButton->Create("Cancel", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rect, this, IDCANCEL);
	m_pCancelButton->SetFont(pDlgFont);

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	int nDlgHeight = rect.bottom + nButtonSpace;
	nDlgHeight += rcWindow.Height() - rcClient.Height();

	SetWindowPos(NULL, 0, 0, nRequiredWidth, nDlgHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
