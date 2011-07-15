// NewFilterGroupDlg.cpp: implementation of the CNewFilterGroupDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewFilterGroupDlg.h"
#include "DlgTemplate.h"
#include "LangRuleDlg.h"
#include "LangGrammarMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static LPCTSTR lpFilterGroupName = _T("Filter Group Name:");
static LPCTSTR lpLangRuleTypeName = _T("Language Rule Type:");

#define ID_FILTER_GROUP			48297
#define ID_LANG_RULE_TYPE		48298

CNewFilterGroupDlg::CNewFilterGroupDlg()
{	
	m_nCurSelectedIndex = -100;

	DWORD style = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 600, 35, _T("MS Sans Serif"), 8);
	
	m_pLangRuleDlg = new CLangRuleDlg();

	m_pFilterNameLabel = new CStatic();
	m_pLangRuleTypeLabel = new CStatic();
	m_pFilterName = new CEdit();
	m_pLangRuleType = new CComboBox();
	
	m_pOkButton = new CButton();
	m_pCancelButton = new CButton();
	
	InitModalIndirect(*m_dlgTemplate);
}

CNewFilterGroupDlg::~CNewFilterGroupDlg()
{
	delete m_pLangRuleDlg;
	delete m_pFilterNameLabel;
	delete m_pLangRuleTypeLabel;
	delete m_pFilterName;
	delete m_pLangRuleType;

	delete m_pOkButton;
	delete m_pCancelButton;

	delete m_dlgTemplate;
}

BEGIN_MESSAGE_MAP(CNewFilterGroupDlg, CDialog)
	ON_CBN_SELCHANGE(ID_LANG_RULE_TYPE, OnSelchangeComboBox)
	ON_REGISTERED_MESSAGE(CLangRuleDlg::WM_LANG_SEL_CHANGED, OnLangListBoxSelChanged)
END_MESSAGE_MAP()

BOOL CNewFilterGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T("Add Filter Group"));

	InitGUI();
	InitComboBox();

	m_pLangRuleDlg->AddMsgReceiver(m_hWnd);

	return TRUE;
}

