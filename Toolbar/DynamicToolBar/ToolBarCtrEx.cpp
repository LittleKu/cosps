// ToolBarCtrEx.cpp : implementation file
//

#include "stdafx.h"
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
	m_normalIL.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CToolBarCtrEx, CToolBarCtrl)
	//{{AFX_MSG_MAP(CToolBarCtrEx)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(TB_AUTOSIZE, OnAutoSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarCtrEx message handlers
BOOL CToolBarCtrEx::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, COLORREF crMask, UINT nFirstButtonID, UINT nLastButtonID)
{
	BOOL bRet = CToolBarCtrl::Create(dwStyle, rect, pParentWnd, nID);
	
	CBitmap tbBitmap;
	tbBitmap.LoadBitmap(nID);

 	BITMAP bmpInfo;
	tbBitmap.GetBitmap(&bmpInfo);

	CSize bitmapSize(bmpInfo.bmHeight, bmpInfo.bmHeight);
	SetBitmapSize(bitmapSize);

	CSize buttonSize(bmpInfo.bmHeight + 7, bmpInfo.bmHeight + 7);
	SetButtonSize(buttonSize);

	int m_nButtonCount = nLastButtonID - nFirstButtonID + 1;
	
	m_normalIL.Create(bmpInfo.bmHeight, bmpInfo.bmHeight, ILC_COLOR24|ILC_MASK, 1, 1);
	m_normalIL.Add(&tbBitmap, crMask);
	SetImageList(&m_normalIL);
	tbBitmap.DeleteObject();

	//For less than 256 color bitmap
//	VERIFY(AddBitmap(m_nButtonCount,nID) != -1);
	
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
	
//	AutoAdjust();

	return bRet;
}

void CToolBarCtrEx::AutoAdjust()
{
	AutoSize();

	CRect toolbarRect;
	GetClientRect(&toolbarRect);
	
	CWnd* pParentWnd = GetParent();
	ASSERT(pParentWnd);
	CRect parentRect;
	pParentWnd->GetClientRect(&parentRect);
	//Increase the dlg height for toolbar's space
	pParentWnd->SetWindowPos(&wndTop, 0, 0, parentRect.Width(), parentRect.Height() + toolbarRect.Height(), 
		SWP_NOZORDER | SWP_NOMOVE); 
	
	//Move all the children window down for "toolbarRect.Height()"
	CRect  rcChild;
	CWnd* pwndChild = pParentWnd->GetWindow(GW_CHILD);
	for( ; pwndChild != NULL; pwndChild = pwndChild->GetNextWindow())
	{
		if(pwndChild == this)
		{
			continue;
		}
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(0, toolbarRect.Height());
		pwndChild->MoveWindow(rcChild, TRUE);
	}
}

BOOL CToolBarCtrEx::CreateFromStatic(UINT nID, CWnd* pParent)
{

	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();

// 	BOOL bResult = Create(/*WS_BORDER | */WS_VISIBLE | WS_CHILD
// 		| CCS_TOP | CCS_NODIVIDER /*| CCS_NOHILITE*/ | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT  | TBSTYLE_LIST,
// 		rc,pParent, IDB_TOOLBAR, RGB(255, 0, 255), IDM_TEST, IDM_TEST7);
	AutoSize();
// 	CRect rect;
// 	GetClientRect(&rect);
// 	::SetWindowPos(m_hWnd, NULL, 10, 0, rect.Width(), rect.Height() - 15, 
// 		SWP_NOZORDER); 
//	return bResult;
	return TRUE;
}

BOOL CToolBarCtrEx::OnEraseBkgnd(CDC* pDC) 
{
	CBrush brush(RGB(255, 128, 0));

	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, &brush);

	return TRUE;
}

LRESULT CToolBarCtrEx::OnAutoSize(WPARAM wParam, LPARAM lParam)
{
	AfxTrace("OnAutoSize\n");
	LRESULT lr = 0;
//	lr = CToolBarCtrl::DefWindowProc(TB_AUTOSIZE, 0, 0);

	CRect rect;
	GetClientRect(&rect);
	
	BOOL ret = ::SetWindowPos(m_hWnd, NULL, 100, 0, rect.Width(), rect.Height() - 15, 
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOCOPYBITS); 
	if(!ret)
	{
		AfxTrace("%d\n", GetLastError());
	}
	AfxTrace("%d\n", GetLastError());
	return lr;
}
