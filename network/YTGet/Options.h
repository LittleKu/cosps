// Options.h: interface for the COptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONS_H__A9712DD6_8395_464E_A9A4_F511D4EC521D__INCLUDED_)
#define AFX_OPTIONS_H__A9712DD6_8395_464E_A9A4_F511D4EC521D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum
{
	PME_NO_PROXY,
	PME_SYS_PROXY,
	PME_USER_PROXY
}ProxyMode;

class COptions  
{
private:
	COptions();
	BOOL CreateDefaultFolders();
public:
	virtual ~COptions();
	BOOL Init();
	LPCTSTR GetProxy();
public:
	static COptions* GetInstance();

	enum
	{
		QUALITY_COUNT = 3,
		FORMAT_COUNT = 4
	};

	static const char* POSSIBLE_QUALITIES[];
	static const char* POSSIBLE_FORMATS[];

	static void Swap(const char* pArray[], int x, int y);

	int	GetQualityIndex(LPCTSTR lpQuality);
	int GetFormatIndex(LPCTSTR lpFormat);
//Attributes
public:
	UINT	m_nProxyMode;
	CString	m_szProxy;

	CString	m_szTempFolder;
	CString m_szSaveToFolder;

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
