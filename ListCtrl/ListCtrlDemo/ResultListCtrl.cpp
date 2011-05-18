// ResultListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ResultListCtrl.h"
#include <Shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultListCtrl

CResultListCtrl::CResultListCtrl()
{
}

CResultListCtrl::~CResultListCtrl()
{
}


BEGIN_MESSAGE_MAP(CResultListCtrl, CMultiColumnSortListCtrl)
	//{{AFX_MSG_MAP(CResultListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
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
	
	SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nTotalLines));
    SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nCodeLines));
    SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nCommentLines));
    SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.GetMixedLines()));
    SetItemText(lvi.iItem, ++iSubItem, fi.ToString(fi.m_nBlankLines));
	
	return nRes;
}