void CNewFilterGroupDlg::InitGUI()
{
	CFont* pDlgFont = GetFont();

	//1. Grammar Dialog layout
	m_pLangRuleDlg->CreateModeless(this);
	m_pLangRuleDlg->SetFont(pDlgFont);

	CRect rcLangRuleDlg;
	m_pLangRuleDlg->GetWindowRect(&rcLangRuleDlg);

	CSize szMax;
	CalcLabelSize(szMax);

	int nStartX = 7;
	int nStartY = 7;
	int nSpaceX = 5;
	int nSpaceY = 7;

	int nEditWidth = rcLangRuleDlg.Width() - 2 * nStartX - szMax.cx - nSpaceX;

	CRect rect;

	//Filter Group Label
	rect.left = nStartX;
	rect.top = nStartY;
	rect.right = rect.left + szMax.cx;
	rect.bottom = rect.top + szMax.cy;
	m_pFilterNameLabel->Create(lpFilterGroupName, WS_CHILD | WS_VISIBLE | SS_RIGHT, rect, this, IDC_STATIC);
	m_pFilterNameLabel->SetFont(pDlgFont);

	//Filter Group Edit
	rect.left = rect.right + nSpaceX;
	rect.right = rect.left + nEditWidth;	
	m_pFilterName->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL, rect, this, ID_FILTER_GROUP);
	m_pFilterName->ModifyStyleEx(0, WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY, SWP_DRAWFRAME);
	m_pFilterName->SetFont(pDlgFont);

	//Lang Rule Type Label
	rect.left = nStartX;
	rect.top = nStartY + nSpaceY + szMax.cy;
	rect.right = rect.left + szMax.cx;
	rect.bottom = rect.top + szMax.cy;
	m_pLangRuleTypeLabel->Create(lpLangRuleTypeName, WS_CHILD | WS_VISIBLE | SS_RIGHT, rect, this, IDC_STATIC);
	m_pLangRuleTypeLabel->SetFont(pDlgFont);

	//Lang Rule Type ComboBox
	rect.left = rect.right + nSpaceX;
	rect.right = rect.left + nEditWidth;
	m_pLangRuleType->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, rect, this, ID_LANG_RULE_TYPE);
	m_pLangRuleType->SetFont(pDlgFont);

	//Move the lang rule dialog to the correct position
	m_pLangRuleDlg->SetWindowPos(NULL, 0, rect.bottom + nSpaceY, rcLangRuleDlg.Width(), rcLangRuleDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);

	rect.top = rect.bottom + nSpaceY + rcLangRuleDlg.Height() + nSpaceY;

	//3. OK, Cancel Button layout
	CRect rcButton(0, 0, 50, 14);
	MapDialogRect(&rcButton);
	int nButtonWidth = rcButton.Width();
	int nButtonHeight = rcButton.Height();
	int nButtonSpace = 14;

	int nButtonTotalWidth = nButtonWidth * 2 + nButtonSpace;

	//OK
	rcButton.left = (rcLangRuleDlg.Width() - nButtonTotalWidth) / 2;
	rcButton.top = rect.top;
	rcButton.right = rcButton.left + nButtonWidth;
	rcButton.bottom = rcButton.top + nButtonHeight;
	m_pOkButton->Create("OK", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, rcButton, this, IDOK);
	m_pOkButton->SetFont(pDlgFont);

	//Cancel
	rcButton.left = rcButton.right + nButtonSpace;
	rcButton.right = rcButton.left + nButtonWidth;
	m_pCancelButton->Create("Cancel", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rcButton, this, IDCANCEL);
	m_pCancelButton->SetFont(pDlgFont);

	//Dialog reposition
	CRect rcWindow, rcClient;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	int nDlgHeight = rcButton.bottom + nButtonSpace;
	nDlgHeight += rcWindow.Height() - rcClient.Height();

	int nDlgWidth = rcLangRuleDlg.Width() + rcWindow.Width() - rcClient.Width();
	SetWindowPos(NULL, 0, 0, nDlgWidth, nDlgHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CNewFilterGroupDlg::CalcLabelSize(CSize& szMax)
{
	CSize sz;

	CDC* pDC = GetDC();

	sz = pDC->GetTextExtent(lpFilterGroupName);
	szMax = pDC->GetTextExtent(lpLangRuleTypeName);
	if(sz.cx > szMax.cx)
	{
		szMax = sz;
	}

	szMax.cx += 2;
	szMax.cy += 2;

	ReleaseDC(pDC);
}

void CNewFilterGroupDlg::InitComboBox()
{
	std::map<int, CLangGrammarInfo*>& mapLangGrammar = 
		CLangGrammarMap::GetInstance()->GetLangGrammaInfoMap();
	
	std::map<int, CLangGrammarInfo*>::iterator iter = mapLangGrammar.begin();
	CLangGrammarInfo* pLangGrammarInfo;
	CString sContent;
	for( ; iter != mapLangGrammar.end(); ++iter)
	{
		pLangGrammarInfo = iter->second;
		if(!pLangGrammarInfo->IsUserDefined())
		{
			m_pLangRuleType->AddString(pLangGrammarInfo->m_szLangName);
		}
		else
		{
			sContent.Format("%s%s", "[Customized]: ", pLangGrammarInfo->m_szLangName);
			m_pLangRuleType->AddString(sContent);
		}
	}
}

void CNewFilterGroupDlg::OnSelchangeComboBox()
{
	int nCurSel = m_pLangRuleType->GetCurSel();
	if(m_nCurSelectedIndex != nCurSel)
	{
		::SendMessage(m_pLangRuleDlg->GetSafeHwnd(), CLangRuleDlg::WM_LANG_SEL_CHANGED, m_nCurSelectedIndex, nCurSel);
		m_nCurSelectedIndex = nCurSel;
	}
}

LRESULT CNewFilterGroupDlg::OnLangListBoxSelChanged(WPARAM wParam, LPARAM lParam)
{
	int nCurSel = (int)lParam;
	if(nCurSel != m_pLangRuleType->GetCurSel())
	{
		m_pLangRuleType->SetCurSel(nCurSel);
		m_nCurSelectedIndex = nCurSel;
	}

	return 1L;
}
