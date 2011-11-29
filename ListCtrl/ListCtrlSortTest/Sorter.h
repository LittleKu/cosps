// Sorter.h: interface for the CSortABC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTER_H__056B627C_8CDA_420B_9129_7D225D5B503E__INCLUDED_)
#define AFX_SORTER_H__056B627C_8CDA_420B_9129_7D225D5B503E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SortSupport.h"

class CSListCtrl;

class CSorter : public CSortable
{
public:
	CSorter(CSListCtrl* pParentListCtrl);
	virtual ~CSorter();
public:
	virtual void SortColumn( int iSubItem, BOOL bMultiColumns = FALSE );
private:
	void SortCombinedColumns();
	int  FindItemInCombinedSortedList( int );
	void MoveItemInCombinedSortedListToEnd( int );
private:
	enum
	{
		MAX_COLUMNS = 63
	};
	CSListCtrl* m_pParent;
	BOOL m_bSorting;
	
	CSortCondition m_aSortConditions[MAX_COLUMNS];
	int m_nSortedColCount;
};

#endif // !defined(AFX_SORTER_H__056B627C_8CDA_420B_9129_7D225D5B503E__INCLUDED_)
