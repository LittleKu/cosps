// Preferences.h: interface for the CPreferences class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREFERENCES_H__02134B42_51EC_4552_A2F7_8C691BA11309__INCLUDED_)
#define AFX_PREFERENCES_H__02134B42_51EC_4552_A2F7_8C691BA11309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPreferences  
{
public:
	virtual ~CPreferences();
	static CPreferences* GetInstance();
	BOOL Load();
	BOOL Save();

public:
	CString szOutputFolder;
	CString	szTempFolder;

private:
	CPreferences();
};

CPreferences* SYS_PREF();

#endif // !defined(AFX_PREFERENCES_H__02134B42_51EC_4552_A2F7_8C691BA11309__INCLUDED_)
