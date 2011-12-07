// TaskListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "TaskListCtrl.h"

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

CTaskListCtrl::CTaskListCtrl()
{
}

CTaskListCtrl::~CTaskListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaskListCtrl, CSListCtrl)
	//{{AFX_MSG_MAP(CTaskListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskListCtrl message handlers

void CTaskListCtrl::Init()
{
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
