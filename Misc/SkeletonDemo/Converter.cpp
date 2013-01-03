// Converter.cpp: implementation of the CConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "Converter.h"
#include "OptionContext.h"
#include "MainDlg.h"
#include "VCDlg.h"
#include "DefaultOptionExpBuilder.h"
#include "CmdBuilder.h"
#include "Preferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("Converter"))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConverter::CConverter()
{

}

CConverter::~CConverter()
{

}

CDefaultConverter::CDefaultConverter()
{
}
CDefaultConverter::~CDefaultConverter()
{
}

int CDefaultConverter::Convert(LPCTSTR lpszInputFile)
{
	CVCDlg* pDlg = ((CMainDlg*)SYS_APP()->GetMainWnd())->m_pVCDlg;

	//Get profile context
	OptionContext context;
	pDlg->m_propListMgr.GetPropMap(&context);

	//Input file
	context.Put(IFILE, CFL_T2A(lpszInputFile));

	//Process size
	ProcessSize(&context);
	
	//Builder
	CString szBinFile;
	SysUtils::GetBinFile(szBinFile, _T("mencoder.exe"));

	DefaultOptionExpBuilder builder;
	MeCmdBuilder mcb;
	mcb.SetBinFile(szBinFile).SetOptionExpBuilder(&builder).SetOutputFolder(CFL_T2A((LPCTSTR)SYS_PREF()->szOutputFolder));

	std::vector<cfl::tstring> cmdList;
	cfl::tstring szCmdLine;

	std::string val;
	if(context.Get(PASS_COUNT, val) && val.compare(0, 1, "2") == 0)
	{
		OptionContext mutableCxt;
		for(int pass = 1; pass <= 2; pass++)
		{
			mutableCxt = context;
			mcb.SetPass(pass).SetOptionContext(&mutableCxt);
			
			if(!mcb.Build(szCmdLine))
			{
				cfl::tstring szLog;
				cfl::tformat(szLog, _T("Cmd build failed at pass [%d]"), pass);
				LOG4CPLUS_FATAL_STR(THE_LOGGER, szLog)
				return pass;
			}
			
			//TODO
			cmdList.push_back(szCmdLine);
		}
	}
	else
	{
		mcb.SetOptionContext(&context);
		if(!mcb.Build(szCmdLine))
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Cmd build failed"));
			LOG4CPLUS_FATAL_STR(THE_LOGGER, szLog)
			return 100;
		}

		cmdList.push_back(szCmdLine);
	}

	//TODO: show out now
	for(int i = 0; i < cmdList.size(); i++)
	{
		LOG4CPLUS_INFO_STR(THE_LOGGER, cmdList.at(i))
	}

	return 0;
}

bool ParseSize(const std::string& str, int& w, int& h)
{
	size_t index = str.find('x');
	if(index == std::string::npos || index == 0 || (index == str.size() - 1))
	{
		return false;
	}
	std::string szWidth = str.substr(0, index);
	w = atoi(szWidth.c_str());
	
	std::string szHeight = str.substr(index + 1);
	h = atoi(szHeight.c_str());
	
	return w > 0 && h > 0;
}

bool CDefaultConverter::ProcessSize(OptionContext* pContext)
{
	int w, h;
	std::string val;
	bool bRet = pContext->Get(VIDEO_SIZE, val);
	if(bRet)
	{
		bRet = ParseSize(val, w, h);
		
		if(bRet)
		{
			char buf[16];
			sprintf(buf, "%d", w);
			pContext->Put(VIDEO_WIDTH, buf);
			
			sprintf(buf, "%d", h);
			pContext->Put(VIDEO_HEIGHT, buf);
		}
	}

	return bRet;
}
