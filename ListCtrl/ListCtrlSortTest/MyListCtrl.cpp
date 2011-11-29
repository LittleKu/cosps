// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"
#include "resource.h"

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
    { _T("C1 long name"),	SHC_INDEX_TO_STATE(SHC_UNCHECKED, SHC_SORT_ASC),			50   },
    { _T("Directory"),		SHC_INDEX_TO_STATE(SHC_NONE_CHECK_BOX, SHC_SORT_DES),		100  },
	{ _T(""),				SHC_INDEX_TO_STATE(SHC_CHECKED, SHC_SORT_ASC),				100  },
	{ _T("aaaa"),			SHC_INDEX_TO_STATE(SHC_NONE_CHECK_BOX, SHC_NONE_SORT),		100  }
};


CMyComparator::CMyComparator(CSortCondition* pSortCondtions, int nCount)
{
	m_sortConditions = pSortCondtions;
	m_nCount = nCount;
}
int CMyComparator::Compare(LPARAM lParam1, LPARAM lParam2)
{
	CRowData* pRowData1 = (CRowData*)lParam1;
	CRowData* pRowData2 = (CRowData*)lParam2;

	ASSERT(pRowData1 != NULL && pRowData2 != NULL);

	int nResult = 0;
	for(int i = 0; i < m_nCount; i++)
	{
		nResult = CRowData::Compare(pRowData1, pRowData2, m_sortConditions[i].m_nCol, m_sortConditions[i].m_nDir);
		if(nResult != 0)
		{
			return nResult;
		}
	}
	return nResult;
}	
/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CSListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

void CMyListCtrl::Init()
{
	m_ILSortImages.Create(IDB_HEAD_SORT, 16, 1, RGB(255, 0, 255));
	GetHeaderCtrl()->SetImageList(&m_ILSortImages);

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
// 		if(i % 3 == 0)
// 		{
// 			hditem.fmt |=  HDF_LEFT;
// 		}
// 		else if(i %3 == 1)
// 		{
// 			hditem.fmt |= HDF_CENTER;
// 		}
// 		else
// 		{
// 			hditem.fmt |= HDF_RIGHT;
// 		}
		hditem.fmt |= HDF_RIGHT;
		hditem.iImage = srcDirColumns[i].nType;
		GetHeaderCtrl()->SetItem(i, &hditem);
	}

	for(i = 0; i < 10; i++)
	{
		CRowData* pRowData = new CRowData();
		pRowData->m_nC1 = (i + 4) % 2 + 1;
		pRowData->m_szDir.Format("Hello %d", i % 4);
		pRowData->m_nC3 = (i + 11) % 2 + 1;

		pRowData->m_szReserved.Format("adcdefd %d", i % 8);

		AddRow(pRowData);
	}
}

void CMyListCtrl::AddRow(CRowData* pRowData)
{
	ASSERT(pRowData != NULL);

	int nItem = GetItemCount(); 	
	
    LV_ITEM lvi;	
    lvi.mask = LVIF_TEXT | LVIF_PARAM;
    lvi.iItem = GetItemCount();
    lvi.lParam = (LPARAM)pRowData;
	
    lvi.iSubItem = 0;
    lvi.pszText = _T("");
    int nRes = InsertItem(&lvi);
	
    int iSubItem = 0;

	CString szTemp;
	szTemp.Format("abc %d", pRowData->m_nC1);

	SetItemText(nItem, 0, szTemp);
	SetItemCheckedState(nItem, 0, (pRowData->m_nC1 % 2) + 1, FALSE);
	
	SetItemText(nItem, 1, pRowData->m_szDir);
	
	SetItemCheckedState(nItem, 2, (pRowData->m_nC3 % 2) + 1, FALSE);

	SetItemText(nItem, 3, pRowData->m_szReserved);
}

CComparator* CMyListCtrl::CreateComparator(CSortCondition* pSortCondtions, int nCount)
{
	return new CMyComparator(pSortCondtions, nCount);
}

void CMyListCtrl::OnDestroy() 
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		CRowData* pRowData = (CRowData*)GetItemData(i);
		ASSERT(pRowData);

		if(pRowData)
		{
			delete pRowData;
			pRowData = NULL;
		}
	}

	CSListCtrl::OnDestroy();	
}
