// MainToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "MainToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainToolBar

CMainToolBar::CMainToolBar()
{
}

CMainToolBar::~CMainToolBar()
{
	m_normalIL.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CMainToolBar, CToolBarCtrlEx)
	//{{AFX_MSG_MAP(CMainToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainToolBar message handlers

typedef struct ToolBarButtonData
{
	LPCTSTR lpszText;
	int iBitmap;
    int idCommand;
	UINT nBitmapID;
	COLORREF crBitmapMask;
}ToolBarButtonData;

static ToolBarButtonData tbbuttons[] = 
{
	{_T("Add Files\0\0"), 0, ID_MAIN_TBBTN_ADD_FILES, IDB_TB_ADD, RGB(255, 255, 255)},
	{_T("Start\0\0"), 1, ID_MAIN_TBBTN_START, IDB_TB_START, RGB(255, 0, 255)},
	{_T("Stop\0\0"), 2, ID_MAIN_TBBTN_STOP, IDB_TB_STOP, RGB(255, 0, 255)},
	{_T("Options\0\0"), 3, ID_MAIN_TBBTN_OPTIONS, IDB_TB_OPTIONS, RGB(255, 255, 255)}
};

void CMainToolBar::Init()
{
	COLORREF crMask = RGB(255, 255, 255);
	
	CSize bitmapSize(32, 32);
	SetBitmapSize(bitmapSize);
	
	CSize buttonSize(40, 40);
	SetButtonSize(buttonSize);
	
	int nBtnCount = sizeof(tbbuttons)/sizeof(tbbuttons[0]);
	m_normalIL.Create(32, 32, ILC_COLOR24|ILC_MASK, nBtnCount, 1);

	int i;
	for(i = 0; i < nBtnCount; i++)
	{
		CBitmap bm;
		bm.LoadBitmap(tbbuttons[i].nBitmapID);
		m_normalIL.Add(&bm, tbbuttons[i].crBitmapMask);
	}
	SetImageList(&m_normalIL);

	TBBUTTON tb;
	for (i = 0; i < nBtnCount; i++)
	{		
		VERIFY((tb.iString = AddStrings(tbbuttons[i].lpszText)) != -1);
		tb.fsState = TBSTATE_ENABLED;
		tb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
		tb.dwData = 0;
		tb.iBitmap = tbbuttons[i].iBitmap;
		tb.idCommand = tbbuttons[i].idCommand;
		
		AddButtons(1, &tb);
	}
}
