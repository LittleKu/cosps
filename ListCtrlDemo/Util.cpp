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

CFileInfo::CFileInfo()
{
}

void CFileInfo::SetFileName(LPCTSTR lpszFullFileName)
{
    TCHAR drive[MAX_PATH];
    TCHAR dir[MAX_PATH];
    TCHAR fname[MAX_PATH];
    TCHAR ext[MAX_PATH];

    m_sFullFileName = lpszFullFileName;

    _tsplitpath(m_sFullFileName, drive, dir, fname, ext);

    m_sFilePath = drive;
    m_sFilePath += dir;

	m_sFileExt = ext;

    m_sFileName = fname;
	m_sFileName += ext;
}