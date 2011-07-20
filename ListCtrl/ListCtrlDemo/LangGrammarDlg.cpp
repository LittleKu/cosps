// LangGrammarDlg.cpp: implementation of the CLangGrammarDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LangGrammarDlg.h"
#include "DlgTemplate.h"
#include "ListCtrlDemo.h"
#include "StringUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define ID_TXT_LABEL_FIRST		1500
#define ID_PROP_EDIT_FIRST		1700

int CLangTemplateDlg::nStartPosX = 7;
int CLangTemplateDlg::nStartPosY = 7;
int CLangTemplateDlg::nSpaceX = 5;
int CLangTemplateDlg::nSpaceY = 7;

#define LANG_GRAMMAR_SEPARATOR	STRING_SEPARATOR

UINT WM_LANG_GRAMMAR_DLG_MSG = ::RegisterWindowMessage(_T("WM_LANG_GRAMMAR_DLG_MSG"));

CLangTemplateDlg::CLangTemplateDlg()  : CDialog(), m_dlgTemplate(NULL)
{
	//{{AFX_DATA_INIT(CLangTemplateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	DWORD style = WS_CHILD | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 400, 35, _T("MS Sans Serif"), 8);

	m_nEditWidth = 1024;

	m_nStatus = 0;
	m_hWndMsgReceiver = NULL;
}
CLangTemplateDlg::~CLangTemplateDlg()
{
	//Remove all layout info
	m_mapLayout.RemoveAll();
	m_listLayout.RemoveAll();

	m_mapPropOut.RemoveAll();

	//Remove all static controls
	CStatic* pStatic = NULL;
	POSITION pos = m_listLabel.GetHeadPosition();
	while(pos != NULL)
	{
		pStatic = (CStatic*)m_listLabel.GetNext(pos);
		delete pStatic;
	}
	m_listLabel.RemoveAll();
	
	//Remove all edit controls
	CEdit* pEdit = NULL;
	pos = m_listEdit.GetHeadPosition();
	while(pos != NULL)
	{
		pEdit = (CEdit*)m_listEdit.GetNext(pos);
		delete pEdit;
	}
	m_listEdit.RemoveAll();

	//Remove all button controls
	CButton* pButton = NULL;
	pos = m_listCheckBox.GetHeadPosition();
	while(pos != NULL)
	{
		pButton = (CButton*)m_listCheckBox.GetNext(pos);
		delete pButton;
	}
	m_listCheckBox.RemoveAll();

	//Remove dialog template
	if(m_dlgTemplate != NULL)
	{
		delete m_dlgTemplate;
		m_dlgTemplate = NULL;
	}
}

BEGIN_MESSAGE_MAP(CLangTemplateDlg, CDialog)
	//{{AFX_MSG_MAP(CLangTemplateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLangTemplateDlg message handlers
void CLangTemplateDlg::CreateModeless(CWnd* pParentWnd)
{
	CreateIndirect(*m_dlgTemplate, pParentWnd);
}
BOOL CLangTemplateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	ModifyStyleEx(m_hWnd, 0, WS_EX_WINDOWEDGE, SWP_DRAWFRAME);
	CreateControls();
	
	return TRUE;
}

void CLangTemplateDlg::AddProperty(LPCTSTR lpszName, int gridwidth, LPCTSTR lpszDefaultValue, int type)
{
	LayoutInfo layout(type, lpszName, lpszDefaultValue, gridwidth);
	if(type == LAYOUT_TYPE_SEPARATOR)
	{
		m_listLayout.AddTail(layout);
		return;
	}
	// must not be already there!
	void* lpValue = NULL;
	ASSERT(!m_mapLayout.Lookup(lpszName, lpValue));
	
	// add to the list and the map
	lpValue = m_listLayout.AddTail(layout);
	m_mapLayout.SetAt(lpszName, lpValue);
}

void CLangTemplateDlg::AddSeparator()
{
	AddProperty(NULL, 0, NULL, LAYOUT_TYPE_SEPARATOR);
}

