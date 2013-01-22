#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#pragma once

#include "cflbase/tstring.h"
#include "cfltemplate/TreeMap.h"

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

typedef cfl::TreeMap<int, std::string> MetaMap;

class CTaskInfo
{
public:
	UINT		mask;

	CString		m_szFileName;	/* Input file name */
	CString		m_szFormat;		/* Input file format */

	CString		m_szWidth;
	CString		m_szHeight;		
	DWORD		m_nDuration;	/* Input file duration */
	DWORD		m_nState;		/* Task state */
	double		m_dProgress;	/* Task progress */

	//internal data
	int			m_nTaskID;
	MetaMap*	m_pMetaMap;

	CTaskInfo();
	~CTaskInfo();
	void Reset();
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

class ContentParser
{
public:
	virtual ~ContentParser() {}
	//virtual void Reset() {}
	virtual void ParseContent(std::string& szLine, int nLineCount) {}
	virtual bool Init() { return true; }
	virtual bool DeInit() { return true; }
};


class ExecArgument
{
public:
	cfl::tstring	szCmdLine;		/* Process execute command line */
	cfl::tstring	szOutDumpFile;	/* Output stream dump file */
	cfl::tstring	szErrDumpFile;	/* Error stream dump file */
	ContentParser*	pOutParser;		/* Output stream content parser */
	ContentParser*	pErrParser;		/* Error stream content parser */
	
public:
	ExecArgument();
	~ExecArgument();
};


#endif