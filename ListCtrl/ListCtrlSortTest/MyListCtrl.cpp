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
    { _T("C1"),				SHC_UNCHECKED | (SHC_SORT_ASC << 2),			50   },
    { _T("Directory"),		SHC_NONE_CHECK_BOX | (SHC_SORT_DES << 2),		100  },
	{ _T(""),				SHC_CHECKED | (SHC_SORT_ASC << 3),			100  }
};

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
		// NOTE - the ClassWizard will add and remove mapping macros here.
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
		hditem.iImage = srcDirColumns[i].nType;
		GetHeaderCtrl()->SetItem(i, &hditem);
	}

	for(i = 0; i < 10; i++)
	{
		CRowData* pRowData = new CRowData();
		pRowData->m_nC1 = i + 4;
		pRowData->m_szDir.Format("Hello %d", i);
		pRowData->m_nC3 = i + 11;

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
	SetItemCheckedState(nItem, 0, (pRowData->m_nC1 % 2) + 1, FALSE);
	
	SetItemText(nItem, 1, pRowData->m_szDir);
	
	SetItemCheckedState(nItem, 2, (pRowData->m_nC3 % 2) + 1, FALSE);
}