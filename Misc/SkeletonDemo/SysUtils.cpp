// SysUtils.cpp: implementation of the SysUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysUtils.h"
#include "SkeletonDemo.h"
#include <shlwapi.h>
#include "cflbase/FileUtils.h"
#include "cflbase/tstring.h"
#include <boost/regex.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("SysUtils"))

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

/*
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
*/
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
			if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::WARN_LOG_LEVEL))
			{
				cfl::tstring szLog;
				cfl::tformat(szLog, _T("Val2Str failed"));
				LOG4CPLUS_WARN_STR(THE_LOGGER, szLog)
			}
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
			if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::WARN_LOG_LEVEL))
			{
				cfl::tstring szLog;
				cfl::tformat(szLog, _T("Val2WStr failed"));
				LOG4CPLUS_WARN_STR(THE_LOGGER, szLog)
			}
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


static bool GetMatches(const std::string& str, const std::string szPattern, boost::smatch& matches)
{
	bool bResult = false;
	try
	{
		boost::regex pat( szPattern );
		
		bResult = boost::regex_match(str, matches, pat);
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			cfl::tstring szLog;
			if(bResult)
			{
				cfl::tstring szTmp;
				szLog.append(_T("[Y]:"));
				for(int i = 0; i < matches.size(); i++)
				{
					if(i > 0)
					{
						szLog.append(_T(","));
					}
					cfl::tformat(szTmp, _T("[%d]=[%s]"), i, CFL_STRING_TO_T_STR(matches[i]));
					szLog.append(szTmp);
				}
			}
			else
			{
				szLog.assign(_T("[N]"));
			}
			LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
		}
	}
	catch(const boost::regex_error& r)
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("regex_error: %s"), CFL_C_STR_TO_T_STR(r.what()));
		LOG4CPLUS_ERROR_STR(THE_LOGGER, szLog)
	}
	catch(...)
	{
		LOG4CPLUS_ERROR_STR(THE_LOGGER, _T("Unexpected exception of unknown type"))
	}
	return bResult;
}

bool SysUtils::GetMatches(const std::string& str, const std::string szPattern, const int groupIndex[], 
		const int n, std::vector<std::string>* pResultVec)
{
	boost::smatch matches;

	bool bResult = ::GetMatches(str, szPattern, matches);
	if(bResult && pResultVec != NULL)
	{
		int i;
		if(n < 0)
		{
			for(i = 0; i < matches.size(); i++)
			{
				pResultVec->push_back(matches[i]);
			}
		}
		else
		{
			for(i = 0; i < n; i++)
			{
				pResultVec->push_back(matches[groupIndex[i]]);
			}
		}
	}

	return bResult;
}

bool SysUtils::GetMatch(const std::string& str, const std::string szPattern, int groupIndex, std::string* pResult)
{
	boost::smatch matches;
	
	bool bResult = ::GetMatches(str, szPattern, matches);
	if(bResult && pResult)
	{
		pResult->assign(matches[groupIndex]);
	}
	return bResult;
}

int SysUtils::GetLimitLength()
{
	//FIXME: false when release
	//return 20;
	return -1;
}

static const TCHAR* StatusText[] = 
{
	_T("Ready"),
	_T("Running"),
	_T("Stopped"),
	_T("Error"),
	_T("Done")
};
bool SysUtils::GetTaskStateText(int nTaskState, CString& rText)
{
	ASSERT(nTaskState >= 0 && nTaskState < TSE_COUNT);
	if(nTaskState < 0 || nTaskState >= TSE_COUNT)
	{
		return false;
	}
	rText = StatusText[nTaskState];
	return true;
}
bool SysUtils::Equals(double d1, double d2)
{
	double diff = d1 - d2;
	return (diff > -0.000001f) && (diff < 0.000001f);
}
