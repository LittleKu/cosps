// Preferences.cpp: implementation of the CPreferences class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Preferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPreferences* SYS_PREF()
{
	CPreferences* pPref =  CPreferences::GetInstance();
	ASSERT(pPref);
	return pPref;
}

CPreferences* CPreferences::GetInstance()
{
	static CPreferences singleton;
	return &singleton;
}

CPreferences::CPreferences()
{
	szOutputFolder.Format(_T("C:\\Temp"));
}

CPreferences::~CPreferences()
{
	LOG4CPLUS_DEBUG_STR(ROOT_LOGGER, "CPreferences::~CPreferences() called.")
}
