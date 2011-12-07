// stdafx.cpp : source file that includes just the standard includes
//	YTGet.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "YTGet.h"

CThreadMonitor* SYS_THREAD_MONITOR()
{
	return SYS_APP()->GetThreadMonitor();
}


