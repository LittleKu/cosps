// MEncoderConverter.cpp: implementation of the MEncoderConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MEncoderConverter.h"
#include <assert.h>
#include <boost/regex.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const std::string szDurationPat = "([\\s]*)Duration: (.*?), start: (.*?),(.*?)";
static const std::string szProgressPat = "(.*?)size=(.*?) time=(.*?) bitrate=(.*?)";

MEncoderConverter::ErrParser::ErrParser(HWND hRspWnd)
{
	m_hRspWnd = hRspWnd;
	m_nState = EP_INIT;
}

void MEncoderConverter::ErrParser::Reset()
{
	m_nState = EP_INIT;
}
void MEncoderConverter::ErrParser::ParseContent(std::string& szLine, int nLineCount)
{
	//TODO
}

bool MEncoderConverter::ErrParser::GetRegexVal(const std::string& str, const std::string szPattern, int groupCount, int groupIndex, std::string& szVal)
{
	bool bResult = false;
	try
	{
		boost::regex pat( szPattern );
		boost::smatch matches;
		
		if(boost::regex_match(str, matches, pat))
		{
			assert(matches.size() == groupCount);
			if(matches.size() == groupCount)
			{
				szVal = matches[groupIndex];
				
				std::string szLog, szFmt;
				for(int i = 0; i < matches.size(); i++)
				{
					if(i > 0)
					{
						szLog.append(",");
					}
					cfl::format(szFmt, "[%d]=[", i);
					szLog.append(szFmt).append(matches[i]).append("]");
				}
				CFL_TRACEA("%s", szLog.c_str());
				
				bResult = true;
			}
		}
	}
	catch(const boost::regex_error& r)
	{
		CFL_TRACEA("regex_error: %s", r.what());
	}
	catch(...)
	{
		CFL_TRACEA("Unexpected exception of unknown type");
	}
	return bResult;
}

bool MEncoderConverter::ErrParser::GetDuration(std::string& szLine, std::string& szDuration)
{
	return GetRegexVal(szLine, szDurationPat, 5, 2, szDuration);
}
bool MEncoderConverter::ErrParser::GetProgress(std::string& szLine, std::string& szProgress)
{
	return GetRegexVal(szLine, szProgressPat, 5, 3, szProgress);
}

DWORD WINAPI MEncoderConverter::PlayerThreadProc(LPVOID lpParameter)
{
	MEncoderConverter* pThis = (MEncoderConverter*)lpParameter;
	return pThis->PlayerProc();
}
DWORD WINAPI MEncoderConverter::StdOutThreadProc(LPVOID lpParameter)
{
	MEncoderConverter* pThis = (MEncoderConverter*)lpParameter;
	return pThis->StdOutProc();
}
DWORD WINAPI MEncoderConverter::StdErrThreadProc(LPVOID lpParameter)
{
	MEncoderConverter* pThis = (MEncoderConverter*)lpParameter;
	return pThis->StdErrProc();
}

MEncoderConverter::MEncoderConverter()
{
	m_hConverterThread = NULL;
	m_hOutThread = NULL;
	m_hErrThread = NULL;
	
	m_pMsgQueue = NULL;
	m_pProcess = NULL;
	m_pOutParser = NULL;
	m_pErrParser = NULL;
	
	m_hRspWnd = NULL;
	
	m_outReceiver.Reset();
	m_errReceiver.Reset();
}

MEncoderConverter::~MEncoderConverter()
{
	if(m_pOutParser != NULL)
	{
		delete m_pOutParser;
		m_pOutParser = NULL;
	}
	if(m_pErrParser != NULL)
	{
		delete m_pErrParser;
		m_pErrParser = NULL;
	}
}

void MEncoderConverter::SetResponseWnd(HWND hWnd)
{
	m_hRspWnd = hWnd;
	if(m_pOutParser != NULL)
	{
		delete m_pOutParser;
		m_pOutParser = NULL;
	}
	m_pOutParser = new OutParser(m_hRspWnd);
	
	if(m_pErrParser != NULL)
	{
		delete m_pErrParser;
		m_pErrParser = NULL;
	}
	m_pErrParser = new ErrParser(m_hRspWnd);
}

/* Create the message queue. 
 * If the function succeeds, the return value is 0. otherwise nonzero. 
 */
int MEncoderConverter::Create()
{
	if(m_pMsgQueue != NULL)
	{
		CFL_TRACEA("[Create]: already exising");
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
int MEncoderConverter::Destroy()
{
	if(m_pMsgQueue == NULL)
	{
		return 0;
	}
	int rc = 0;
	//if still playing, try to stop. TODO

	rc = m_pMsgQueue->Destroy();

	delete m_pMsgQueue;
	m_pMsgQueue = NULL;

	return rc;
}

/* Send message to the queue, and wait until the message to be processed */
LRESULT MEncoderConverter::SendMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pMsgQueue->SendMsg(uMsg, wParam, lParam);
}

/* Post message to the queue, returns without waiting for the thread to process the message */
BOOL MEncoderConverter::PostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pMsgQueue->PostMsg(uMsg, wParam, lParam);
}

