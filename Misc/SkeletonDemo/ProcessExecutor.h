// ProcessExecutor.h: interface for the ProcessExecutor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSEXECUTOR_H__561285A2_55BB_43B4_A241_AE78683EF46E__INCLUDED_)
#define AFX_PROCESSEXECUTOR_H__561285A2_55BB_43B4_A241_AE78683EF46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cflbase/tstring.h"
#include "cflwin/WinProcess.h"
#include "cflwin/SyncBuffer.h"
#include "cflwin/MsgQueue.h"

/*
class ContentParser
{
public:
	virtual ~ContentParser() {}
	virtual void Reset() {}
	virtual void ParseContent(std::string& szLine, int nLineCount) = 0;
};
*/

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

class ProcessExecutor : public cfl::MsgHandler
{
public:
	enum ErrorCode
	{
		OK = 0,
		ALREADY_EXIST,
		INVALID_ARGUMENT,
		CREATE_THREAD_FAILED
	};
	ProcessExecutor();
	virtual ~ProcessExecutor();

	/* Set the Window Handle to which the response message should send */
	void SetResponseWnd(HWND hWnd);
	
	/* Create the message queue. 
	* If the function succeeds, the return value is 0. otherwise nonzero. 
	*/
	virtual int Create();
	
	/* Destroy the message queue.
	* If the function succeeds, the return value is 0. otherwise nonzero. 
	*/
	virtual int Destroy();
	
	/* Send message to the queue, and wait until the message to be processed */
	virtual LRESULT SendMsg(UINT uMsg, WPARAM wParam = NULL, LPARAM lParam = NULL);
	
	/* Post message to the queue, returns without waiting for the thread to process the message */
	virtual BOOL PostMsg(UINT uMsg, WPARAM wParam = NULL, LPARAM lParam = NULL);
	
	/* Implement the MsgHandler to process messages in the MsgQueue */
	virtual LRESULT ProcessMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int Execute(ExecArgument* pExecArg);

	static DWORD WINAPI ExeThreadProc(LPVOID lpParameter);
	static DWORD WINAPI OutThreadProc(LPVOID lpParameter);
	static DWORD WINAPI ErrThreadProc(LPVOID lpParameter);
	
	DWORD ExeProc();
	DWORD OutProc();
	DWORD ErrProc();
	
	DWORD OutStreamProc(cfl::SyncBuffer* pBuffer, ContentParser* pParser, LPCTSTR szName);
	
private:
	cfl::MsgQueue*		m_pMsgQueue;
	cfl::WinProcess*	m_pProcess;		
	HANDLE				m_hExeThread;
	HANDLE				m_hOutThread;
	HANDLE				m_hErrThread;
	
	cfl::SyncBuffer		m_outReceiver;
	cfl::SyncBuffer		m_errReceiver;

	HWND				m_hRspWnd;	/* Window to receive the notification message */
	ExecArgument*		m_pExecArg;
};

#endif // !defined(AFX_PROCESSEXECUTOR_H__561285A2_55BB_43B4_A241_AE78683EF46E__INCLUDED_)
