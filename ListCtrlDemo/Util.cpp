#include "StdAfx.h"

CTimeCost::CTimeCost(UINT timeInMs) : m_nDiff(timeInMs), m_clockCurr(0), m_clockLast(0)
{
}

void CTimeCost::UpdateCurrClock()
{
	m_clockCurr = clock();
}

void CTimeCost::UpdateLastClock()
{
	m_clockLast = clock();
}

BOOL CTimeCost::IsTimeOut()
{
	long diff = m_clockCurr - m_clockLast;
#if (CLOCKS_PER_SEC != 1000)
	diff = diff * 1000 / CLOCKS_PER_SEC;
#endif
	if(diff < 0)
	{
		diff = 0;
	}
	return (UINT)diff >= m_nDiff;
}