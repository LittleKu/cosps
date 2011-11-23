// ProgressMeter.h: interface for the CProgressMeter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGRESSMETER_H__F82EB955_F7F9_4E46_92FB_5E40090246CE__INCLUDED_)
#define AFX_PROGRESSMETER_H__F82EB955_F7F9_4E46_92FB_5E40090246CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProgressMeter  
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
		CURR_TIME = 25		/* bucket count */
	};
	public:
		CProgressMeter(DWORD dwMinInterval = 200);
		virtual ~CProgressMeter();
	public:
		void Reset(DWORD dwMinInterval = 200);
		BOOL UpdateData(DWORD64 dwNowData, DWORD dwNowTime);
		DWORD GetSpeed();
	private:
		DWORD CaculateSpeed(DWORD64 dwDataDelta, DWORD dwTimeDelta);
	private:
		DWORD		m_dwMinInterval;
		DWORD		m_dwCurrSpeed;
		
		CSpeeder	m_speeders[CURR_TIME];
		int			m_nSpeederCount;
};

#endif // !defined(AFX_PROGRESSMETER_H__F82EB955_F7F9_4E46_92FB_5E40090246CE__INCLUDED_)
