// Environment.h: interface for the CEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVIRONMENT_H__5446C2DA_5DEE_48AB_870E_B79A4B49056E__INCLUDED_)
#define AFX_ENVIRONMENT_H__5446C2DA_5DEE_48AB_870E_B79A4B49056E__INCLUDED_

#include <windows.h>
#include <string>

class CEnvironment  
{
public:
	CEnvironment();
	virtual ~CEnvironment();

	virtual BOOL SetEnv(LPCWSTR lpName,	const std::wstring& strValue) = 0;
	virtual BOOL GetEnv(LPCWSTR lpName, std::wstring& strValue) = 0;

	virtual BOOL Append(LPCWSTR lpName,	const std::wstring& strValue);
	virtual BOOL Prepend(LPCWSTR lpName, const std::wstring& strValue);
};

#endif // !defined(AFX_ENVIRONMENT_H__5446C2DA_5DEE_48AB_870E_B79A4B49056E__INCLUDED_)
