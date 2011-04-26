// CheckListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CheckLCDemo.h"
#include "CheckListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl

CCheckListCtrl::CCheckListCtrl() : m_blInited(FALSE)
{
}

CCheckListCtrl::~CCheckListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheckListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCheckListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnItemChanged)		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl message handlers
void CCheckListCtrl::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*)pNMHDR;
	*pResult = 0;

	if ( m_blInited /*&& LVIF_STATE == pNMLV->uChanged*/)
	{
		LV_ITEM lvi;
		lvi.iItem = pNMLV->iItem;
		lvi.iSubItem = pNMLV->iSubItem;
		lvi.mask = LVIF_IMAGE;
		ListView_GetItem(GetSafeHwnd(), &lvi);

		if(lvi.iImage == 1)
		{
			lvi.iImage = 2;
		}
		else if(lvi.iImage == 2)
		{
			lvi.iImage = 1;
		}
		ListView_SetItem(GetSafeHwnd(), &lvi);
		BOOL blAllChecked = TRUE;
		int nCount = GetItemCount();
		for(int nItem = 0; nItem < nCount; nItem++)
		{
			lvi.iItem = nItem;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_IMAGE;
			ListView_GetItem(GetSafeHwnd(), &lvi);
			if(lvi.iImage != 2)
			{
				blAllChecked = FALSE;
				break;
			}
		}
		
		HDITEM hdItem;
		hdItem.mask = HDI_IMAGE;
		if (blAllChecked)
			hdItem.iImage = 2;
		else
			hdItem.iImage = 1;
		VERIFY( m_checkHeadCtrl.SetItem(0, &hdItem) );
	}

}

BOOL CCheckListCtrl::Init()
{
	if (m_blInited)
		return TRUE;

	CHeaderCtrl* pHeadCtrl = this->GetHeaderCtrl();
	ASSERT(pHeadCtrl->GetSafeHwnd());

	VERIFY( m_checkHeadCtrl.SubclassWindow(pHeadCtrl->GetSafeHwnd()) );
	VERIFY( m_checkImgList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255)) );
	int i = m_checkImgList.GetImageCount();
	m_checkHeadCtrl.SetImageList(&m_checkImgList);
	this->SetImageList(&m_checkImgList, LVSIL_SMALL);
	
	HDITEM hdItem;
	hdItem.mask = HDI_IMAGE | HDI_FORMAT;
	VERIFY( m_checkHeadCtrl.GetItem(0, &hdItem) );
	hdItem.iImage = 1;
	hdItem.fmt |= HDF_IMAGE;
	
	VERIFY( m_checkHeadCtrl.SetItem(0, &hdItem) );

	m_blInited = TRUE;

	return TRUE;
}