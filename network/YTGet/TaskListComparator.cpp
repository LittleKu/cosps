// TaskListComparator.cpp: implementation of the CTaskListComparator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YTGet.h"
#include "TaskListComparator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskListComparator::CTaskListComparator()
{

}

CTaskListComparator::~CTaskListComparator()
{

}

void CTaskListComparator::Init(CSortCondition* pSortCondtions, int nCount)
{
	m_sortConditions = pSortCondtions;
	m_nCount = nCount;
}

int CTaskListComparator::Compare(LPARAM lParam1, LPARAM lParam2)
{
	CTaskInfo* pRowData1 = (CTaskInfo*)lParam1;
	CTaskInfo* pRowData2 = (CTaskInfo*)lParam2;
	
	ASSERT(pRowData1 != NULL && pRowData2 != NULL);
	
	int nResult = 0;
	for(int i = 0; i < m_nCount; i++)
	{
		nResult = Compare(pRowData1, pRowData2, m_sortConditions[i].m_nCol, m_sortConditions[i].m_nDir);
		if(nResult != 0)
		{
			return nResult;
		}
	}
	return nResult;
}

int CTaskListComparator::Compare(CTaskInfo* p1, CTaskInfo* p2, int nCol, int nDir)
{
	int nResult = 0;
	switch(nCol)
	{
	case CTaskInfo::COL_STATUS:
		{
			nResult = p1->m_dlState.m_nState - p2->m_dlState.m_nState;
		}
		break;
	case CTaskInfo::COL_FILE_NAME:
		{
			nResult = p1->m_szFileName.Compare(p2->m_szFileName);
		}
		break;
	case CTaskInfo::COL_FILE_SIZE:
		{
			nResult = p1->m_nFileSize - p2->m_nFileSize;
		}
		break;
	case CTaskInfo::COL_PROGRESS:
		{
			double d1 = 0.0f;
			if(p1->m_nDlNow > 0 && p1->m_nFileSize > 0)
			{
				d1 = ((double)p1->m_nDlNow) / p1->m_nFileSize;
			}

			double d2 = 0.0f;
			if(p2->m_nDlNow > 0 && p2->m_nFileSize > 0)
			{
				d2 = ((double)p2->m_nDlNow) / p2->m_nFileSize;
			}

			if(d1 > d2)
			{
				nResult = 1;
			}
			else if(d1 < d2)
			{
				nResult = -1;
			}
			else
			{
				nResult = 0;
			}
		}
		break;
	case CTaskInfo::COL_SPEED:
		{
			nResult = p1->m_nCurrSpeed - p2->m_nCurrSpeed;
		}
		break;
	case CTaskInfo::COL_LEFT_TIME:
		{
			nResult = p1->m_nLeftTime - p2->m_nLeftTime;
		}
		break;
	case CTaskInfo::COL_COST_TIME:
		{
			nResult = p1->m_nCostTime - p2->m_nCostTime;
		}
		break;
	case CTaskInfo::COL_DETAIL:
		{
			nResult = p1->m_dlState.m_szDetail.Compare(p2->m_dlState.m_szDetail);
		}
		break;
	case CTaskInfo::COL_URL:
		{
			nResult = p1->m_szUrl.Compare(p2->m_szUrl);
		}
		break;
	default:
		{
			AfxTrace("Unexpected column index: %d\n", nCol);
		}
		break;
	}
	
	if(nDir == SHC_SORT_DES && nResult != 0)
	{
		nResult *= (-1);
	}
	return nResult;
}
