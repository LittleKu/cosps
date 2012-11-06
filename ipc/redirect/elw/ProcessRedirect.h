#ifndef __PROCESS_REDIRECT_H__
#define __PROCESS_REDIRECT_H__

#pragma once

#include "cflbase/Properties.h"
#include <windows.h>

class ProcessRedirect
{
public:
	ProcessRedirect(const char* cmdline, cfl::Properties* pProp);
	virtual ~ProcessRedirect();

	int Redirect();

private:
	BOOL RedirectStdHandle(DWORD nStdHandle, BOOL bOut, PHANDLE phOrig, PHANDLE phRead, PHANDLE phWrite);
	BOOL CreateChildProcess();
	FILE* CreateFile(LPCTSTR lpszKey);
private:
	enum
	{
		CMD_BUF_SIZE = 4096,
		ERR_STR_SIZE = 256
	};

	char m_szCmdLine[CMD_BUF_SIZE];
	char m_szErrorMsg[ERR_STR_SIZE];
	PROCESS_INFORMATION m_pi;

	cfl::Properties* m_prop;
};

#endif