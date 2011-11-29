// Sortable.h: interface for the CSortable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTABLE_H__CDF0DCAB_91B5_4573_A423_8E191A958F3F__INCLUDED_)
#define AFX_SORTABLE_H__CDF0DCAB_91B5_4573_A423_8E191A958F3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_COLUMNS	       63
enum SORT_STATE{ DESCENDING, ASCENDING };

class CSListCtrl;

class CSortable  
{
public:
	CSortable(CSListCtrl* pParentListCtrl);
	virtual ~CSortable();
public:
	virtual void SortColumn( int iSubItem, BOOL bMultiColumns = FALSE );
private:
	void SortCombinedColumns();

	int FindItemInCombinedSortedList( int );
	void MoveItemInCombinedSortedListToEnd( int );
	bool NotInCombinedSortedColumnList( int iItem ) const;
	
	void EmptyArray( int* pArray, int nVal );
	const int GetNumCombinedSortedColumns() const;
	
	void SetItemSortState( int iItem, SORT_STATE bSortState );
	const SORT_STATE GetItemSortState( int iItem ) const;
	void SetDefaultItemSortState( int iItem, SORT_STATE bSortState );
	const SORT_STATE GetDefaultItemSortState( int iItem ) const;
	
	const int IsControlPressed() const;
private:
	CSListCtrl* m_pParent;
	BOOL m_bSorting;
	DWORD64 m_lColumnSortStates;
	int m_aCombinedSortedColumns[MAX_COLUMNS];

	SORT_STATE m_aDefaultColumnsSort[MAX_COLUMNS];
};

#endif // !defined(AFX_SORTABLE_H__CDF0DCAB_91B5_4573_A423_8E191A958F3F__INCLUDED_)
