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

struct ColumnInfo
{
    LPCTSTR     name;
    SORT_TYPE   sortType;
    SORT_STATE  sortState;
    int         lengthExtension;
    int         fmt;
};

static ColumnInfo columns[] =
{
    { _T("File Name"),  				TYPE_TEXT,    DESCENDING,   40 },
    { _T("Extension"),  				TYPE_TEXT,    DESCENDING,   0  },
    { _T("Path"),       				TYPE_TEXT,    DESCENDING,   25 },
    { _T("Lines"),						TYPE_NUMERIC, DESCENDING,   15, LVCFMT_RIGHT },
    { _T("Code"),						TYPE_NUMERIC, DESCENDING,  -10, LVCFMT_RIGHT },
    { _T("Comment"),					TYPE_NUMERIC, DESCENDING,  -10, LVCFMT_RIGHT },
    { _T("Mixed"),						TYPE_NUMERIC, DESCENDING,  -75, LVCFMT_RIGHT },
    { _T("Blank"),      				TYPE_NUMERIC, DESCENDING,   15, LVCFMT_RIGHT }
};

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

    LV_ITEM lvi;	
    lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
    lvi.iItem = GetItemCount();
    lvi.lParam = (LPARAM)&fi;
	lvi.iImage = CommonUtils::GetIconIndex(fi.m_sFullFileName);
	
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

void CResultListCtrl::Init()
{
    SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM) 
        LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT |
		SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE));
	
	int         i;
    LVCOLUMN    lvc;
	
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	int size = sizeof(columns)/sizeof(columns[0]);
	
    for(i = 0; i < size; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = (char *)columns[i].name;
        lvc.cx = GetStringWidth(lvc.pszText) + 
            columns[i].lengthExtension + 15;
        if (lvc.cx < 40) lvc.cx = 40;
        lvc.fmt = columns[i].fmt;
        InsertColumn(i, columns[i].sortType, columns[i].sortState, 
            &lvc);
    }
	
	SortColumn( 2, false );
	SortColumn( 1, true );
	SortColumn( 0, true );
	
	SetUniqueName( "ResultsListCtrl" );
	LoadColumnWidths();
    LoadColumnOrder();
    LoadColumnSort();
}

void CResultListCtrl::ExportAsExcel(CDatabase& database)
{
	int i, j;
	CString sSQL;

	//0. Create Table
	int size = sizeof(columns)/sizeof(columns[0]);	
	sSQL = "CREATE TABLE StatResult (";
    for(i = 0; i < size; i++)
	{
		sSQL += QMColumnName(columns[i].name);
		sSQL += " ";
		if(columns[i].sortType == TYPE_TEXT)
		{
			sSQL += "TEXT";
		}
		else if(columns[i].sortType == TYPE_NUMERIC)
		{
			sSQL += "NUMBER";
		}

		if(i < (size - 1))
		{
			sSQL += ", ";
		}
	}
	sSQL += ")";
	database.ExecuteSQL(sSQL);
	
	//1. Insert values to TABLE
	CString sPrefix = "INSERT INTO StatResult (";
	for(i = 0; i < size; i++)
	{
		sPrefix += QMColumnName(columns[i].name);
		if(i < (size - 1))
		{
			sPrefix += ", ";
		}
	}
	sPrefix += ") VALUES (";

	int nRowCount = GetItemCount();
	for(i = 0; i < nRowCount; i++)
	{
		sSQL = sPrefix;
		for(j = 0; j < size; j++)
		{
			if(columns[j].sortType == TYPE_TEXT)
			{
				sSQL += "\'";
				sSQL += GetItemText(i, j);
				sSQL += "\'";
			}
			else if(columns[j].sortType == TYPE_NUMERIC)
			{
				sSQL += GetItemText(i, j);
			}
			if(j < (size - 1))
			{
				sSQL += ", ";
			}
		}
		sSQL += ")";
		database.ExecuteSQL(sSQL);
	}
}

CString CResultListCtrl::QMColumnName(LPCTSTR lpStr)
{
	CString str;
	if(strchr(lpStr, ' '))
	{
		str.Format("`%s`", lpStr);
	}
	else
	{
		str = lpStr;
	}
	return str;
}
