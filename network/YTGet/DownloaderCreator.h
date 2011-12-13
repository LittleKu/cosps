// DownloaderCreator.h: interface for the CDownloaderCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADERCREATOR_H__B9CF79C2_88B4_4C68_A041_10C3272AF13C__INCLUDED_)
#define AFX_DOWNLOADERCREATOR_H__B9CF79C2_88B4_4C68_A041_10C3272AF13C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Downloader.h"

class CDownloaderContext;
class CDownloaderCreator  
{
public:
	CDownloaderCreator();
	virtual ~CDownloaderCreator();
public:
	virtual CDownloader* CreateDownloader(const CString& szContent, CDownloaderContext* pContext, 
		CDownloadParam dlParam) { return NULL; }
};

#define DECLARE_NEW_CREATOR(T)   static CDownloaderCreator* T::new##T(){ return new T(); }

#endif // !defined(AFX_DOWNLOADERCREATOR_H__B9CF79C2_88B4_4C68_A041_10C3272AF13C__INCLUDED_)