/* Implement the MsgHandler to process messages in the MsgQueue */
LRESULT MEncoderConverter::ProcessMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = 0;
	switch (uMsg)
	{
	case REQ_START_PROCESS:
		{
			lr = Convert((ConvertParam*)lParam);
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
			if(m_hConverterThread)
			{
				::WaitForSingleObject(m_hConverterThread, INFINITE);
				::CloseHandle(m_hConverterThread);
				m_hConverterThread = NULL;
			}	
			
			if(m_pProcess != NULL)
			{
				delete m_pProcess;
				m_pProcess = NULL;
			}
			
			::PostMessage(m_hRspWnd, NOTIFY_PROCESS_END, wParam, lParam);
		}
		break;
	default:
		{
			CFL_TRACEA("Unkown msg: %u\n", uMsg);
		}
		break;
	}
	
	return lr;
}

int MEncoderConverter::Convert(ConvertParam* lpParam)
{
	int rc = 0;
	do 
	{
		//already started
		if(m_pProcess != NULL)
		{
			break;
		}

		m_szCmdLine = lpParam->szCmdLine;

		CFL_TRACE(_T("Cmd: [%s]"), m_szCmdLine.c_str());
		
		//start player thread
		m_hConverterThread = ::CreateThread(NULL, 0, PlayerThreadProc, this, 0, NULL);
		if(m_hConverterThread == NULL)
		{
			CFL_TRACEA("CreateThread m_hConverterThread failed");
			rc = CREATE_THREAD_FAILED;
			break;
		}
		
	} while (FALSE);

	//used over
	delete lpParam;
	lpParam = NULL;

	if(rc != 0)
	{
		//convert failed
		::PostMessage(m_hRspWnd, REQ_START_PROCESS, 0, (LPARAM)0);
	}

	return rc;
}

DWORD MEncoderConverter::PlayerProc()
{
	assert(m_pProcess == NULL && m_hOutThread == NULL && m_hErrThread == NULL);
	
	//child process
	m_pProcess = new cfl::WinProcess(m_szCmdLine.c_str());
	m_pProcess->SetStdOutDataReceiver(&m_outReceiver);
	m_pProcess->SetStdErrDataReceiver(&m_errReceiver);
	
	int rc = -1, nExitCode = 0;
	do 
	{
		//stdout thread
		m_hOutThread = ::CreateThread(NULL, 0, StdOutThreadProc, this, 0, NULL);
		if(m_hOutThread == NULL)
		{
			CFL_TRACEA("CreateThread stdout failed");
			break;
		}
		
		//stderr thread
		m_hErrThread = ::CreateThread(NULL, 0, StdErrThreadProc, this, 0, NULL);
		if(m_hErrThread == NULL)
		{
			CFL_TRACEA("CreateThread stderr failed");
			break;
		}
		
		rc = 0;
		
	} while (0);
	
	//send start player response
	::PostMessage(m_hRspWnd, RSP_START_PROCESS, 0, (rc == 0 ? (LPARAM)1 : (LPARAM)0));
	
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

DWORD MEncoderConverter::StdOutProc()
{
	return OutStreamProc(&m_outReceiver, m_pOutParser, _T("stdout"));
}
DWORD MEncoderConverter::StdErrProc()
{
	return OutStreamProc(&m_errReceiver, m_pErrParser, _T("stderr"));
}

DWORD MEncoderConverter::OutStreamProc(cfl::SyncBuffer* pBuffer, ContentParser* pParser, LPCTSTR szName)
{
	CFL_TRACE(_T("[%s]: thread enter"), szName);
	
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
			CFL_TRACEA("[%s]: failed to open", CFL_T2A(szName));
			break;
		}
		
		CFL_TRACEA("[%s]: thread start to read line", CFL_T2A(szName));
		while( (nByteCount = pBuffer->ReadLine(szLine)) >= 0 )
		{
			nLineCount++;
			fwrite(szLine.data(), 1, szLine.size(), fp);
			fwrite("\n", 1, 1, fp);
			
			//parse
			pParser->ParseContent(szLine, nLineCount);
		}
		CFL_TRACEA("[%s]: thread read line done. nLineCount=%d", CFL_T2A(szName), nLineCount);
		
	} while (FALSE);
	
	if(fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	
	CFL_TRACEA("[%s]: thread exit", CFL_T2A(szName));
	
	return 817;
}
