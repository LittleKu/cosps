// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FlickerFreeListCtrlDemo.h"
#include "MyListCtrl.h"
#include "MemDC.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyListCtrl

MyListCtrl::MyListCtrl()
{
}

MyListCtrl::~MyListCtrl()
{
}


BEGIN_MESSAGE_MAP(MyListCtrl, CFlickerFreeListCtrl)
	//{{AFX_MSG_MAP(MyListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyListCtrl message handlers

void MyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (!lpDrawItemStruct->itemData)
		return;

	CMemDC dc(CDC::FromHandle(lpDrawItemStruct->hDC), &lpDrawItemStruct->rcItem);
	BOOL bCtrlFocused;
	InitItemMemDC(dc, lpDrawItemStruct, bCtrlFocused);
	CRect cur_rec(lpDrawItemStruct->rcItem);
	CRect rcClient;
	GetClientRect(&rcClient);
	CTaskInfo* pTaskInfo = (CTaskInfo*)lpDrawItemStruct->itemData;

	
	CDC* pDC = &dc;
	int iItem = lpDrawItemStruct->itemID;
	//get rectangles for drawing
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(iItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(iItem, rcLabel, LVIR_LABEL);
	GetItemRect(iItem, rcIcon, LVIR_ICON);
	CRect rcCol(rcBounds);

	//the label!
	CString sLabel = GetItemText(iItem, 0);
	//labels are offset by a certain amount
	//this offset is related to the width of a space character
	CRect rcHighlight;
	CRect rcWnd;

	//should I check (GetExtendedStyle() & LVS_EX_FULLROWSELECT) ?
	rcHighlight.top    = rcBounds.top;
	rcHighlight.bottom = rcBounds.bottom;
	rcHighlight.left   = rcBounds.left;
	rcHighlight.right  = rcBounds.right;


	CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
	//update column
	rcCol.right = rcCol.left + GetColumnWidth(0);
	if (m_crWindowTextBk == CLR_NONE)
		pDC->SetBkMode(TRANSPARENT);

	//draw item label (column 0)
	rcLabel.left += sm_iLabelOffset;
	rcLabel.right -= sm_iLabelOffset;
	pDC->DrawText(sLabel, -1, rcLabel, MLC_DT_TEXT | DT_NOCLIP);

	//draw labels for remaining columns
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right : rcBounds.right;

	int iCount = pHeaderCtrl->GetItemCount();
	for(int iCurrent = 1; iCurrent < iCount; iCurrent++) 
	{
		int iColumn = pHeaderCtrl->OrderToIndex(iCurrent);
		//don't draw column 0 again
		if(iColumn == 0)
			continue;

		GetColumn(iColumn, &lvc);
		//don't draw anything with 0 width
		if(lvc.cx == 0)
			continue;

		rcCol.left = rcCol.right;
		rcCol.right += lvc.cx;

		if (rcCol.left < rcCol.right && HaveIntersection(rcClient, rcCol))
		{
			sLabel = GetItemText(iItem, iColumn);
			if (sLabel.GetLength() == 0)
				continue;

			//get the text justification
			UINT nJustify = DT_LEFT;
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK) 
			{
				case LVCFMT_RIGHT:
					nJustify = DT_RIGHT;
					break;
				case LVCFMT_CENTER:
					nJustify = DT_CENTER;
					break;
				default:
					break;
			}

			rcLabel = rcCol;
			rcLabel.left += sm_iLabelOffset + sm_iSubItemInset;
			rcLabel.right -= sm_iLabelOffset + sm_iSubItemInset;

			if(iColumn == 3)
			{
				double percent = ((double)(pTaskInfo->m_nProgress % 1000)) / 1000;
				int nWidth = (int)(percent * rcLabel.Width());
				CRect rcTemp = rcLabel;
				rcTemp.right = rcTemp.left + nWidth;
				rcTemp.DeflateRect(0, 2);
				pDC->FillSolidRect(&rcTemp, RGB(255, 128, 0));

				CString szMyText;
				szMyText.Format(_T("%.3f%c"), percent, _T('%'));
				pDC->DrawText(szMyText, -1, rcLabel, MLC_DT_TEXT | nJustify);
			}
			else
			{
				pDC->DrawText(sLabel, -1, rcLabel, MLC_DT_TEXT | nJustify);
			}
		}
	}
}

//application-depend functions
int MyListCtrl::AddRow(CTaskInfo *pTaskInfo)
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
	
	//file format
	SetItemText(lvi.iItem, iSubItem, pTaskInfo->m_szFormat);
	iSubItem++;
	
	//duration
	szTemp.Format(_T("%d ms"), pTaskInfo->m_nDuration);
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;
	
	//state
	szTemp.Format(_T("%d"), pTaskInfo->m_nState);
	SetItemText(lvi.iItem, iSubItem, szTemp);
	iSubItem++;
	
	return nResult;
}

void MyListCtrl::UpdateRow(int nIndex, CTaskInfo* pTaskInfo)
{
	
}

BOOL MyListCtrl::DeleteItem(int nItem)
{
	CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(nItem);
	ASSERT(pTaskInfo != NULL);
	
	if(pTaskInfo)
		delete pTaskInfo;
	
	return CFlickerFreeListCtrl::DeleteItem(nItem);
}
BOOL MyListCtrl::DeleteAllItems()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		CTaskInfo* pTaskInfo = (CTaskInfo*)GetItemData(i);
		ASSERT(pTaskInfo != NULL);
		
		if(pTaskInfo)
			delete pTaskInfo;
	}
	
	return CFlickerFreeListCtrl::DeleteAllItems();
}
