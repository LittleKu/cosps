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
public:
	static COptions* GetInstance();

//Attributes
public:
	UINT	m_nProxyMode;
	CString	m_szProxy;

	CString	m_szTempFolder;
	CString m_szSaveToFolder;

	UINT	m_nMaxConnectionCount;
	UINT	m_nMinSegmentSize;
	UINT	m_nMaxRetryTimes;
};

COptions* SYS_OPTIONS();

#endif // !defined(AFX_OPTIONS_H__A9712DD6_8395_464E_A9A4_F511D4EC521D__INCLUDED_)
