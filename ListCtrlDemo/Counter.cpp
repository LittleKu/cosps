#include "StdAfx.h"
#include "Counter.h"
#include "resource.h"
#include "ProgressDlg.h"


UINT CCounter::CountThreadProc(LPVOID lpvData)
{
	HWND hMainWnd = (HWND)lpvData;
	ASSERT(hMainWnd);

	LRESULT lresult = ::SendMessage(hMainWnd, WM_START_COUNT, 0, 0);
	HWND hProgWnd = (HWND)lresult;
	ASSERT(hProgWnd);

//	Sleep(5000);
		
	int cAllFiles = 10000;
	bool bInitialShowDlg = false;
	int progress = 0;
	for (int f = 0; f < cAllFiles; ++f)
	{
		progress = (f + 1)*100/cAllFiles;
		lresult = ::SendMessage(hProgWnd, WM_UPDATE_PROGRESS, (WPARAM)f, (LPARAM)progress);
		if(lresult == 0)
		{
			::SendMessage(hMainWnd, WM_END_COUNT, 0, 0);
			break;
		}
		Sleep(1);
	}
	
	::SendMessage(hMainWnd, WM_END_COUNT, 0, 0);

	return 0;
}