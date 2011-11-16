// Downloader.cpp: implementation of the CDownloader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Downloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloader::CDownloader()
{
}

CDownloader::~CDownloader()
{
}

DWORD CDownloader::GetState()
{
	CDownloadState dlState;
	GetState(dlState);

	return dlState.GetState();
}



//@Deprecated
int CDownloader::Stop()
{
	return 0;
}
int CDownloader::Resume()
{
	return 0;
}
BOOL CDownloader::IsResumable()
{
	return 0;
}
void CDownloader::WaitUntilStop()
{
}


