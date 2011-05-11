#include "stdafx.h"
#include "ToolBarCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT UM_TOOLBARCTRLX_REFRESH = ::RegisterWindowMessage(_T("TOOLBARCTRLX_REFRESH_MESSAGE"));

CToolBarCtrlEx::CToolBarCtrlEx()
{
	m_eLabelType = LabelsRight;
	m_hReBarWnd = NULL;
}

CToolBarCtrlEx::~CToolBarCtrlEx()
{
	m_msgReceivers.RemoveAll();
}

IMPLEMENT_DYNAMIC(CToolBarCtrlEx, CToolBarCtrl)

BEGIN_MESSAGE_MAP(CToolBarCtrlEx, CToolBarCtrl)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CToolBarCtrlEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if(!CToolBarCtrl::Create(dwStyle, rect, pParentWnd, nID))
	{
		return FALSE;
	}
	ASSERT(pParentWnd != NULL && pParentWnd->m_hWnd != NULL);
	AfxTrace("Add Msg Receiver: %x\n", pParentWnd->m_hWnd);
	m_msgReceivers.Add(pParentWnd->m_hWnd);
	return TRUE;
}
EToolbarLabelType CToolBarCtrlEx::GetLabelType() const
{
	return m_eLabelType;
}
void CToolBarCtrlEx::SetParentReBarWnd(HWND hReBarWnd)
{
	m_hReBarWnd = hReBarWnd;
}
void CToolBarCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CToolBarCtrl::OnSize(nType, cx, cy);
}
BOOL CToolBarCtrlEx::GetMaxSize(LPSIZE pSize) const
{
	BOOL bResult = CToolBarCtrl::GetMaxSize(pSize);

	//Get the size width by ourseleves
	{
		int iWidth = 0;
		int iButtons = GetButtonCount();
		for (int i = 0; i < iButtons; i++)
		{
			CRect rcButton;
			if (GetItemRect(i, &rcButton))
				iWidth += rcButton.Width();
		}
		pSize->cx = iWidth;
	}
	
	return bResult;
}


void CToolBarCtrlEx::UpdateReBarIdealSize()
{
	if(m_hReBarWnd == NULL || ::IsWindow(m_hReBarWnd))
	{
		return;
	}
	// let the rebar know what's our new current ideal size, so the chevron is handled correctly..
	CSize sizeBar;
	GetMaxSize(&sizeBar);
	ASSERT( sizeBar.cx != 0 && sizeBar.cy != 0 );
	
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_IDEALSIZE;
	rbbi.cxIdeal = sizeBar.cx;
	VERIFY( ::SendMessage(m_hReBarWnd, RB_SETBANDINFO, REBAR_BAND_INDEX0, (LPARAM)&rbbi) );
}

void CToolBarCtrlEx::ChangeTextLabelStyle(EToolbarLabelType eLabelType, bool bRefresh, bool bForceUpdateButtons)
{
	
	if(m_eLabelType == eLabelType && !bForceUpdateButtons)
	{
		return;
	}
	switch (eLabelType)
	{
	case NoLabels:
		SetStyle(GetStyle() & ~TBSTYLE_LIST);
		SetMaxTextRows(0);
		break;
	case LabelsBelow:
		SetStyle(GetStyle() & ~TBSTYLE_LIST);

		SetMaxTextRows(1);
		break;
	case LabelsRight:
		SetStyle(GetStyle() | TBSTYLE_LIST);
		SetMaxTextRows(1);
		break;
	}
	m_eLabelType = eLabelType;	
	
	if (bRefresh)
		Refresh();
}

void CToolBarCtrlEx::Refresh()
{
	// Causes a toolbar to be resized.
	AutoSize();

	//Send message to all the receivers.
	int nMsgReceiverCount = m_msgReceivers.GetSize();
	LRESULT lr = 0;
	for(int i = 0; i < nMsgReceiverCount; i++)
	{
		AfxTrace("Send Msg to: %x\n", m_msgReceivers.GetAt(i));
		lr = ::SendMessage(m_msgReceivers.GetAt(i), UM_TOOLBARCTRLX_REFRESH, 0, 0);
	}
}

void CToolBarCtrlEx::SetMaxTextRows(int iMaxRows)
{
	// Force a recalc of the toolbar's layout to work around a comctl bug
	if(iMaxRows != GetMaxTextRows())
	{
		CToolBarCtrl::SetMaxTextRows(iMaxRows);
	}
	else
	{
		SetRedraw(FALSE);
		CToolBarCtrl::SetMaxTextRows(iMaxRows + 1);	
		CToolBarCtrl::SetMaxTextRows(iMaxRows);
		SetRedraw(TRUE);
	}
}

void CToolBarCtrlEx::UpdateBackground(HBITMAP hbmp)
{
	if(m_hReBarWnd == NULL || !::IsWindow(m_hReBarWnd))
	{
		return;
	}

	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_STYLE;

	//First get the band info of the rebar
	if (::SendMessage(m_hReBarWnd, RB_GETBANDINFO, 0, (LPARAM)&rbbi))
	{
		rbbi.fMask = RBBIM_STYLE | RBBIM_BACKGROUND;
		rbbi.hbmBack = hbmp;

		if(hbmp)
		{
			rbbi.fStyle |= RBBS_FIXEDBMP;
		}
		else
		{
			rbbi.fStyle &= ~RBBS_FIXEDBMP;
		}

		//Set the band info
		::SendMessage(m_hReBarWnd, RB_SETBANDINFO, 0, (LPARAM)&rbbi);
	}
}
