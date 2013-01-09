// TaskListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "TaskListCtrl.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct ColumnInfo
{
	TCHAR		lpszName[32];
	int         nType;
	int         nWidth;
	int			nAlign;
};

static ColumnInfo columns[] =
{
	{ _T("Name"),		0,	170,	HDF_LEFT	},
    { _T("Format"),  	0,	70,		HDF_LEFT	},
    { _T("Duration"),  	0,	80,		HDF_RIGHT	},
	{ _T("Status"),  	0,	80,		HDF_LEFT	},
	{ _T("Progress"),  	0,	90,		HDF_LEFT	}
};

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl

CTaskListCtrl::CTaskListCtrl()
{
}

CTaskListCtrl::~CTaskListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaskListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CTaskListCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl message handlers

void CTaskListCtrl::OnDestroy() 
{
	DeleteAllItems();
	CListCtrl::OnDestroy();
}

void CTaskListCtrl::Init()
{
	//1. Set Extended Style
	DWORD dwExtendedStyle = GetExtendedStyle();
	dwExtendedStyle = (dwExtendedStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SetExtendedStyle(dwExtendedStyle);
	
	//2. Insert Columns
    LVCOLUMN    lvc;
    lvc.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	
	int i, size = sizeof(columns)/sizeof(columns[0]);
    for(i = 0; i < size; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = columns[i].lpszName;
		lvc.cx = columns[i].nWidth;
		
        InsertColumn(i, &lvc);
    }
	
	//3. Init Header Ctrl
	HDITEM hditem;
	for(i = 0; i < size; i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		GetHeaderCtrl()->GetItem(i, &hditem);
		
		hditem.fmt |= columns[i].nAlign;
// 		hditem.fmt |=  HDF_IMAGE | columns[i].nAlign;
// 		hditem.iImage = columns[i].nType;
		
		GetHeaderCtrl()->SetItem(i, &hditem);
	}
}

int CTaskListCtrl::AddRow(CTaskInfo *pTaskInfo)
{
	int nResult = -1;
	
    LV_ITEM lvi;	
    lvi.mask = LVIF_PARAM;
	lvi.iSubItem = 0;
    lvi.iItem = GetItemCount();
    lvi.lParam = (LPARAM)pTaskInfo;
	
    nResult = InsertItem(&lvi);
	if(nResult < 0)
	{
		return nResult;
	}

	//update row: TODO 
	CString szTemp;
    int iSubItem = 0;

	//file name
	szTemp = pTaskInfo->m_szFileName;
	::PathStripPath(szTemp.GetBuffer(0));
	szTemp.ReleaseBuffer(-1);

	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	//file format : TODO
	//SetItemText(lvi.iItem, iSubItem, pTaskInfo->m_szFormat);
	iSubItem++;

	//duration : TODO
	//szTemp.Format(_T("%d ms"), pTaskInfo->m_nDuration);
	//SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	//Status
	SysUtils::GetTaskStateText(pTaskInfo->m_nState, szTemp);
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	//Progress
	FormatProgress(pTaskInfo->m_dProgress, szTemp);
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	return nResult;
}

void CTaskListCtrl::UpdateRow(int nIndex, CTaskInfo* pNewTaskInfo)
{
	ASSERT(pNewTaskInfo != NULL);
	ASSERT(nIndex >= 0 && nIndex < GetItemCount());
	
	CTaskInfo* pItemData = (CTaskInfo*)GetItemData(nIndex);
	ASSERT(pItemData != NULL);

	int pSubItems[COL_COUNT];
	int nSubItemCount = 0;
	CString szTemp;
	
	//File Name
	if((pNewTaskInfo->mask & TIF_FILE_NAME) && pItemData->m_szFileName.Compare(pNewTaskInfo->m_szFileName) != 0)
	{
		pItemData->m_szFileName = pNewTaskInfo->m_szFileName;
		SetItemText(nIndex, COL_FILE_NAME, pItemData->m_szFileName);
		
		pSubItems[nSubItemCount++] = COL_FILE_NAME;
	}
	
	//Status
	if((pNewTaskInfo->mask & TIF_STATUS) && (pItemData->m_nState != pNewTaskInfo->m_nState))
	{
		pItemData->m_nState = pNewTaskInfo->m_nState;

		SysUtils::GetTaskStateText(pItemData->m_nState, szTemp);
		SetItemText(nIndex, COL_STATUS, szTemp);

		pSubItems[nSubItemCount++] = COL_STATUS;
	}

	//Progress
	if((pNewTaskInfo->mask & TIF_PROGRESS) && !SysUtils::Equals(pItemData->m_dProgress, pNewTaskInfo->m_dProgress))
	{
		pItemData->m_dProgress = pNewTaskInfo->m_dProgress;
		FormatProgress(pItemData->m_dProgress, szTemp);
		SetItemText(nIndex, COL_PROGRESS, szTemp);

		pSubItems[nSubItemCount++] = COL_PROGRESS;
	}

	//TODO: invalidate the changed columns
}

BOOL CTaskListCtrl::DeleteItem(int nItem)
{
	CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(nItem);
	ASSERT(pTaskInfo != NULL);
	
	if(pTaskInfo)
		delete pTaskInfo;
	
	return CListCtrl::DeleteItem(nItem);
}
BOOL CTaskListCtrl::DeleteAllItems()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(i);
		ASSERT(pTaskInfo != NULL);
		
		if(pTaskInfo)
			delete pTaskInfo;
	}
	
	return CListCtrl::DeleteAllItems();
}

int CTaskListCtrl::AddTask(LPCTSTR lpszFileName)
{
	CTaskInfo* pTaskInfo = new CTaskInfo();
	pTaskInfo->m_szFileName = lpszFileName;

	//TODO: parse file
	return AddRow(pTaskInfo);
}

BOOL CTaskListCtrl::GetTaskInfo(int nTaskID, int* pIndex, CTaskInfo** ppTaskInfo)
{
	ASSERT(pIndex != NULL && ppTaskInfo != NULL);
	
	//Treat nTaskID as row Index now
	if(nTaskID < 0 || nTaskID >= GetItemCount())
	{
		return FALSE;
	}

	int nIndex = nTaskID;
	*pIndex = nIndex;
	CTaskInfo* ptr = (CTaskInfo*)GetItemData(nIndex);
	ASSERT(ptr != NULL);
	*ppTaskInfo = ptr;
	return TRUE;

	/*
	int i, nRowCount;
	CTaskInfo* ptr = NULL;
	for(i = 0, nRowCount = GetItemCount(); i < nRowCount; i++)
	{
		ptr = (CTaskInfo*)GetItemData(i);
		ASSERT(ptr != NULL);
		
		if(ptr->m_nTaskID == nTaskID)
		{
			*pIndex = i;
			*ppTaskInfo = ptr;			
			return TRUE;
		}
	}
	
	return FALSE;
	*/
}

void CTaskListCtrl::FormatProgress(double dPercent, CString& rText)
{
	//bound check
	if(dPercent < 0)
	{
		dPercent = 0;
	}
	else if(dPercent > 100) 
	{
		dPercent = 100;
	}

	//special process for 0 and 100
	if(SysUtils::Equals(dPercent, 0))
	{
		rText = _T("0%");
	}
	else if(SysUtils::Equals(dPercent, 100))
	{
		rText = _T("100%");
	}
	else
	{
		rText.Format(_T("%.2f%c"), dPercent, _T('%'));
	}
}
