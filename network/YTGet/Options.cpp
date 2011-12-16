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

const char* COptions::POSSIBLE_QUALITIES[] = {"large", "medium", "small"};
const char* COptions::POSSIBLE_FORMATS[] = {"flv", "mp4", "webm"};

void COptions::Swap(const char* pArray[], int x, int y)
{
	const char* pTemp = pArray[x];
	pArray[x] = pArray[y];
	pArray[y] = pTemp;
}
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

	m_bKeepTempFiles = TRUE;

	m_nMaxConnectionCount = 8;
	m_nMinSegmentSize = 1024 * 1024;
	m_nMaxRetryTimes = 5;

	m_szQuality = "large";
	m_szFormat = "mp4";

	int i;
	for(i = 0; i < LENGTH_OF(POSSIBLE_QUALITIES); i++)
	{
		if(i != 0 && m_szQuality.Compare(POSSIBLE_QUALITIES[i]) == 0)
		{
			Swap(POSSIBLE_QUALITIES, 0, i);
			break;
		}
	}
	for(i = 0; i < LENGTH_OF(POSSIBLE_FORMATS); i++)
	{
		if(i != 0 && m_szFormat.Compare(POSSIBLE_FORMATS[i]) == 0)
		{
			Swap(POSSIBLE_FORMATS, 0, i);
			break;
		}
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

LPCTSTR COptions::GetProxy()
{
	if(m_nProxyMode == PME_SYS_PROXY || m_nProxyMode == PME_USER_PROXY)
	{
		if(!m_szProxy.IsEmpty())
		{
			return (LPCTSTR)m_szProxy;
		}
	}
	return NULL;
}

int	COptions::GetQualityIndex(LPCTSTR lpQuality)
{
	for(int i = 0; i < LENGTH_OF(POSSIBLE_QUALITIES); i++)
	{
		if(_tcscmp(lpQuality, POSSIBLE_QUALITIES[i]) == 0)
		{
			return i;
		}
	}
	return CCommonUtils::MAX_INTEGER;
}
int COptions::GetFormatIndex(LPCTSTR lpFormat)
{
	for(int i = 0; i < LENGTH_OF(POSSIBLE_FORMATS); i++)
	{
		if(_tcscmp(lpFormat, POSSIBLE_FORMATS[i]) == 0)
		{
			return i;
		}
	}
	return CCommonUtils::MAX_INTEGER;
}