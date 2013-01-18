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
	m_pMetaMap = new MetaMap();
	Reset();
}

CTaskInfo::~CTaskInfo()
{
	if(m_pMetaMap)
	{
		m_pMetaMap->Clear();
		delete m_pMetaMap;
		m_pMetaMap = NULL;
	}
}
void CTaskInfo::Reset()
{
	mask = 0;
	m_szFileName.Empty();
	m_szFormat.Empty();
	m_nDuration = 0;
	m_nState = TSE_READY;
	m_dProgress = 0.0f;

	m_nTaskID = -1;

	if(m_pMetaMap)
	{
		m_pMetaMap->Clear();
	}
}