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

CProgressMeter::CProgressMeter(DWORD dwPrgsInterval, DWORD dwSampleInterval)
{
	m_dwPrgsInterval = dwPrgsInterval;
	m_dwSampleInterval = dwSampleInterval;

	Reset(0, 0, 0);
}

CProgressMeter::~CProgressMeter()
{

}

void CProgressMeter::Reset(DWORD dwStartTime, DWORD64 dwTotal, DWORD64 dwTotalNow)
{
	m_dwStartTime = dwStartTime;
	m_dwLastTime = dwStartTime;

	m_dwTotal = dwTotal;
	m_dwTotalNow = dwTotalNow;
	m_dwCurrentNow = 0;

	m_nSpeederCount = 0;

	m_dwCurrSpeed = 0;
	m_dwTimeCost = 0;
	m_bPrgsTimeOut = FALSE;

	ZeroMemory(m_speeders, sizeof(m_speeders));
}
BOOL CProgressMeter::UpdateSample(DWORD dwNowTime, DWORD64 dwTotalNowData, DWORD64 dwCurrentNowData)
{
	//1. Update metric data
	m_dwTotalNow = dwTotalNowData;
	m_dwCurrentNow = dwCurrentNowData;
	ASSERT(dwNowTime >= m_dwStartTime);
	m_dwTimeCost = dwNowTime - m_dwStartTime;

	//2. Check if we should send progress information to client
	m_bPrgsTimeOut = ((dwNowTime - m_dwLastTime) > m_dwPrgsInterval);
	if(m_bPrgsTimeOut)
	{
		m_dwLastTime = dwNowTime;
	}

	//3. Check if we should update speed meters for the current speed calculation
	/* figure out the last updated data index, for faster speed show at startup, 
	only check when all the bucket has been updated at least once */
	int nLastIndexForSpeed = (m_nSpeederCount > BUCKET_COUNT) ? ((m_nSpeederCount - 1) % BUCKET_COUNT) : -1;
	if(nLastIndexForSpeed >= 0)
	{
		//Make sure the time difference is greater than m_dwSampleInterval ms
		if(dwNowTime - m_speeders[nLastIndexForSpeed].m_nTime < m_dwSampleInterval)
		{
			return FALSE;
		}
	}

	//4. Calculate the current speed
	int nCurrIndex = m_nSpeederCount % BUCKET_COUNT;
	m_speeders[nCurrIndex].m_nData = dwTotalNowData;
	m_speeders[nCurrIndex].m_nTime = dwNowTime;

	//Advance the speeder count
	m_nSpeederCount++;

	//Calculate the speed
	if(m_nSpeederCount > 1)
	{
		/* Get the index position to compare with the 'nCurrIndex' position.
		Get the oldest entry possible. While we have less than CURR_TIME
		entries, the first entry will remain the oldest. */
		int nOldestIndex = (m_nSpeederCount >= BUCKET_COUNT) ? (m_nSpeederCount % BUCKET_COUNT) : 0;

		ASSERT(m_speeders[nCurrIndex].m_nTime >= m_speeders[nOldestIndex].m_nTime);
		ASSERT(m_speeders[nCurrIndex].m_nData >= m_speeders[nOldestIndex].m_nData);

		DWORD dwTimeDelta = m_speeders[nCurrIndex].m_nTime - m_speeders[nOldestIndex].m_nTime;
		DWORD64 dwDataDelta = m_speeders[nCurrIndex].m_nData - m_speeders[nOldestIndex].m_nData;

		m_dwCurrSpeed = CaculateSpeed(dwDataDelta, dwTimeDelta);
	}	

	return TRUE;
}

DWORD CProgressMeter::GetCurrentSpeed()
{
	return m_dwCurrSpeed;
}

DWORD CProgressMeter::GetAverageSpeed()
{
	return CaculateSpeed(m_dwCurrentNow, m_dwTimeCost);
}

DWORD CProgressMeter::GetLeftTime()
{
	DWORD64 dwDataLeft = m_dwTotal - m_dwTotalNow;
	DWORD dwAvgSpeed = CaculateSpeed(m_dwCurrentNow, m_dwTimeCost, TRUE);

	if(dwAvgSpeed == 0)
	{
		dwAvgSpeed = 1;
	}
	DWORD64 dwTimeLeft = dwDataLeft / dwAvgSpeed;

	return (DWORD)dwTimeLeft;
}
DWORD CProgressMeter::GetCostTime()
{
	return m_dwTimeCost;
}
BOOL CProgressMeter::IsProgressTimeOut()
{
	return m_bPrgsTimeOut;
}

DWORD CProgressMeter::CaculateSpeed(DWORD64 dwDataDelta, DWORD dwTimeDelta, BOOL bMilliSeconds)
{
	if(dwTimeDelta == 0)
	{
		/* at least one millisecond MUST have passed */
		dwTimeDelta = 1;
	}
	DWORD64 dwSpeed;
	
	if(bMilliSeconds)
	{
		dwSpeed = dwDataDelta / dwTimeDelta;
	}
	else
	{
		dwSpeed = dwDataDelta * 1000 / dwTimeDelta;
	}

	return (DWORD)dwSpeed;
}