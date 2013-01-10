// SyncProcessExecutor.cpp: implementation of the SyncProcessExecutor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SyncProcessExecutor.h"
#include "cflwin/ThreadUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("SyncProcessExecutor"))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD WINAPI SyncProcessExecutor::ExeThreadProc(LPVOID lpParameter)
{
	SyncProcessExecutor* pThis = (SyncProcessExecutor*)lpParameter;
	return pThis->ExeProc();
}
DWORD WINAPI SyncProcessExecutor::OutThreadProc(LPVOID lpParameter)
{
	SyncProcessExecutor* pThis = (SyncProcessExecutor*)lpParameter;
	return pThis->OutProc();
}
DWORD WINAPI SyncProcessExecutor::ErrThreadProc(LPVOID lpParameter)
{
	SyncProcessExecutor* pThis = (SyncProcessExecutor*)lpParameter;
	return pThis->ErrProc();
}

SyncProcessExecutor::SyncProcessExecutor()
{
	m_pProcess = NULL;
	m_hExeThread = NULL;
	m_hOutThread = NULL;
	m_hErrThread = NULL;
	
	m_pExecArg = NULL;
}

SyncProcessExecutor::~SyncProcessExecutor()
{
	ASSERT(m_pExecArg == NULL);
}

int SyncProcessExecutor::Execute(ExecArgument* pExecArg)
{
	if(pExecArg == NULL)
	{
		return INVALID_ARGUMENT;
	}

	//save the argument
	m_pExecArg = pExecArg;
	
	int rc = OK;
	do 
	{
		//1. Start exe thread
		m_hExeThread = ::CreateThread(NULL, 0, ExeThreadProc, this, 0, NULL);
		if(m_hExeThread == NULL)
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("CreateThread for executor failed"));
			LOG4CPLUS_ERROR_STR(THE_LOGGER, szLog)
				
			rc = CREATE_THREAD_FAILED;
			break;
		}

		//2. Wait exe to terminate
		rc = WaitForExec();

	} while (FALSE);
	
	//Clean up
	delete m_pExecArg;
	m_pExecArg = NULL;

	return rc;
}

int SyncProcessExecutor::WaitForExec()
{
	BOOL bRet;
	DWORD dwResult = 0;
	int rc = 0;

	do 
	{
		//check if received WM_QUIT message
		bRet = cfl::WndWaitForObjects(1, &m_hExeThread, INFINITE, &dwResult);
		if(!bRet)
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Received WM_QUIT message"));
			LOG4CPLUS_INFO_STR(THE_LOGGER, szLog)
				
			rc = RECV_WM_QUIT;
			break;
		}
		
		//input queue has data
		if(dwResult == WAIT_OBJECT_0 + 1)
		{
			continue;
		}
		
		//abnormal wait result
		if(dwResult != WAIT_OBJECT_0)
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Wait Result abnormal: %d"), dwResult);
			LOG4CPLUS_INFO_STR(THE_LOGGER, szLog)
				
			rc = WAIT_ABNORMAL;
			break;
		}
		else
		{
			//Player thread exit
			::CloseHandle(m_hExeThread);
			m_hExeThread = NULL;
			
			rc = OK;
			break;
		}
		
	} while (bRet);
	
	return rc;
}

DWORD SyncProcessExecutor::ExeProc()
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

	if(m_pProcess != NULL)
	{
		delete m_pProcess;
		m_pProcess = NULL;
	}
	
	//process exit
	if(rc == 0)
	{
		rc = nExitCode;
	}
	
	return rc;
}
DWORD SyncProcessExecutor::OutProc()
{
	return OutStreamProc(&m_outReceiver, m_pExecArg->pOutParser, 
		(m_pExecArg->szOutDumpFile.empty() ? _T("stdout") : m_pExecArg->szOutDumpFile.c_str()));
}
DWORD SyncProcessExecutor::ErrProc()
{
	return OutStreamProc(&m_errReceiver, m_pExecArg->pErrParser, 
		(m_pExecArg->szErrDumpFile.empty() ? _T("stderr") : m_pExecArg->szErrDumpFile.c_str()));
}

DWORD SyncProcessExecutor::OutStreamProc(cfl::SyncBuffer* pBuffer, ContentParser* pParser, LPCTSTR szName)
{
	cfl::tstring szLog;
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		cfl::tformat(szLog, _T("[%s]: thread enter"), szName);
		LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
	}
	
	int nByteCount = 0, nLineCount = 0;
	std::string szLine;
	bool bRet = false;
	
	pBuffer->Reset();
	
	do 
	{
		bRet = pParser->Init();
		if(!bRet)
		{
			cfl::tformat(szLog, _T("[%s]: parser init failed"), szName);
			LOG4CPLUS_ERROR_STR(THE_LOGGER, szLog)
			break;
		}
		
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			cfl::tformat(szLog, _T("[%s]: thread start to read line"), szName);
			LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
		}
		while( (nByteCount = pBuffer->ReadLine(szLine)) >= 0 )
		{
			nLineCount++;
			//parse
			pParser->ParseContent(szLine, nLineCount);
		}
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			cfl::tformat(szLog, _T("[%s]: thread read line done. nLineCount=%d"), szName, nLineCount);
			LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
		}
		
		bRet = true;
		
	} while (FALSE);
	
	//always call DeInit
	pParser->DeInit();
	
	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		cfl::tformat(szLog, _T("[%s]: thread exit"), szName);
		LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
	}
	
	return 817;
}
