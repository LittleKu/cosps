// ProgressMeter.cpp: implementation of the CProgressMeter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProgressMeter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProgressMeter::CProgressMeter(DWORD dwMinInterval)
 : m_dwMinInterval(dwMinInterval), m_dwCurrSpeed(0), m_nSpeederCount(0)
{
	Reset(dwMinInterval);
}

CProgressMeter::~CProgressMeter()
{

}

void CProgressMeter::Reset(DWORD dwMinInterval)
{
	m_dwMinInterval = dwMinInterval;
	m_dwCurrSpeed = 0;
	m_nSpeederCount = 0;

	ZeroMemory(m_speeders, sizeof(m_speeders));
}
BOOL CProgressMeter::UpdateData(DWORD64 dwNowData, DWORD dwNowTime)
{
	/* figure out the last updated data index, for faster speed show at startup, 
	only check when all the bucket has been updated at least once */
	int nLastIndex = (m_nSpeederCount > CURR_TIME) ? ((m_nSpeederCount - 1) % CURR_TIME) : -1;
	if(nLastIndex >= 0)
	{
		//Make sure the time difference is greater than m_dwMinInterval ms
		if(dwNowTime - m_speeders[nLastIndex].m_nTime < m_dwMinInterval)
		{
			return FALSE;
		}
	}

	int nCurrIndex = m_nSpeederCount % CURR_TIME;
	m_speeders[nCurrIndex].m_nData = dwNowData;
	m_speeders[nCurrIndex].m_nTime = dwNowTime;

	//Advance the speeder count
	m_nSpeederCount++;

	//Calculate the speed
	if(m_nSpeederCount > 1)
	{
		/* Get the index position to compare with the 'nCurrIndex' position.
		Get the oldest entry possible. While we have less than CURR_TIME
		entries, the first entry will remain the oldest. */
		int nOldestIndex = (m_nSpeederCount >= CURR_TIME) ? (m_nSpeederCount % CURR_TIME) : 0;

		ASSERT(m_speeders[nCurrIndex].m_nTime >= m_speeders[nOldestIndex].m_nTime);
		ASSERT(m_speeders[nCurrIndex].m_nData >= m_speeders[nOldestIndex].m_nData);

		DWORD dwTimeDelta = m_speeders[nCurrIndex].m_nTime - m_speeders[nOldestIndex].m_nTime;
		DWORD64 dwDataDelta = m_speeders[nCurrIndex].m_nData - m_speeders[nOldestIndex].m_nData;

		m_dwCurrSpeed = CaculateSpeed(dwDataDelta, dwTimeDelta);
	}	

	return TRUE;
}

DWORD CProgressMeter::GetSpeed()
{
	return m_dwCurrSpeed;
}

DWORD CProgressMeter::CaculateSpeed(DWORD64 dwDataDelta, DWORD dwTimeDelta)
{
	if(dwTimeDelta == 0)
	{
		/* at least one millisecond MUST have passed */
		dwTimeDelta = 1;
	}
	DWORD64 dwSpeed = dwDataDelta * 1000 / dwTimeDelta;

	return (DWORD)dwSpeed;
}