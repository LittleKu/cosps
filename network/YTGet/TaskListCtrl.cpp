// TaskListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "TaskListCtrl.h"
#include "CommonUtils.h"
#include "DownloaderMgr.h"
#include "TaskListComparator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct ColumnInfo
{
	LPCTSTR     lpszName;
	int         nType;
	int         nWidth;
	int			nAlign;
};

static ColumnInfo columns[] =
{
	{ _T("Status"),			0,   70,	HDF_LEFT	},
    { _T("File Name"),  	0,   150,	HDF_LEFT	},
    { _T("File Size"),  	0,   80,	HDF_RIGHT	},
	{ _T("Progress"),  		0,   90,	HDF_LEFT	},
	{ _T("Speed"),  		0,   80,	HDF_LEFT	},
	{ _T("Left Time"),  	0,   100,	HDF_LEFT	},
	{ _T("Elapsed Time"),  	0,   100,	HDF_LEFT	},
	{ _T("Detail"),  		0,   180,	HDF_LEFT	},
	{ _T("URL"),  			0,   100,	HDF_LEFT	}
};

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl

CTaskListCtrl::CTaskListCtrl() : m_pComparator(NULL)
{
}

CTaskListCtrl::~CTaskListCtrl()
{
	if(m_pComparator != NULL)
	{
		delete m_pComparator;
		m_pComparator = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTaskListCtrl, CSListCtrl)
	//{{AFX_MSG_MAP(CTaskListCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl message handlers

void CTaskListCtrl::Init()
{
	InitShellImageList();

	SetRowHeight(19);
	
	m_ILSortImages.Create(IDB_HEAD_SORT, 16, 1, RGB(255, 0, 255));
	GetHeaderCtrl()->SetImageList(&m_ILSortImages);
	
	m_ILTaskStatus.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 1);
	{
		CBitmap bmp;
		bmp.LoadBitmap(IDB_TASK_STATUS);
		m_ILTaskStatus.Add(&bmp, RGB(255, 0, 255));
	}
	
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
        lvc.pszText = (LPTSTR)columns[i].lpszName;
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
	int nRes = -1;
	
    LV_ITEM lvi;	
    lvi.mask = LVIF_PARAM;
	lvi.iSubItem = 0;
    lvi.iItem = GetItemCount();
    lvi.lParam = (LPARAM)pTaskInfo;

    nRes = InsertItem(&lvi);
	
	CString szTemp;
    int iSubItem = 0;

	//Status
	SetItemImage(lvi.iItem, iSubItem++, StatusToILIndex(pTaskInfo->m_dlState.GetState()), &m_ILTaskStatus);

	//File Name
	SetItemText(lvi.iItem, iSubItem, pTaskInfo->m_szFileName);
	if(!pTaskInfo->m_szFileName.IsEmpty())
	{
		SetItemImage(lvi.iItem, iSubItem, CCommonUtils::GetIconIndex(pTaskInfo->m_szFileName), m_pILShell);
	}
	iSubItem++;

	//File Size
	if(pTaskInfo->m_nFileSize > 0)
	{
		CCommonUtils::FormatFileSize(pTaskInfo->m_nFileSize, szTemp);
		SetItemText(lvi.iItem, iSubItem, szTemp);
	}
	else
	{
		SetItemText(lvi.iItem, iSubItem, "");
	}
	iSubItem++;

	//Progress
	SetItemProgress(lvi.iItem, iSubItem, pTaskInfo->m_nDlNow, pTaskInfo->m_nFileSize);
	CCommonUtils::FormatPercent(pTaskInfo->m_nDlNow, pTaskInfo->m_nFileSize, szTemp);
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;

	//Speed
	CCommonUtils::FormatSpeed(pTaskInfo->m_nCurrSpeed, szTemp);
	SetItemText(lvi.iItem, iSubItem++, szTemp);

	//Left Time
	CCommonUtils::FormatTime(pTaskInfo->m_nLeftTime, szTemp);
	SetItemText(lvi.iItem, iSubItem++, szTemp);

	//Cost Time
	CCommonUtils::FormatTime(pTaskInfo->m_nCostTime, szTemp);
	SetItemText(lvi.iItem, iSubItem++, szTemp);

	//Detail
	SetItemText(lvi.iItem, iSubItem++, pTaskInfo->m_dlState.m_szDetail);

	//URL
	SetItemText(lvi.iItem, iSubItem++, pTaskInfo->m_szUrl);
	
	return nRes;
}

