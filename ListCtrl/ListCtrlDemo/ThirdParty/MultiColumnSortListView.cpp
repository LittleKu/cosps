/*
	
	Usage:
		You generally should not use this class directly, though it
		is possible. You need to do two things to use it directly.
		Set m_strUniqueName to someting, and set m_strColumnWidthSection
		to where you want the column widths to be saved.
		and set m_strColumnOrderSection to where you want the column 
		order to be saved.

		The purpose of m_strUniqueName is to allow for saving of
		multiple instances of listview objects. So obviously you would
		need to set this differently for each instance. SetUniqueName must be called
		before calling InsertColumn() or LoadColumnWidths().

		If you are deriving from this class, you need to do the following:
		Add a class to your project derived from CListView, then go into the
		header file and include MultiColumnSortListView.h and change all
		references to CListView to CMultiColumnSortListCtrl. Then in the .cpp
		file of your class, change all the message maps to CMultiColumnSortListCtrl
		instead of CListView. That should do it.

  Compiling:
		One problem you will have is finding IDB_ARROWUP and IDB_ARROWDOWN.
		Those bitmaps will be included with this set of classes, You should
		add them to your project or add your own bitmaps named correctly.
		These are the bitmaps that show the sort order on the header control.

        I hope this is simple enough, kind of a pain to get started but after
		that it should be cake and hopefully it will be useful.
  
  Things to be aware of:
		
		Multithreading:
		     If you delete all the items from another thread
			 in the middle of a sort, it will crash. This is the only
			 bug i have found.
		
		Column Widths:
			 
			MINCOLWIDTH - Minimum width a column can be.
			MAXCOLWIDTH - Maximum width a column can be.
			These are hard coded in the header file. Be aware.
			
			MAXCOLUMNS  - The most columns you can have right
			now is 64, that is only because im use __int64 to
			hold the column sort states. Who needs more than
			64 columns anyway? If you do, you can change it to
			CUIntArray, i just didnt see the need for a whole int
			when all i needed was a bit. 
		        

  Credits:
		
		Iuri Apollonio -- Sorting Class ( great class )
		Zafir Anjum    -- CMultiColumnSortListCtrl::GetColumnCount
		Roger Onslow   -- CMultiColumnSortListCtrl::AutoSizeColumns
		Zafir Anjum    -- CSortableHeaderCtrl::SetSortImage
		Me             -- The Rest, I think. //JK - Craig Lucas <craig@alpeng.com>

*/

// MultiColumnSortListView.cpp : implementation file
//

#include "stdafx.h"
#include "MultiColumnSortListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiColumnSortListCtrl

/*
When deriving from this class you must set m_strUniqueName to something
this name is used to save each instances column widths to the registry
*/
CMultiColumnSortListCtrl::CMultiColumnSortListCtrl()
{	
	m_strUniqueName.Empty();
	m_strColumnWidthSection = REG_COLUMN_WIDTH_SECTION;
	m_strColumnOrderSection = REG_COLUMN_ORDER_SECTION;
	m_strColumnSortSection = REG_COLUMN_SORT_SECTION;

	m_bSorting = false;
	m_lColumnSortStates = 0;
	EmptyArray( m_aCombinedSortedColumns, -1 );
	EmptyArray( (int*)m_aDefaultColumnsSort, (int)DESCENDING );
	EmptyArray( (int*)m_aColumnType, (int)TYPE_TEXT );
}

BEGIN_MESSAGE_MAP(CMultiColumnSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMultiColumnSortListCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked) 
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiColumnSortListCtrl message handlers

