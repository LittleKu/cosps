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
	BOOL IsRegistered();
	BOOL GetLicenseInfo(CString& szName, CString& szCode);
	BOOL Register(const CString& szName, const CString& szCode);
};

#endif // !defined(AFX_LICENSEMGR_H__1F6F14FE_2C39_4043_A2A4_8A80EEFC48B9__INCLUDED_)
