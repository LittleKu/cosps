#include <windows.h>
#include <stdio.h>

//http://www.codeproject.com/Articles/6129/Using-WIN32-Message-Queue-in-C-WIN32-Multithreaded

#define NUM_THREADS 2

HANDLE hEvent;

void processMsg(MSG* pMsg)
{
	printf("MSG: %d, w=%d, l=%d\n", pMsg->message, pMsg->wParam, pMsg->lParam);
}

DWORD WINAPI MyThreadProc(LPVOID lpParameter)
{
	MSG msg;
	BOOL bRet;

	printf("MyThreadProc start\n");
	
	bRet = ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	printf("Message queue created. %d\n", bRet);

	::SetEvent(hEvent);

	while( (bRet = ::GetMessage( &msg, NULL, 0, 0 )) != 0)
	{ 
		if (bRet == -1)
		{
			// handle the error and possibly exit
			printf("GetMessage error: %d\n", GetLastError());
		}
		else
		{
			processMsg(&msg);
		}
	}

	const char* yesorno = (msg.message == WM_QUIT) ? "yes" : "no";
	printf("MyThreadProc exit. msg.message=%d, WM_QUIT=%s\n", msg.message, yesorno);
	return msg.wParam;
}

int main(int argc, char* argv[])
{
	HANDLE hThread;
	DWORD dwThreadId;
	BOOL bRet;
	int i;
	DWORD dwExitCode;

	hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	hThread = ::CreateThread(NULL, 0, MyThreadProc, NULL, 0, &dwThreadId);
	if(hThread == NULL)
	{
		printf("CreateThread Failed.\n");
		return -1;
	}
	
	//First test message
	::WaitForSingleObject(hEvent, INFINITE);
	bRet = ::PostThreadMessage(dwThreadId, WM_USER + 100, 100, 200);
	if(!bRet)
	{
		printf("first PostThreadMessage failed.\n");
		return -2;
	}
	//Not used any more
	::CloseHandle(hEvent);

	for(i = 0; i < 5; i++)
	{
		bRet = ::PostThreadMessage(dwThreadId, WM_USER + i, i, i * 10);
		if(!bRet)
		{
			printf("PostThreadMessage failed.\n");
			return -2;
		}
		Sleep(500);
	}

	bRet = ::PostThreadMessage(dwThreadId, WM_QUIT, 100, 0);
	if(!bRet)
	{
		printf("PostThreadMessage WM_QUIT failed.\n");
		return -3;
	}

	::WaitForSingleObject(hThread, INFINITE);
	
	bRet = GetExitCodeThread(hThread, &dwExitCode);
	if(!bRet)
	{
		printf("GetExitCodeThread failed.\n");
		return -4;
	}
	printf("Thread exit code: %d\n", dwExitCode);

	::CloseHandle(hThread);
	
	return 0;
}