/*
This function saves the columns widths of the listctrl to the registry.
This is called in two places, OnDestroy, and OnEndTrack in the headerCtrl class.
*/
void CMultiColumnSortListCtrl::SaveColumnSettings()
{
	//You must set a unique name for every listctrl
	ASSERT( m_strUniqueName.GetLength() );

	int nIndex;	//for the loops

	/*
	Save the widths
	*/
	CString strEntry( m_strUniqueName );
	CString strValue;

	int iNumColumns = GetColumnCount();
	for( nIndex = 0; nIndex < iNumColumns; nIndex++ )
	{
		CString strTemp;
		strTemp.Format( "%d,", GetColumnWidth(nIndex) );
		strValue += strTemp;
	}

	//write to the registry
	AfxGetApp()->WriteProfileString( m_strColumnWidthSection, strEntry, strValue );

	/*
	Save the column's order
	*/
	LPINT pnOrder = (LPINT)malloc(iNumColumns*sizeof(int));
	ASSERT(pnOrder != NULL);

	if( pnOrder )
	{
		GetColumnOrderArray(pnOrder, iNumColumns);
		
		CString strColumnOrder;
		
		for( nIndex = 0; nIndex < iNumColumns; nIndex++ )
		{
			CString strTemp;
			strTemp.Format( "%d,", pnOrder[nIndex] );
			strColumnOrder += strTemp;
		}
		
		//write to the registry
		AfxGetApp()->WriteProfileString( m_strColumnOrderSection, strEntry, strColumnOrder );

		free(pnOrder);
	}

	/*
	Save the sort order
	It saves the column numbers, it's negative if ASCENDING
	*/
	CString strColumnSort;
	int iNumCombinedSortedCols = GetNumCombinedSortedColumns();

	//store the m_lColumnSortStates first
	{
		CString strTemp;
		strTemp.Format( "%d,", m_lColumnSortStates );
		strColumnSort += strTemp;
	}

	//iterate the columns
	for( nIndex = 0; nIndex < iNumCombinedSortedCols; nIndex++ )
	{
		SORT_STATE ssEachItem = GetItemSortState( m_aCombinedSortedColumns[nIndex] );
		
		CString strTemp;
		strTemp.Format( "%d,", m_aCombinedSortedColumns[nIndex] );
		strColumnSort += strTemp;
	}

	//write to the registry
	AfxGetApp()->WriteProfileString( m_strColumnSortSection, strEntry, strColumnSort );
}

/*
This function loads all the column widths for each column that was saved and applies the width
to the column. This function should be called when you are done inserting data. Or you 
can call SetColumnWidth to set the column width right after you InsertColumn(), If you call 
my InsertColumn it will do this for you.
*/
int CMultiColumnSortListCtrl::LoadColumnWidths()
{
	//This function will load all the column widths and apply the widths to each respective column
	int iNumColumns = GetColumnCount();

	//set the column width
	for( int nIndex = 0; nIndex < iNumColumns; nIndex++ )
	{
		SetColumnWidth( nIndex );
	}

	return 1;
}

/*
This function loads the columns order as the columns can be re-arranged with drag & drop
You should call it BEFORE you fill in the contents or it might not redraw properly
*/
int CMultiColumnSortListCtrl::LoadColumnOrder()
{
	ASSERT( m_strUniqueName.GetLength() );	//You must set a unique name for every listctrl

	int iNumColumns = GetColumnCount();

	//set the colum order
	CString strEntry( m_strUniqueName );
	CString strValue, strSubString;
	CString strSection( m_strColumnOrderSection );

	strValue = AfxGetApp()->GetProfileString( strSection, strEntry, "" );

	if( !strValue.IsEmpty() )
	{
		LPINT pnOrder = (LPINT)malloc(iNumColumns*sizeof(int));
		ASSERT(pnOrder != NULL);

		if( pnOrder )
		{
			for( int nIndex = 0; nIndex < iNumColumns; nIndex++ )
			{
				
				AfxExtractSubString(strSubString, strValue, nIndex, ',');
				if( !strSubString.IsEmpty() )
				{
					pnOrder[nIndex] = atoi( (LPCTSTR)strSubString );
				}
			}

			SetColumnOrderArray( iNumColumns, pnOrder );

			free(pnOrder);
		}
	}

	return 1;
}

/*
This function loads the columns sort
To apply the changes you have to call SortCombinedColumns()
*/
int CMultiColumnSortListCtrl::LoadColumnSort()
{
	ASSERT( m_strUniqueName.GetLength() );	//You must set a unique name for every listctrl

	//set the colum order
	CString strEntry( m_strUniqueName );
	CString strValue, strSubString;
	CString strSection( m_strColumnSortSection );

	strValue = AfxGetApp()->GetProfileString( strSection, strEntry, "" );

	if( !strValue.IsEmpty() )
	{
		m_ctlHeaderCtrl.RemoveAllSortImages();
    	EmptyArray(m_aCombinedSortedColumns, -1);
	    m_lColumnSortStates = 0;

		int nIndex = 0;
		while( AfxExtractSubString(strSubString, strValue, nIndex, ',') )
		{
			//get the m_lColumnSortStates first
			if( !nIndex )
			{
				m_lColumnSortStates = atol( (LPCTSTR)strSubString );
				nIndex++;
				continue;
			}
			
			//get the other columns
			if( !strSubString.IsEmpty() )
			{
				m_aCombinedSortedColumns[nIndex-1] = atoi( (LPCTSTR)strSubString );
			}
			else
			{
				break;
			}
			
			//next please
			nIndex++;
		}
	}

	return 1;
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
void CMultiColumnSortListCtrl::SortColumn( int iSubItem, bool bSortingMultipleColumns )
{	
	m_bSorting = true;

	if( bSortingMultipleColumns )
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
		
		m_ctlHeaderCtrl.RemoveAllSortImages();
		EmptyArray(m_aCombinedSortedColumns, -1);
		m_lColumnSortStates = 0;
		
		m_aCombinedSortedColumns[ 0 ] = iSubItem;
		SetItemSortState( iSubItem, 
			bNotInList ? GetDefaultItemSortState(iSubItem) : (SORT_STATE)!ssEachItem );

		//sort it
		SortCombinedColumns();
	}

	m_bSorting = false;
}

