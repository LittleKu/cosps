// StdStreamParser.h: interface for the StdStreamParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDSTREAMPARSER_H__F2CF92B2_F321_48DE_8557_66EDC0787511__INCLUDED_)
#define AFX_STDSTREAMPARSER_H__F2CF92B2_F321_48DE_8557_66EDC0787511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MEncoderOutParser : public ContentParser 
{
public:
	MEncoderOutParser(HWND hWnd);
	virtual ~MEncoderOutParser();
	virtual void Reset();
	virtual void ParseContent(std::string& szLine, int nLineCount);
	
private:
	HWND m_hWnd;
};

#endif // !defined(AFX_STDSTREAMPARSER_H__F2CF92B2_F321_48DE_8557_66EDC0787511__INCLUDED_)
