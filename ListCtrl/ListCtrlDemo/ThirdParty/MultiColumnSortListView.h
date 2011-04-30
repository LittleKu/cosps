#if !defined(AFX_MULTICOLUMNSORTLISTVIEW_H__E58FDCA1_0449_11D2_9AED_2F8C0624A000__INCLUDED_)
#define AFX_MULTICOLUMNSORTLISTVIEW_H__E58FDCA1_0449_11D2_9AED_2F8C0624A000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MultiColumnSortListView.h : header file
//

#include "SortableHeaderCtrl.h"
#include "SortClass.h"


#define MINCOLWIDTH	       0
#define MAXCOLWIDTH	       500
#define MAX_COLUMNS	       63
#define MULTI_COLUMN_SORT  1
#define SINGLE_COLUMN_SORT 0

#define REG_COLUMN_WIDTH_SECTION	"ColumnWidths"
#define REG_COLUMN_ORDER_SECTION	"ColumnOrder"
#define REG_COLUMN_SORT_SECTION		"ColumnSort"

enum SORT_STATE{ DESCENDING, ASCENDING };
/////////////////////////////////////////////////////////////////////////////
// CMultiColumnSortListCtrl 

class CMultiColumnSortListCtrl : public CListCtrl
{
public:
// Constructor
	CMultiColumnSortListCtrl();
	
// Attributes
public:
protected:
	CString m_strColumnWidthSection;
	CString m_strColumnOrderSection;
	CString m_strColumnSortSection;
	CString m_strUniqueName;
	bool m_bAutoSizeOnInsert;
	bool m_bSorting;
	CSortableHeaderCtrl m_ctlHeaderCtrl;	
	
// Operations
private:
	//we hide these two to force the use of the new methods
	int InsertColumn( int nCol, const LVCOLUMN* pColumn );
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1 );
public:
	int InsertColumn( int nCol, SORT_TYPE nType, SORT_STATE nDefaultSort, const LVCOLUMN* pColumn );
	int InsertColumn( int nCol, SORT_TYPE nType, SORT_STATE nDefaultSort, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1 );
	int InsertColumnEx( int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = 1, int nSubItem = 1 );

public:
	void SetUniqueName( LPCTSTR );

	void SetColumnType( int iCol, SORT_TYPE nType );
	const SORT_TYPE GetColumnType( int iCol ) const;
	
	void SetColumnWidth( int );
	const int GetRegColumnWidth( int ) const;
	void AutoSizeColumn( int iColumn );
	void AutoSizeAllColumns();
	
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

	void SortColumn( int, bool = false );
	void SortCombinedColumns(bool bSetSort = false);

	int LoadColumnWidths();
	int LoadColumnOrder();
	int LoadColumnSort();
	void SaveColumnSettings();
	
	UINT GetColumnCount();
	void DeleteAllColumns();
	void DeleteAllItems();
	int FindShowAndSelect(DWORD dwData, BOOL bSelect = TRUE);
	int FindShowAndSelect(LPCTSTR szText, BOOL bSelect = TRUE);
	const bool IsSorting() const;
	
private:
	__int64 m_lColumnSortStates;
	int m_aCombinedSortedColumns[MAX_COLUMNS];

	SORT_TYPE m_aColumnType[MAX_COLUMNS];
	SORT_STATE m_aDefaultColumnsSort[MAX_COLUMNS];

	CImageList m_ilImageList;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiColumnSortListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CMultiColumnSortListCtrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult) ;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTICOLUMNSORTLISTVIEW_H__E58FDCA1_0449_11D2_9AED_2F8C0624A000__INCLUDED_)
