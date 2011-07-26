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
	return FALSE;
}

BOOL CLicenseMgr::Register(const CString& szName, const CString& szCode)
{
	BOOL bResult = CRegKeyMgr::CheckKey(szName, szCode);
	return bResult;
}

BOOL CLicenseMgr::GetLicenseInfo(CString& szName, CString& szCode)
{
	szName.Format("%s", "TestUser1");
	szCode.Format("123456-7890AB-CDEFGH-IJKLMN-OPQRST-UVWXYZ");
	return TRUE;
}