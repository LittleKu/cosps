#ifndef __STREAM_GOBBLER_H__
#define __STREAM_GOBBLER_H__

#include <windows.h>
#include <stdio.h>

class StreamGobbler
{
public:
	StreamGobbler(HANDLE hIn, HANDLE hOut, const char* name, FILE* fd, FILE* pAllFile);
	virtual ~StreamGobbler();

	HANDLE Start(LPDWORD lpThreadId = NULL);
	DWORD ThreadProc();
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
private:
	enum
	{
		BUFFER_SIZE = 4096,
		NAME_SIZE = 32
	};
	HANDLE	m_hIn;
	HANDLE	m_hOut;
	char	m_szName[NAME_SIZE];
	char	m_buf[BUFFER_SIZE];
	FILE*	m_pFile;
	FILE*	m_pAllFile;
};

#endif