/*
Traverse and sort the columns, the sort order is in the internal array
use bSetSort as true for a call from outside the class
*/
void CMultiColumnSortListCtrl::SortCombinedColumns(bool bSetSort /*= false*/)
{
	if( bSetSort )
	{
		m_bSorting = true;
	}

	int iNumCombinedSortedCols = GetNumCombinedSortedColumns();
	
	for( int nIndex = iNumCombinedSortedCols - 1; nIndex >= 0 ; nIndex-- )
	{
		SORT_STATE ssEachItem = GetItemSortState( m_aCombinedSortedColumns[nIndex] );
		SORT_TYPE sortType = GetColumnType(m_aCombinedSortedColumns[nIndex]);
		
		CSortClass csc( this, m_aCombinedSortedColumns[nIndex], sortType );	
		csc.Sort( DESCENDING == ssEachItem );	//Ariel Benary <Ariel.Benary@ness.com>
		
		//refresh the sort image
		m_ctlHeaderCtrl.SetSortImage( m_aCombinedSortedColumns[nIndex], ssEachItem );
	}

	if( bSetSort )
	{
		m_bSorting = false;
	}
}

/*
My version of InsertColumn that will automatically load the last column
width from the registry. 
*/
int CMultiColumnSortListCtrl::InsertColumnEx(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem );
	SetColumnWidth( nCol );
	return 1;
}

/*
Utility function to size columns based on its data.
written by Roger Onslow
*/
void CMultiColumnSortListCtrl::AutoSizeColumn( int iColumn )
{
	SetRedraw(false);
	
	CListCtrl::SetColumnWidth(iColumn,LVSCW_AUTOSIZE);
	int wc1 = CListCtrl::GetColumnWidth( iColumn );
	CListCtrl::SetColumnWidth(iColumn,LVSCW_AUTOSIZE_USEHEADER);
	int wc2 = GetColumnWidth( iColumn );
	int wc = max(MINCOLWIDTH,max( wc1,wc2 ));
	
	if( wc > MAXCOLWIDTH )
	{
		wc = MAXCOLWIDTH;
	}
	
	CListCtrl::SetColumnWidth( iColumn,wc );  
	SetRedraw(true);
}

/*
Utility function to get rid of all the columns
*/
void CMultiColumnSortListCtrl::DeleteAllColumns()
{
	int iNumCols = GetColumnCount();
	for ( int i = 0; i < iNumCols; i++ )
	{
		DeleteColumn(0);
	}
}

/*
Utility function to get rid of all items.
*/
void CMultiColumnSortListCtrl::DeleteAllItems()
{
	if( GetItemCount() > 0 )
	{
		CListCtrl::DeleteAllItems();	//Ariel Benary <Ariel.Benary@ness.com>
	}
}

/*
Utility function to find the item by item data, make it visible and select.
return the index of the found item or -1 if item not found
*/
int CMultiColumnSortListCtrl::FindShowAndSelect(DWORD dwData, BOOL bSelect /*= TRUE*/)
{
	int nCurrentFilePos = -1;

	//try to find the item
	LVFINDINFO sFindInfo;
	sFindInfo.flags = LVFI_PARAM;
	sFindInfo.lParam = dwData;
	nCurrentFilePos = FindItem(&sFindInfo);

	//once found - select and make visible
	if( 0 <= nCurrentFilePos && bSelect )
	{
		EnsureVisible( nCurrentFilePos, FALSE );
		SetItemState( nCurrentFilePos, LVIS_SELECTED, LVIS_SELECTED );
	}

	return nCurrentFilePos;
}

