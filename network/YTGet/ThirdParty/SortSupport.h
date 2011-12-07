// SortSupport.h: interface for the CSortSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTSUPPORT_H__236104BB_68AF_402D_8E46_2F7F55C43057__INCLUDED_)
#define AFX_SORTSUPPORT_H__236104BB_68AF_402D_8E46_2F7F55C43057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SHC_NONE_SORT				0
#define SHC_SORT_ASC				1
#define SHC_SORT_DES				2

#define TOGGLE_SORT_DIR(i)			(((i) == SHC_SORT_DES) ? SHC_SORT_ASC : SHC_SORT_DES)

class CSortCondition  
{	
public:
	int m_nCol;
    int m_nDir;
	CSortCondition() : m_nCol(-1), m_nDir(SHC_NONE_SORT) {}
	CSortCondition(int nCol, int nDir) : m_nCol(nCol), m_nDir(nDir) {}
};

class CSortable
{
public:
	virtual ~CSortable() {};
	virtual void SortColumn( int iSubItem, BOOL bMultiColumns = FALSE ) = 0;
};

class CComparator
{
public:
	virtual ~CComparator() {};
	virtual int Compare(LPARAM lParam1, LPARAM lParam2) = 0;
};

#endif // !defined(AFX_SORTSUPPORT_H__236104BB_68AF_402D_8E46_2F7F55C43057__INCLUDED_)