void CLangTemplateDlg::AddCheckBox(LPCTSTR lpszName, BOOL bEnd, BOOL bCheck)
{
	int gridwidth = bEnd ? 0 : -1;
	LPCTSTR lpszDefaultValue = bCheck ? "1" : "0";
	AddProperty(lpszName, gridwidth, lpszDefaultValue, LAYOUT_TYPE_CHECKBOX);
}

BOOL CLangTemplateDlg::CalcMaxLabelSize(CSize& szMax, int& nMaxGridWidth)
{
	//Init
	szMax.cx = 0;
	szMax.cy = 0;

	nMaxGridWidth = 1;

	CSize sz;
	POSITION pos = m_listLayout.GetHeadPosition();
	CDC* pDC = GetDC();
	while (pos != NULL)
	{
		// get layout info
		LayoutInfo& layout = m_listLayout.GetNext(pos);
		sz = pDC->GetTextExtent(layout.sPropName);
		if(sz.cx > szMax.cx)
		{
			szMax = sz;
		}
		if(layout.nGridWidth > nMaxGridWidth)
		{
			nMaxGridWidth = layout.nGridWidth;
		}
	}
	ReleaseDC(pDC);
	
	szMax.cx += 2;
	szMax.cy += 2;

	return TRUE;
}

void CLangTemplateDlg::CreateControls()
{
	CSize szMax;
	int nMaxGridWidth = 1;
	CalcMaxLabelSize(szMax, nMaxGridWidth);

	CRect clientRect;
	GetClientRect(&clientRect);
	int nGridWidthInPixel = (clientRect.Width() - 2 * nStartPosX - (nMaxGridWidth - 1) * nSpaceX) / nMaxGridWidth;
	if( nGridWidthInPixel > (szMax.cx + nSpaceX + m_nEditWidth))
	{
		nGridWidthInPixel = szMax.cx + nSpaceX + m_nEditWidth;
	}
	ASSERT(nGridWidthInPixel > 0);

	CFont* pDlgFont = GetFont();

	CRect rect;
	CRect lastRect(0, 0, 0, 0);
	int i = 0;
	int nRow = 0, nCol = 0;
	POSITION pos = m_listLayout.GetHeadPosition();
	POSITION posEdit = NULL;
	while (pos != NULL)
	{
		// get layout info
		LayoutInfo& layout = m_listLayout.GetNext(pos);

		if(layout.nType == LAYOUT_TYPE_SEPARATOR)
		{
			ASSERT(nCol == 0);
			CStatic* pStatic = new CStatic();
			CalcSepRect(&rect, &lastRect, clientRect.Width());
			pStatic->Create(layout.sPropName, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, rect, this, IDC_STATIC);
			m_listLabel.AddTail(pStatic);

			nRow++;
		}
		else if(layout.nType == LAYOUT_TYPE_CHECKBOX)
		{
			CalcLabelRect(&rect, &lastRect, nRow, nCol, szMax, nGridWidthInPixel);
			//Re-calculate the rect width
			rect.right = rect.left + nGridWidthInPixel;
			CButton* pButton = new CButton();
			pButton->Create(layout.sPropName, WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX, rect, this, ID_PROP_EDIT_FIRST + i);
			pButton->SetFont(pDlgFont, TRUE);
			posEdit = m_listCheckBox.AddTail(pButton);
			layout.posEdit = posEdit;

			int iValue = atoi(layout.sPropValue);
			pButton->SetCheck((iValue > 0) ? BST_CHECKED : BST_UNCHECKED);

			nRow++;
			nCol = 0;
			i++;
		}
		else
		{
			//Label
			CStatic* pStatic = new CStatic();
			CalcLabelRect(&rect, &lastRect, nRow, nCol, szMax, nGridWidthInPixel);
			pStatic->Create(layout.sPropName, WS_CHILD | WS_VISIBLE | SS_RIGHT, rect, this, ID_TXT_LABEL_FIRST + i);
			pStatic->SetFont(pDlgFont, TRUE);
			m_listLabel.AddTail(pStatic);
			
			//Edit
			CEdit* pEdit = new CEdit();
			rect.left = rect.right + nSpaceX;
			rect.right = rect.left + nGridWidthInPixel - szMax.cx - nSpaceX;
			if(nMaxGridWidth == layout.nGridWidth)
			{
				rect.right = clientRect.right - nStartPosX;
			}
			pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL, rect, this, ID_PROP_EDIT_FIRST + i);
			pEdit->ModifyStyleEx(0,   WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY, SWP_DRAWFRAME);
			pEdit->SetFont(pDlgFont, TRUE);
			pEdit->SetWindowText(layout.sPropValue);
			posEdit = m_listLabel.AddTail(pEdit);
			layout.posEdit = posEdit;

			nCol += layout.nGridWidth;
			ASSERT(nCol <= nMaxGridWidth);
			if(nCol == nMaxGridWidth)
			{
				nRow++;
				nCol = 0;
			}
			i++;
		}

		lastRect = rect;	
	}

	int nDlgHeight = rect.bottom + 21;
	CRect windowRect;
	GetWindowRect(&windowRect);
	nDlgHeight += windowRect.Height() - clientRect.Height();
	SetWindowPos(NULL, 0, 0, windowRect.Width(), nDlgHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CLangTemplateDlg::CalcLabelRect(LPRECT lpRect, LPCRECT lpLastRect, int nRow, int nCol, CSize szMax, int nGridWidthInPixel)
{
	//X
	lpRect->left = nStartPosX + nCol * (nGridWidthInPixel + nSpaceX);
	lpRect->right = lpRect->left + szMax.cx;
	
	//Y
	if(nCol == 0)
	{
		lpRect->top = lpLastRect->bottom + nSpaceY;
		lpRect->bottom = lpRect->top + szMax.cy;
	}
	else
	{
		lpRect->top = lpLastRect->top;
		lpRect->bottom = lpLastRect->bottom;
	}
}

void CLangTemplateDlg::CalcSepRect(LPRECT lpRect, LPCRECT lpLastRect, int nClientWidth)
{
	//X
	lpRect->left = nStartPosX;
	lpRect->right = lpRect->left + 1 + (nClientWidth - 2 * nStartPosX);
	
	//Y
	lpRect->top = lpLastRect->bottom + nSpaceY;
	lpRect->bottom = lpRect->top + 2;
}

void CLangTemplateDlg::UpdatePropValue()
{
	m_mapPropOut.RemoveAll();

	POSITION pos = m_mapLayout.GetStartPosition();
	CString szLabelName, szLabelValue;
	void* pData;
	while (pos != NULL)
	{
		m_mapLayout.GetNextAssoc(pos, szLabelName, pData);
		LayoutInfo& layout = m_listLayout.GetAt((POSITION)pData);
		if(layout.nType == LAYOUT_TYPE_PROP)
		{
			CEdit* pEdit = (CEdit*)m_listEdit.GetAt(layout.posEdit);
			pEdit->GetWindowText(szLabelValue);
			m_mapPropOut.SetAt(szLabelName, szLabelValue);
		}
		else if(layout.nType == LAYOUT_TYPE_CHECKBOX)
		{
			CButton* pButton = (CButton*)m_listCheckBox.GetAt(layout.posEdit);
			m_mapPropOut.SetAt(szLabelValue, (pButton->GetCheck() == BST_CHECKED) ? "1" : "0");
		}
	}
}

CEdit* CLangTemplateDlg::GetEdit(LPCTSTR lpszName)
{
	void* pData = NULL;
	if(!m_mapLayout.Lookup(lpszName, pData))
	{
		return NULL;
	}
	ASSERT(pData != NULL);
	
	LayoutInfo& layout = m_listLayout.GetAt((POSITION)pData);	
	return (CEdit*)m_listEdit.GetAt(layout.posEdit);
}

void CLangTemplateDlg::ModifyAllEditStatus(DWORD dwAdd, DWORD dwRemove)
{
	DWORD nStatus = GetAllEditStatus();
	nStatus |= dwAdd;
	nStatus &= ~dwRemove;
	SetAllEditStatus(nStatus);
}

void CLangTemplateDlg::SetAllEditStatus(DWORD nStatus)
{
	if(m_nStatus == nStatus)
	{
		return;
	}
	m_nStatus = nStatus;

	POSITION pos = m_mapLayout.GetStartPosition();
	CString szLabelName, szLabelValue;
	void* pData;
	while (pos != NULL)
	{
		m_mapLayout.GetNextAssoc(pos, szLabelName, pData);
		LayoutInfo& layout = m_listLayout.GetAt((POSITION)pData);

		if(layout.nType == LAYOUT_TYPE_PROP)
		{
			CEdit* pEdit = (CEdit*)m_listEdit.GetAt(layout.posEdit);
			
			if(m_nStatus & ALL_EDIT_STATUS_EMPTY)
			{
				pEdit->SetWindowText("");
			}
			pEdit->SetReadOnly(m_nStatus & ALL_EDIT_STATUS_READONLY);
		}
		else if(layout.nType == LAYOUT_TYPE_CHECKBOX)
		{
			CButton* pButton = (CButton*)m_listCheckBox.GetAt(layout.posEdit);
			if(m_nStatus & ALL_EDIT_STATUS_EMPTY)
			{
				pButton->SetCheck(BST_UNCHECKED);
			}
			pButton->EnableWindow( (m_nStatus & ALL_EDIT_STATUS_READONLY) ? FALSE : TRUE );
		}
	}
	if( (m_nStatus & ALL_EDIT_STATUS_READONLY) == 0)
	{
		Invalidate();
		UpdateWindow();
	}
}

BOOL CLangTemplateDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int nCode = HIWORD(wParam);
	if(nCode == EN_CHANGE && m_hWndMsgReceiver != NULL)
	{
		::SendMessage(m_hWndMsgReceiver, WM_LANG_GRAMMAR_DLG_MSG, wParam, lParam);
	}	
	return CDialog::OnCommand(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CLangGrammarDlg, CLangTemplateDlg)
	//{{AFX_MSG_MAP(CLangGrammarDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LPCTSTR CLangGrammarDlg::lpszLangName = _T("Name:");
LPCTSTR CLangGrammarDlg::lpszLineComment = _T("Line Comment:");
LPCTSTR CLangGrammarDlg::lpszStartsAtFirstChar = _T("Start at first character");
LPCTSTR CLangGrammarDlg::lpszEscapeString = _T("Escape String:");
LPCTSTR CLangGrammarDlg::lpszBlockCommentOn = _T("Block Comment On:");
LPCTSTR CLangGrammarDlg::lpszBlockCommentOff = _T("Block Comment Off:");
LPCTSTR CLangGrammarDlg::lpszStringOn = _T("String On:");
LPCTSTR CLangGrammarDlg::lpszStringOff = _T("String Off:");
LPCTSTR CLangGrammarDlg::lpszCharacterOn = _T("Character On:");
LPCTSTR CLangGrammarDlg::lpszCharacterOff = _T("Character Off:");

CLangGrammarDlg::CLangGrammarDlg()
{
	AddProperty(lpszLangName, 2);
	
	AddSeparator();
	
	AddProperty(lpszLineComment, 2);
//	AddCheckBox(lpszStartsAtFirstChar, TRUE, FALSE);
	AddProperty(lpszEscapeString, 2);
	AddProperty(lpszBlockCommentOn, 1);
	AddProperty(lpszBlockCommentOff, 1);
	AddProperty(lpszStringOn, 1);
	AddProperty(lpszStringOff, 1);
	AddProperty(lpszCharacterOn, 1);
	AddProperty(lpszCharacterOff, 1);
}

CLangGrammarDlg::~CLangGrammarDlg()
{
}

BOOL CLangGrammarDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);	
	CBkDraw::GetInstance()->Draw(pDC, &rect);	
	return TRUE;
}

