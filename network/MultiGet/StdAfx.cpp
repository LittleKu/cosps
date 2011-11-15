// stdafx.cpp : source file that includes just the standard includes
//	MultiGet.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "MultiGet.h"

CThreadMonitor* SYS_THREAD_MONITOR()
{
	CMultiGetApp* pApp = (CMultiGetApp*)AfxGetApp();
	ASSERT(pApp);
	return pApp->GetThreadMonitor();
}



