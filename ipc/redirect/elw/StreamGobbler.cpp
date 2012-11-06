#include "StreamGobbler.h"
#include "log4cplus_config.h"
#include <stdio.h>

DECLARE_THE_LOGGER_NAME("StreamGobbler")

DWORD WINAPI StreamGobbler::ThreadProc(LPVOID lpParameter)
{
	StreamGobbler* pThis = (StreamGobbler*)lpParameter;
	return pThis->ThreadProc();
}
StreamGobbler::StreamGobbler(HANDLE hIn, HANDLE hOut, const char* name, FILE* fd, FILE* pAllFile)
 : m_hIn(hIn), m_hOut(hOut), m_pFile(fd), m_pAllFile(pAllFile)
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

		if(m_pFile != NULL)
		{
			dwWritten = fwrite(m_buf, 1, dwRead, m_pFile);
			if( dwWritten != dwRead )
			{
				sprintf(msg, "fwrite pFile failed. dwRead != dwWritten", dwRead, dwWritten);
				LOG4CPLUS_ERROR(THE_LOGGER, msg)
			}
			fflush(m_pFile);
		}

		if(m_pAllFile != NULL)
		{
			dwWritten = fwrite(m_buf, 1, dwRead, m_pAllFile);
			if( dwWritten != dwRead )
			{
				sprintf(msg, "fwrite AllFile failed. dwRead != dwWritten", dwRead, dwWritten);
				LOG4CPLUS_ERROR(THE_LOGGER, msg)
			}
			fflush(m_pAllFile);
		}
	}

	return 0;
}