HBRUSH CLangGrammarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CLangTemplateDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	int nCtrlID = pWnd->GetDlgCtrlID();
	if( (nCtlColor == CTLCOLOR_STATIC) && (nCtrlID >= ID_TXT_LABEL_FIRST && nCtrlID < ID_PROP_EDIT_FIRST))
    {
		pDC->SetBkMode(TRANSPARENT);
		//Comment:
		//1. If we have paint the background, return HOLLOW_BRUSH is the simple way
		//2. If we don't explicitly paint background, return a same brush with the background.
		return (HBRUSH)::GetStockObject(HOLLOW_BRUSH); //1st way
//		return (HBRUSH)::GetStockObject(WHITE_BRUSH);  //2nd way
//		return (HBRUSH)(SYS_APP()->m_pSysBkBrush->GetSafeHandle());
    }
	return hbr;
}
BOOL CLangGrammarDlg::GetLangGrammarInfo(CLangGrammarInfo*& pLangGrammarInfo, BOOL bValidate, BOOL bShowError)
{
	if(bValidate)
	{
		if(!Validate(TRUE))
		{
			return FALSE;
		}
	}

	//1. validation
	pLangGrammarInfo = new CLangGrammarInfo();
	BOOL bResult = FALSE;
	
	CStringList tokenList1, tokenList2;
	POSITION pos1, pos2;
	CString szTemp1, szTemp2;
	do 
	{
		//Lang Name
		m_mapPropOut.Lookup(lpszLangName, pLangGrammarInfo->m_szLangName);
		pLangGrammarInfo->m_szLangName.TrimLeft();
		if(pLangGrammarInfo->m_szLangName.IsEmpty())
		{
			if(bShowError)
			{
				AfxMessageBox(_T("Language name can't be empty."));
			}
			break;
		}
		
		CLangGrammarBuilder builder;
		
		//Line Comment
		m_mapPropOut.Lookup(lpszLineComment, szTemp1);
		CStringUtils::Split(szTemp1, LANG_GRAMMAR_SEPARATOR, tokenList1, TRUE, FALSE);
		for(pos1 = tokenList1.GetHeadPosition(); pos1 != NULL; )
		{
			builder.AddSingleComment(tokenList1.GetNext(pos1));
		}
		
		//Escape String
		m_mapPropOut.Lookup(lpszEscapeString, szTemp1);
		tokenList1.RemoveAll();
		CStringUtils::Split(szTemp1, LANG_GRAMMAR_SEPARATOR, tokenList1, TRUE, FALSE);
		for(pos1 = tokenList1.GetHeadPosition(); pos1 != NULL; )
		{
			builder.AddEscapeStr(tokenList1.GetNext(pos1));
		}
		
		//Block Comment
		m_mapPropOut.Lookup(lpszBlockCommentOn, szTemp1);
		tokenList1.RemoveAll();
		CStringUtils::Split(szTemp1, LANG_GRAMMAR_SEPARATOR, tokenList1, TRUE, FALSE);

		m_mapPropOut.Lookup(lpszBlockCommentOff, szTemp2);
		tokenList2.RemoveAll();
		CStringUtils::Split(szTemp2, LANG_GRAMMAR_SEPARATOR, tokenList2, TRUE, FALSE);

		if(tokenList1.GetCount() != tokenList2.GetCount())
		{
			if(bShowError)
			{
				AfxMessageBox(_T("The token number of Block Comment On and Block Comment Off is not same."));
			}
			pLangGrammarInfo->m_pLangGrammar = builder.GetResult();
			break;
		}
		else
		{
			pos1 = tokenList1.GetHeadPosition();
			pos2 = tokenList2.GetHeadPosition();
			while(pos1 != NULL && pos2 != NULL)
			{
				builder.AddMultiComment(tokenList1.GetNext(pos1), tokenList2.GetNext(pos2));
			}
		}
		
		//String
		m_mapPropOut.Lookup(lpszStringOn, szTemp1);
		tokenList1.RemoveAll();
		CStringUtils::Split(szTemp1, LANG_GRAMMAR_SEPARATOR, tokenList1, TRUE, FALSE);

		m_mapPropOut.Lookup(lpszStringOff, szTemp2);
		tokenList2.RemoveAll();
		CStringUtils::Split(szTemp2, LANG_GRAMMAR_SEPARATOR, tokenList2, TRUE, FALSE);
		if(tokenList1.GetCount() != tokenList2.GetCount())
		{
			if(bShowError)
			{
				AfxMessageBox(_T("The token number of String On and String Off is not same."));
			}
			pLangGrammarInfo->m_pLangGrammar = builder.GetResult();
			break;
		}
		else
		{
			pos1 = tokenList1.GetHeadPosition();
			pos2 = tokenList2.GetHeadPosition();
			while(pos1 != NULL && pos2 != NULL)
			{
				builder.AddStringMark(tokenList1.GetNext(pos1), tokenList2.GetNext(pos2));
			}
		}
		
		//Character
		m_mapPropOut.Lookup(lpszCharacterOn, szTemp1);
		tokenList1.RemoveAll();
		CStringUtils::Split(szTemp1, LANG_GRAMMAR_SEPARATOR, tokenList1, TRUE, FALSE);

		m_mapPropOut.Lookup(lpszCharacterOff, szTemp2);
		tokenList2.RemoveAll();
		CStringUtils::Split(szTemp2, LANG_GRAMMAR_SEPARATOR, tokenList2, TRUE, FALSE);
		if(tokenList1.GetCount() != tokenList2.GetCount())
		{
			if(bShowError)
			{
				AfxMessageBox(_T("The token number of Single Character On and Single Character Off is not same."));
			}
			pLangGrammarInfo->m_pLangGrammar = builder.GetResult();
			break;
		}
		else
		{
			pos1 = tokenList1.GetHeadPosition();
			pos2 = tokenList2.GetHeadPosition();
			while(pos1 != NULL && pos2 != NULL)
			{
				builder.AddCharMark(tokenList1.GetNext(pos1), tokenList2.GetNext(pos2));
			}
		}
		
		pLangGrammarInfo->m_pLangGrammar = builder.GetResult();
		bResult = TRUE;
	} while (0);
	
	if(!bResult)
	{
		delete pLangGrammarInfo;
		pLangGrammarInfo = NULL;
		return FALSE;
	}
	
	return TRUE;
}
BOOL CLangGrammarDlg::Validate(BOOL bShowError)
{
	//TODO
	BOOL bResult = FALSE;
	CString szTemp1, szTemp2;
	do 
	{
		//Lang Name
		m_mapPropOut.Lookup(lpszLangName, szTemp1);
		szTemp1.TrimLeft();
		if(szTemp1.IsEmpty())
		{
			if(bShowError)
			{
				AfxMessageBox(_T("Language name can't be empty."));
			}
			break;
		}
		
		//TODO
		//Line Comment
		bResult = TRUE;
	} while (0);
	
	if(!bResult)
	{
		return FALSE;
	}
	return TRUE;
}

