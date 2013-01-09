#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#pragma once

#include "cflbase/tstring.h"

enum TaskStateEnum
{
	TSE_READY,
	TSE_RUNNING,
	TSE_STOPPED,
	TSE_ERROR,		/* some error happened, have the same effect as TSE_STOPPED */
	TSE_DONE,
	TSE_COUNT
};

enum TaskInfoFlag
{
	TIF_FILE_NAME	= (1 << 0),
	TIF_STATUS		= (1 << 1),
	TIF_PROGRESS	= (1 << 2),
	TIF_FORMAT		= (1 << 3),
	TIF_DURATION	= (1 << 4)
};

class CTaskInfo
{
public:
	UINT		mask;
	CString		m_szFileName;
	CString		m_szFormat;
	DWORD		m_nDuration;
	CString		m_szInfo;

	DWORD		m_nState;
	CString		m_szStatus;
	double		m_dProgress;

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
		m_nState = TSE_READY;
		m_szStatus.Empty();
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