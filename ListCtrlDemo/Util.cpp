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

CFileInfo::CFileInfo() : m_nTotalLines(0), m_nCodeLines(0), m_nCommentLines(0), m_nBlankLines(0)
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

UINT CFileInfo::GetMixedLines() const
{
	return (m_nCodeLines + m_nCommentLines) - (m_nTotalLines - m_nBlankLines);
}

CString CFileInfo::ToString(UINT n) const
{
	CString str;
	str.Format("%d", n);
	return str;
}

// the following function based on a function by Jack Handy - you may find 
// his original article at: http://www.codeproject.com/useritems/wildcmp.asp
int wildcmp(const char *wild, const char *string) {
	const char *cp, *mp;
	
	while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}
	
	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}
	
	while (*wild == '*'  ||  *wild == '?') {
		wild++;
	}
	return !*wild;
}