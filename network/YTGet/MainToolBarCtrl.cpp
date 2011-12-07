// MainToolBarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MainToolBarCtrl.h"
#include "Constants.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct ToolBarButtonInfo
{
	UINT		m_nID;
	LPCTSTR		m_lpszName;
	int			m_nBitmap;
};

static ToolBarButtonInfo TB_BTN_INFOS[] =
{
    { ID_TBBTN_ADD_TASK,		_T("Add Task"),			0 },
	{ ID_TBBTN_ADD_BATCH_TASKS,	_T("Add Batch Tasks"),	1 },
	{ ID_TBBTN_START,			_T("Start"),			2 },
	{ ID_TBBTN_PAUSE,			_T("Pause"),			3 },
	{ ID_TBBTN_REMOVE,			_T("Remove"),			4 },
	{ ID_TBBTN_RESTART,			_T("Restart"),			5 }
};

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

	m_normalIL.Create(IDB_MAIN_TOOLBAR_BTN, 30, 1, RGB(255, 0, 255));
	SetImageList(&m_normalIL);

	int nButtonCount = ( sizeof(TB_BTN_INFOS) / sizeof(TB_BTN_INFOS[0]) );

	TBBUTTON tb;
	for (int nIndex = 0; nIndex < nButtonCount; nIndex++)
	{
		tb.fsState = TBSTATE_ENABLED;
		tb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
		tb.dwData = 0;
		tb.idCommand = TB_BTN_INFOS[nIndex].m_nID;
		VERIFY((tb.iString = AddStrings(TB_BTN_INFOS[nIndex].m_lpszName)) != -1);
		tb.iBitmap = TB_BTN_INFOS[nIndex].m_nBitmap;
		
		AddButtons(1, &tb);
	}
}
