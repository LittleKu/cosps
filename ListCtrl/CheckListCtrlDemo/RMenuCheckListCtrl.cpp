// RMenuCheckListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RMenuCheckListCtrl.h"
#include "MenuCmds.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRMenuCheckListCtrl

CRMenuCheckListCtrl::CRMenuCheckListCtrl()
{
	BCMenu::SetMenuDrawMode(BCMENU_DRAWMODE_XP);
}

CRMenuCheckListCtrl::~CRMenuCheckListCtrl()
{
}


BEGIN_MESSAGE_MAP(CRMenuCheckListCtrl, CCheckListCtrl)
	//{{AFX_MSG_MAP(CRMenuCheckListCtrl)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRMenuCheckListCtrl message handlers

void CRMenuCheckListCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_popupMenu.GetSafeHmenu())
	{
		m_popupMenu.DestroyMenu();
	}
 	m_popupMenu.CreatePopupMenu();
// 	
// 	m_popupMenu.AppendMenu(MF_STRING,ID_CXIMAGE_MIRROR, "Check");
// 	m_popupMenu.AppendMenu(MF_STRING,ID_CXIMAGE_FLIP, "UnCheck");

	CString sStr;
	for(int i = ID_CXIMAGE_MIRROR; i <= ID_CXIMAGE_DILATE; i++)
	{
		sStr.Format("Menu %d", i);
		m_popupMenu.AppendMenu(MF_STRING, i, sStr);
	}

	m_popupMenu.LoadToolbar(IDR_TOOLBAR1);

	m_popupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_LEFTBUTTON, point.x, point.y, this);
}
