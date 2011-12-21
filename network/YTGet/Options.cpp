// Options.cpp: implementation of the COptions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Options.h"
#include <memory>
#include "GenericTools.h"
#include "CommonUtils.h"
#include "Properties.h"

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
	InitAppDataFolders();

	m_nProxyMode = PME_SYS_PROXY;

	m_bKeepTempFiles = TRUE;

	m_nMaxTaskCount = 5;
	m_nMaxConnectionCount = 8;
	m_nMinSegmentSize = 1024 * 1024;
	m_nMaxRetryTimes = 5;

	m_szQuality = "large";
	m_szFormat = "mp4";
}

COptions::~COptions()
{
	Save();
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, "COptions::~COptions() called.")
}

void COptions::GetUInt(CProperties* pProp, LPCTSTR lpszKey, UINT* pResult, UINT nMin, UINT nMax)
{
	UINT uval;
	if(pProp->GetUInt(lpszKey, &uval))
	{
		if(uval < nMin)
		{
			uval = nMin;
		}
		else if(uval > nMax)
		{
			uval = nMax;
		}

		*pResult = uval;
	}
}

void COptions::GetProp(CProperties* pProp, LPCTSTR lpszKey, CString& szResult)
{
	CString szValue;
	if(pProp->GetProperty(lpszKey, szValue))
	{
		szResult = szValue;
	}
}

BOOL COptions::Init()
{
	CProperties prop;

	BOOL bResult = FALSE;
	do 
	{
		if(!prop.Load(m_szOptionFile))
		{
			break;
		}

		//Folders
		GetProp(&prop, _T("OutputFolder"), m_szSaveToFolder);
		GetProp(&prop, _T("TempFolder"), m_szTempFolder);
		
		//Connection
		GetUInt(&prop, _T("MaxTaskCount"), &m_nMaxTaskCount, 1, 10);
		GetUInt(&prop, _T("MaxConnectionCount"), &m_nMaxConnectionCount, 1, 16);
		GetUInt(&prop, _T("MinSegmentSize"), &m_nMinSegmentSize, 1024 * 1024);
		GetUInt(&prop, _T("MaxRetryTimes"), &m_nMaxRetryTimes, 1, 100);
		
		//Proxy
		GetUInt(&prop, _T("ProxyMode"), &m_nProxyMode, PME_NO_PROXY, PME_SYS_PROXY);
		if(m_nProxyMode == PME_USER_PROXY)
		{
			GetProp(&prop, _T("ProxyServer"), m_szProxy);
		}
		else if(m_nProxyMode == PME_SYS_PROXY)
		{
			CCommonUtils::GetProxyInfo(m_szProxy, CCommonUtils::PROXY_SERVER_HTTP);
		}

		//YTB
		GetProp(&prop, _T("YoutubeQuality"), m_szQuality);
		GetProp(&prop, _T("YoutubeFormat"), m_szFormat);

		//Success now
		bResult = TRUE;
	} while (FALSE);
	
	//Make sure the output and temp folders exist
	if(!::PathFileExists(m_szSaveToFolder))
	{
		TCHAR szPath[MAX_PATH];
		HRESULT hResult = ::SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath);
		if(SUCCEEDED(hResult))
		{
			CString szTemp;
			szTemp.Format("%s\\%s", szPath, THE_APP_NAME);
			CCommonUtils::VerifyDirectoryExist(szTemp);
			m_szSaveToFolder = szTemp;
		}
	}
	if(!::PathFileExists(m_szTempFolder))
	{
		CString szTemp;
		szTemp.Format("%s\\%s", m_szAppDataFolder, "DlData");
		CCommonUtils::VerifyDirectoryExist(szTemp);
		m_szTempFolder = szTemp;
	}

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

	return bResult;
}

BOOL COptions::Save()
{
	CProperties prop;

	//Folders
	prop.SetProperty(_T("OutputFolder"), m_szSaveToFolder);
	prop.SetProperty(_T("TempFolder"), m_szTempFolder);

	//Connection
	prop.SetUInt(_T("MaxTaskCount"), m_nMaxTaskCount);
	prop.SetUInt(_T("MaxConnectionCount"), m_nMaxConnectionCount);
	prop.SetUInt(_T("MinSegmentSize"), m_nMinSegmentSize);
	prop.SetUInt(_T("MaxRetryTimes"), m_nMaxRetryTimes);

	//Proxy
	prop.SetUInt(_T("ProxyMode"), m_nProxyMode);
	if(m_nProxyMode == PME_USER_PROXY)
	{
		prop.SetProperty(_T("ProxyServer"), m_szProxy);
	}

	//YTB
	prop.SetProperty(_T("YoutubeQuality"), m_szQuality);
	prop.SetProperty(_T("YoutubeFormat"), m_szFormat);
	
	return prop.Save(m_szOptionFile);
}


BOOL COptions::InitAppDataFolders()
{
	TCHAR szPath[MAX_PATH];
	HRESULT hResult = 0;
	BOOL bResult = FALSE;

	CString szTemp, szLog;
	
	//1. App Data
	hResult = ::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath);
	if(SUCCEEDED(hResult))
	{
		szTemp.Format("%s\\%s", szPath, THE_APP_NAME);
		bResult = CCommonUtils::VerifyDirectoryExist(szTemp);
		if(!bResult)
		{
			return FALSE;
		}
		m_szAppDataFolder = szTemp;

		//Option file
		m_szOptionFile.Format("%s\\options.ini", m_szAppDataFolder);
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