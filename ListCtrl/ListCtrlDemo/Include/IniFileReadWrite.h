#ifndef _INI_FILE_READ_WRITE_H_
#define _INI_FILE_READ_WRITE_H_

#include "gtbutil.h"

BEGIN_NAMESPACE(gtb)

AFX_EXT_API	CString GetAllSectionNames(LPCTSTR lpFileName);
AFX_EXT_API	CString GetAllKeyNames(LPCTSTR lpFileName, LPCTSTR lpSectionName);
AFX_EXT_API	CString GetKeyValue(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyName);

END_NAMESPACE()

#endif