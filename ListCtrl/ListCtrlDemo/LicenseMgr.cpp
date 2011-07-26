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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLicenseMgr* CLicenseMgr::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<CLicenseMgr> ptr(new CLicenseMgr);	
 	return ptr.get();
}
CLicenseMgr::CLicenseMgr()
{
}

CLicenseMgr::~CLicenseMgr()
{
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, "CLicenseMgr::~CLicenseMgr() called.")
}


BOOL CLicenseMgr::IsRegistered()
{
	LONG lResult = ERROR_SUCCESS;
	CString szName, szCode;
	lResult = ReadRegString(HKEY_LOCAL_MACHINE, "Software\\Code Project\\LicenseInfo", "Registration Name", szName);
	if(lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}
	lResult = ReadRegString(HKEY_LOCAL_MACHINE, "Software\\Code Project\\LicenseInfo", "Registration Code", szCode);
	if(lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	BOOL bResult = CRegKeyMgr::CheckKey(szName, szCode);
	if(!bResult)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLicenseMgr::Register(const CString& szName, const CString& szCode)
{
	BOOL bResult = CRegKeyMgr::CheckKey(szName, szCode);
	if(!bResult)
	{
		return FALSE;
	}

	LONG lResult;
	lResult = WriteRegString(HKEY_LOCAL_MACHINE, "Software\\Code Project\\LicenseInfo", "Registration Name", szName);
	lResult = WriteRegString(HKEY_LOCAL_MACHINE, "Software\\Code Project\\LicenseInfo", "Registration Code", szCode);

	return bResult;
}

BOOL CLicenseMgr::GetLicenseInfo(CString& szName, CString& szCode)
{
	LONG lResult;
	lResult = ReadRegString(HKEY_LOCAL_MACHINE, "Software\\Code Project\\LicenseInfo", "Registration Name", szName);
	if(lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}
	lResult = ReadRegString(HKEY_LOCAL_MACHINE, "Software\\Code Project\\LicenseInfo", "Registration Code", szCode);
	if(lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
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