/*
Utility function to find the item by text, make it visible and select.
return the index of the found item or -1 if item not found
*/
int CMultiColumnSortListCtrl::FindShowAndSelect(LPCTSTR szText, BOOL bSelect /*= TRUE*/)
{
	int nCurrentFilePos = -1;
	
	//try to find the item
	LVFINDINFO sFindInfo;
	sFindInfo.flags = LVFI_STRING;
	sFindInfo.psz = szText;
	nCurrentFilePos = FindItem(&sFindInfo);
	
	//once found - select and make visible
	if( 0 <= nCurrentFilePos && bSelect )
	{
		EnsureVisible( nCurrentFilePos, FALSE );
		SetItemState( nCurrentFilePos, LVIS_SELECTED, LVIS_SELECTED );
	}
	
	return nCurrentFilePos;
}

/*
Utility function to get the number of columns
written by Zafir Anjum
*/
UINT CMultiColumnSortListCtrl::GetColumnCount()
{
	CHeaderCtrl *pHeaderCtrl = (CHeaderCtrl*)/*GetListCtrl().*/GetDlgItem(0);
	return pHeaderCtrl->GetItemCount();
}

/*
Just add some extended styles from the new IE4 stuff.
Of course you can either change the code or change your
derived class's OnCreate to call CListView::OnCreate
*/
//DEL int CMultiColumnSortListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
//DEL {
//DEL 	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
//DEL 		return -1;
//DEL 	// set list control's style to hilight the entire row
//DEL 	DWORD dwStyle = SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
//DEL 	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
//DEL 	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
//DEL 	m_ctlHeaderCtrl.SubclassWindow( GetDlgItem(0)->m_hWnd );		
//DEL 
//DEL 	return 0;
//DEL }

/*
We are only sorting in report view so far.
*/
//DEL BOOL CMultiColumnSortListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
//DEL {
//DEL 	cs.style |= LVS_REPORT;
//DEL 	return CListCtrl::PreCreateWindow(cs);
//DEL }

/*
Utility function to tell you if a sort is taking place
*/
const bool CMultiColumnSortListCtrl::IsSorting() const
{
	return CMultiColumnSortListCtrl::m_bSorting;
}

/*
Utility function to tell you if the control key is being pressed
*/
const int CMultiColumnSortListCtrl::IsControlPressed() const
{
	return (::GetKeyState( VK_CONTROL ) < 0 );
}

/*
Message handler for when a header is clicked.
*/
void CMultiColumnSortListCtrl::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *pHDN = (HD_NOTIFY *) pNMHDR;
	if( pHDN->iButton == 0 )
	{
		if( IsControlPressed() )
			SortColumn( pHDN->iItem, MULTI_COLUMN_SORT );
		else
			SortColumn( pHDN->iItem, SINGLE_COLUMN_SORT );
	}
	*pResult = 0;
}

/*
Message handler for when control is about to be destroyed.
This is where the column widths are saved.
*/
void CMultiColumnSortListCtrl::OnDestroy() 
{
	SaveColumnSettings();	
	CListCtrl::OnDestroy();
}

/*
Utility function to tell you if a column is in the combined sorted list.
*/
bool CMultiColumnSortListCtrl::NotInCombinedSortedColumnList(int iItem) const
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
Utility function to set the sort state of a column
*/
void CMultiColumnSortListCtrl::SetItemSortState(int iItem, SORT_STATE bSortState)
{
	if( bSortState != GetItemSortState( iItem ) )
	{
		m_lColumnSortStates ^= (1 << iItem);
	}
}

/*
Utility function to get the sort state of a column
*/
const SORT_STATE CMultiColumnSortListCtrl::GetItemSortState( int iItem ) const
{
	return (SORT_STATE)((m_lColumnSortStates) & ( 1 << iItem ));
}

/*
Utility function to set the default sort state of a column
*/
void CMultiColumnSortListCtrl::SetDefaultItemSortState( int iItem, SORT_STATE bSortState )
{
	if( iItem >= 0 && iItem < MAX_COLUMNS )
	{
		m_aDefaultColumnsSort[iItem] = bSortState;
	}
}

/*
Utility function to get the default sort state of a column
*/
const SORT_STATE CMultiColumnSortListCtrl::GetDefaultItemSortState( int iItem ) const
{
	SORT_STATE nRes = DESCENDING;

	if( iItem >= 0 && iItem < MAX_COLUMNS )
	{
		nRes = (SORT_STATE)m_aDefaultColumnsSort[iItem];
	}

	return nRes;
}

/*
Utility function to get you the number of combined sorted columns
*/
const int CMultiColumnSortListCtrl::GetNumCombinedSortedColumns() const
{
	for( int i = 0; i < MAX_COLUMNS; i++ )
		if( m_aCombinedSortedColumns[i] == -1 )
			return i;
	return MAX_COLUMNS;
}

