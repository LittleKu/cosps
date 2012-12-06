#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#pragma once

class CTaskInfo
{
public:
	UINT		mask;
	CString		m_szFileName;
	CString		m_szFormat;
	DWORD		m_nDuration;
	DWORD		m_nState;

	//internal data
	int			m_nTaskID;

	CTaskInfo()
	{
		Reset();
	}
	void Reset()
	{
		mask = 0;
		m_szFileName.Empty();
		m_szFormat.Empty();
		m_nDuration = 0;
		m_nState = 0;
		m_nTaskID = -1;
	}
};

#endif