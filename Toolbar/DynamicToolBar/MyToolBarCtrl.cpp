// MyToolBarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicToolBar.h"
#include "MyToolBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyToolBarCtrl

CMyToolBarCtrl::CMyToolBarCtrl()
{
}

CMyToolBarCtrl::~CMyToolBarCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyToolBarCtrl, CToolBarCtrlEx)
	//{{AFX_MSG_MAP(CMyToolBarCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolBarCtrl message handlers
void CMyToolBarCtrl::Init()
{
	COLORREF crMask = RGB(255, 0, 255);
	CBitmap tbBitmap;
	tbBitmap.LoadBitmap(IDB_TB_HOT);
	
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

	tbBitmap.LoadBitmap(IDB_TB_HOT);
	m_hotIL.Create(bmpInfo.bmHeight - 2, bmpInfo.bmHeight, ILC_COLOR24|ILC_MASK, 1, 1);
	m_hotIL.Add(&tbBitmap, crMask);
	SetHotImageList(&m_hotIL);
	tbBitmap.DeleteObject();

	tbBitmap.LoadBitmap(IDB_TB_HOT);
	m_disableIL.Create(bmpInfo.bmHeight - 2, bmpInfo.bmHeight, ILC_COLOR24|ILC_MASK, 1, 1);
	m_disableIL.Add(&tbBitmap, crMask);
	SetDisabledImageList(&m_disableIL);
	tbBitmap.DeleteObject();

	int nFirstButtonID = FISRT_TOOLBAR_BUTTON_ID;
	int m_nButtonCount = LAST_TOOLBAR_BUTTON_ID - nFirstButtonID + 1;
	
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
		tb.iBitmap = nIndex;
		tb.idCommand = nIndex + nFirstButtonID;
		if(tb.idCommand >= TBBTN_DUMMY2)
		{
			tb.fsState = 0;
		}
		
		AddButtons(1, &tb);
	}
}