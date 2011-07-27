// LicenseMgr.h: interface for the CLicenseMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LICENSEMGR_H__1F6F14FE_2C39_4043_A2A4_8A80EEFC48B9__INCLUDED_)
#define AFX_LICENSEMGR_H__1F6F14FE_2C39_4043_A2A4_8A80EEFC48B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLicenseMgr  
{
private:
	CLicenseMgr();
public:
	static CLicenseMgr* GetInstance();
	virtual ~CLicenseMgr();
	BOOL IsRegistered(BOOL bRefreshData = FALSE);
	BOOL GetLicenseInfo(CString& szName, CString& szCode);
	BOOL Register(const CString& szName, const CString& szCode);

	LONG WriteRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR pszData);
	LONG ReadRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, CString &szValue);

	LONG WriteRegData(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, const BYTE *pcbData, UINT nSize);
	LONG ReadRegData(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, BYTE **pcbData, DWORD *pdwSize);
private:
	BOOL m_bRegistered;
};

#endif // !defined(AFX_LICENSEMGR_H__1F6F14FE_2C39_4043_A2A4_8A80EEFC48B9__INCLUDED_)
