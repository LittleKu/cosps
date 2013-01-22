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

#include <vector>

class SysUtils  
{
public:
	static LPCTSTR lpszInputFileFilter;
public:
	enum
	{
		SECONDS_IN_MINUTE	= 60,
		SECONDS_IN_HOUR		= 3600,
	};
	static bool GetDataFileFQPath(CString& szFQPath, LPCTSTR lpFileName, LPCTSTR lpSubDir, 
		int nMode = FQPM_AUTO, LPCTSTR lpBaseDir = NULL);

	static bool GetProfile(CString& szFQPath, LPCTSTR lpFileName, int nMode = FQPM_AUTO);

	static bool GetBinFile(CString& szFQPath, LPCTSTR lpFileName);

	/*
	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileExt, 
		const char* szSrcFile, bool bHasQuote = true);
	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileName, 
		bool bHasQuote = true);
		*/
	
	static bool Val2Str(const _variant_t& var, std::string& str);
	static bool Val2WStr(const _variant_t& var, std::wstring& str);

	static bool GetMatches(const std::string& str, const std::string szPattern, const int groupIndex[], 
		const int n, std::vector<std::string>* pResultVec = NULL);
	static bool GetMatch(const std::string& str, const std::string szPattern, int groupIndex, 
		std::string* pResult = NULL);

	static int GetLimitLength();

	static bool GetTaskStateText(int nTaskState, CString& rText);
	static bool Equals(double d1, double d2);
	static void FormatTime(UINT nSeconds, CString& rText);

private:
	SysUtils();
	virtual ~SysUtils();
};

#endif // !defined(AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_)
