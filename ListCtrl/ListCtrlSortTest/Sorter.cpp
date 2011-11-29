// Sorter.cpp: implementation of the CSortABC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sorter.h"
#include "SListCtrl.h"
#include "SortSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSorter::CSorter(CSListCtrl* pParentListCtrl)
 : m_pParent(pParentListCtrl), m_bSorting(FALSE), m_nSortedColCount(0)
{
}

CSorter::~CSorter()
{
}

/*
This function is the heart of the class. This will get called automatically
when you click a header, and if you press control while clicking the header,
a multi column sort will take place (ie: sorting the current columns within all the
previously control+clicked columns). The multi column sort saves all the previosuly
control+clicked columns in an array and sorts them in reverse order. So if you click 
column 0, 2, 3, 5 while holding control, it will sort 5, 3, 2, 0. ( this acheives a
muli-column sort).
*/
void CSorter::SortColumn( int iSubItem, BOOL bMultiColumns )
{
	m_bSorting = TRUE;
	
	int nIndex = FindItemInCombinedSortedList(iSubItem);
	if( bMultiColumns )
	{
		if( nIndex < 0 )
		{
			m_aSortConditions[m_nSortedColCount].m_nCol = iSubItem;
			m_aSortConditions[m_nSortedColCount].m_nDir = SHC_SORT_DES;
			m_nSortedColCount++;
		}
		else
		{
			m_aSortConditions[nIndex].m_nDir = TOGGLE_SORT_DIR(m_aSortConditions[nIndex].m_nDir);
			MoveItemInCombinedSortedListToEnd( nIndex );
		}
	}
	else
	{
		int nDir = SHC_SORT_DES;
		if(nIndex >= 0)
		{
			nDir = m_aSortConditions[nIndex].m_nDir;
			nDir = TOGGLE_SORT_DIR(nDir);
		}
		
		m_pParent->m_HeaderCtrl.RemoveAllSortImages();

		m_nSortedColCount = 1;
		m_aSortConditions[0].m_nCol = iSubItem;
		m_aSortConditions[0].m_nDir = nDir;
	}
	//sort it
	SortCombinedColumns();
	
	m_bSorting = FALSE;
}

/*
Traverse and sort the columns, the sort order is in the internal array
use bSetSort as true for a call from outside the class
*/
void CSorter::SortCombinedColumns()
{
	ASSERT(m_nSortedColCount > 0);

	//Sort now: TODO
	m_pParent->Sort(m_aSortConditions, m_nSortedColCount);
	
	for( int nIndex = m_nSortedColCount - 1; nIndex >= 0 ; nIndex-- )
	{
		//refresh the sort image
		m_pParent->m_HeaderCtrl.SetSortState(m_aSortConditions[nIndex].m_nCol, 
			m_aSortConditions[nIndex].m_nDir);
	}
}

/*
Utility function to find an item in the combined sorted list.
*/
int CSorter::FindItemInCombinedSortedList( int iItem )
{
	int nIndex = -1;
	for(int i = 0; i < m_nSortedColCount; i++)
	{
		if(m_aSortConditions[i].m_nCol == iItem)
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}

/*
This function will move a clicked column to the end of the combined
column list. This is useful when you move backwards through column clicks.
Like click columns: 0, 1, 2, 1. The array will hold [0,1,2] after the first 3
clicks, this function will change it to [0,2,1] after the 4th click.
*/
void CSorter::MoveItemInCombinedSortedListToEnd(int iItemIndex)
{
	ASSERT(iItemIndex >= 0);
	if( iItemIndex >= 0 )	//Frank Chen <mkchen@iii.org.tw>
	{
		CSortCondition theCondition = m_aSortConditions[iItemIndex];
		for(int i = iItemIndex; i < m_nSortedColCount - 1; i++)
		{
			m_aSortConditions[i] = m_aSortConditions[i + 1];
		}
		m_aSortConditions[m_nSortedColCount - 1] = theCondition;
	}
}
