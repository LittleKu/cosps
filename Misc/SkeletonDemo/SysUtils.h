// SysUtils.h: interface for the SysUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_)
#define AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(_countof)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#if !defined(ARRAY_LEN)
#define ARRAY_LEN(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

enum GetFileFQPathMode
{
	FQPM_AUTO	= 0,
	FQPM_USR,
	FQPM_DEFAULT
};

class SysUtils  
{
public:
	static bool GetDataFileFQPath(CString& szFQPath, LPCTSTR lpFileName, LPCTSTR lpSubDir, 
		int nMode = FQPM_AUTO, LPCTSTR lpBaseDir = NULL);

	static bool GetProfile(CString& szFQPath, LPCTSTR lpFileName, int nMode = FQPM_AUTO);

	static bool GetBinFile(CString& szFQPath, LPCTSTR lpFileName);

	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileExt, 
		const char* szSrcFile, bool bHasQuote = true);
	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileName, 
		bool bHasQuote = true);
	
	static bool Val2Str(const _variant_t& var, std::string& str);
	static bool Val2WStr(const _variant_t& var, std::wstring& str);

private:
	SysUtils();
	virtual ~SysUtils();
};

#endif // !defined(AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_)
