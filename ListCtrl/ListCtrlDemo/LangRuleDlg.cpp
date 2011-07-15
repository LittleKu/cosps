// LangRuleDlg.cpp: implementation of the CLangRuleDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LangRuleDlg.h"
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
#define ID_LANG_LIST_BOX		48297
#define ID_LANG_LIST_BOX_BK_WND	48298

const LPCTSTR lpUserDefinedLangPrefix = _T("[Customized]: ");

UINT CLangRuleDlg::WM_LANG_SEL_CHANGED = ::RegisterWindowMessage(_T("WM_LANG_SEL_CHANGED"));

CLangRuleDlg::CLangRuleDlg() : CDialog()
{
	m_nCurSelectedIndex = -100;
	m_hWndMsgReceiver = NULL;
	
	DWORD style = WS_CHILD | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 100, 35, _T("MS Sans Serif"), 8);
	
	m_pLangGrammarDlg = new CLangGrammarDlg();
	m_pLangListBox = new CListBox();
	m_pListBoxBkWnd = new CBkWnd();
	
	InitModalIndirect(*m_dlgTemplate);
}

CLangRuleDlg::~CLangRuleDlg()
{
	delete m_pLangGrammarDlg;
	delete m_pLangListBox;
	delete m_pListBoxBkWnd;

	delete m_dlgTemplate;
}

BEGIN_MESSAGE_MAP(CLangRuleDlg, CDialog)
	ON_REGISTERED_MESSAGE(WM_LANG_SEL_CHANGED, OnLangSelChanged)
	ON_LBN_SELCHANGE(ID_LANG_LIST_BOX, OnSelchangeListLangGrammar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLangTemplateDlg message handlers
void CLangRuleDlg::CreateModeless(CWnd* pParentWnd)
{
	CreateIndirect(*m_dlgTemplate, pParentWnd);
}
BOOL CLangRuleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitGUI();

	InitLangGrammarData();

	int nIndex = (m_pLangListBox->GetCount() > 0) ? 0 : -1;
	SelChanged(nIndex);
	
	return TRUE;
}

