#include "StdAfx.h"
#include "Counter.h"

UINT CCounter::CountThreadProc(LPVOID lpvData)
{
	HWND hMainWnd = (HWND)lpvData;
	ASSERT(hMainWnd);

	int cAllFiles = 10000000;
	LRESULT lresult = ::SendMessage(hMainWnd, WM_START_COUNT, 0, (LPARAM)cAllFiles);
	HWND hProgWnd = (HWND)lresult;
	ASSERT(hProgWnd);
	
	BOOL bProgDlgEnded = FALSE;	
	for (int f = 0; f < cAllFiles; ++f)
	{
		lresult = ::SendMessage(hProgWnd, WM_UPDATE_PROGRESS, (WPARAM)f, 0);
		if(lresult == 0)
		{
			::SendMessage(hMainWnd, WM_END_COUNT, 0, 0);
			bProgDlgEnded = TRUE;
			break;
		}
	}
	
	if(!bProgDlgEnded)
	{
		::SendMessage(hMainWnd, WM_END_COUNT, 0, 0);
	}

	return 0;
}