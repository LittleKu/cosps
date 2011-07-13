// LangTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LangTemplateDlg.h"
#include "DlgTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLangTemplateDlg dialog
#define ID_TXT_LABEL_FIRST		1500
#define ID_PROP_EDIT_FIRST		1700

int CLangTemplateDlg::nStartPosX = 7;
int CLangTemplateDlg::nStartPosY = 7;
int CLangTemplateDlg::nSpaceX = 5;
int CLangTemplateDlg::nSpaceY = 7;

CLangTemplateDlg::CLangTemplateDlg()  : CDialog(), m_dlgTemplate(NULL)
{
	//{{AFX_DATA_INIT(CLangTemplateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	DWORD style = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | DS_SETFONT;
	m_dlgTemplate = new CDlgTemplate(_T(""), style, 0, 0, 400, 35, _T("MS Sans Serif"), 8);

//	InitModalIndirect(*m_dlgTemplate, pParent);
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

	delete m_pOkButton;
	m_pOkButton = NULL;

	delete m_pCancelButton;
	m_pCancelButton = NULL;

	//Remove dialog template
	if(m_dlgTemplate != NULL)
	{
		delete m_dlgTemplate;
		m_dlgTemplate = NULL;
	}
}


void CLangTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLangTemplateDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLangTemplateDlg, CDialog)
	//{{AFX_MSG_MAP(CLangTemplateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLangTemplateDlg message handlers

BOOL CLangTemplateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_szTitle);
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

void CLangTemplateDlg::SetTitle(LPCTSTR lpTitle)
{
	m_szTitle = lpTitle;
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

	short btnWidth = 75;
	short btnHeight = 21;
	short btnSpace = 14;
	short btnNum = 2;
	int   btnTotalWidth = btnWidth * btnNum + btnSpace * (btnNum - 1);
	short btnFirstX = (short)((clientRect.Width() - btnTotalWidth) / btnNum);

	m_pOkButton = new CButton();

	rect.left = btnFirstX;
	rect.top  = rect.bottom + btnHeight;
	rect.right = rect.left + btnWidth;
	rect.bottom = rect.top + btnHeight;
	m_pOkButton->Create("OK", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, rect, this, IDOK);
	m_pOkButton->SetFont(pDlgFont, TRUE);

	rect.left = rect.right + btnSpace;
	rect.right = rect.left + btnWidth;

	m_pCancelButton = new CButton();
	m_pCancelButton->Create("Cancel", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, rect, this, IDCANCEL);
	m_pCancelButton->SetFont(pDlgFont, TRUE);

	int nDlgHeight = rect.bottom + btnHeight;
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

void CLangTemplateDlg::OnOK()
{
	CDialog::OnOK();
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

void CLangTemplateDlg::SetModal(BOOL bModal, CWnd* pParentWnd)
{
	if(bModal)
	{
		InitModalIndirect(*m_dlgTemplate, pParentWnd);
	}
	else
	{
		CreateIndirect(*m_dlgTemplate, pParentWnd);
	}
}

void CLangTemplateDlg::UpdatePropValue()
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