void CLangRuleDlg::InitGUI()
{
	CFont* pDlgFont = GetFont();
	
	//1. Grammar Dialog layout
	m_pLangGrammarDlg->CreateModeless(this);
	m_pLangGrammarDlg->SetFont(pDlgFont);
	
	CRect rcGrammarDlg;
	m_pLangGrammarDlg->GetWindowRect(&rcGrammarDlg);

	//2. List Box Background Wnd
	int nListBoxHeight = (rcGrammarDlg.Height() > 376) ? rcGrammarDlg.Height() : 376;
	int nStartX = 7;
	int nStartY = 7;

	CRect rcListBoxBkWnd;
	rcListBoxBkWnd.left = nStartX;
	rcListBoxBkWnd.top = nStartY;
	rcListBoxBkWnd.right = rcListBoxBkWnd.left + 150;
	rcListBoxBkWnd.bottom = rcListBoxBkWnd.top + nListBoxHeight;

	//ListBox Background
	m_pListBoxBkWnd->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcListBoxBkWnd, this, ID_LANG_LIST_BOX_BK_WND);

	//3. ListBox
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT;
	CRect rcListBox = rcListBoxBkWnd;
	rcListBox.DeflateRect(1, 1);
	m_pLangListBox->Create(dwStyle, rcListBox, this, ID_LANG_LIST_BOX);
	m_pLangListBox->SetFont(pDlgFont);
	
	//4. Adjust position
	m_pLangGrammarDlg->SetWindowPos(NULL, rcListBoxBkWnd.right + 5, rcListBoxBkWnd.top, rcGrammarDlg.Width(), nListBoxHeight, SWP_NOZORDER | SWP_NOACTIVATE);
	m_pListBoxBkWnd->SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	
	//The Dialog reposition
	CRect rcWindow, rcClient;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);

	int nDlgWidth = rcListBoxBkWnd.Width() + 5 + rcGrammarDlg.Width() + 2 * nStartX;
	nDlgWidth += rcWindow.Width() - rcClient.Width();

	int nDlgHeight = rcListBoxBkWnd.bottom + 10;
	nDlgHeight += rcWindow.Height() - rcClient.Height();

	SetWindowPos(NULL, 0, 0, nDlgWidth, nDlgHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CLangRuleDlg::InitLangGrammarData()
{
	std::map<int, CLangGrammarInfo*>& mapLangGrammar = 
		CLangGrammarMap::GetInstance()->GetLangGrammaInfoMap();
	
	std::map<int, CLangGrammarInfo*>::iterator iter = mapLangGrammar.begin();
	CDC* pDC = GetDC();
	for( ; iter != mapLangGrammar.end(); ++iter)
	{
		AddStringToListBox(iter->second->m_szLangName, iter->first, pDC);
	}
	ReleaseDC(pDC);
}

void CLangRuleDlg::SelChanged(int nIndex)
{
	if(nIndex == m_nCurSelectedIndex)
	{
		return;
	}
	if(nIndex < 0)
	{
		m_pLangGrammarDlg->SetAllEditStatus(ALL_EDIT_STATUS_READONLY | ALL_EDIT_STATUS_EMPTY);
	}
	else
	{
		DWORD nItemData = m_pLangListBox->GetItemData(nIndex);
		CLangGrammarInfo* pLangGrammarInfo = CLangGrammarMap::GetInstance()->GetLangGrammarInfo(nItemData);
		ASSERT(pLangGrammarInfo != NULL);
		m_pLangGrammarDlg->SetLangGrammarInfo(pLangGrammarInfo);

		m_pLangGrammarDlg->SetAllEditStatus(ALL_EDIT_STATUS_READONLY);
	}
	m_nCurSelectedIndex = nIndex;
}

void CLangRuleDlg::OnSelchangeListLangGrammar()
{
	int nCurSel = m_pLangListBox->GetCurSel();
	int nOldSel = m_nCurSelectedIndex;
	SelChanged(nCurSel);

	if(m_hWndMsgReceiver != NULL && nCurSel != nOldSel)
	{
		::SendMessage(m_hWndMsgReceiver, WM_LANG_SEL_CHANGED, (WPARAM)nOldSel, (LPARAM)nCurSel);
	}
}

LRESULT CLangRuleDlg::OnLangSelChanged(WPARAM wParam, LPARAM lParam)
{
	int nCurSel = (int)lParam;
	m_pLangListBox->SetCurSel(nCurSel);
	SelChanged(nCurSel);
	return 1L;
}

void CLangRuleDlg::AddStringToListBox(LPCTSTR lpszString, DWORD nItemData, CDC* pDC)
{
	CLangGrammarInfo* pLangInfo = CLangGrammarMap::GetInstance()->GetLangGrammarInfo(nItemData);
	ASSERT(pLangInfo != NULL);

	CString sContent;
	if(pLangInfo->IsUserDefined())
	{
		sContent.Format("%s%s", lpUserDefinedLangPrefix, pLangInfo->m_szLangName);
	}
	else
	{
		sContent = lpszString;
	}
	CSize sz = pDC->GetTextExtent(sContent);
	
	LONG nCurScrollWidth = (LONG)::SendDlgItemMessage(m_hWnd, ID_LANG_LIST_BOX, LB_GETHORIZONTALEXTENT, 0, 0);
	if(sz.cx > nCurScrollWidth)
	{
		::SendDlgItemMessage(m_hWnd, ID_LANG_LIST_BOX, LB_SETHORIZONTALEXTENT, (WPARAM)sz.cx, 0);
	}
	int nIndex = m_pLangListBox->AddString(sContent);
	m_pLangListBox->SetItemData(nIndex, nItemData);
}