void CLangGrammarDlg::SetLangGrammarInfo(CLangGrammarInfo* pLangGrammarInfo)
{
	if(pLangGrammarInfo == NULL)
	{
		return;
	}
	CEdit* pEdit;

	//Lang Name
	pEdit = GetEdit(lpszLangName);
	pEdit->SetWindowText(pLangGrammarInfo->m_szLangName);

	ILangGrammar* pLangGrammar = pLangGrammarInfo->m_pLangGrammar;
	CString szText, szText2;
	int i, size;

	//Line Comment
	for(i = 0, size = pLangGrammar->GetCountOfSingleLineComment(); i < size; i++)
	{
		CSingleLineComment& lineComment = pLangGrammar->GetSingleLineComment(i);
		szText += MakeEscapeString(lineComment.m_szTag);
		if(i < size - 1)
		{
			szText += STRING_SEPARATOR;
		}
	}
	pEdit = GetEdit(lpszLineComment);
	pEdit->SetWindowText(szText);
	
	//Escape
	szText.Empty();
	for(i = 0, size = pLangGrammar->GetCountOfEscStr(); i < size; i++)
	{
		LG_STRING& escapeStr = pLangGrammar->GetEscapeStr(i);
		szText += MakeEscapeString(escapeStr);
		if(i < size - 1)
		{
			szText += STRING_SEPARATOR;
		}
	}
	pEdit = GetEdit(lpszEscapeString);
	pEdit->SetWindowText(szText);

	//Block Comment
	szText.Empty();
	szText2.Empty();
	for(i = 0, size = pLangGrammar->GetCountOfMultiLineComment(); i < size; i++)
	{
		CMultiLineComment& blockComment = pLangGrammar->GetMultiLineComment(i);
		szText += MakeEscapeString(blockComment.m_szStart);
		szText2 += MakeEscapeString(blockComment.m_szEnd);
		if(i < size - 1)
		{
			szText += STRING_SEPARATOR;
			szText2 += STRING_SEPARATOR;
		}
	}

	pEdit = GetEdit(lpszBlockCommentOn);
	pEdit->SetWindowText( szText );

	pEdit = GetEdit(lpszBlockCommentOff);
	pEdit->SetWindowText( szText2 );

	//String
	szText.Empty();
	szText2.Empty();
	for(i = 0, size = pLangGrammar->GetCountOfStringMark(); i < size; i++)
	{
		CPair& stringMark = pLangGrammar->GetStringMark(i);
		szText += MakeEscapeString(stringMark.m_szStart);
		szText2 += MakeEscapeString(stringMark.m_szEnd);
		if(i < size - 1)
		{
			szText += STRING_SEPARATOR;
			szText2 += STRING_SEPARATOR;
		}
	}
	pEdit = GetEdit(lpszStringOn);
	pEdit->SetWindowText( szText );
	
	pEdit = GetEdit(lpszStringOff);
	pEdit->SetWindowText( szText2 );

	//Char
	szText.Empty();
	szText2.Empty();
	for(i = 0, size = pLangGrammar->GetCountOfCharMark(); i < size; i++)
	{
		CPair& charMark = pLangGrammar->GetCharMark(i);
		szText += MakeEscapeString(charMark.m_szStart);
		szText2 += MakeEscapeString(charMark.m_szEnd);
		if(i < size - 1)
		{
			szText += STRING_SEPARATOR;
			szText2 += STRING_SEPARATOR;
		}
	}
	pEdit = GetEdit(lpszCharacterOn);
	pEdit->SetWindowText( szText );
	
	pEdit = GetEdit(lpszCharacterOff);
	pEdit->SetWindowText( szText2 );
}

CString CLangGrammarDlg::MakeEscapeString(const CString& str, BOOL bToEscape)
{
	//No escape char
	if(str.Find(STRING_SEPARATOR) == -1)
	{
		return str;
	}
	TCHAR lpszOld[4];
	_stprintf(lpszOld, _T("%c%c"), STRING_SEPARATOR, STRING_SEPARATOR);
	TCHAR lpszNew[4];
	_stprintf(lpszNew, _T("%c"), STRING_SEPARATOR);

	CString szResult = str;
	if(bToEscape)
	{
		szResult.Replace(lpszNew, lpszOld);
	}
	else
	{
		szResult.Replace(lpszOld, lpszNew);
	}
	return szResult;
}
