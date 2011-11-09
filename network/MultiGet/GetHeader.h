// GetHeader.h: interface for the CGetHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETHEADER_H__E7B9758A_FF9F_4366_805E_BC19306B857C__INCLUDED_)
#define AFX_GETHEADER_H__E7B9758A_FF9F_4366_805E_BC19306B857C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <curl/curl.h>

class CGetHeader  
{
public:
	CGetHeader();
	virtual ~CGetHeader();
	void Start(const char* url);
	void Stop();
	CHeaderInfo* GetHeaderInfo();
private:
	int ProcessProgress(double dltotal, double dlnow, double ultotal, double ulnow);
	
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
private:
	CURL* m_curl;
	CHeaderInfo m_headerInfo;

	CController m_controller;
};

#endif // !defined(AFX_GETHEADER_H__E7B9758A_FF9F_4366_805E_BC19306B857C__INCLUDED_)
