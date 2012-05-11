// Environment.cpp: implementation of the CEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#include "Environment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnvironment::CEnvironment()
{
}

CEnvironment::~CEnvironment()
{
}

BOOL CEnvironment::Append(LPCWSTR lpName, const std::wstring& strValue)
{
	std::wstring szValue;

	GetEnv(lpName, szValue);
	szValue.append(strValue);

	return SetEnv(lpName, szValue);
}
BOOL CEnvironment::Prepend(LPCWSTR lpName, const std::wstring& strValue)
{
	std::wstring szValue;
	
	GetEnv(lpName, szValue);
	szValue.insert(0, strValue);
	
	return SetEnv(lpName, szValue);
}
