// TaskListComparator.h: interface for the CTaskListComparator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTCOMPARATOR_H__F220C005_210A_4035_BF89_71D07C9B74AE__INCLUDED_)
#define AFX_TASKLISTCOMPARATOR_H__F220C005_210A_4035_BF89_71D07C9B74AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./ThirdParty/SortSupport.h"

class CTaskListComparator : public CComparator  
{
public:
	CTaskListComparator();
	virtual ~CTaskListComparator();

	void Init(CSortCondition* pSortCondtions, int nCount);
	int Compare(LPARAM lParam1, LPARAM lParam2);

	static int Compare(CTaskInfo* p1, CTaskInfo* p2, int nCol, int nDir);
private:
	CSortCondition* m_sortConditions;
	int m_nCount;
};

#endif // !defined(AFX_TASKLISTCOMPARATOR_H__F220C005_210A_4035_BF89_71D07C9B74AE__INCLUDED_)
