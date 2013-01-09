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

MEncoderOutParser::MEncoderOutParser(HWND hWnd) : m_hWnd(hWnd)
{
	
}

MEncoderOutParser::~MEncoderOutParser()
{
	
}

void MEncoderOutParser::Reset()
{
	
}
void MEncoderOutParser::ParseContent(std::string& szLine, int nLineCount)
{
	static const std::string szProgressPat = "Pos:([\\s]*)(.*?)s(.*?)\\(([\\s]*)(\\d+)%\\)(.*?)";
	static const int groupIndex[] = {2, 5};

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
