#ifndef _INI_FILE_READ_WRITE_H_
#define _INI_FILE_READ_WRITE_H_

namespace IniFile
{
	CString GetAllSectionNames(LPCTSTR lpFileName);
	CString GetAllKeyNames(LPCTSTR lpFileName, LPCTSTR lpSectionName);
	CString GetKeyValue(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyName);
}

#endif