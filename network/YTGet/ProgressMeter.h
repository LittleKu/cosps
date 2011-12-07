// ProgressMeter.h: interface for the CProgressMeter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGRESSMETER_H__F82EB955_F7F9_4E46_92FB_5E40090246CE__INCLUDED_)
#define AFX_PROGRESSMETER_H__F82EB955_F7F9_4E46_92FB_5E40090246CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProgressMeter : public CProgressMetric
{
private:
	class CSpeeder
	{
	public:
		DWORD64	m_nData;
		DWORD	m_nTime;
		CSpeeder() : m_nData(0), m_nTime(0) {}
	};
	enum
	{
		BUCKET_COUNT = 25		/* bucket count */
	};
	public:
		CProgressMeter(DWORD dwPrgsInterval = 100, DWORD dwSampleInterval = 200);
		virtual ~CProgressMeter();
	public:
		void	Reset(DWORD dwStartTime, DWORD64 dwTotal, DWORD64 dwTotalNow);
		BOOL	UpdateSample(DWORD dwNowTime, DWORD64 dwTotalNowData, DWORD64 dwCurrentNowData);

		DWORD	GetCurrentSpeed();
		DWORD	GetAverageSpeed();
		DWORD	GetLeftTime();
		DWORD	GetCostTime();
		BOOL	IsProgressTimeOut();
	private:
		DWORD	CaculateSpeed(DWORD64 dwDataDelta, DWORD dwTimeDelta, BOOL bMilliSeconds = FALSE);
	private:
		DWORD		m_dwPrgsInterval;	//Progress update interval
		DWORD		m_dwSampleInterval;	//Current speed calculation sample interval

		DWORD		m_dwStartTime;		//Start time
		DWORD		m_dwLastTime;		//Last progress update time		

		DWORD64		m_dwTotal;			//The total size of the task
		DWORD64		m_dwTotalNow;		//The total downloaded data size, including the previous connection
		DWORD64		m_dwCurrentNow;		//The downloaded data size of the current connection				
		
		CSpeeder	m_speeders[BUCKET_COUNT];
		int			m_nSpeederCount;

		DWORD		m_dwCurrSpeed;		//[Output]: by calculation
		DWORD		m_dwTimeCost;		//[Output]: How many milliseconds has been cost from beginning, by calculation
		BOOL		m_bPrgsTimeOut;		//[Output]: Should we update the progress now
};

#endif // !defined(AFX_PROGRESSMETER_H__F82EB955_F7F9_4E46_92FB_5E40090246CE__INCLUDED_)
