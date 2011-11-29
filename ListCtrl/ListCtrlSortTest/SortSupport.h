// SortSupport.h: interface for the CSortSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTSUPPORT_H__236104BB_68AF_402D_8E46_2F7F55C43057__INCLUDED_)
#define AFX_SORTSUPPORT_H__236104BB_68AF_402D_8E46_2F7F55C43057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSortSupport  
{
public:
	CSortSupport();
	virtual ~CSortSupport();

};

class CComparator
{
public:
	virtual ~CComparator() {};
	virtual int Compare(LPARAM lParam1, LPARAM lParam2) = 0;
};

#endif // !defined(AFX_SORTSUPPORT_H__236104BB_68AF_402D_8E46_2F7F55C43057__INCLUDED_)
