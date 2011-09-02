// MainToolBarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MainToolBarCtrl.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_MAIN_TOOLBAR_BUTTON_FIRST		ID_TBBTN_ADD_SOURCE_DIR
#define IDC_MAIN_TOOLBAR_BUTTON_LAST		ID_TBBTN_EXIT

/////////////////////////////////////////////////////////////////////////////
// CMainToolBarCtrl

CMainToolBarCtrl::CMainToolBarCtrl()
{
}

CMainToolBarCtrl::~CMainToolBarCtrl()
{
	m_normalIL.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CMainToolBarCtrl, CToolBarCtrlEx)
	//{{AFX_MSG_MAP(CMainToolBarCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainToolBarCtrl message handlers
void CMainToolBarCtrl::Init()
{
	COLORREF crMask = RGB(255, 255, 255);
	
	CSize bitmapSize(32, 32);
	SetBitmapSize(bitmapSize);
	
	CSize buttonSize(40, 40);
	SetButtonSize(buttonSize);
	
	m_normalIL.Create(32, 32, ILC_COLOR24|ILC_MASK, 6, 1);
	UINT nBtnID[] = {IDB_TB_ADD, IDB_TB_START, IDB_TB_CLEAR, IDB_TB_EXPORT, IDB_TB_OPTION, IDB_TB_EXIT};
	int i;
	for(i = 0; i < (sizeof(nBtnID)/sizeof(nBtnID[0])); i++)
	{
		CBitmap bm;
		bm.LoadBitmap(nBtnID[i]);
		m_normalIL.Add(&bm, crMask);
	}
	SetImageList(&m_normalIL);
	
	int nFirstButtonID = IDC_MAIN_TOOLBAR_BUTTON_FIRST;
	int m_nButtonCount = IDC_MAIN_TOOLBAR_BUTTON_LAST - nFirstButtonID + 1;
	
	int nBitmapIndex[] = {0, 1, 2, 3, 4, 5};
	TBBUTTON tb;
	for (int nIndex = 0; nIndex < m_nButtonCount; nIndex++)
	{
		CString string;
		string.LoadString(nIndex + nFirstButtonID);
		
		// Add second '\0'
		int nStringLength = string.GetLength() + 1;
		TCHAR * pString = string.GetBufferSetLength(nStringLength);
		pString[nStringLength] = 0;
		
		VERIFY((tb.iString = AddStrings(pString)) != -1);
		
		string.ReleaseBuffer();
		
		tb.fsState = TBSTATE_ENABLED;
		tb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
		tb.dwData = 0;
		tb.iBitmap = nBitmapIndex[nIndex];
		tb.idCommand = nIndex + nFirstButtonID;
		
		AddButtons(1, &tb);
	}
}