// Options.h: interface for the COptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONS_H__A9712DD6_8395_464E_A9A4_F511D4EC521D__INCLUDED_)
#define AFX_OPTIONS_H__A9712DD6_8395_464E_A9A4_F511D4EC521D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProperties;
class COptions  
{	
private:
	COptions();
	BOOL InitAppDataFolders();

	static void GetUInt(CProperties* pProp, LPCTSTR lpszKey, UINT* pResult, UINT nMin = 0, UINT nMax = 0xFFFFFFFF);
	static void GetProp(CProperties* pProp, LPCTSTR lpszKey, CString& szResult);
	static void Swap(const char* pArray[], int x, int y);
public:
	virtual ~COptions();
	BOOL Init();
	BOOL Save();

	int	GetQualityIndex(LPCTSTR lpQuality);
	int GetFormatIndex(LPCTSTR lpFormat);

	LPCTSTR GetProxy();
public:
	enum
	{
		PME_NO_PROXY,
		PME_USER_PROXY,
		PME_SYS_PROXY
	};
	static COptions* GetInstance();

	static const char* POSSIBLE_QUALITIES[];
	static const char* POSSIBLE_FORMATS[];
//Attributes
public:
	CString m_szAppDataFolder;
	CString m_szOptionFile;

	CString	m_szTempFolder;
	CString m_szSaveToFolder;

	UINT	m_nProxyMode;
	CString	m_szProxy;

	UINT	m_nMaxTaskCount;
	UINT	m_nMaxConnectionCount;
	UINT	m_nMinSegmentSize;
	UINT	m_nMaxRetryTimes;

	//Preferred output
	CString m_szQuality;
	CString m_szFormat;

//Debug options
	BOOL	m_bKeepTempFiles;
};

COptions* SYS_OPTIONS();

#endif // !defined(AFX_OPTIONS_H__A9712DD6_8395_464E_A9A4_F511D4EC521D__INCLUDED_)
