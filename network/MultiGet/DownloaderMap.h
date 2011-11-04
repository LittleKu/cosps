// DownloaderMap.h: interface for the CDownloaderMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADERMAP_H__B65FDA6E_85A2_4AD1_8F92_B81BD9C686F1__INCLUDED_)
#define AFX_DOWNLOADERMAP_H__B65FDA6E_85A2_4AD1_8F92_B81BD9C686F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Downloader.h"

typedef CMap<int, int, CDownloader*, CDownloader*> CMapID2Downloader;

class CDownloaderMap  
{
public:
	virtual ~CDownloaderMap();
	CDownloader* GetDownloader(int nTaskID);
	void AddDownloader(int nTaskID, CDownloader* pDownloader);
	void RemoveDownloader(int nTaskID);
private:
	CDownloaderMap();	
	CMapID2Downloader m_mapDownloader;
public:
	static CDownloaderMap* GetInstance();
};

#endif // !defined(AFX_DOWNLOADERMAP_H__B65FDA6E_85A2_4AD1_8F92_B81BD9C686F1__INCLUDED_)
