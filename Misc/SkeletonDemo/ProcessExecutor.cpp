// ProcessExecutor.cpp: implementation of the ProcessExecutor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProcessExecutor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("ProcessExecutor"))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExecArgument::ExecArgument() : pOutParser(NULL), pErrParser(NULL)
{
}
ExecArgument::~ExecArgument()
{
	if(pOutParser)
	{
		delete pOutParser;
		pOutParser = NULL;
	}
	if(pErrParser)
	{
		delete pErrParser;
		pErrParser = NULL;
	}
}

DWORD WINAPI ProcessExecutor::ExeThreadProc(LPVOID lpParameter)
{
	ProcessExecutor* pThis = (ProcessExecutor*)lpParameter;
	return pThis->ExeProc();
}
DWORD WINAPI ProcessExecutor::OutThreadProc(LPVOID lpParameter)
{
	ProcessExecutor* pThis = (ProcessExecutor*)lpParameter;
	return pThis->OutProc();
}
DWORD WINAPI ProcessExecutor::ErrThreadProc(LPVOID lpParameter)
{
	ProcessExecutor* pThis = (ProcessExecutor*)lpParameter;
	return pThis->ErrProc();
}

ProcessExecutor::ProcessExecutor()
{
	m_pMsgQueue = NULL;
	m_pProcess = NULL;
	m_hExeThread = NULL;
	m_hOutThread = NULL;
	m_hErrThread = NULL;
	
	m_hRspWnd = NULL;
	m_pExecArg = NULL;
}

ProcessExecutor::~ProcessExecutor()
{
	ASSERT(m_pExecArg == NULL);
}

void ProcessExecutor::SetResponseWnd(HWND hWnd)
{
	m_hRspWnd = hWnd;
}

/* Create the message queue. 
 * If the function succeeds, the return value is 0. otherwise nonzero. 
 */
int ProcessExecutor::Create()
{
	if(m_pMsgQueue != NULL)
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("MsgQueue already existing"));
		LOG4CPLUS_WARN_STR(THE_LOGGER, szLog)

		return ALREADY_EXIST;
	}

	int rc = 0;

	m_pMsgQueue = new cfl::MsgQueue(cfl::MsgQueue::MQIMPL_DEFAULT);
	m_pMsgQueue->SetMsgHandler(this);

	rc =  m_pMsgQueue->Create();

	return rc;
}

/* Destroy the message queue.
 * If the function succeeds, the return value is 0. otherwise nonzero. 
 */
int ProcessExecutor::Destroy()
{
	if(m_pMsgQueue == NULL)
	{
		return 0;
	}

	int rc = 0;

	//if the executor thread is still running, try to stop it.
	rc = m_pMsgQueue->Destroy();

	delete m_pMsgQueue;
	m_pMsgQueue = NULL;

	return rc;
}

/* Send message to the queue, and wait until the message to be processed */
LRESULT ProcessExecutor::SendMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pMsgQueue->SendMsg(uMsg, wParam, lParam);
}

/* Post message to the queue, returns without waiting for the thread to process the message */
BOOL ProcessExecutor::PostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pMsgQueue->PostMsg(uMsg, wParam, lParam);
}

/* Implement the MsgHandler to process messages in the MsgQueue */
LRESULT ProcessExecutor::ProcessMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = 0;
	switch (uMsg)
	{
	case REQ_START_PROCESS:
		{
			lr = Execute((ExecArgument*)lParam);
		}
		break;
	case REQ_STOP_PROCESS:
		{
			//http://stackoverflow.com/questions/7018139/pyserial-how-to-send-ctrl-c-command-on-the-serial-line
			//http://en.wikipedia.org/wiki/ASCII
			if(m_pProcess != NULL)
			{
				m_pProcess->WriteToStdin("\x03", 1);
			}
		}
		break;
	case NOTIFY_PROCESS_END:
		{
			if(m_hExeThread)
			{
				::WaitForSingleObject(m_hExeThread, INFINITE);
				::CloseHandle(m_hExeThread);
				m_hExeThread = NULL;
			}	
			
			if(m_pProcess != NULL)
			{
				delete m_pProcess;
				m_pProcess = NULL;
			}
			
			ASSERT(m_pExecArg != NULL);
			delete m_pExecArg;
			m_pExecArg = NULL;

			::PostMessage(m_hRspWnd, NOTIFY_PROCESS_END, wParam, lParam);
		}
		break;
	default:
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Unkown msg: %u"), uMsg);
			LOG4CPLUS_WARN_STR(THE_LOGGER, szLog)
		}
		break;
	}
	
	return lr;
}

