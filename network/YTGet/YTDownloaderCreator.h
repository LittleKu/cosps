// YTDownloaderCreator.h: interface for the CYTDownloaderCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTDOWNLOADERCREATOR_H__EED99648_6690_40E1_A967_DDA23CF5EB43__INCLUDED_)
#define AFX_YTDOWNLOADERCREATOR_H__EED99648_6690_40E1_A967_DDA23CF5EB43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DownloaderCreator.h"

class CDownloaderContext;
class CYTDownloaderCreator : public CDownloaderCreator  
{
public:
	CYTDownloaderCreator();
	virtual ~CYTDownloaderCreator();
public:
	virtual CDownloader* CreateDownloader(const CString& szContent, CDownloaderContext* pContext, 
		CDownloadParam dlParam);

	DECLARE_NEW_CREATOR(CYTDownloaderCreator)
};

#endif // !defined(AFX_YTDOWNLOADERCREATOR_H__EED99648_6690_40E1_A967_DDA23CF5EB43__INCLUDED_)
