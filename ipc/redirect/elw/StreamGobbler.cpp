#include "StreamGobbler.h"
#include "log4cplus_config.h"
#include <stdio.h>

DECLARE_THE_LOGGER_NAME("StreamGobbler")

DWORD WINAPI StreamGobbler::ThreadProc(LPVOID lpParameter)
{
	StreamGobbler* pThis = (StreamGobbler*)lpParameter;
	return pThis->ThreadProc();
}
StreamGobbler::StreamGobbler(HANDLE hIn, HANDLE hOut, const char* name) : m_hIn(hIn), m_hOut(hOut)
{
	if(name != NULL)
	{
		strcpy(m_szName, name);
	}
	else
	{
		m_szName[0] = '\0';
	}
}

StreamGobbler::~StreamGobbler()
{
}

HANDLE StreamGobbler::Start(LPDWORD lpThreadId)
{
	HANDLE hThread = ::CreateThread(NULL, 0, StreamGobbler::ThreadProc, (LPVOID)this, 0, lpThreadId);

	return hThread;
}

DWORD StreamGobbler::ThreadProc()
{
	DWORD dwRead, dwWritten;

	char msg[256];
	while(TRUE)
	{
		if( !ReadFile(m_hIn, m_buf, BUFFER_SIZE, &dwRead, NULL) || dwRead == 0)
		{
			sprintf(msg, "ReadFile or dwRead=0. dwRead=%d", dwRead);
			LOG4CPLUS_ERROR(THE_LOGGER, msg)
			break;
		}

		if( !WriteFile(m_hOut, m_buf, dwRead, &dwWritten, NULL) || dwRead != dwWritten)
		{
			sprintf(msg, "WriteFile or dwRead != dwWritten. dwRead=%d, dwWritten=%d", dwRead, dwWritten);
			LOG4CPLUS_ERROR(THE_LOGGER, msg)
			break;
		}
	}

	return 0;
}