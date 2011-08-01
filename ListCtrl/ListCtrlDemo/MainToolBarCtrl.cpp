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
	COLORREF crMask = RGB(255, 0, 255);
	CBitmap tbBitmap;
	tbBitmap.LoadBitmap(IDB_MAIN_TOOLBAR_HOT);
	
	BITMAP bmpInfo;
	tbBitmap.GetBitmap(&bmpInfo);
	
	CSize bitmapSize(bmpInfo.bmHeight - 2, bmpInfo.bmHeight);
	SetBitmapSize(bitmapSize);
	
	CSize buttonSize(bmpInfo.bmHeight + 7, bmpInfo.bmHeight + 7);
	SetButtonSize(buttonSize);
	
	m_normalIL.Create(bmpInfo.bmHeight - 2, bmpInfo.bmHeight, ILC_COLOR24|ILC_MASK, 1, 1);
	m_normalIL.Add(&tbBitmap, crMask);
	SetImageList(&m_normalIL);
	tbBitmap.DeleteObject();
	
	int nFirstButtonID = IDC_MAIN_TOOLBAR_BUTTON_FIRST;
	int m_nButtonCount = IDC_MAIN_TOOLBAR_BUTTON_LAST - nFirstButtonID + 1;
	
	int nBitmapIndex[] = {0, 18, 4, 6, 1, 11, 0, 0};
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