// StdStreamParser.cpp: implementation of the StdStreamParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdStreamParser.h"
#include "cflbase/tstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("StdStreamParser"))

FilterContentParser::FilterContentParser(ContentParser* parser /* = NULL */) : m_parser(parser)
{
}
FilterContentParser::~FilterContentParser()
{
	if(m_parser)
	{
		delete m_parser;
		m_parser = NULL;
	}
}
bool FilterContentParser::Init()
{
	return ((m_parser == NULL) ? true : m_parser->Init());
}
bool FilterContentParser::DeInit()
{
	return ((m_parser == NULL) ? true : m_parser->DeInit());
}
void FilterContentParser::ParseContent(std::string& szLine, int nLineCount)
{
	if(m_parser)
	{
		m_parser->ParseContent(szLine, nLineCount);
	}
}

SaveFileParser::SaveFileParser(LPCTSTR lpszFile) : m_fp(NULL)
{
	if(lpszFile)
	{
		m_szOutFile.assign(lpszFile);
	}
}

SaveFileParser::~SaveFileParser()
{

}

bool SaveFileParser::Init()
{
	if(m_szOutFile.empty())
	{
		return false;
	}

	m_fp = _tfopen(m_szOutFile.c_str(), _T("wb"));
	if(m_fp == NULL)
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("[SaveFileParser]: failed to open file [%s]"), m_szOutFile.c_str());
		LOG4CPLUS_WARN_STR(THE_LOGGER, szLog)
	}

	return m_fp != NULL;
}

bool SaveFileParser::DeInit()
{
	bool bRet = true;
	if(m_fp != NULL)
	{
		bRet = (fclose(m_fp) == 0);
		m_fp = NULL;
	}
	return bRet;
}

void SaveFileParser::ParseContent(std::string& szLine, int nLineCount)
{
	if(m_fp)
	{
		fwrite(szLine.data(), 1, szLine.size(), m_fp);
		fwrite("\n", 1, 1, m_fp);
	}
}

MEncoderOutParser::MEncoderOutParser(HWND hWnd, ContentParser* parser)
 : m_parser(parser), m_hWnd(hWnd)
{
	
}

MEncoderOutParser::~MEncoderOutParser()
{
	if(m_parser)
	{
		delete m_parser;
		m_parser = NULL;
	}
}
bool MEncoderOutParser::Init()
{
	if(m_parser)
	{
		m_parser->Init();
	}
	return true;
}
bool MEncoderOutParser::DeInit()
{
	if(m_parser)
	{
		m_parser->DeInit();
	}
	return true;
}

void MEncoderOutParser::ParseContent(std::string& szLine, int nLineCount)
{
	static const std::string szProgressPat = "Pos:([\\s]*)(.*?)s(.*?)\\(([\\s]*)(\\d+)%\\)(.*?)";
	static const int groupIndex[] = {2, 5};

	if(m_parser)
	{
		m_parser->ParseContent(szLine, nLineCount);
	}
	std::vector<std::string> resultVec;
	if(SysUtils::GetMatches(szLine, szProgressPat, groupIndex, 2, &resultVec))
	{
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Time=%s(s), Percent=%s%c"), CFL_STRING_TO_T_STR(resultVec[0]), 
				CFL_STRING_TO_T_STR(resultVec[1]), _T('%'));
			LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
		}

		::SendMessage(m_hWnd, NOTIFY_PROCESS_PROGRESS, (WPARAM)(&resultVec[0]), (LPARAM)(&resultVec[1]));
	}
}
