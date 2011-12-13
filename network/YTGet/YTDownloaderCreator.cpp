// YTDownloaderCreator.cpp: implementation of the CYTDownloaderCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YTDownloaderCreator.h"
#include "CommonUtils.h"
#include "HeaderDownloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME("YTDC")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYTDownloaderCreator::CYTDownloaderCreator()
{

}

CYTDownloaderCreator::~CYTDownloaderCreator()
{

}

CDownloader* CYTDownloaderCreator::CreateDownloader(const CString& szContent, CDownloaderContext* pContext, 
	CDownloadParam dlParam)
{
	return NULL;
}
