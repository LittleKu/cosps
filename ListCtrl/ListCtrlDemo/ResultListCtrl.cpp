// ResultListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ResultListCtrl.h"
#include <Shellapi.h>
#include "FileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultListCtrl
#define IDM_RESULT_LIST_FIRST				48297

#define IDM_RESULT_LIST_OPEN				(IDM_RESULT_LIST_FIRST + 0)
#define IDM_RESULT_LIST_OPEN_DIR			(IDM_RESULT_LIST_FIRST + 1)

#define IDM_RESULT_LIST_LAST				IDM_RESULT_LIST_OPEN_DIR

CResultListCtrl::CResultListCtrl()
{
}

CResultListCtrl::~CResultListCtrl()
{
}


BEGIN_MESSAGE_MAP(CResultListCtrl, CMultiColumnSortListCtrl)
	//{{AFX_MSG_MAP(CResultListCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultListCtrl message handlers

void CResultListCtrl::PreSubclassWindow() 
{
	CMultiColumnSortListCtrl::PreSubclassWindow();

	LPITEMIDLIST  pidl = NULL;
	SHGetSpecialFolderLocation(GetSafeHwnd(), (UINT)CSIDL_DESKTOP, &pidl);
	
    SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	HIMAGELIST hShellImageList = (HIMAGELIST) SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidl,
		0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	ImageList_SetBkColor(hShellImageList, CLR_NONE);
	
	SendMessage(LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hShellImageList);

	IMalloc* pIface = NULL;
	SHGetMalloc(&pIface);

	if( SUCCEEDED(::SHGetMalloc(&pIface)) )
	{
		pIface->Free(pidl);
		pIface->Release();
		pIface = NULL;
	}
}

int CResultListCtrl::AddRow(const CFileInfo& fi)
{
	int nRes = -1;
	
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	UINT uFlags = SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON | SHGFI_SYSICONINDEX;
	::SHGetFileInfo(fi.m_sFullFileName, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), uFlags);

    LV_ITEM lvi;
	
    lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
    lvi.iItem = GetItemCount();
    lvi.lParam = (LPARAM)&fi;
	lvi.iImage = sfi.iIcon;
	
    lvi.iSubItem = 0;
    lvi.pszText = (LPTSTR)(LPCTSTR)fi.m_sFileName;
    nRes = InsertItem(&lvi);
	
    int iSubItem = 0;
    SetItemText(lvi.iItem, ++iSubItem, fi.m_sFileExt);
    SetItemText(lvi.iItem, ++iSubItem, fi.m_sFilePath);
	
	SetItemText(lvi.iItem, ++iSubItem, CommonUtils::ToString(fi.m_nTotalLines));
    SetItemText(lvi.iItem, ++iSubItem, CommonUtils::ToString(fi.m_nCodeLines));
    SetItemText(lvi.iItem, ++iSubItem, CommonUtils::ToString(fi.m_nCommentLines));
    SetItemText(lvi.iItem, ++iSubItem, CommonUtils::ToString(fi.GetMixedLines()));
    SetItemText(lvi.iItem, ++iSubItem, CommonUtils::ToString(fi.m_nBlankLines));
	
	return nRes;
}

void CResultListCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu ctMenu;
	ctMenu.CreatePopupMenu();
	ctMenu.AppendMenu(MF_STRING, IDM_RESULT_LIST_OPEN,		_T("Open"));
	ctMenu.AppendMenu(MF_STRING, IDM_RESULT_LIST_OPEN_DIR,	_T("Open Folder"));

	//Set menu enable status
	int nCount = GetSelectedCount();
	if(nCount != 1)
	{
		ctMenu.EnableMenuItem(IDM_RESULT_LIST_OPEN,		MF_BYCOMMAND | MF_GRAYED);
		ctMenu.EnableMenuItem(IDM_RESULT_LIST_OPEN_DIR, MF_BYCOMMAND | MF_GRAYED);
	}

	ctMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}

BOOL CResultListCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch(wParam)
	{
	case IDM_RESULT_LIST_OPEN:
		{
			OpenFile();
		}
		break;
	case IDM_RESULT_LIST_OPEN_DIR:
		{
			OpenFolder();
		}
		break;
	}
	
	return CMultiColumnSortListCtrl::OnCommand(wParam, lParam);
}

void CResultListCtrl::OpenFolder()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		return;
	}
	int nItem = GetNextSelectedItem(pos);
	CFileInfo* pFileInfo = (CFileInfo*)GetItemData(nItem);
	ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,\"") + pFileInfo->m_sFullFileName + _T("\""), NULL, SW_SHOW);
}

void CResultListCtrl::OpenFile()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		return;
	}
	int nItem = GetNextSelectedItem(pos);
	CFileInfo* pFileInfo = (CFileInfo*)GetItemData(nItem);
	HINSTANCE hInst = ::ShellExecute(NULL, _T("open"), pFileInfo->m_sFullFileName, NULL, NULL, SW_SHOW);

	//Failed to Open
	if((int)hInst < 32)
	{
		AfxTrace("Failed to open file %s\n", pFileInfo->m_sFullFileName);
	}
}

void CResultListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OpenFile();
}
