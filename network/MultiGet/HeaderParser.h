// HeaderParser.h: interface for the CHeaderParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADERPARSER_H__B8265B11_1893_4767_AE6C_1583782CC366__INCLUDED_)
#define AFX_HEADERPARSER_H__B8265B11_1893_4767_AE6C_1583782CC366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <curl/curl.h>


class CHeaderParser  
{
public:
	CHeaderParser(const char* url);
	virtual ~CHeaderParser();
	CHeaderInfo* GetHeaderInfo();

private:
	CURL* m_curl;
	CHeaderInfo m_headerInfo;
};

#endif // !defined(AFX_HEADERPARSER_H__B8265B11_1893_4767_AE6C_1583782CC366__INCLUDED_)
