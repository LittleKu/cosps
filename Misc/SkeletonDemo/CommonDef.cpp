#include "StdAfx.h"
#include "CommonDef.h"

ExecArgument::ExecArgument() : pOutParser(NULL), pErrParser(NULL)
{
}
ExecArgument::~ExecArgument()
{
	if(pOutParser)
	{
		delete pOutParser;
		pOutParser = NULL;
	}
	if(pErrParser)
	{
		delete pErrParser;
		pErrParser = NULL;
	}
}

CTaskInfo::CTaskInfo()
{
	Reset();
}

CTaskInfo::~CTaskInfo()
{

}
void CTaskInfo::Reset()
{
	mask = 0;
	m_szFileName.Empty();
	m_szFormat.Empty();
	m_nDuration = 0;
	m_nState = TSE_READY;
	m_nTaskID = -1;
}