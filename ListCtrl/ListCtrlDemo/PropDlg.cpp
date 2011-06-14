// PropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "PropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropDlg dialog
#define ID_TXT_LABEL_FIRST		1500
#define ID_PROP_EDIT_FIRST		1700

CPropDlg::CPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROP_DLG, pParent)
{
	//{{AFX_DATA_INIT(CPropDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropDlg::~CPropDlg()
{
	m_mapPropInOrder.RemoveAll();
	m_mapPropIn.RemoveAll();

	CStatic* pStatic = NULL;
	POSITION pos = m_LabelList.GetHeadPosition();
	while(pos != NULL)
	{
		pStatic = (CStatic*)m_LabelList.GetNext(pos);
		delete pStatic;
	}
	m_LabelList.RemoveAll();
	
	CEdit* pEdit = NULL;
	pos = m_EditList.GetHeadPosition();
	while(pos != NULL)
	{
		pEdit = (CEdit*)m_EditList.GetNext(pos);
		delete pEdit;
	}
	m_EditList.RemoveAll();

	m_mapPropOut.RemoveAll();
}


void CPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropDlg, CDialog)
	//{{AFX_MSG_MAP(CPropDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDlg message handlers
void CPropDlg::SetTitle(LPCTSTR lpszTitle)
{
	m_lpTitle = lpszTitle;
}
void CPropDlg::AddProperty(LPCTSTR lpszName, LPCTSTR lpszDefaultValue)
{
	int nCount = m_mapPropIn.GetCount();
	CString sName = lpszName;
	m_mapPropInOrder.SetAt(nCount, sName);

	m_mapPropIn.SetAt(lpszName, lpszDefaultValue);
}

BOOL CPropDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();

	if(m_lpTitle != NULL)
	{
		SetWindowText(m_lpTitle);
	}

	CDC* pDC = GetDC();
	int i, nSize = m_mapPropInOrder.GetCount();
	CSize sz;
	CSize szMax(0, 0);
	CString szPropName;
	for(i = 0; i < nSize; i++)
	{
		m_mapPropInOrder.Lookup(i, szPropName);
		sz = pDC->GetTextExtent(szPropName);
		if(sz.cx > szMax.cx)
		{
			szMax = sz;
		}
	}
	szMax.cx += 2;
	szMax.cy += 2;
	ReleaseDC(pDC);

	int nStartPosX = 7;
	int nStartPosY = 7;
	int nSpaceX = 5;
	int nSpaceY = 7;

	CRect clientRect;
	GetClientRect(&clientRect);
	int nEditWidth = clientRect.Width() - szMax.cx - 2 * nStartPosX - nSpaceX;
	ASSERT(nEditWidth > 0);
 	
	CFont* pDlgFont = GetFont();
	CRect rect;
	CString szDefaultValue;
	for(i = 0; i < nSize; i++)
	{
		m_mapPropInOrder.Lookup(i, szPropName);

		CStatic* pStatic = new CStatic();
		rect.left = nStartPosX;
		rect.top = nStartPosY + i * (szMax.cy + nSpaceY);
		rect.right = rect.left + szMax.cx;
		rect.bottom = rect.top + szMax.cy;
		pStatic->Create(szPropName, WS_CHILD | WS_VISIBLE | SS_RIGHT, rect, this, ID_TXT_LABEL_FIRST + i);
		pStatic->SetFont(pDlgFont, TRUE);
		m_LabelList.AddTail(pStatic);

		CEdit* pEdit = new CEdit();
		rect.left = rect.right + nSpaceX;
		rect.right = rect.left + nEditWidth;	
		pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL, rect, this, ID_PROP_EDIT_FIRST + i);
		pEdit->ModifyStyleEx(0,   WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY, SWP_DRAWFRAME);
		pEdit->SetFont(pDlgFont, TRUE);
		
		m_mapPropIn.Lookup(szPropName, szDefaultValue);
		if(!szDefaultValue.IsEmpty())
		{
			pEdit->SetWindowText(szDefaultValue);
		}

		m_EditList.AddTail(pEdit);
	}

	int nButtonSpace = 10;

	MoveWindowTo(IDOK, -1, rect.bottom + nButtonSpace);
	MoveWindowTo(IDCANCEL, -1, rect.bottom + nButtonSpace);

	CWnd* pWndOK = GetDlgItem(IDOK);
	CRect okButtonRect;
	pWndOK->GetWindowRect(&okButtonRect);

	int nDlgHeight = rect.bottom + nButtonSpace + okButtonRect.Height() + nButtonSpace;
	
	CRect windowRect;
	GetWindowRect(&windowRect);
	nDlgHeight += windowRect.Height() - clientRect.Height();

	SetWindowPos(NULL, 0, 0, windowRect.Width(), nDlgHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	return TRUE;
}

void CPropDlg::OnOK() 
{
	m_mapPropOut.RemoveAll();
	int i, size = m_mapPropInOrder.GetCount();
	CString szPropValue;
	CString szPropName;
	for(i = 0; i < size; i++)
	{
		m_mapPropInOrder.Lookup(i, szPropName);

		szPropValue.Empty();
		GetDlgItemText(ID_PROP_EDIT_FIRST + i, szPropValue);

		m_mapPropOut.SetAt(szPropName, szPropValue);
	}
	
	CDialog::OnOK();
}

BOOL CPropDlg::GetProperty(LPCTSTR lpszName, CString& szValue)
{
	return m_mapPropOut.Lookup(lpszName, szValue);
}
CString CPropDlg::GetProperty(LPCTSTR lpszName)
{
	CString szValue;
	m_mapPropOut.Lookup(lpszName, szValue);
	return szValue;
}

void CPropDlg::MoveWindowTo(int nWndID, int x, int y)
{
	CWnd* pWnd = GetDlgItem(nWndID);
	ASSERT(pWnd);

	CRect wndRect;
	pWnd->GetWindowRect(&wndRect);
	ScreenToClient(&wndRect);

	int nWidth = wndRect.Width();
	int nHeight = wndRect.Height();
	if(x != -1)
	{
		wndRect.left = x;
		wndRect.right = wndRect.left + nWidth;
	}
	if(y != -1)
	{
		wndRect.top = y;
		wndRect.bottom = wndRect.top + nHeight;
	}
	pWnd->MoveWindow(&wndRect);
}