int ProcessExecutor::Execute(ExecArgument* pExecArg)
{
	if(pExecArg == NULL)
	{
		return INVALID_ARGUMENT;
	}

	int rc = 0;
	do 
	{
		//already started
		if(m_pProcess != NULL)
		{
			rc = ALREADY_EXIST;
			break;
		}
		
		m_pExecArg = pExecArg;
		
		//start executor thread
		m_hExeThread = ::CreateThread(NULL, 0, ExeThreadProc, this, 0, NULL);
		if(m_hExeThread == NULL)
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("CreateThread for executor failed"));
			LOG4CPLUS_ERROR_STR(THE_LOGGER, szLog)

			rc = CREATE_THREAD_FAILED;
			break;
		}
		
	} while (FALSE);
	
	if(rc != 0)
	{
		delete pExecArg;
		m_pExecArg = NULL;

		//convert failed
		::PostMessage(m_hRspWnd, RSP_START_PROCESS, 0, (LPARAM)rc);
	}
	
	return rc;
}

DWORD ProcessExecutor::ExeProc()
{
	ASSERT(m_pProcess == NULL && m_hOutThread == NULL && m_hErrThread == NULL);
	
	//child process
	m_pProcess = new cfl::WinProcess(m_pExecArg->szCmdLine.c_str());
	m_pProcess->SetStdOutDataReceiver(&m_outReceiver);
	m_pProcess->SetStdErrDataReceiver(&m_errReceiver);
	
	int rc = -1, nExitCode = 0;
	do 
	{
		//stdout thread
		m_hOutThread = ::CreateThread(NULL, 0, OutThreadProc, this, 0, NULL);
		if(m_hOutThread == NULL)
		{
			LOG4CPLUS_ERROR_STR(THE_LOGGER, _T("CreateThread stdout failed"))
			break;
		}
		
		//stderr thread
		m_hErrThread = ::CreateThread(NULL, 0, ErrThreadProc, this, 0, NULL);
		if(m_hErrThread == NULL)
		{
			LOG4CPLUS_ERROR_STR(THE_LOGGER, _T("CreateThread stderr failed"))
			break;
		}
		
		rc = 0;
		
	} while (0);
	
	//send start player response
	::PostMessage(m_hRspWnd, RSP_START_PROCESS, 0, (LPARAM)rc);
	
	if(rc == 0)
	{
		//wait until the child process to terminate
		nExitCode = m_pProcess->Run();
		
		//wait for std streams thread to terminate
		WaitForSingleObject(m_hErrThread, INFINITE);
		WaitForSingleObject(m_hOutThread, INFINITE);
	}
	
	BOOL bResult;
	if(m_hOutThread)
	{
		bResult = ::CloseHandle(m_hOutThread);
		m_hOutThread = NULL;
	}
	if(m_hErrThread)
	{
		bResult = ::CloseHandle(m_hErrThread);
		m_hErrThread = NULL;
	}
	
	//process exit
	if(rc == 0)
	{
		rc = nExitCode;
	}
	
	PostMsg(NOTIFY_PROCESS_END, 0, (LPARAM)rc);
	
	return rc;
}

DWORD ProcessExecutor::OutProc()
{
	return OutStreamProc(&m_outReceiver, m_pExecArg->pOutParser, 
		(m_pExecArg->szOutDumpFile.empty() ? _T("stdout") : m_pExecArg->szOutDumpFile.c_str()));
}
DWORD ProcessExecutor::ErrProc()
{
	return OutStreamProc(&m_errReceiver, m_pExecArg->pErrParser, 
		(m_pExecArg->szErrDumpFile.empty() ? _T("stderr") : m_pExecArg->szErrDumpFile.c_str()));
}

DWORD ProcessExecutor::OutStreamProc(cfl::SyncBuffer* pBuffer, ContentParser* pParser, LPCTSTR szName)
{
	cfl::tstring szLog;
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		cfl::tformat(szLog, _T("[%s]: thread enter"), szName);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, szLog)
	}
	
	int nByteCount = 0, nLineCount = 0;
	std::string szLine;
	FILE* fp = NULL;
	
	pBuffer->Reset();
	pParser->Reset();
	
	do 
	{
		fp = _tfopen(szName, _T("wb"));
		if(fp == NULL)
		{
			cfl::tformat(szLog, _T("[%s]: failed to open file [%s]"), szName, szName);
			LOG4CPLUS_WARN_STR(THE_LOGGER, szLog)
			break;
		}
		
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			cfl::tformat(szLog, _T("[%s]: thread start to read line"), szName);
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, szLog)
		}
		while( (nByteCount = pBuffer->ReadLine(szLine)) >= 0 )
		{
			nLineCount++;
			fwrite(szLine.data(), 1, szLine.size(), fp);
			fwrite("\n", 1, 1, fp);
			
			//parse
			pParser->ParseContent(szLine, nLineCount);
		}
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			cfl::tformat(szLog, _T("[%s]: thread read line done. nLineCount=%d"), szName, nLineCount);
			LOG4CPLUS_DEBUG_STR(THE_LOGGER, szLog)
		}
		
	} while (FALSE);
	
	if(fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		cfl::tformat(szLog, _T("[%s]: thread exit"), szName);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, szLog)
	}
	
	return 817;
}
