// RegistryKeyW.cpp: implementation of the CRegistryKeyW class.
//
//////////////////////////////////////////////////////////////////////

#include "RegistryKey.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistryKey::CRegistryKey() : m_hKey(NULL)
{

}

CRegistryKey::~CRegistryKey()
{
	if(m_hKey)
	{
		::RegCloseKey(m_hKey); 
	}
}

LONG CRegistryKey::Create(HKEY hKey, LPCWSTR lpSubKey)
{
	LONG lResult = ERROR_SUCCESS;
	//create the Key if it does not exist
	lResult = RegCreateKeyExW( hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, NULL );

	if( ERROR_SUCCESS != lResult )
	{
		m_hKey = NULL;
	}

	return lResult;
}

LONG CRegistryKey::Open(HKEY hKey, LPCWSTR lpSubKey, BOOL bReadOnly)
{
	LONG lResult = ERROR_SUCCESS;
	// Open the Key
	lResult = RegOpenKeyExW( hKey, lpSubKey, 0, bReadOnly ? KEY_READ : KEY_ALL_ACCESS, &m_hKey );
	if( ERROR_SUCCESS != lResult )
	{
		m_hKey = NULL;
	}
	return lResult;
}
LONG CRegistryKey::Delete(LPCWSTR lpValueName)
{
	if(m_hKey == NULL)
	{
		return ERROR_INVALID_HANDLE;
	}
	return ::RegDeleteValueW(m_hKey, lpValueName);
}

LONG CRegistryKey::SetData(LPCWSTR lpValueName, const BYTE *lpData, DWORD dwSize, DWORD dwValueType)
{
	if(m_hKey == NULL)
	{
		return ERROR_INVALID_HANDLE;
	}
	return RegSetValueExW( m_hKey, lpValueName, 0, dwValueType, lpData, dwSize);
}
LONG CRegistryKey::GetData(LPCWSTR lpValueName, BYTE **pcbData, DWORD *pdwSize)
{
	if(m_hKey == NULL)
	{
		return ERROR_INVALID_HANDLE;
	}

	// Query for needed buffer size
	LONG lResult = RegQueryValueExW( m_hKey, lpValueName, 0, NULL, NULL, pdwSize );
	if( ERROR_SUCCESS != lResult )
	{
		return lResult;
	}
	
	// Allocate a buffer
	*pcbData = new BYTE[ *pdwSize + sizeof(WCHAR) ];
	ZeroMemory( *pcbData, (*pdwSize + sizeof(WCHAR)) );
	
	// Query for the actual value
	lResult = RegQueryValueExW( m_hKey, lpValueName, 0, NULL, *pcbData, pdwSize );

	return lResult;
}

LONG CRegistryKey::SetBinary(LPCWSTR lpValueName, const BYTE *lpData, DWORD dwSize)
{
	return SetData(lpValueName, lpData, dwSize, REG_BINARY);
}
LONG CRegistryKey::GetBinary(LPCWSTR lpValueName, BYTE **lpData, DWORD *pdwSize)
{
	return GetData(lpValueName, lpData, pdwSize);
}

LONG CRegistryKey::SetString(LPCWSTR lpValueName, LPCWSTR lpszStr, DWORD dwValueType)
{
	const BYTE *pcbData = reinterpret_cast<const BYTE*>( lpszStr );
	DWORD dwSize = ( ::lstrlenW( lpszStr ) + 1 ) * sizeof( WCHAR );
	
	return SetData(lpValueName, pcbData, dwSize, dwValueType);
}

LONG CRegistryKey::GetString(LPCWSTR lpValueName, WCHAR **ppStr, DWORD * pdwSize)
{
	return GetData(lpValueName, (BYTE**)ppStr, pdwSize);
}

LONG CRegistryKey::SetString(LPCWSTR lpValueName, const std::wstring& strValue, DWORD dwValueType)
{
	return SetString(lpValueName, strValue.c_str(), dwValueType);
}

LONG CRegistryKey::GetString(LPCWSTR lpValueName, std::wstring& strValue)
{
	LPWSTR lpStr = NULL;
	DWORD dwSize = 0;

	LONG lResult = GetString(lpValueName, &lpStr, &dwSize);

	do 
	{
		if(lResult != ERROR_SUCCESS)
		{
			break;
		}
		
		if(lpStr != NULL)
		{
			strValue.assign(lpStr);
		}
	} while (FALSE);
	

	if(lpStr != NULL)
	{
		delete []lpStr;
		lpStr = NULL;
		
		dwSize = 0;
	}

	return lResult;
}

LONG CRegistryKey::SetDWORD(LPCWSTR lpValueName, DWORD dwValue)
{
	const BYTE *pcbData = reinterpret_cast<const BYTE*>( &dwValue );	
	return SetData(lpValueName, pcbData, sizeof(DWORD), REG_DWORD);
}
LONG CRegistryKey::GetDWORD(LPCWSTR lpValueName, LPDWORD lpValue)
{
	if(m_hKey == NULL)
	{
		return ERROR_INVALID_HANDLE;
	}

	DWORD dwSize = sizeof(DWORD);
	// Query for the actual value
	return RegQueryValueExW( m_hKey, lpValueName, 0, NULL, (LPBYTE)lpValue, &dwSize );
}

