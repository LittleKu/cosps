// CommonUtils.cpp: implementation of the CCommonUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonUtils.h"
#include <curl/curl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommonUtils::CCommonUtils()
{

}

CCommonUtils::~CCommonUtils()
{

}

void CCommonUtils::FormatErrorMsg(DWORD dwCode, CString& szErrorMsg)
{
	WORD nMajor = LOWORD(dwCode);
	switch(nMajor)
	{
	case RC_MAJOR_OK:
		{
			szErrorMsg = "OK";
		}
		break;
	case RC_MAJOR_PAUSED:
		{
			szErrorMsg = "Paused";
		}
		break;
	case RC_MAJOR_STOPPED:
		{
			szErrorMsg = "Stopped";
		}
		break;
	case RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR:
		{
			FormatInternalErrorMsg(HIWORD(dwCode), szErrorMsg);
		}
		break;
	case RC_MAJOR_TERMINATED_BY_CURL_CODE:
		{
			szErrorMsg.Format("[Transfer Error]: %d - %s", HIWORD(dwCode), curl_easy_strerror((CURLcode)HIWORD(dwCode)));
		}
		break;
	default:
		{
			szErrorMsg.Format("Unkonwn error - %d", dwCode);
		}
		break;
	}
}

void CCommonUtils::FormatInternalErrorMsg(int nCode, CString& szErrorMsg)
{
	switch(nCode)
	{
	case RC_MINOR_OK:
		{
			szErrorMsg = "[Internal Error]: no error";
		}
		break;
	case RC_MINOR_MULTI_FDSET_ERROR:
		{
			szErrorMsg = "[Internal Error]: multi_fdset error";
		}
		break;
	case RC_MINOR_MULTI_TIMEOUT_ERROR:
		{
			szErrorMsg = "[Internal Error]: multi_timout error";
		}
		break;
	case RC_MINOR_SELECT_ERROR:
		{
			szErrorMsg = "[Internal Error]: select error";
		}
		break;
	case RC_MINOR_RETRY_OVER_ERROR:
		{
			szErrorMsg = "[Internal Error]: retry over error";
		}
		break;
	default:
		{
			szErrorMsg.Format("[Internal Error]: Unkonwn error - %d", nCode);
		}
		break;
	}
}
