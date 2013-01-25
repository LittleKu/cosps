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
#include "ContainerCmdBuilder.h"
#include "OptionExpDef.h"
#include "ProfileMgr.h"

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
	/*
	CVCDlg* pDlg = ((CMainDlg*)SYS_APP()->GetMainWnd())->m_pVCDlg;

	//Get profile context
	OptionContext optionCxt;
	pDlg->m_propListMgr.GetPropMap(&optionCxt);

	//Input file
	//optionCxt.Put(IFILE, CFL_T2A(lpszInputFile));

	//Process size
	ProcessSize(&optionCxt);

	//Limit file length
	if(SysUtils::GetLimitLength() > 0)
	{
		optionCxt.Put(SEEK_TIME, "0");

		std::string szEndTime;
		cfl::format(szEndTime, "%d", SysUtils::GetLimitLength());
		optionCxt.Put(END_TIME, szEndTime.c_str());
	}
	
	//Builder
	CString szBinFile;
	SysUtils::GetBinFile(szBinFile, _T("mencoder.exe"));

	ContainerCmdBuilder builder;
	builder.SetContainer(_T("mp4"));
	builder.SetMEncoderBin(szBinFile);

	SysUtils::GetBinFile(szBinFile, _T("mp4creator.exe"));
	builder.SetMP4CreatorBin(szBinFile);
	builder.SetInput(lpszInputFile);
	builder.SetOutputFolder(SYS_PREF()->szOutputFolder);
	builder.SetOptionContext(&optionCxt);

	std::vector<CmdInfo> commands;
	StrObjPtrContext builderCxt;
	if(!builder.Build(commands, builderCxt))
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("Cmd Build failed"));
		LOG4CPLUS_FATAL_STR(THE_LOGGER, szLog)
	}
	int i;
	for(i = 0; i < commands.size(); i++)
	{
		LOG4CPLUS_INFO_STR(THE_LOGGER, commands.at(i).m_szCmdLine)

		pDlg->m_cmdInfos.push_back(commands.at(i));
	}

	//Find the del list
	TStrVectorObj* pStrVecObj = NULL;
	builderCxt.Get(PARAM_DEL_LIST, (cfl::Object**)&pStrVecObj);
	ASSERT(pStrVecObj != NULL);
	TStrVector* pDelList = (TStrVector*)pStrVecObj->GetData();
	ASSERT(pDelList != NULL);

	for(i = 0; i < pDelList->size(); i++)
	{
		LOG4CPLUS_INFO_STR(THE_LOGGER, pDelList->at(i))
		pDlg->m_delList.push_back(pDelList->at(i));
	}

	return 0;
	*/

	CVCDlg* pDlg = ((CMainDlg*)SYS_APP()->GetMainWnd())->m_pVCDlg;
	
	//Get profile context
	OptionContext optionCxt;
	pDlg->m_propListMgr.GetPropMap(&optionCxt);
	
	//Process size
	ProcessSize(&optionCxt);

	ProfileNode* pSelectedProfile = pDlg->GetSelectedProfile();
	if(pSelectedProfile == NULL)
	{
		return -1;
	}
	
	//Limit file length
	if(SysUtils::GetLimitLength() > 0)
	{
		optionCxt.Put(SEEK_TIME, "0");
		
		std::string szEndTime;
		cfl::format(szEndTime, "%d", SysUtils::GetLimitLength());
		optionCxt.Put(END_TIME, szEndTime.c_str());
	}

	StrObjPtrContext builderCxt;
	
	//Builder
	CString szBinFile;
	SysUtils::GetBinFile(szBinFile, _T("mencoder.exe"));
	builderCxt.Put(PARAM_BIN_MENCODER, new TStrObj((LPCTSTR)szBinFile));
	
	//ContainerCmdBuilder builder;
	//builder.SetContainer(_T("mp4"));
	//builder.SetMEncoderBin(szBinFile);
	
	SysUtils::GetBinFile(szBinFile, _T("mp4creator.exe"));
	builderCxt.Put(PARAM_BIN_MP4CREATOR, new TStrObj((LPCTSTR)szBinFile));

	//builder.SetMP4CreatorBin(szBinFile);
	//builder.SetInput(lpszInputFile);
	builderCxt.Put(PARAM_INPUT_FILE, new TStrObj((LPCTSTR)lpszInputFile));

	//builder.SetOutputFolder(SYS_PREF()->szOutputFolder);
	builderCxt.Put(PARAM_OUTPUT_FOLDER, new TStrObj((LPCTSTR)SYS_PREF()->szOutputFolder));

	//builder.SetOptionContext(&optionCxt);
	builderCxt.Put(PARAM_OPT_CXT, new OptionExpObj<OptionContext*>(&optionCxt));

	TStrVector* pDelList = GetObjPtrData<TStrVector>(builderCxt, PARAM_DEL_LIST, true);
	ASSERT(pDelList);
	
	std::vector<CmdInfo> commands;
	
	std::string szProfileId;
	AttribMap* pAttribMap = (AttribMap*)pSelectedProfile->GetData();
	if(!pAttribMap->Get("id", szProfileId))
	{
		return -2;
	}
	CmdListBuilder* pCmdListBuilder = ProfileMgr::GetInstance()->CreateBuilder(szProfileId, &builderCxt);
	if(pCmdListBuilder == NULL)
	{
		return -3;
	}
	if(!pCmdListBuilder->Build(commands, builderCxt))
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("Cmd Build failed"));
		LOG4CPLUS_FATAL_STR(THE_LOGGER, szLog)
	}
	delete pCmdListBuilder;
	pCmdListBuilder = NULL;

	int i;
	for(i = 0; i < commands.size(); i++)
	{
		LOG4CPLUS_INFO_STR(THE_LOGGER, commands.at(i).m_szCmdLine)
		pDlg->m_cmdInfos.push_back(commands.at(i));
	}
	
	//Find the del list	
	for(i = 0; i < pDelList->size(); i++)
	{
		LOG4CPLUS_INFO_STR(THE_LOGGER, pDelList->at(i))
		pDlg->m_delList.push_back(pDelList->at(i));
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
