// SourceDirListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "SourceDirListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSourceDirListCtrl

CSourceDirListCtrl::CSourceDirListCtrl()
{
}

CSourceDirListCtrl::~CSourceDirListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSourceDirListCtrl, CCheckListCtrl)
	//{{AFX_MSG_MAP(CSourceDirListCtrl)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSourceDirListCtrl message handlers

void CSourceDirListCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu srcDirMenu;
	srcDirMenu.CreatePopupMenu();
	srcDirMenu.AppendMenu(MF_STRING, IDM_SOURCE_DIR_REMOVE,		_T("Remove"));
	srcDirMenu.AppendMenu(MF_STRING, IDM_SOURCE_DIR_REMOVE_ALL,	_T("Remove All"));
	srcDirMenu.AppendMenu(MF_SEPARATOR);
	srcDirMenu.AppendMenu(MF_STRING, IDM_SOURCE_DIR_SELECT_ALL,	_T("Select All\tCtrl+A"));
	srcDirMenu.AppendMenu(MF_SEPARATOR);
	srcDirMenu.AppendMenu(MF_STRING, IDM_SOURCE_DIR_CHECK,		_T("Check"));
	srcDirMenu.AppendMenu(MF_STRING, IDM_SOURCE_DIR_UNCHECK,	_T("Uncheck"));
	srcDirMenu.AppendMenu(MF_SEPARATOR);
	srcDirMenu.AppendMenu(MF_STRING, IDM_SOURCE_DIR_OPEN_DIR,	_T("Open Folder"));

	//Set menu enable status
	int nCount = GetSelectedCount();
	if(nCount <= 0)
	{
		srcDirMenu.EnableMenuItem(IDM_SOURCE_DIR_REMOVE, MF_BYCOMMAND | MF_GRAYED);
		srcDirMenu.EnableMenuItem(IDM_SOURCE_DIR_CHECK, MF_BYCOMMAND | MF_GRAYED);
		srcDirMenu.EnableMenuItem(IDM_SOURCE_DIR_UNCHECK, MF_BYCOMMAND | MF_GRAYED);
	}
	if(nCount != 1)
	{
		srcDirMenu.EnableMenuItem(IDM_SOURCE_DIR_OPEN_DIR, MF_BYCOMMAND | MF_GRAYED);
	}

	srcDirMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
}

BOOL CSourceDirListCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch(wParam)
	{
	case IDM_SOURCE_DIR_REMOVE:
		{
			RemoveSelectedItems();
		}
		break;
	case IDM_SOURCE_DIR_REMOVE_ALL:
		{
			DeleteAllItems();
			SetHeaderCheckedState(0, CL_UNCHECKED);
			m_HeaderCtrl.UpdateWindow();
		}
		break;
	case IDM_SOURCE_DIR_SELECT_ALL:
		{
			SelectAllItems();
		}
		break;
	case IDM_SOURCE_DIR_CHECK:
		{
			CheckSelectedItems(TRUE);
		}
		break;
	case IDM_SOURCE_DIR_UNCHECK:
		{
			CheckSelectedItems(FALSE);
		}
		break;
	case IDM_SOURCE_DIR_OPEN_DIR:
		{
			OpenFolder();
		}
		break;
	}
	
	return CCheckListCtrl::OnCommand(wParam, lParam);
}

void CSourceDirListCtrl::RemoveSelectedItems()
{
	SetRedraw(FALSE);
	//Delete all selected items
	POSITION pos = GetFirstSelectedItemPosition();
	
	int nItem = -1;
	while (pos != NULL)
	{
		nItem = GetNextSelectedItem(pos);

		DeleteItem(nItem);
		
		//Delete the previous one item will affect the pos
		pos = GetFirstSelectedItemPosition();
	}

	if(GetItemCount() <= 0)
	{
		SetHeaderCheckedState(0, CL_UNCHECKED);
	}

	SetRedraw(TRUE);
	UpdateWindow();
}

void CSourceDirListCtrl::SelectAllItems()
{
	SetRedraw(FALSE);
	int nCount = GetItemCount();
	int i;
	for(i = 0; i < nCount; i++)
	{
		SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
	SetRedraw(TRUE);
	UpdateWindow();
}

void CSourceDirListCtrl::CheckSelectedItems(BOOL bChecked)
{
	SetRedraw(FALSE);

	int nCheckedState = ((bChecked) ? CL_CHECKED : CL_UNCHECKED);
	POSITION pos = GetFirstSelectedItemPosition();	
	int nItem = -1;
	while (pos != NULL)
	{
		nItem = GetNextSelectedItem(pos);
		SetItemCheckedState(nItem, 0, nCheckedState, FALSE);
	}
	ValidateCheck();

	SetRedraw(TRUE);
	UpdateWindow();
}

void CSourceDirListCtrl::OpenFolder()
{
	POSITION pos = GetFirstSelectedItemPosition();	
	if(pos == NULL)
	{
		return;
	}
	int nItem = GetNextSelectedItem(pos);
	CString szFolderPath = GetItemText(nItem, 1);
	ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,\"") + szFolderPath + _T("\""), NULL, SW_SHOW);
}
