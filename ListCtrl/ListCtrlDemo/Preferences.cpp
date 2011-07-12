#include "StdAfx.h"
#include "Preferences.h"
#include <memory>
#include "ListCtrlDemo.h"
#include "IniFileReadWrite.h"

CPreferences* SYS_PREF()
{
	CPreferences* pPref =  CPreferences::GetInstance();
	ASSERT(pPref);
	return pPref;
}
CPreferences* CPreferences::GetInstance()
{
	//This is the std way to make singleton
 	static std::auto_ptr<CPreferences> ptr(new CPreferences);	
 	return ptr.get();
}

CPreferences::CPreferences()
{
	m_bSearchIncludeSubFolders = TRUE;
	m_nMaxItemsInFilterComboBox = 10;
}

CPreferences::~CPreferences()
{
	LOG4CPLUS_INFO_STR(ROOT_LOGGER, "CPreferences::~CPreferences() called.")
}

BOOL CPreferences::Init()
{
	LPCTSTR lpSectionName = _T("Options");

	CString szValue;
	int nValue;

	//m_bSearchIncludeSubFolders
	szValue = gtb::GetKeyValue(SYS_PREF_INI_FILE(), lpSectionName, "SearchIncludeSubFolders");
	if(!szValue.IsEmpty())
	{
		nValue = _ttoi(szValue);
		m_bSearchIncludeSubFolders = (nValue != 0);
	}

	//m_nMaxItemsInFilterComboBox
	szValue = gtb::GetKeyValue(SYS_PREF_INI_FILE(), lpSectionName, "MaxItemsInFilterComboBox");
	if(!szValue.IsEmpty())
	{
		m_nMaxItemsInFilterComboBox = _ttoi(szValue);
	}

	return TRUE;
}

BOOL CPreferences::Save()
{
	LPCTSTR lpSectionName = _T("Options");
	
	CString szValue;

	szValue.Format("%d", m_bSearchIncludeSubFolders);
	::WritePrivateProfileString(lpSectionName, "SearchIncludeSubFolders", szValue, SYS_PREF_INI_FILE());

	szValue.Format("%d", m_nMaxItemsInFilterComboBox);
	::WritePrivateProfileString(lpSectionName, "MaxItemsInFilterComboBox", szValue, SYS_PREF_INI_FILE());

	return TRUE;
}