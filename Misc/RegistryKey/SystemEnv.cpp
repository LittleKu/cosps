// SystemEnv.cpp: implementation of the CSystemEnv class.
//
//////////////////////////////////////////////////////////////////////

#include "SystemEnv.h"
#include "RegistryKey.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPCWSTR CSystemEnv::lpszEnvironmentSubKey = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
//LPCWSTR CSystemEnv::lpszEnvironmentSubKey = L"Environment";

CSystemEnv::CSystemEnv(BOOL bAutoFlush) : m_bAutoFlush(bAutoFlush)
{
	m_pRegKey = new CRegistryKey();
	m_pRegKey->Open(HKEY_LOCAL_MACHINE, lpszEnvironmentSubKey, FALSE);
}

CSystemEnv::~CSystemEnv()
{
	if(m_bAutoFlush)
	{
		Flush();
	}
	
	if(m_pRegKey != NULL)
	{
		delete m_pRegKey;
		m_pRegKey = NULL;
	}
}

void CSystemEnv::Flush()
{
	DWORD dwReturnValue = 0;
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000, &dwReturnValue);
}

BOOL CSystemEnv::SetEnv(LPCWSTR lpName, const std::wstring& strValue)
{
	DWORD dwValueType = REG_SZ;
	//If there exist an '%', assume this value is REG_EXPAND_SZ
	if(strValue.find( (wchar_t)'%' ) != std::string::npos)
	{
		dwValueType = REG_EXPAND_SZ;
	}
	
	BOOL bResult = (m_pRegKey->SetString(lpName, strValue.c_str(), dwValueType) == ERROR_SUCCESS);
// 	if(bResult)
// 	{
// 		SetEnvironmentVariableW(lpName, strValue.c_str());
// 		Flush();
// 	}
	return bResult;
}
BOOL CSystemEnv::GetEnv(LPCWSTR lpName, std::wstring& strValue )
{
	return (m_pRegKey->GetString(lpName, strValue) == ERROR_SUCCESS);
}

BOOL CSystemEnv::Delete(LPCWSTR lpName)
{
	return (m_pRegKey->Delete(lpName) == ERROR_SUCCESS);
}
