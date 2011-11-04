// DownloaderMap.cpp: implementation of the CDownloaderMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloaderMap.h"
#include <memory>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloaderMap::CDownloaderMap()
{

}

CDownloaderMap::~CDownloaderMap()
{

}


CDownloaderMap* CDownloaderMap::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<CDownloaderMap> ptr(new CDownloaderMap);	
	return ptr.get();
}

CDownloader* CDownloaderMap::GetDownloader(int nTaskID)
{
	CDownloader* pDownloader = NULL;

	if(m_mapDownloader.Lookup(nTaskID, pDownloader))
	{
		return pDownloader;
	}
	return NULL;
}
void CDownloaderMap::AddDownloader(int nTaskID, CDownloader* pDownloader)
{
	m_mapDownloader.SetAt(nTaskID, pDownloader);
}
void CDownloaderMap::RemoveDownloader(int nTaskID)
{
	CDownloader* pDownloader = GetDownloader(nTaskID);
	if(pDownloader == NULL)
	{
		return;
	}
	delete pDownloader;
	pDownloader = NULL;

	m_mapDownloader.RemoveKey(nTaskID);
}
