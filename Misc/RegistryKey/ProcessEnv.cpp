// ProcessEnv.cpp: implementation of the CProcessEnv class.
//
//////////////////////////////////////////////////////////////////////

#include "ProcessEnv.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcessEnv::CProcessEnv()
{

}

CProcessEnv::~CProcessEnv()
{

}

BOOL CProcessEnv::SetEnv(LPCWSTR lpName, const std::wstring& strValue)
{
	return SetEnvironmentVariableW(lpName, strValue.c_str());
}
BOOL CProcessEnv::GetEnv(LPCWSTR lpName, std::wstring& strValue )
{
	DWORD dwSize = GetEnvironmentVariableW(lpName, NULL, 0);
	if(dwSize == 0)
	{
		return FALSE;
	}

	wchar_t* wchBuffer = new wchar_t[dwSize];
	GetEnvironmentVariableW(lpName, wchBuffer, dwSize);

	strValue = wchBuffer;
	delete [] wchBuffer;

	return TRUE;
}
