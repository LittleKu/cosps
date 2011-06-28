// SourceDirListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "SourceDirListCtrl.h"
#include "IniFileReadWrite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct SrcDirColumnInfo
{
	LPCTSTR     lpszName;
	int         nType;
	int         nWidth;
};

static SrcDirColumnInfo srcDirColumns[] =
{
    { _T(""),				CL_UNCHECKED,         50   },
    { _T("Directory"),		CL_NONE_CHECK_BOX,    400  }
};

#define SDLC_SECTION_NAME			_T("SRC_DIR_LIST")
#define SDLC_KEY_BASE_NAME_DIR		_T("Dir")
#define SDLC_KEY_BASE_NAME_CHECKED	_T("Checked")
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
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSourceDirListCtrl message handlers
void CSourceDirListCtrl::OnDestroy()
{
	SaveHistory();
	CCheckListCtrl::OnDestroy();
}
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

void CSourceDirListCtrl::Init()
{
	//1. Set Extended Style
	DWORD dwExtendedStyle = GetExtendedStyle();
	dwExtendedStyle = (dwExtendedStyle | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(dwExtendedStyle);
	
	//2. Insert Columns
    LVCOLUMN    lvc;
    lvc.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	
	int size = sizeof(srcDirColumns)/sizeof(srcDirColumns[0]);
	int i;
    for(i = 0; i < size; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = (LPTSTR)srcDirColumns[i].lpszName;
		lvc.cx = srcDirColumns[i].nWidth;
		
        InsertColumn(i, &lvc);
    }
	
	//3. Init CheckBox Header Ctrl
	HDITEM hditem;
	
	size = GetHeaderCtrl()->GetItemCount();
	for(i = 0; i < size; i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		GetHeaderCtrl()->GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		hditem.iImage = srcDirColumns[i].nType;
		GetHeaderCtrl()->SetItem(i, &hditem);
	}

	LoadHistory();
}

void CSourceDirListCtrl::AddSrcDir(LPCTSTR lpszDir, int nCheckedState)
{
	int nItem = GetItemCount();
	
	InsertItem(nItem, _T(""));
	SetItemCheckedState(nItem, 0, nCheckedState, FALSE);
	
	SetItemText(nItem, 1, lpszDir);
	
	ValidateCheck();

	//Update the preferences
	SYS_PREF()->m_szLastSelectedSrcFolder = lpszDir;
}

void CSourceDirListCtrl::LoadHistory()
{
	if(!CommonUtils::IsFileExist(SYS_PREF_INI_FILE()))
	{
		return;
	}
	//Load history items	
	CString sKey;
	CString sDir;
	CString sChecked;
	int n = 0;
	do
	{
		sKey.Format(_T("%s%d"), SDLC_KEY_BASE_NAME_DIR, n);
		sDir = gtb::GetKeyValue(SYS_PREF_INI_FILE(), SDLC_SECTION_NAME, sKey);
		if (!sDir.IsEmpty())
		{
			sKey.Format(_T("%s%d"), SDLC_KEY_BASE_NAME_CHECKED, n);
			sChecked = gtb::GetKeyValue(SYS_PREF_INI_FILE(), SDLC_SECTION_NAME, sKey);
			if(sChecked.IsEmpty())
			{
				sChecked.Format("%d", CL_CHECKED);
			}
			int nCheckedState = atoi(sChecked);
			AddSrcDir(sDir, nCheckedState);
		}
		n++;
	}while(!sDir.IsEmpty());

	sKey = _T("LastSelectedSrcFolder");
	SYS_PREF()->m_szLastSelectedSrcFolder = gtb::GetKeyValue(SYS_PREF_INI_FILE(), SDLC_SECTION_NAME, sKey);
}

void CSourceDirListCtrl::SaveHistory()
{
	//Save all the items
	//Clear the old items
	::WritePrivateProfileSection(SDLC_SECTION_NAME, "", SYS_PREF_INI_FILE());
	
	CString sKey;
	CString sDir;
	CString sChecked;
	
	int i, nCheckStatus, nCount = GetItemCount();
	for(i = 0; i < nCount; i++)
	{
		sDir = GetItemText(i, 1);
		sKey.Format(_T("%s%d"), SDLC_KEY_BASE_NAME_DIR, i);
		::WritePrivateProfileString(SDLC_SECTION_NAME, sKey, sDir, SYS_PREF_INI_FILE());
		
		nCheckStatus = GetItemCheckedState(i, 0);
		sChecked.Format("%d", nCheckStatus);
		sKey.Format(_T("%s%d"), SDLC_KEY_BASE_NAME_CHECKED, i);
		::WritePrivateProfileString(SDLC_SECTION_NAME, sKey, sChecked, SYS_PREF_INI_FILE());
	}

	sKey = _T("LastSelectedSrcFolder");
	::WritePrivateProfileString(SDLC_SECTION_NAME, sKey, SYS_PREF()->m_szLastSelectedSrcFolder, SYS_PREF_INI_FILE());
}
