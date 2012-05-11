// RegistryKeyW.h: interface for the CRegistryKeyW class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRYKEYW_H__C1763A13_E924_46DE_A404_DFE45D4F6C9A__INCLUDED_)
#define AFX_REGISTRYKEYW_H__C1763A13_E924_46DE_A404_DFE45D4F6C9A__INCLUDED_

#include <windows.h>
#include <string>

class CRegistryKey  
{
public:
	CRegistryKey();
	virtual ~CRegistryKey();

	// Keys creation/destruction
	LONG Create(HKEY hKey, LPCWSTR lpSubKey);
	LONG Open(HKEY hKey, LPCWSTR lpSubKey, BOOL bReadOnly = FALSE);
	LONG Delete(LPCWSTR lpValueName);
	
	//Read and Write raw data
	LONG SetData(LPCWSTR lpValueName, const BYTE *lpData, DWORD dwSize, DWORD dwValueType);
	
	//NOTE: lpData should be deleted by user
	LONG GetData(LPCWSTR lpValueName, BYTE **lpData, DWORD *pdwSize);
	
	//Binary
	LONG SetBinary(LPCWSTR lpValueName, const BYTE *lpData, DWORD dwSize);
	LONG GetBinary(LPCWSTR lpValueName, BYTE **lpData, DWORD *pdwSize);
	
	//String
	LONG SetString(LPCWSTR lpValueName, LPCWSTR lpszStr, DWORD dwValueType = REG_SZ);
	LONG GetString(LPCWSTR lpValueName, WCHAR **ppStr, DWORD *pdwSize);

	LONG SetString(LPCWSTR lpValueName, const std::wstring& strValue, DWORD dwValueType = REG_SZ);
	LONG GetString(LPCWSTR lpValueName, std::wstring& strValue);
	
	//DWORD
	LONG SetDWORD(LPCWSTR lpValueName, DWORD dwValue);
	LONG GetDWORD(LPCWSTR lpValueName, LPDWORD lpValue);
	
private:
	HKEY m_hKey;
};

#endif // !defined(AFX_REGISTRYKEYW_H__C1763A13_E924_46DE_A404_DFE45D4F6C9A__INCLUDED_)
