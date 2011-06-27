#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#pragma once

class CPreferences
{
private:
	CPreferences();
public:
	~CPreferences();
	BOOL Init();
	BOOL Save();
public:
	static CPreferences* GetInstance();

//Attributes
public:
	BOOL m_bSearchIncludeSubFolders;
	UINT m_nMaxItemsInFilterComboBox;
};

CPreferences* SYS_PREF();

#endif