// LicenseMgr.cpp: implementation of the CLicenseMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LicenseMgr.h"
#include <memory>
#include "RegKeyMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define THE_REG_SUB_KEY					"Software\\" SZ_PRODUCT_NAME "\\Common"
#define THE_REG_LICENSE_VALUE_NAME		"UID"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLicenseMgr* CLicenseMgr::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<CLicenseMgr> ptr(new CLicenseMgr);	
 	return ptr.get();
}
CLicenseMgr::CLicenseMgr() : m_bRegistered(FALSE)
{
}

CLicenseMgr::~CLicenseMgr()
{
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, "CLicenseMgr::~CLicenseMgr() called.")
}


BOOL CLicenseMgr::IsRegistered(BOOL bRefreshData)
{
	if(bRefreshData)
	{
		CString szName, szCode;
		m_bRegistered = GetLicenseInfo(szName, szCode);
		if(!m_bRegistered)
		{
			return FALSE;
		}
		m_bRegistered = CRegKeyMgr::CheckKey(szName, szCode);
	}
	return m_bRegistered;
}

BOOL CLicenseMgr::Register(const CString& szName, const CString& szCode)
{
	BOOL bResult = CRegKeyMgr::CheckKey(szName, szCode);
	if(!bResult)
	{
		return FALSE;
	}

	BYTE* pBinaryStr = NULL;
	DWORD dwSize = 0;
	
	CRegKeyMgr::Serialize(szName, szCode, &pBinaryStr, &dwSize);
	ASSERT(pBinaryStr != NULL && dwSize > 0);

	LONG lResult;
	lResult = WriteRegData(HKEY_LOCAL_MACHINE, THE_REG_SUB_KEY, THE_REG_LICENSE_VALUE_NAME, pBinaryStr, dwSize);
	//Release data
	delete [] pBinaryStr;
	pBinaryStr = NULL;

	if(lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	m_bRegistered = TRUE;
	return m_bRegistered;
}

BOOL CLicenseMgr::GetLicenseInfo(CString& szName, CString& szCode)
{
	BYTE* pBinaryStr = NULL;
	DWORD dwSize = 0;

	BOOL bResult = FALSE;
	do 
	{
		LONG lResult = ReadRegData(HKEY_LOCAL_MACHINE, THE_REG_SUB_KEY, THE_REG_LICENSE_VALUE_NAME, 
			&pBinaryStr, &dwSize);
		if(lResult != ERROR_SUCCESS)
		{
			break;
		}

		bResult = CRegKeyMgr::Deserialize(pBinaryStr, dwSize, szName, szCode);

	} while (FALSE);

	//Release data
	if(pBinaryStr != NULL)
	{
		delete [] pBinaryStr;
		pBinaryStr = NULL;
	}

	return bResult;
}

LONG CLicenseMgr::WriteRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR pszData)
{
	LONG lResult = ERROR_SUCCESS;
    HKEY hKeyResult = NULL;
	do 
	{
		// When writing, create the Key if it does not exist
		lResult = RegCreateKeyEx( hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, 
			KEY_WRITE, NULL, &hKeyResult, NULL );
		if(lResult != ERROR_SUCCESS)
		{
			break;
		}

		// Paydirt
		lResult = RegSetValueEx( hKeyResult, lpValueName, 0, REG_SZ, 
			reinterpret_cast<const BYTE*>( pszData ),  ( ::lstrlen( pszData ) + 1 ) * sizeof( TCHAR ) );
	} while (FALSE);
	
	// Cleanup...
    if( NULL != hKeyResult ) 
	{ 
		RegCloseKey( hKeyResult ); 
	}
    return lResult;
}

LONG CLicenseMgr::ReadRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, CString &szValue)
{
    LONG lResult = ERROR_SUCCESS;
    HKEY hKeyResult = NULL;
    DWORD dwSize = 0;
    BYTE* pcbData = NULL;

	do 
	{
		// Open the key - do not create
		lResult = RegOpenKeyEx( hKey, lpSubKey, 0, KEY_READ, &hKeyResult );
		// Sanity Check
		if( ERROR_SUCCESS != lResult ) 
		{ 
			break; 
		}

		// Query for needed buffer size
		lResult = RegQueryValueEx( hKeyResult, lpValueName, 0, NULL, NULL, &dwSize );
		// Sanity Check
		if( ERROR_SUCCESS != lResult ) 
		{ 
			break; 
		}

		// Add '+ sizeof(TCHAR)' in case the string is not
		//   stored in the registry with a trailing '\0'
		pcbData = new BYTE[ dwSize + sizeof(TCHAR) ];
		// Sanity Check
		if( NULL == pcbData ) 
		{        
			lResult = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		// Add the trailing '\0', taking into account UNICODEness
		//reinterpret_cast<TCHAR*>(pcbData)[ dwSize/sizeof(TCHAR) - 1 ] = _T('\0');
		ZeroMemory( pcbData, (dwSize + sizeof(TCHAR)) );


		// Query for the actual value
		lResult = RegQueryValueEx( hKeyResult, lpValueName, 0, NULL, pcbData, &dwSize );
		// Sanity Check
		if( ERROR_SUCCESS != lResult ) 
		{ 
			break; 
		}

		// Paydirt...
		szValue = reinterpret_cast<TCHAR*>(pcbData);
	} while (FALSE);
	
	// Cleanup...
    if( NULL != hKeyResult ) { RegCloseKey( hKeyResult ); }
    if( NULL != pcbData ) { delete[] pcbData; }
	
    return lResult;
}

LONG CLicenseMgr::WriteRegData(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, const BYTE *pcbData, UINT nSize)
{
    LONG lResult = ERROR_SUCCESS;
    HKEY hKeyResult = NULL;
	do 
	{
		// When writing, create the Key if it does not exist
		lResult = RegCreateKeyEx( hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, 
			KEY_WRITE, NULL, &hKeyResult, NULL );
		// Sanity Check
		if( ERROR_SUCCESS != lResult ) { break; }

		// Paydirt
		lResult = RegSetValueEx( hKeyResult, lpValueName, 0, REG_BINARY, pcbData, nSize );

	} while (FALSE);	

    // Cleanup...
    if( NULL != hKeyResult ) { RegCloseKey( hKeyResult ); }
	
    return lResult;
}

LONG CLicenseMgr::ReadRegData(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, BYTE **pcbData, DWORD *pdwSize)
{
	ASSERT( NULL != pcbData && NULL != pdwSize );

	LONG lResult = ERROR_SUCCESS;
	HKEY hKeyResult = NULL;
	do 
	{
		// Sanity Check
		if( NULL == pcbData || NULL == pdwSize ) 
		{        
			lResult = ERROR_INVALID_PARAMETER;
			break;
		}

		// Open the Key
		lResult = RegOpenKeyEx( hKey, lpSubKey, 0, KEY_READ, &hKeyResult );
		if( ERROR_SUCCESS != lResult )
		{
			break;
		}

		// Query for needed buffer size
		lResult = RegQueryValueEx( hKeyResult, lpValueName, 0, NULL, NULL, pdwSize );
		if( ERROR_SUCCESS != lResult )
		{
			break;
		}

		// Allocate a buffer
		*pcbData = new BYTE[ *pdwSize + sizeof(TCHAR) ];
		// Sanity Check
		if( NULL == *pcbData ) 
		{
			lResult = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		// Query for the actual value
		lResult = RegQueryValueEx( hKeyResult, lpValueName, 0, NULL, *pcbData, pdwSize );
	} while (FALSE);

	if( NULL != hKeyResult ) 
	{ 
		RegCloseKey( hKeyResult ); 
	}

	return lResult;
}