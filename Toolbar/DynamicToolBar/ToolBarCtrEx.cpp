// ToolBarCtrEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarCtrEx.h"
//#include "DynamicToolBarDlg.h"

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
	ON_NOTIFY_REFLECT_EX(TBN_ENDADJUST, OnTbnEndAdjust)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CToolBarCtrEx::GetMaxSize(LPSIZE pSize) const
{
	BOOL bResult = CToolBarCtrl::GetMaxSize(pSize);
	int iWidth = 0;
	int iButtons = GetButtonCount();
	for (int i = 0; i < iButtons; i++)
	{
		CRect rcButton;
		if (GetItemRect(i, &rcButton))
		{
			iWidth += rcButton.Width();
		}
			
	}
	pSize->cx = iWidth;
	
	return bResult;
}
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

// 	m_hotIL.Create(bmpInfo.bmHeight, bmpInfo.bmHeight, ILC_COLOR24|ILC_MASK, 1, 1);
// 	m_hotIL.Add(&tbBitmap, crMask);
// 	SetHotImageList(&m_hotIL);
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

	return bRet;
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

BOOL CToolBarCtrEx::OnTbnEndAdjust(NMHDR*, LRESULT* pResult)
{
	CSize sizeBar;
	GetMaxSize(&sizeBar);
	ASSERT( sizeBar.cx != 0 && sizeBar.cy != 0 );
	
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_IDEALSIZE;
	rbbi.cxIdeal = sizeBar.cx;
	CWnd* pParent = this->GetParent();
//	CDynamicToolBarDlg* pDlg = (CDynamicToolBarDlg*)AfxGetApp()->GetMainWnd();
// 	VERIFY( pDlg->m_ctlMainTopReBar.SetBandInfo(0, &rbbi) );
	*pResult = 0; // return value is ignored

	return FALSE;
}

void CToolBarCtrEx::OnSize(UINT nType, int cx, int cy)
{
	AfxTrace("OnSize\n");
	CWnd* pParent = AfxGetApp()->GetMainWnd();
	pParent->SendMessage(48297, cx, 0);
	CToolBarCtrl::OnSize(nType, cx, cy);
	
}

BOOL CToolBarCtrEx::OnEraseBkgnd(CDC* pDC) 
{
	return CToolBarCtrl::OnEraseBkgnd(pDC);
// 	CBrush brush(RGB(255, 128, 0));
// 
// 	CRect rect;
// 	GetClientRect(&rect);
// 	pDC->FillRect(&rect, &brush);

//	return TRUE;
}

LRESULT CToolBarCtrEx::OnAutoSize(WPARAM wParam, LPARAM lParam)
{
	AfxTrace("OnAutoSize\n");
	LRESULT lr = 0;
	lr = CToolBarCtrl::DefWindowProc(TB_AUTOSIZE, 0, 0);
	return lr;
}
