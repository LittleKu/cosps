// DownloaderCreator.cpp: implementation of the CDownloaderCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderCreator.h"
#include "HeaderDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloaderCreator::CDownloaderCreator()
{

}

CDownloaderCreator::~CDownloaderCreator()
{

}

CDownloader* CDownloaderCreator::CreateDownloader(const CString& szContent, CDownloaderContext* pContext, 
												  CDownloadParam dlParam)
{
	CHeaderDownloader* pHeaderDownloader = new CHeaderDownloader(pContext);
	pHeaderDownloader->Init(dlParam);

	return pHeaderDownloader;
}
