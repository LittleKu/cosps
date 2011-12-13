// CNETDownloaderCreator.h: interface for the CCNETDownloaderCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CNETDOWNLOADERCREATOR_H__061D3C15_2494_476D_A9EB_515E7C09FC13__INCLUDED_)
#define AFX_CNETDOWNLOADERCREATOR_H__061D3C15_2494_476D_A9EB_515E7C09FC13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DownloaderCreator.h"

class CCNETDownloaderCreator : public CDownloaderCreator  
{
public:
	CCNETDownloaderCreator();
	virtual ~CCNETDownloaderCreator();
public:
	virtual CDownloader* CreateDownloader(const CString& szContent, CDownloaderContext* pContext, 
		CDownloadParam dlParam);
	
	DECLARE_NEW_CREATOR(CCNETDownloaderCreator)
};

#endif // !defined(AFX_CNETDOWNLOADERCREATOR_H__061D3C15_2494_476D_A9EB_515E7C09FC13__INCLUDED_)
