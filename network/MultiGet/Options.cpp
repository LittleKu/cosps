// Options.cpp: implementation of the COptions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Options.h"
#include <memory>
#include "GenericTools.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COptions* SYS_OPTIONS()
{
	COptions* pPref =  COptions::GetInstance();
	ASSERT(pPref);
	return pPref;
}
COptions* COptions::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<COptions> ptr(new COptions);	
	return ptr.get();
}

COptions::COptions()
{
	m_nProxyMode = PME_SYS_PROXY;

	CreateDefaultFolders();

	if(m_nProxyMode == PME_SYS_PROXY)
	{
		CCommonUtils::GetProxyInfo(m_szProxy, CCommonUtils::PROXY_SERVER_HTTP);
	}
}

COptions::~COptions()
{
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, "COptions::~COptions() called.")
}

BOOL COptions::Init()
{
	
	return TRUE;
}


BOOL COptions::CreateDefaultFolders()
{
	TCHAR szPath[MAX_PATH];
	HRESULT hResult = 0;

	CString szTemp, szLog;
	
	//1. App Data
	hResult = ::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath);
	if(SUCCEEDED(hResult))
	{
		szTemp.Format("%s\\%s", szPath, THE_APP_NAME);
		if(!::PathFileExists(szTemp))
		{
			if(!CreateDirectory(szTemp, NULL))
			{
				szLog = CGenericTools::LastErrorStr("CreateDirectory", szTemp);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				return FALSE;
			}
		}
		m_szTempFolder = szTemp;
	}

	//2. My Documents
	hResult = ::SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath);
	if(SUCCEEDED(hResult))
	{
		szTemp.Format("%s\\%s", szPath, THE_APP_NAME);
		if(!::PathFileExists(szTemp))
		{
			if(!CreateDirectory(szTemp, NULL))
			{
				szLog = CGenericTools::LastErrorStr("CreateDirectory", szTemp);
				LOG4CPLUS_ERROR_STR(ROOT_LOGGER, (LPCTSTR)szLog)
					
				return FALSE;
			}
		}
		m_szSaveToFolder = szTemp;
	}

	return TRUE;
}