void CTaskListCtrl::UpdateRow(int nIndex, CTaskInfo* pNewTaskInfo)
{
	ASSERT(pNewTaskInfo != NULL);
	ASSERT(nIndex >= 0 && nIndex < GetItemCount());

	CTaskInfo* pItemData = (CTaskInfo*)GetItemData(nIndex);
	ASSERT(pItemData != NULL);
	
	int pSubItems[CTaskInfo::COL_COUNT];
	int nSubItemCount = 0;

	//State
	if((pNewTaskInfo->mask & CTaskInfo::TIF_STATE) && (pItemData->m_dlState.m_nState != pNewTaskInfo->m_dlState.m_nState))
	{
		pItemData->m_dlState = pNewTaskInfo->m_dlState;

		//Status
		SetItemImage(nIndex, CTaskInfo::COL_STATUS, StatusToILIndex(pItemData->m_dlState.GetState()), &m_ILTaskStatus);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_STATUS;

		//Detail
		SetItemText(nIndex, CTaskInfo::COL_DETAIL, pItemData->m_dlState.m_szDetail);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_DETAIL;

		//Successfully
		if(pItemData->m_dlState.m_nState == TSE_COMPLETE)
		{
			CString szFullPath;
			szFullPath.Format("%s\\%s", SYS_OPTIONS()->m_szSaveToFolder, pItemData->m_szFileName);
			SetItemImage(nIndex, CTaskInfo::COL_FILE_NAME, CCommonUtils::GetIconIndex(szFullPath), m_pILShell);

			pSubItems[nSubItemCount++] = CTaskInfo::COL_FILE_NAME;
		}
	}

	//File Name
	if((pNewTaskInfo->mask & CTaskInfo::TIF_FILE_NAME) && pItemData->m_szFileName.Compare(pNewTaskInfo->m_szFileName) != 0)
	{
		pItemData->m_szFileName = pNewTaskInfo->m_szFileName;

		SetItemText(nIndex, CTaskInfo::COL_FILE_NAME, pItemData->m_szFileName);
		SetItemImage(nIndex, CTaskInfo::COL_FILE_NAME, CCommonUtils::GetIconIndex(pItemData->m_szFileName), m_pILShell);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_FILE_NAME;
	}

	//File Size
	if((pNewTaskInfo->mask & CTaskInfo::TIF_FILE_SIZE) && pItemData->m_nFileSize != pNewTaskInfo->m_nFileSize)
	{
		pItemData->m_nFileSize = pNewTaskInfo->m_nFileSize;

		CString szTemp;
		CCommonUtils::FormatFileSize(pItemData->m_nFileSize, szTemp);
		SetItemText(nIndex, CTaskInfo::COL_FILE_SIZE, szTemp);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_FILE_SIZE;
	}

	//Progress
	if((pNewTaskInfo->mask & CTaskInfo::TIF_DLNOW) && pItemData->m_nDlNow != pNewTaskInfo->m_nDlNow)
	{
		pItemData->m_nDlNow = pNewTaskInfo->m_nDlNow;
		
		SetItemProgress(nIndex, CTaskInfo::COL_PROGRESS, pItemData->m_nDlNow, pItemData->m_nFileSize);

		CString szTemp;
		CCommonUtils::FormatPercent(pItemData->m_nDlNow, pItemData->m_nFileSize, szTemp);
		SetItemText(nIndex, CTaskInfo::COL_PROGRESS, szTemp);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_PROGRESS;
	}

	//Speed
	if((pNewTaskInfo->mask & CTaskInfo::TIF_CURR_SPEED) && pItemData->m_nCurrSpeed != pNewTaskInfo->m_nCurrSpeed)
	{
		pItemData->m_nCurrSpeed = pNewTaskInfo->m_nCurrSpeed;
		
		CString szTemp;
		CCommonUtils::FormatSpeed(pItemData->m_nCurrSpeed, szTemp);
		SetItemText(nIndex, CTaskInfo::COL_SPEED, szTemp);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_SPEED;
	}

	//Left Time
	if((pNewTaskInfo->mask & CTaskInfo::TIF_LEFT_TIME) && pItemData->m_nLeftTime != pNewTaskInfo->m_nLeftTime)
	{
		pItemData->m_nLeftTime = pNewTaskInfo->m_nLeftTime;
		
		CString szTemp;
		CCommonUtils::FormatTime(pItemData->m_nLeftTime, szTemp);
		SetItemText(nIndex, CTaskInfo::COL_LEFT_TIME, szTemp);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_LEFT_TIME;
	}

	//Cost Time
	if((pNewTaskInfo->mask & CTaskInfo::TIF_COST_TIME) && pItemData->m_nCostTime != pNewTaskInfo->m_nCostTime)
	{
		pItemData->m_nCostTime = pNewTaskInfo->m_nCostTime;
		
		CString szTemp;
		CCommonUtils::FormatTime(pItemData->m_nCostTime, szTemp);
		SetItemText(nIndex, CTaskInfo::COL_COST_TIME, szTemp);

		pSubItems[nSubItemCount++] = CTaskInfo::COL_COST_TIME;
	}

	InvalidateSubItems(nIndex, pSubItems, nSubItemCount);
}

