// TaskListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl
struct ColumnInfo
{
    LPCTSTR     name;
    int         lengthExtension;
    int         fmt;
};

static ColumnInfo columns[] =
{
    { _T("URL"),  				160 },
    { _T("File Name"),  		25  },
    { _T("File Size"),			25, LVCFMT_RIGHT },
    { _T("Progress"),			50},
	{ _T("Status"),				50},
	{ _T("Speed"),				50},
	{ _T("Index"),				10}
};

CTaskListCtrl::CTaskListCtrl()
{
}

CTaskListCtrl::~CTaskListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaskListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CTaskListCtrl)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl message handlers

void CTaskListCtrl::Init()
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
        InsertColumn(i, &lvc);
    }
}

int CTaskListCtrl::AddRow(const CTaskInfo &taskInfo)
{
	int nRes = -1;
	
    LV_ITEM lvi;	
    lvi.mask = LVIF_TEXT | LVIF_PARAM;
    lvi.iItem = GetItemCount();
    lvi.lParam = (LPARAM)&taskInfo;
	
    lvi.iSubItem = 0;
    lvi.pszText = (LPTSTR)(LPCTSTR)taskInfo.m_url;
    nRes = InsertItem(&lvi);
	
    int iSubItem = 0;
    SetItemText(lvi.iItem, ++iSubItem, taskInfo.m_fileName);
	
	SetItemText(lvi.iItem, ++iSubItem, taskInfo.m_fileSize);
    SetItemText(lvi.iItem, ++iSubItem, taskInfo.m_progress);

	SetItemText(lvi.iItem, ++iSubItem, "Ready");

	SetItemText(lvi.iItem, ++iSubItem, "");

	char buf[10];
	sprintf(buf, "%d", nRes);
	SetItemText(lvi.iItem, ++iSubItem, buf);
	
	return nRes;
}

BOOL CTaskListCtrl::GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect)
{	
	BOOL bRC = CListCtrl::GetSubItemRect(nItem, nSubItem, nArea, rect);
	
	// if nSubItem == 0, the rect returned by CListCtrl::GetSubItemRect
	// is the entire row, so use left edge of second subitem
	
	if (nSubItem == 0)
	{
		if (GetHeaderCtrl()->GetItemCount() > 1)
		{
			CRect rect1;
			bRC = GetSubItemRect(nItem, 1, LVIR_BOUNDS, rect1);
			rect.right = rect1.left;
		}
	}
	
	//+++
	if (nSubItem == 0)
	{
		if (GetHeaderCtrl()->GetItemCount() > 1)
		{
			CRect rect1;
			// in case 2nd col width = 0
			for (int i = 1; i < GetHeaderCtrl()->GetItemCount(); i++)
			{
				bRC = GetSubItemRect(nItem, i, LVIR_BOUNDS, rect1);
				if (rect1.Width() > 0)
				{
					rect.right = rect1.left;
					break;
				}
			}
		}
	}
	
	return bRC;
}

void CTaskListCtrl::InvalidateSubItem(int nItem, int nSubItem)
{
	
	CRect rect;
	if (nSubItem == -1)
	{
		GetItemRect(nItem, &rect, LVIR_BOUNDS);
	}
	else
	{
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	}
	
	rect.InflateRect(2, 2);
	
	InvalidateRect(&rect);
}

void CTaskListCtrl::RemoveSelectedItems()
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
	
	SetRedraw(TRUE);
	UpdateWindow();
}

void CTaskListCtrl::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(pNMListView->iItem);
	ASSERT(pTaskInfo != NULL);

	delete pTaskInfo;
	pTaskInfo = NULL;

//	SetItemData(pNMListView->iItem, 0);

	*pResult = 0;
}