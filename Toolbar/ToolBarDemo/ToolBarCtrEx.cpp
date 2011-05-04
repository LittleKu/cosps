// ToolBarCtrEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarDemo.h"
#include "ToolBarCtrEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarCtrEx

CToolBarCtrEx::CToolBarCtrEx()
{
}

CToolBarCtrEx::~CToolBarCtrEx()
{
}


BEGIN_MESSAGE_MAP(CToolBarCtrEx, CToolBarCtrl)
	//{{AFX_MSG_MAP(CToolBarCtrEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarCtrEx message handlers
BOOL CToolBarCtrEx::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT nFirstButtonID, UINT nLastButtonID)
{
	BOOL bRet = CToolBarCtrl::Create(dwStyle, rect, pParentWnd, nID);
	
	CBitmap cBitmap;
	cBitmap.LoadBitmap(nID);

 	BITMAP bmp;
	cBitmap.GetBitmap(&bmp);
	bmp.bmHeight = 24;

	CSize bitmapSize(bmp.bmHeight, bmp.bmHeight);
	SetBitmapSize(bitmapSize);

	CSize buttonSize(bmp.bmHeight + 7, bmp.bmHeight + 7);
	SetButtonSize(buttonSize);
/*	cBitmap.DeleteObject();*/

	int m_nButtonCount = nLastButtonID - nFirstButtonID + 1;
	m_nButtonCount = 9;
	
	m_normalIL.Create(24, 24, ILC_COLOR24|ILC_MASK, 1, 1);
	m_normalIL.Add(&cBitmap, RGB(255, 0, 255));
//	SetHotImageList(&m_normalIL);
	SetImageList(&m_normalIL);
	cBitmap.DeleteObject();

// 	if(AddBitmap(m_nButtonCount,nID) == -1)
// 	{
// 		AfxTrace("%d\n", GetLastError());
// 		ASSERT(FALSE);
// 	}
	
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
		
		AddButtons(1, &tb);
	}
	
	return bRet;
}