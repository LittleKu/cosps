// LangGrammarDlg.cpp: implementation of the CLangGrammarDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LangGrammarDlg.h"
#include "DlgTemplate.h"

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

int CLangGrammarDlg::nStartPosX = 7;
int CLangGrammarDlg::nStartPosY = 7;
int CLangGrammarDlg::nSpaceX = 5;
int CLangGrammarDlg::nSpaceY = 7;

static const char* lpszLangName = "Name:";
static const char* lpszLineComment = "Line Comment:";
static const char* lpszEscapeString = "Escape String:";
static const char* lpszBlockCommentOn = "Block Comment On:";
static const char* lpszBlockCommentOff = "Block Comment Off:";
static const char* lpszStringOn = "String On:";
static const char* lpszStringOff = "String Off:";
static const char* lpszCharacterOn = "Character On:";
static const char* lpszCharacterOff = "Character Off:";

CLangGrammarDlg::CLangGrammarDlg()  : CDialog(), m_dlgTemplate(NULL)
{
	//{{AFX_DATA_INIT(CLangTemplateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	DWORD style = WS_CHILD | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 400, 35, _T("MS Sans Serif"), 8);

	AddProperty(lpszLangName, 2);
	
	AddSeparator();
	
	AddProperty(lpszLineComment, 2);
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

	//Remove dialog template
	if(m_dlgTemplate != NULL)
	{
		delete m_dlgTemplate;
		m_dlgTemplate = NULL;
	}
}

BEGIN_MESSAGE_MAP(CLangGrammarDlg, CDialog)
	//{{AFX_MSG_MAP(CLangTemplateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLangTemplateDlg message handlers
void CLangGrammarDlg::CreateModeless(CWnd* pParentWnd)
{
	CreateIndirect(*m_dlgTemplate, pParentWnd);
}
BOOL CLangGrammarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	ModifyStyleEx(m_hWnd, 0, WS_EX_WINDOWEDGE, SWP_DRAWFRAME);
	CreateControls();
	
	return TRUE;
}

void CLangGrammarDlg::AddProperty(LPCTSTR lpszName, int gridwidth, LPCTSTR lpszDefaultValue, int type)
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

void CLangGrammarDlg::AddSeparator()
{
	AddProperty(NULL, 0, NULL, LAYOUT_TYPE_SEPARATOR);
}

BOOL CLangGrammarDlg::CalcMaxLabelSize(CSize& szMax, int& nMaxGridWidth)
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

void CLangGrammarDlg::CreateControls()
{
	CSize szMax;
	int nMaxGridWidth = 1;
	CalcMaxLabelSize(szMax, nMaxGridWidth);

	CRect clientRect;
	GetClientRect(&clientRect);
	int nGridWidthInPixel = (clientRect.Width() - 2 * nStartPosX - (nMaxGridWidth - 1) * nSpaceX) / nMaxGridWidth;
	ASSERT(nGridWidthInPixel > 0);

	CFont* pDlgFont = GetFont();

	CRect rect;
	CRect lastRect(0, 0, 0, 0);
	int i = 0;
	int nRow = 0, nCol = 0;
	POSITION pos = m_listLayout.GetHeadPosition();
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
			m_listLabel.AddTail(pEdit);

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

void CLangGrammarDlg::CalcLabelRect(LPRECT lpRect, LPCRECT lpLastRect, int nRow, int nCol, CSize szMax, int nGridWidthInPixel)
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

void CLangGrammarDlg::CalcSepRect(LPRECT lpRect, LPCRECT lpLastRect, int nClientWidth)
{
	//X
	lpRect->left = nStartPosX;
	lpRect->right = lpRect->left + 1 + (nClientWidth - 2 * nStartPosX);
	
	//Y
	lpRect->top = lpLastRect->bottom + nSpaceY;
	lpRect->bottom = lpRect->top + 2;
}

void CLangGrammarDlg::UpdatePropValue()
{
	m_mapPropOut.RemoveAll();
	
	int nCount = m_mapLayout.GetCount();
	CString szLabelName, szLabelValue;
	for(int i = 0; i < nCount; i++)
	{
		GetDlgItemText(ID_TXT_LABEL_FIRST + i, szLabelName);
		GetDlgItemText(ID_PROP_EDIT_FIRST + i, szLabelValue);
		m_mapPropOut.SetAt(szLabelName, szLabelValue);
	}
}
