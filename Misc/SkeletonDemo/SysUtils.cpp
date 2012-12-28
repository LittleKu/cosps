// SysUtils.cpp: implementation of the SysUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysUtils.h"
#include "SkeletonDemo.h"
#include <shlwapi.h>
#include "cflbase/FileUtils.h"
#include "cflbase/tstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SysUtils::SysUtils()
{

}

SysUtils::~SysUtils()
{

}

bool SysUtils::GetDataFileFQPath(CString& szFQPath, LPCTSTR lpFileName, LPCTSTR lpSubDir, int nMode, LPCTSTR lpBaseDir)
{
	if(lpBaseDir == NULL)
	{
		lpBaseDir = SYS_APP()->m_szWorkDir;
	}
	if(nMode == FQPM_AUTO)
	{
		szFQPath.Format(_T("%s\\dat\\usr\\%s\\%s"), lpBaseDir, lpSubDir, lpFileName);

		if(!::PathFileExists(szFQPath))
		{
			szFQPath.Format(_T("%s\\dat\\default\\%s\\%s"), lpBaseDir, lpSubDir, lpFileName);
		}
	}
	else if(nMode == FQPM_USR)
	{
		szFQPath.Format(_T("%s\\dat\\usr\\%s\\%s"), lpBaseDir, lpSubDir, lpFileName);
	}
	else if(nMode == FQPM_DEFAULT)
	{
		szFQPath.Format(_T("%s\\dat\\default\\%s\\%s"), lpBaseDir, lpSubDir, lpFileName);
	}
	return true;
}

bool SysUtils::GetProfile(CString& szFQPath, LPCTSTR lpFileName, int nMode)
{
	return GetDataFileFQPath(szFQPath, lpFileName, _T("profile"), nMode, NULL);
}

bool SysUtils::GetBinFile(CString& szFQPath, LPCTSTR lpFileName)
{
	szFQPath.Format(_T("%s\\bin\\%s"), (LPCTSTR)SYS_APP()->m_szWorkDir, lpFileName);
	return true;
}

bool SysUtils::BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileExt, 
								  const char* szSrcFile, bool bHasQuote)
{
	std::string szTemp;
	cfl::FileUtilsA::GetFileNameOnly(szTemp, szSrcFile);
	
	if(szOutputFolder != NULL && szOutputFolder[0] != '\0')
	{
		cfl::format(str, "%s\\%s%s", szOutputFolder, szTemp.c_str(), szOutputFileExt);
		int nCount = 0;
		while(::PathFileExistsA(str.c_str()))
		{
			nCount++;
			cfl::format(str, "%s\\%s_%d%s", szOutputFolder, szTemp.c_str(), nCount, szOutputFileExt);
		}
	}
	else
	{
		cfl::format(str, "%s%s", szTemp.c_str(), szOutputFileExt);
		int nCount = 0;
		while(::PathFileExistsA(str.c_str()))
		{
			nCount++;
			cfl::format(str, "%s_%d%s", szTemp.c_str(), nCount, szOutputFileExt);
		}
	}
	cfl::format(str, "\"%s\"", str.c_str());
	return true;
}

bool SysUtils::BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileName, 
								  bool bHasQuote)
{
	if(szOutputFolder != NULL && szOutputFolder[0] != '\0')
	{
		const char* fmt = bHasQuote ? "\"%s\\%s\"" : "%s\\%s";
		cfl::format(str, fmt, szOutputFolder, szOutputFileName);
	}
	else
	{
		const char* fmt = bHasQuote ? "\"%s\"" : "%s";
		cfl::format(str, fmt, szOutputFileName);
	}
	return true;
}

bool SysUtils::Val2Str(const _variant_t& var, std::string& str)
{
	bool bDone = true;
	switch(var.vt)   
	{
	case VT_BSTR:
	case VT_LPSTR:
	case VT_LPWSTR:
		try
		{
			str.assign((LPCSTR)(_bstr_t)var);
		}
		catch (...)
		{
			bDone = false;
			opt_msg(OPT_LL_WARN, "Val2Str failed\n");
		}
		break;
	case VT_EMPTY:
		str.erase();
		break;
	default:
		bDone = false;
		break;
	}
	return bDone;
}
bool SysUtils::Val2WStr(const _variant_t& var, std::wstring& str)
{
	bool bDone = true;
	switch(var.vt)   
	{
	case VT_BSTR:
	case VT_LPSTR:
	case VT_LPWSTR:
		try
		{
			str.assign((LPCWSTR)(_bstr_t)var);
		}
		catch (...)
		{
			bDone = false;
			opt_msg(OPT_LL_WARN, "Val2WStr failed\n");
		}
		break;
	case VT_EMPTY:
		str.erase();
		break;
	default:
		bDone = false;
		break;
	}
	return bDone;
}