void CTaskListCtrl::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(pNMListView->iItem);
	ASSERT(pTaskInfo != NULL);
	
	delete pTaskInfo;
	pTaskInfo = NULL;
	
	*pResult = 0;
}

int CTaskListCtrl::StatusToILIndex(int nStatus)
{
	int nIndex = 0;
	switch(nStatus)
	{
	case TSE_READY:
		{
			nIndex = 4;
		}
		break;
	case TSE_TRANSFERRING:
		{
			nIndex = 1;
		}
		break;
	case TSE_COMPLETE:
		{
			nIndex = 2;
		}
		break;
	case TSE_END_WITH_ERROR:
		{
			nIndex = 5;
		}
		break;
	case TSE_PAUSED:
		{
			nIndex = 3;
		}
		break;
	case TSE_DESTROYED:
		{
			nIndex = 6;
		}
		break;
	case TSE_PAUSING:
	case TSE_DESTROYING:
	default:
		{
			nIndex = 0;
		}
		break;
	}
	return nIndex;
}

void CTaskListCtrl::OnDestroy() 
{
	StopAllTasks();
	CSListCtrl::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// DeleteItem
BOOL CTaskListCtrl::DeleteItem(int nItem)
{	
	CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(nItem);
	ASSERT(pTaskInfo != NULL);

	if(pTaskInfo)
		delete pTaskInfo;

	return CSListCtrl::DeleteItem(nItem);
}

///////////////////////////////////////////////////////////////////////////////
// DeleteAllItems
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

void CTaskListCtrl::RemoveSelectedItems()
{
	SetRedraw(FALSE);

	int nItem = -1;
	//Delete all selected items
	POSITION pos = GetFirstSelectedItemPosition();
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

void CTaskListCtrl::StopAllTasks()
{
	CTaskInfo* pTaskInfo = NULL;
	
	int nItem, nRowCount = GetItemCount();
	for(nItem = 0; nItem < nRowCount; nItem++)
	{
		pTaskInfo = (CTaskInfo*)GetItemData(nItem);
		ASSERT(pTaskInfo != NULL);
		
		if(pTaskInfo->m_lpDownloaderMgr == NULL)
		{
			continue;
		}
		if(pTaskInfo->m_lpDownloaderMgr->Destroy() == CFinalizer::DELETE_BY_EXTERNAL)
		{
			delete pTaskInfo->m_lpDownloaderMgr;
		}
	}
	
	DeleteAllItems();
}

CComparator* CTaskListCtrl::CreateComparator(CSortCondition* pSortCondtions, int nCount)
{
	if(m_pComparator == NULL)
	{
		m_pComparator = new CTaskListComparator();
	}
	m_pComparator->Init(pSortCondtions, nCount);
	return m_pComparator;
}

void CTaskListCtrl::InitShellImageList()
{
	LPITEMIDLIST  pidl = NULL;
	SHGetSpecialFolderLocation(GetSafeHwnd(), (UINT)CSIDL_DESKTOP, &pidl);
	
    SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	HIMAGELIST hShellImageList = (HIMAGELIST) SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidl,
		0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	ImageList_SetBkColor(hShellImageList, CLR_NONE);

	m_pILShell = CImageList::FromHandle(hShellImageList);
	
	//Release PIDL
	IMalloc* pIface = NULL;
	SHGetMalloc(&pIface);
	if( SUCCEEDED(::SHGetMalloc(&pIface)) )
	{
		pIface->Free(pidl);
		pIface->Release();
		pIface = NULL;
	}
}