/*
Utility function clear some internal arrays
*/
void CMultiColumnSortListCtrl::EmptyArray( int *pArray, int nVal )
{
	memset( pArray, -1, MAX_COLUMNS );
}

/*
This function will move a clicked column to the end of the combined
column list. This is useful when you move backwards through column clicks.
Like click columns: 0, 1, 2, 1. The array will hold [0,1,2] after the first 3
clicks, this function will change it to [0,2,1] after the 4th click.
*/
void CMultiColumnSortListCtrl::MoveItemInCombinedSortedListToEnd(int iItem)
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
Utility function to find an item in the combined sorted list.
*/
int CMultiColumnSortListCtrl::FindItemInCombinedSortedList( int iItem )
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
Utility function to look up a columns width in the registry.
If it returns -1 then it means that it was not found in the registry
*/
const int CMultiColumnSortListCtrl::GetRegColumnWidth( int iColumn ) const
{
	int nRes = -1;
	ASSERT( m_strUniqueName.GetLength() );	//You must set a unique name for every listctrl
	
	CString strEntry( m_strUniqueName );
	CString strValue, strSubString;
	CString strSection( m_strColumnWidthSection );
	strValue = AfxGetApp()->GetProfileString( strSection, strEntry, "" );
	AfxExtractSubString(strSubString, strValue, iColumn, ',');
	if( !strSubString.IsEmpty() )
	{
		nRes = atoi( (LPCTSTR)strSubString );
	}

	return nRes;
}

/*
Utility function to Autosize all columns in the case there is no registry entry.
*/
void CMultiColumnSortListCtrl::AutoSizeAllColumns()
{
	int iNumCols = GetColumnCount();
	for( int i = 0; i < iNumCols; i++ )
	{
		AutoSizeColumn( i );
	}
}

/*
Utility function to set the width on the column based on the registry
value and a set minimum column width.
*/
void CMultiColumnSortListCtrl::SetColumnWidth( int nCol )
{
	int iWidth = GetRegColumnWidth( nCol );
	if( iWidth > -1 )
	{
		if( iWidth < MINCOLWIDTH )
		{
			AutoSizeColumn( nCol );
		}
		else
		{
			CListCtrl::SetColumnWidth( nCol, iWidth );
		}
	}
}

/*
Set the column sort type
*/
void CMultiColumnSortListCtrl::SetColumnType( int iCol, SORT_TYPE nType )
{
	if( iCol >= 0 && iCol < MAX_COLUMNS )
	{
		m_aColumnType[iCol] = nType;
	}
}

/*
Get the column sort type
*/
const SORT_TYPE CMultiColumnSortListCtrl::GetColumnType( int iCol ) const
{
	return (iCol >= 0 && iCol < MAX_COLUMNS) ? m_aColumnType[iCol] : TYPE_TEXT;
}

/*
Set the unique name for the registry operations
*/
void CMultiColumnSortListCtrl::SetUniqueName(LPCTSTR lpszUniqueName)
{
	m_strUniqueName = lpszUniqueName;
}

void CMultiColumnSortListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ctlHeaderCtrl.SubclassWindow( GetDlgItem(0)->m_hWnd );
	
	CListCtrl::PreSubclassWindow();
}

/*
Modified version of InsertColumn to accomodate for the sort type and sort state
*/
int CMultiColumnSortListCtrl::InsertColumn( int nCol, SORT_TYPE nType, SORT_STATE nDefaultSort, const LVCOLUMN* pColumn )
{
	int nRes = CListCtrl::InsertColumn( nCol, pColumn );

	SetColumnType(nRes, nType);
	SetDefaultItemSortState(nRes, nDefaultSort);

	return nRes;
}

/*
Modified version of InsertColumn to accomodate for the sort type and sort state
*/
int CMultiColumnSortListCtrl::InsertColumn( int nCol, SORT_TYPE nType, SORT_STATE nDefaultSort, LPCTSTR lpszColumnHeading, int nFormat /*= LVCFMT_LEFT*/, int nWidth /*= -1*/, int nSubItem /*= -1*/ )
{
	int nRes = CListCtrl::InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem );
	
	SetColumnType(nRes, nType);
	SetDefaultItemSortState(nRes, nDefaultSort);
	
	return nRes;
}

//hidden from the world
int CMultiColumnSortListCtrl::InsertColumn( int nCol, const LVCOLUMN* pColumn )
{
	return CListCtrl::InsertColumn( nCol, pColumn );
}

//hidden from the world
int CMultiColumnSortListCtrl::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat /*= LVCFMT_LEFT*/, int nWidth /*= -1*/, int nSubItem /*= -1*/ )
{
	return CListCtrl::InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}
