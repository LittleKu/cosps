// Sortable.cpp: implementation of the CSortable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sortable.h"
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

CSortable::CSortable(CSListCtrl* pParentListCtrl) : m_pParent(pParentListCtrl)
{
	m_bSorting = FALSE;
	m_lColumnSortStates = 0;
	EmptyArray( m_aCombinedSortedColumns, -1 );
	EmptyArray( (int*)m_aDefaultColumnsSort, (int)DESCENDING );
}

CSortable::~CSortable()
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
void CSortable::SortColumn( int iSubItem, BOOL bMultiColumns )
{
	m_bSorting = TRUE;
	
	if( bMultiColumns )
	{
		int iNumCombinedSortedCols = GetNumCombinedSortedColumns();
		
		if( NotInCombinedSortedColumnList( iSubItem ) )
		{
			m_aCombinedSortedColumns[ iNumCombinedSortedCols++ ] = iSubItem;
			SetItemSortState( iSubItem, GetDefaultItemSortState(iSubItem) );
		}
		else
		{
			SORT_STATE ssEachItem = GetItemSortState( iSubItem );
			SetItemSortState( iSubItem, (SORT_STATE)!ssEachItem );
			MoveItemInCombinedSortedListToEnd( iSubItem );
		}
		
		//sort it
		SortCombinedColumns();
	}
	else
	{
		SORT_STATE ssEachItem = GetItemSortState( iSubItem );
		bool bNotInList = NotInCombinedSortedColumnList( iSubItem );
		
		m_pParent->m_HeaderCtrl.RemoveAllSortImages();
		EmptyArray(m_aCombinedSortedColumns, -1);
		m_lColumnSortStates = 0;
		
		m_aCombinedSortedColumns[ 0 ] = iSubItem;
		SetItemSortState( iSubItem, 
			bNotInList ? GetDefaultItemSortState(iSubItem) : (SORT_STATE)!ssEachItem );
		
		//sort it
		SortCombinedColumns();
	}
	
	m_bSorting = FALSE;
}

/*
Traverse and sort the columns, the sort order is in the internal array
use bSetSort as true for a call from outside the class
*/
void CSortable::SortCombinedColumns()
{	
	int iNumCombinedSortedCols = GetNumCombinedSortedColumns();
	ASSERT(iNumCombinedSortedCols > 0);

	//Sort now: TODO
	CSortCondition* pSortConditions = new CSortCondition[iNumCombinedSortedCols];
	for(int i = 0; i < iNumCombinedSortedCols; i++)
	{
		pSortConditions[i].m_nCol = m_aCombinedSortedColumns[i];

		SORT_STATE ssEachItem = GetItemSortState( m_aCombinedSortedColumns[i] );
		if(ssEachItem == DESCENDING)
		{
			pSortConditions[i].m_nDir = 1;
		}
		else
		{
			pSortConditions[i].m_nDir = 0;
		}
	}
	m_pParent->Sort(pSortConditions, iNumCombinedSortedCols);
	delete []pSortConditions;
	pSortConditions = NULL;
	
	for( int nIndex = iNumCombinedSortedCols - 1; nIndex >= 0 ; nIndex-- )
	{
		SORT_STATE ssEachItem = GetItemSortState( m_aCombinedSortedColumns[nIndex] );	
		
		UINT nSortIndex = (ssEachItem == DESCENDING ? SHC_SORT_DES : SHC_SORT_ASC);
		//refresh the sort image
		m_pParent->m_HeaderCtrl.SetSortState(m_aCombinedSortedColumns[nIndex], nSortIndex);
	}
}

/*
Utility function to find an item in the combined sorted list.
*/
int CSortable::FindItemInCombinedSortedList( int iItem )
{
	int iNumCombinedSortedColumns = GetNumCombinedSortedColumns();
	for( int i = 0; i < iNumCombinedSortedColumns; i++ )
	{
		if(m_aCombinedSortedColumns[i] == iItem )
			return i;
	}
	return -1;
}

/*
This function will move a clicked column to the end of the combined
column list. This is useful when you move backwards through column clicks.
Like click columns: 0, 1, 2, 1. The array will hold [0,1,2] after the first 3
clicks, this function will change it to [0,2,1] after the 4th click.
*/
void CSortable::MoveItemInCombinedSortedListToEnd(int iItem)
{
	int iNumCombinedSortedColumns = GetNumCombinedSortedColumns();
	int aCombinedSortedColumns[MAX_COLUMNS];
	memset( aCombinedSortedColumns, -1, MAX_COLUMNS );
	int iItemIndex = FindItemInCombinedSortedList( iItem );
	if( iItemIndex != -1 )
	{
		if( iItemIndex >= 0 )	//Frank Chen <mkchen@iii.org.tw>
		{
			memcpy( aCombinedSortedColumns, m_aCombinedSortedColumns, iItemIndex * sizeof( int ) );
			memcpy( &aCombinedSortedColumns[iItemIndex], &m_aCombinedSortedColumns[iItemIndex + 1], (iNumCombinedSortedColumns - iItemIndex - 1) * sizeof(int) );
		}
	}
	aCombinedSortedColumns[ iNumCombinedSortedColumns - 1 ] = iItem;
	memcpy( m_aCombinedSortedColumns, aCombinedSortedColumns, MAX_COLUMNS * sizeof(int) );
	for( int i = 0; i < MAX_COLUMNS ; i++ )
	{
		if( aCombinedSortedColumns[i] == -1 )
			break;
	}
}

/*
Utility function to tell you if a column is in the combined sorted list.
*/
bool CSortable::NotInCombinedSortedColumnList(int iItem) const
{
	int iNumCombinedSortedColumns = GetNumCombinedSortedColumns();
	for( int i = 0; i < iNumCombinedSortedColumns; i++ )
	{
		if( m_aCombinedSortedColumns[i] == iItem )
			return false;
	}
	return true;
}

/*
Utility function clear some internal arrays
*/
void CSortable::EmptyArray( int *pArray, int nVal )
{
	memset( pArray, -1, MAX_COLUMNS );
}

/*
Utility function to get you the number of combined sorted columns
*/
const int CSortable::GetNumCombinedSortedColumns() const
{
	for( int i = 0; i < MAX_COLUMNS; i++ )
		if( m_aCombinedSortedColumns[i] == -1 )
			return i;
		return MAX_COLUMNS;
}

/*
Utility function to set the sort state of a column
*/
void CSortable::SetItemSortState(int iItem, SORT_STATE bSortState)
{
	if( bSortState != GetItemSortState( iItem ) )
	{
		m_lColumnSortStates ^= (1 << iItem);
	}
}

/*
Utility function to get the sort state of a column
*/
const SORT_STATE CSortable::GetItemSortState( int iItem ) const
{
	return (SORT_STATE)((m_lColumnSortStates) & ( 1 << iItem ));
}

/*
Utility function to set the default sort state of a column
*/
void CSortable::SetDefaultItemSortState( int iItem, SORT_STATE bSortState )
{
	if( iItem >= 0 && iItem < MAX_COLUMNS )
	{
		m_aDefaultColumnsSort[iItem] = bSortState;
	}
}

/*
Utility function to get the default sort state of a column
*/
const SORT_STATE CSortable::GetDefaultItemSortState( int iItem ) const
{
	SORT_STATE nRes = DESCENDING;
	
	if( iItem >= 0 && iItem < MAX_COLUMNS )
	{
		nRes = (SORT_STATE)m_aDefaultColumnsSort[iItem];
	}
	
	return nRes;
}
