// SyncProcessExecutor.h: interface for the SyncProcessExecutor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCPROCESSEXECUTOR_H__1BCC2DE2_C4EF_45B6_AA9C_CBF67B189BA6__INCLUDED_)
#define AFX_SYNCPROCESSEXECUTOR_H__1BCC2DE2_C4EF_45B6_AA9C_CBF67B189BA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cflbase/tstring.h"
#include "cflwin/WinProcess.h"
#include "cflwin/SyncBuffer.h"
#include "cflwin/MsgQueue.h"

class SyncProcessExecutor  
{
public:
	SyncProcessExecutor();
	virtual ~SyncProcessExecutor();

	int Execute(ExecArgument* pExecArg);

	enum ErrorCode
	{
		OK = 0,
		ALREADY_EXIST,
		INVALID_ARGUMENT,
		CREATE_THREAD_FAILED,
		RECV_WM_QUIT,
		WAIT_ABNORMAL
	};
private:
	int WaitForExec();

	static DWORD WINAPI ExeThreadProc(LPVOID lpParameter);
	static DWORD WINAPI OutThreadProc(LPVOID lpParameter);
	static DWORD WINAPI ErrThreadProc(LPVOID lpParameter);
	
	DWORD ExeProc();
	DWORD OutProc();
	DWORD ErrProc();
	
	DWORD OutStreamProc(cfl::SyncBuffer* pBuffer, ContentParser* pParser, LPCTSTR szName);
	
private:
	cfl::WinProcess*	m_pProcess;		
	HANDLE				m_hExeThread;
	HANDLE				m_hOutThread;
	HANDLE				m_hErrThread;
	
	cfl::SyncBuffer		m_outReceiver;
	cfl::SyncBuffer		m_errReceiver;

	ExecArgument*		m_pExecArg;
};

#endif // !defined(AFX_SYNCPROCESSEXECUTOR_H__1BCC2DE2_C4EF_45B6_AA9C_CBF67B189BA6__INCLUDED_)
