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
#define ID_LANG_LIST_BOX		48297
#define ID_BUTTON_APPLY			8888
#define ID_BUTTON_MODIFY		8889
#define ID_BUTTON_DELETE		8890
#define ID_BUTTON_NEW			8891

#define CUR_EDIT_STATUS_INVALID		-1
#define CUR_EDIT_STATUS_NEW			0
#define CUR_EDIT_STATUS_MODIFY		1

CShowLangRuleDlg::CShowLangRuleDlg()
{
	m_nCurSelectedIndex = -1;
	m_nCurEditStatus = CUR_EDIT_STATUS_INVALID;
	m_bModified = FALSE;

	DWORD style = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 600, 35, _T("MS Sans Serif"), 8);

	m_pLangGrammarDlg = new CLangGrammarDlg();
	m_pLangListBox = new CListBox();

	m_pOkButton = new CButton();
	m_pCancelButton = new CButton();

	m_pApplyButton = new CButton();
	m_pDeleteButton = new CButton();
	m_pNewButton = new CButton();
	m_pModifyButton = new CButton();

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
	delete m_pApplyButton;
	delete m_pDeleteButton;
	delete m_pNewButton;
	delete m_pModifyButton;
}
BEGIN_MESSAGE_MAP(CShowLangRuleDlg, CDialog)
	ON_LBN_SELCHANGE(ID_LANG_LIST_BOX, OnSelchangeListLangGrammar)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(ID_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(ID_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(ID_BUTTON_MODIFY, OnButtonModify)
	ON_REGISTERED_MESSAGE(WM_LANG_GRAMMAR_DLG_MSG, OnLangGrammarDlgEditChanged)
END_MESSAGE_MAP()

BOOL CShowLangRuleDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
	{
		return FALSE;
	}
	SetWindowText("Language Rules");

	InitGUI();

	InitLangGrammarData();

	m_pLangGrammarDlg->AddMsgReceiver(GetSafeHwnd());

	m_pLangGrammarDlg->ModifyAllEditStatus(ALL_EDIT_STATUS_READONLY);
	UpdateButtonStatus();
	
	return TRUE;
}

void CShowLangRuleDlg::InitGUI()
{
	CFont* pDlgFont = GetFont();

	//1. Grammar Dialog layout
	m_pLangGrammarDlg->CreateModeless(this);
	
	CRect rcGrammarDlg;
	m_pLangGrammarDlg->GetWindowRect(&rcGrammarDlg);

	int nListBoxHeight = (rcGrammarDlg.Height() > 376) ? rcGrammarDlg.Height() : 376;
	int nStartX = 7;
	int nStartY = 7;
	//2. List Box layout
	CRect rcListBox;
	rcListBox.left = nStartX;
	rcListBox.top = nStartY;
	rcListBox.right = rcListBox.left + 150;
	rcListBox.bottom = rcListBox.top + nListBoxHeight;

	m_pLangListBox->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY, rcListBox, this, ID_LANG_LIST_BOX);
	m_pLangListBox->SetFont(pDlgFont);

	m_pLangGrammarDlg->SetWindowPos(NULL, rcListBox.right + 5, rcListBox.top, rcGrammarDlg.Width(), nListBoxHeight, SWP_NOZORDER);

	int nRequiredWidth = rcListBox.Width() + 5 + rcGrammarDlg.Width() + 2 * nStartX;

	//3. OK, Cancel Button layout
	int nButtonWidth = 75;
	int nButtonHeight = 23;
	int nButtonSpace = 14;
	
	int nButtonTotalWidth = nButtonWidth * 2 + nButtonSpace;
	CRect rcButton;

	//From Right to Left: Apply, Cancel, OK, Delete, New
	rcButton.right = nRequiredWidth - 20;
	rcButton.left = rcButton.right - nButtonWidth;
	rcButton.top = rcListBox.bottom + nButtonSpace;
	rcButton.bottom = rcButton.top + nButtonHeight;
	


	//Cancel Button

	m_pCancelButton->Create("Cancel", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rcButton, this, IDCANCEL);
	m_pCancelButton->SetFont(pDlgFont);

	//OK Button
	rcButton.right = rcButton.left - nButtonSpace;
	rcButton.left = rcButton.right - nButtonWidth;
	m_pOkButton->Create("OK", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, rcButton, this, IDOK);
	m_pOkButton->SetFont(pDlgFont);

	//Apply Button
	rcButton.right = rcButton.left - nButtonSpace;
	rcButton.left = rcButton.right - nButtonWidth;
	m_pApplyButton->Create("Apply", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rcButton, this, ID_BUTTON_APPLY);
	m_pApplyButton->SetFont(pDlgFont);

	rcButton.right = rcButton.left - nButtonSpace;
	rcButton.left = rcButton.right - nButtonWidth;
	m_pModifyButton->Create("Modify", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rcButton, this, ID_BUTTON_MODIFY);
	m_pModifyButton->SetFont(pDlgFont);

	rcButton.right = rcButton.left - nButtonSpace;
	rcButton.left = rcButton.right - nButtonWidth;
	m_pDeleteButton->Create("Delete", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rcButton, this, ID_BUTTON_DELETE);
	m_pDeleteButton->SetFont(pDlgFont);
	
	rcButton.right = rcButton.left - nButtonSpace;
	rcButton.left = rcButton.right - nButtonWidth;
	m_pNewButton->Create("New", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rcButton, this, ID_BUTTON_NEW);
	m_pNewButton->SetFont(pDlgFont);

	//4. The Dialog reposition
	CRect rcWindow, rcClient;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);

	int nDlgHeight = rcButton.bottom + 10;
	nDlgHeight += rcWindow.Height() - rcClient.Height();
	SetWindowPos(NULL, 0, 0, nRequiredWidth, nDlgHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CShowLangRuleDlg::InitLangGrammarData()
{
	std::map<int, CLangGrammarInfo*>& mapLangGrammar = 
		CLangGrammarMap::GetInstance()->GetLangGrammaInfoMap();

	std::map<int, CLangGrammarInfo*>::iterator iter = mapLangGrammar.begin();
	CLangGrammarInfo* pLangGrammarInfo;
	int iIndex;
	for( ; iter != mapLangGrammar.end(); ++iter)
	{
		pLangGrammarInfo = iter->second;
		iIndex = m_pLangListBox->AddString(pLangGrammarInfo->m_szLangName);
		m_pLangListBox->SetItemData(iIndex, iter->first);
	}
}

void CShowLangRuleDlg::OnSelchangeListLangGrammar()
{
	int nCurSel = m_pLangListBox->GetCurSel();
	//Selection doesn't change actually
	if(m_nCurSelectedIndex == nCurSel)
	{
		return;
	}
	m_nCurEditStatus = CUR_EDIT_STATUS_INVALID;	

	DWORD nItemData = m_pLangListBox->GetItemData(nCurSel);

	CLangGrammarInfo* pLangGrammarInfo = CLangGrammarMap::GetInstance()->GetLangGrammarInfo(nItemData);
	ASSERT(pLangGrammarInfo != NULL);

	m_pLangGrammarDlg->SetLangGrammarInfo(pLangGrammarInfo);

	m_nCurSelectedIndex = nCurSel;
	//Button Status update
	UpdateButtonStatus();
}

void CShowLangRuleDlg::SetModified(BOOL bModified) 
{
	m_bModified = bModified;
	m_pApplyButton->EnableWindow(GetModified() && (m_nCurEditStatus == CUR_EDIT_STATUS_NEW 
			|| m_nCurEditStatus == CUR_EDIT_STATUS_MODIFY));
}

void CShowLangRuleDlg::UpdateButtonStatus()
{
	//Apply
	m_pApplyButton->EnableWindow(GetModified() && (m_nCurEditStatus == CUR_EDIT_STATUS_NEW 
		|| m_nCurEditStatus == CUR_EDIT_STATUS_MODIFY));

	//New
	m_pNewButton->EnableWindow(m_nCurEditStatus != CUR_EDIT_STATUS_NEW);

	//Delete
	int nCurSel = m_pLangListBox->GetCurSel();
	if(nCurSel == LB_ERR)
	{
		m_pModifyButton->EnableWindow(FALSE);
		m_pDeleteButton->EnableWindow(FALSE);
		if(m_nCurEditStatus == CUR_EDIT_STATUS_INVALID)
		{
			m_pLangGrammarDlg->ModifyAllEditStatus(ALL_EDIT_STATUS_EMPTY | ALL_EDIT_STATUS_READONLY);
		}
		else
		{
			m_pLangGrammarDlg->ModifyAllEditStatus(0, ALL_EDIT_STATUS_EMPTY | ALL_EDIT_STATUS_READONLY);
		}
	}
	else
	{
		DWORD nItemData = m_pLangListBox->GetItemData(nCurSel);
		CLangGrammarInfo* pLangGrammarInfo = CLangGrammarMap::GetInstance()->GetLangGrammarInfo(nItemData);
		if(pLangGrammarInfo != NULL && pLangGrammarInfo->IsUserDefined())
		{			
			m_pDeleteButton->EnableWindow(TRUE);
			if(m_nCurEditStatus == CUR_EDIT_STATUS_MODIFY)
			{
				m_pModifyButton->EnableWindow(FALSE);
				m_pLangGrammarDlg->ModifyAllEditStatus(0, ALL_EDIT_STATUS_EMPTY | ALL_EDIT_STATUS_READONLY);
			}
			else
			{
				m_pModifyButton->EnableWindow(TRUE);
				m_pLangGrammarDlg->ModifyAllEditStatus(ALL_EDIT_STATUS_READONLY, ALL_EDIT_STATUS_EMPTY);
			}
		}
		else
		{
			m_pModifyButton->EnableWindow(FALSE);
			m_pDeleteButton->EnableWindow(FALSE);
			m_pLangGrammarDlg->ModifyAllEditStatus(ALL_EDIT_STATUS_READONLY, ALL_EDIT_STATUS_EMPTY);
		}
	}
}

void CShowLangRuleDlg::OnOK()
{
	OnButtonApply();
	CDialog::OnOK();
}

void CShowLangRuleDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CShowLangRuleDlg::OnButtonApply()
{
	if(m_nCurEditStatus != CUR_EDIT_STATUS_NEW && m_nCurEditStatus != CUR_EDIT_STATUS_MODIFY)
	{
		return;
	}
	m_pLangGrammarDlg->UpdatePropValue();
	CLangGrammarInfo* pLangGrammarInfo = NULL;
	if(!m_pLangGrammarDlg->GetLangGrammarInfo(pLangGrammarInfo, FALSE, TRUE))
	{
		return;
	}

	if(m_nCurEditStatus == CUR_EDIT_STATUS_NEW)
	{
		//Add Lang Grammar into map
		CLangGrammarMap::GetInstance()->AddLangGrammarInfo(pLangGrammarInfo);

		int iIndex = m_pLangListBox->AddString(pLangGrammarInfo->m_szLangName);
		m_pLangListBox->SetItemData(iIndex, pLangGrammarInfo->m_nLangType);
	}
	else if(m_nCurEditStatus == CUR_EDIT_STATUS_MODIFY)
	{
		DWORD dwOldData = m_pLangListBox->GetItemData(m_nCurSelectedIndex);
		CLangGrammarInfo* pOldLangInfo = CLangGrammarMap::GetInstance()->GetLangGrammarInfo(dwOldData);
		ASSERT(pOldLangInfo);

		//Use the old lang type
		pLangGrammarInfo->m_nLangType = pOldLangInfo->m_nLangType;

		//Check if the name changed
		BOOL bLangNameChanged = (pOldLangInfo->m_szLangName.Compare(pLangGrammarInfo->m_szLangName) != 0);

		CLangGrammarMap::GetInstance()->ModifyLangGrammarInfo(pLangGrammarInfo);

		if(bLangNameChanged)
		{
			m_pLangListBox->LockWindowUpdate(); 
			int iIndex = m_pLangListBox->DeleteString(m_nCurSelectedIndex); 
			iIndex = m_pLangListBox->InsertString(m_nCurSelectedIndex, pLangGrammarInfo->m_szLangName);
			m_pLangListBox->SetItemData(iIndex, pLangGrammarInfo->m_nLangType);
			m_pLangListBox->UnlockWindowUpdate();

			m_pLangListBox->SetCurSel(m_nCurSelectedIndex);
		}

// 		m_nCurSelectedIndex = -1;
// 		m_pLangListBox->SetCurSel(-1);
	}
	SetModified(FALSE);
	m_nCurEditStatus = CUR_EDIT_STATUS_INVALID;
	UpdateButtonStatus();
}
void CShowLangRuleDlg::OnButtonNew()
{
	m_nCurSelectedIndex = -1;
	m_pLangListBox->SetCurSel(-1);

	m_pLangGrammarDlg->ModifyAllEditStatus(ALL_EDIT_STATUS_EMPTY, ALL_EDIT_STATUS_READONLY);

	CEdit* pEdit = m_pLangGrammarDlg->GetEdit(CLangGrammarDlg::lpszLangName);
	pEdit->SetFocus();

	m_nCurEditStatus = CUR_EDIT_STATUS_NEW;

	UpdateButtonStatus();
}
void CShowLangRuleDlg::OnButtonDelete()
{
	int nCurSel = m_pLangListBox->GetCurSel();
	if(nCurSel < 0)
	{
		AfxMessageBox("You must select one item to delete");
		return;
	}
	DWORD nItemData = m_pLangListBox->GetItemData(nCurSel);
	
	CLangGrammarMap::GetInstance()->DeleteLangGrammarInfo(nItemData);

	m_pLangListBox->DeleteString(nCurSel);
	m_nCurSelectedIndex = -1;

	m_nCurEditStatus = CUR_EDIT_STATUS_INVALID;

	UpdateButtonStatus();
}
void CShowLangRuleDlg::OnButtonModify()
{
//	m_pLangGrammarDlg->ModifyAllEditStatus(0, ALL_EDIT_STATUS_READONLY);

	m_nCurEditStatus = CUR_EDIT_STATUS_MODIFY;
	UpdateButtonStatus();
}


LRESULT CShowLangRuleDlg::OnLangGrammarDlgEditChanged(WPARAM wParam, LPARAM lParam)
{	
	HWND hWndEdit = (HWND)lParam;
	if(::IsWindow(m_hWnd) && ::IsWindowVisible(m_hWnd) && ::GetFocus() == hWndEdit)
	{
		AfxTrace("This should be in user edit mode.\n");
		SetModified(TRUE);
	}
	return 1L;
}
