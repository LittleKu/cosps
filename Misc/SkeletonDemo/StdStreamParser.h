// StdStreamParser.h: interface for the StdStreamParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDSTREAMPARSER_H__F2CF92B2_F321_48DE_8557_66EDC0787511__INCLUDED_)
#define AFX_STDSTREAMPARSER_H__F2CF92B2_F321_48DE_8557_66EDC0787511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FilterContentParser : public ContentParser
{
public:
	FilterContentParser(ContentParser* parser = NULL);
	virtual ~FilterContentParser();
	virtual void ParseContent(std::string& szLine, int nLineCount);
	virtual bool Init();
	virtual bool DeInit();
protected:
	ContentParser* m_parser;
};

class SaveFileParser : public ContentParser
{
public:
	SaveFileParser(LPCTSTR lpszFile);
	virtual ~SaveFileParser();
	virtual void ParseContent(std::string& szLine, int nLineCount);
	virtual bool Init();
	virtual bool DeInit();

protected:
	cfl::tstring m_szOutFile;
	FILE* m_fp;
};

class MEncoderOutParser : public ContentParser 
{
public:
	MEncoderOutParser(HWND hWnd, ContentParser* parser = NULL);
	virtual ~MEncoderOutParser();
	virtual void ParseContent(std::string& szLine, int nLineCount);
	virtual bool Init();
	virtual bool DeInit();
	
protected:
	HWND m_hWnd;
	ContentParser* m_parser;
};

#endif // !defined(AFX_STDSTREAMPARSER_H__F2CF92B2_F321_48DE_8557_66EDC0787511__INCLUDED_)
