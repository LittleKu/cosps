// Downloader.h: interface for the CDownloader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADER_H__F60EFBBF_218A_45D1_9804_D882B60DA127__INCLUDED_)
#define AFX_DOWNLOADER_H__F60EFBBF_218A_45D1_9804_D882B60DA127__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDownloader  
{
public:
	CDownloader();
	virtual ~CDownloader();
	
	int download(HWND hwnd);
};

#endif // !defined(AFX_DOWNLOADER_H__F60EFBBF_218A_45D1_9804_D882B60DA127__INCLUDED_)
