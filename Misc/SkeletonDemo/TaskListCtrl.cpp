// TaskListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "TaskListCtrl.h"
#include <Shlwapi.h>
#include "cflwin/ShellUtils.h"

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
	{ _T(""),			SHC_INDEX_TO_STATE(SHC_UNCHECKED, SHC_NONE_SORT),		40,		HDF_LEFT	},
	{ _T("Status"),  	SHC_INDEX_TO_STATE(SHC_NONE_CHECK_BOX, SHC_NONE_SORT),	80,		HDF_LEFT	},
	{ _T("Name"),		SHC_INDEX_TO_STATE(SHC_NONE_CHECK_BOX, SHC_NONE_SORT),	170,	HDF_LEFT	},
    { _T("Format"),  	SHC_INDEX_TO_STATE(SHC_NONE_CHECK_BOX, SHC_NONE_SORT),	50,		HDF_LEFT	},
    { _T("Duration"),  	SHC_INDEX_TO_STATE(SHC_NONE_CHECK_BOX, SHC_NONE_SORT),	60,		HDF_RIGHT	},
	{ _T("Progress"),  	SHC_INDEX_TO_STATE(SHC_NONE_CHECK_BOX, SHC_NONE_SORT),	90,		HDF_LEFT	}
};

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl

CTaskListCtrl::CTaskListCtrl()
{
}

CTaskListCtrl::~CTaskListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaskListCtrl, CSListCtrl)
	//{{AFX_MSG_MAP(CTaskListCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl message handlers

void CTaskListCtrl::OnDestroy() 
{
	DeleteAllItems();
	CSListCtrl::OnDestroy();

	m_ILShell.Detach();
}

void CTaskListCtrl::Init()
{
	//Shell ImageList
	HIMAGELIST hShellImageList = cfl::ShellUtils::GetFileSysImageList(GetSafeHwnd());
	ImageList_SetBkColor(hShellImageList, CLR_NONE);
	m_ILShell.Attach(hShellImageList);

	//Task Status ImageList
	m_ILTaskStatus.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 1);
	{
		CBitmap bmp;
		bmp.LoadBitmap(IDB_TASK_STATUS);
		m_ILTaskStatus.Add(&bmp, RGB(255, 0, 255));
	}

	SetRowHeight(20);

	//1. Set Extended Style
	DWORD dwExtendedStyle = GetExtendedStyle();
	dwExtendedStyle = (dwExtendedStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SetExtendedStyle(dwExtendedStyle);

	ModifyStyle(0, LVS_SHOWSELALWAYS, 0);
	
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
		
 		hditem.fmt |=  HDF_IMAGE | columns[i].nAlign;
 		hditem.iImage = columns[i].nType;
		
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

	CString szTemp;
    int iSubItem = 0;

	//check
	SetItemCheckedState(lvi.iItem, iSubItem, SHC_CHECKED);
	iSubItem++;

	//Status
	SetItemImage(lvi.iItem, iSubItem, StatusToILIndex(pTaskInfo->m_nState), &m_ILTaskStatus);
	SysUtils::GetTaskStateText(pTaskInfo->m_nState, szTemp);
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	//file name
	szTemp = pTaskInfo->m_szFileName;
	::PathStripPath(szTemp.GetBuffer(0));
	szTemp.ReleaseBuffer(-1);

	if(!szTemp.IsEmpty())
	{
		SetItemImage(lvi.iItem, iSubItem, cfl::ShellUtils::GetFileSysIconIndex(pTaskInfo->m_szFileName), &m_ILShell);
	}
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	//file format
	SetItemText(lvi.iItem, iSubItem, pTaskInfo->m_szFormat);
	iSubItem++;

	//duration
	SysUtils::FormatTime(pTaskInfo->m_nDuration, szTemp);
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	//Progress
	SetItemProgress(lvi.iItem, iSubItem, pTaskInfo->m_dProgress);
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

	
	//File Name: TODO?
	
	//Status
	if((pNewTaskInfo->mask & TIF_STATUS) && (pItemData->m_nState != pNewTaskInfo->m_nState))
	{
		pItemData->m_nState = pNewTaskInfo->m_nState;

		SetItemImage(nIndex, COL_STATUS, StatusToILIndex(pItemData->m_nState), &m_ILTaskStatus);

		SysUtils::GetTaskStateText(pItemData->m_nState, szTemp);
		SetItemText(nIndex, COL_STATUS, szTemp);

		pSubItems[nSubItemCount++] = COL_STATUS;
	}

	//Progress
	if((pNewTaskInfo->mask & TIF_PROGRESS) && !SysUtils::Equals(pItemData->m_dProgress, pNewTaskInfo->m_dProgress))
	{
		pItemData->m_dProgress = pNewTaskInfo->m_dProgress;

		SetItemProgress(nIndex, COL_PROGRESS, pItemData->m_dProgress);

		FormatProgress(pItemData->m_dProgress, szTemp);
		SetItemText(nIndex, COL_PROGRESS, szTemp);

		pSubItems[nSubItemCount++] = COL_PROGRESS;
	}

	//invalidate the changed columns
	InvalidateSubItems(nIndex, pSubItems, nSubItemCount);
}

BOOL CTaskListCtrl::DeleteItem(int nItem)
{
	CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(nItem);
	ASSERT(pTaskInfo != NULL);
	
	if(pTaskInfo)
		delete pTaskInfo;
	
	return CSListCtrl::DeleteItem(nItem);
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
	
	return CSListCtrl::DeleteAllItems();
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
	//Treat nTaskID as row Index now
	if(nTaskID < 0 || nTaskID >= GetItemCount())
	{
		return FALSE;
	}

	int nIndex = nTaskID;
	if(pIndex)
	{
		*pIndex = nIndex;
	}
	
	CTaskInfo* ptr = (CTaskInfo*)GetItemData(nIndex);
	ASSERT(ptr != NULL);

	if(ppTaskInfo)
	{
		*ppTaskInfo = ptr;
	}
	
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
	else if(dPercent > 1.0f) 
	{
		dPercent = 1.0f;
	}

	dPercent *= 100;
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

bool CTaskListCtrl::GetMetaInfo(int nTaskID, int nMetaID, std::string& val)
{
	CTaskInfo* pTaskInfo = NULL;
	if(GetTaskInfo(nTaskID, NULL, &pTaskInfo))
	{
		return pTaskInfo->m_pMetaMap->Get(nMetaID, val);
	}
	return false;
}

int CTaskListCtrl::StatusToILIndex(int nStatus)
{
	return nStatus + 1;
}
