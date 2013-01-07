#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#pragma once

#include "cflbase/tstring.h"

class CTaskInfo
{
public:
	UINT		mask;
	CString		m_szFileName;
	CString		m_szFormat;
	DWORD		m_nDuration;
	DWORD		m_nState;
	CString		m_szInfo;

	//internal data
	int			m_nTaskID;

	CTaskInfo()
	{
		Reset();
	}
	void Reset()
	{
		mask = 0;
		m_szFileName.Empty();
		m_szFormat.Empty();
		m_nDuration = 0;
		m_nState = 0;
		m_szInfo.Empty();
		m_nTaskID = -1;
	}
};

typedef enum _PROCESS_CMD_TAG
{
	/* Notify */
	//Process ended
	NOTIFY_PROCESS_END = WM_USER + 1000,
	//Process progress information
	NOTIFY_PROCESS_PROGRESS,

	/* Request */
	//Start
	REQ_START_PROCESS = WM_USER + 2000,
	//Stop
	REQ_STOP_PROCESS,
	//Pause
	REQ_PAUSE_PROCESS,

	/* Response */
	RSP_START_PROCESS = WM_USER + 3000,
	RSP_STOP_PROCESS,
	RSP_PAUSE_PROCESS

} ProcessCmdCode;

typedef struct _START_PLAYER_PARAM
{
	cfl::tstring szAppPath;
	cfl::tstring szInFile;
	HWND hWnd;
} StartPlayerParam;

typedef struct _CONVERT_PARAM
{
	cfl::tstring szCmdLine;
} ConvertParam;

class ContentParser
{
public:
	virtual ~ContentParser() {}
	virtual void Reset() {}
	virtual void ParseContent(std::string& szLine, int nLineCount) {}
};

#endif