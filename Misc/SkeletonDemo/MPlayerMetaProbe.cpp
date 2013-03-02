// MPlayerMetaProbe.cpp: implementation of the MPlayerMetaProbe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPlayerMetaProbe.h"
#include "SyncProcessExecutor.h"
#include "Preferences.h"
#include "StdStreamParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef struct  
{
	int nID;
	const char* szValue;
} id_value_pair;

static const id_value_pair id_value_pairs[] =
{
	{ ID_LENGTH, "ID_LENGTH=" },
	{ ID_START_TIME, "ID_START_TIME=" },
	{ ID_VIDEO_FORMAT, "ID_VIDEO_FORMAT=" },
	{ ID_VIDEO_WIDTH, "ID_VIDEO_WIDTH=" },
	{ ID_VIDEO_HEIGHT, "ID_VIDEO_HEIGHT=" },
	{ ID_VIDEO_FPS, "ID_VIDEO_FPS=" },
	{ ID_VIDEO_BITRATE, "ID_VIDEO_BITRATE=" },
	{ ID_AUDIO_BITRATE, "ID_AUDIO_BITRATE=" },
	{ ID_AUDIO_RATE, "ID_AUDIO_RATE=" },
	{ ID_AUDIO_NCH, "ID_AUDIO_NCH=" },
	{ -1, NULL }
};

MPlayerMetaProbe::MPlayerMetaOutParser::MPlayerMetaOutParser(MetaMap* pMap, ContentParser* parser /* = NULL */)
{
	m_pMap = pMap;
	m_pParser = parser;
}
MPlayerMetaProbe::MPlayerMetaOutParser::~MPlayerMetaOutParser()
{
	if(m_pParser)
	{
		delete m_pParser;
		m_pParser = NULL;
	}
}

void MPlayerMetaProbe::MPlayerMetaOutParser::ParseContent(std::string& szLine, int nLineCount)
{
	if(m_pParser)
	{
		m_pParser->ParseContent(szLine, nLineCount);
	}

	int nLen;
	for(int i = 0; id_value_pairs[i].szValue != NULL; i++)
	{
		nLen = strlen(id_value_pairs[i].szValue);
		if((szLine.size() >= nLen) && (szLine.compare(0, nLen, id_value_pairs[i].szValue, nLen) == 0))
		{
			//(*m_pMap)[id_value_pairs[i].nID] = szLine.substr(nLen);
			m_pMap->Put(id_value_pairs[i].nID, szLine.substr(nLen));
		}
	}
}
bool MPlayerMetaProbe::MPlayerMetaOutParser::Init()
{
	if(m_pParser)
	{
		m_pParser->Init();
	}
	return true;
}
bool MPlayerMetaProbe::MPlayerMetaOutParser::DeInit()
{
	if(m_pParser)
	{
		m_pParser->DeInit();
	}
	return true;
}

MPlayerMetaProbe::MPlayerMetaProbe(MetaMap* pMap) : m_pMap(pMap)
{

}

MPlayerMetaProbe::~MPlayerMetaProbe()
{

}

int MPlayerMetaProbe::Probe(LPCTSTR lpszFileName)
{
	ExecArgument* pArg = new ExecArgument();
	CString szBinFile;
	SysUtils::GetBinFile(szBinFile, _T("mplayer.exe"));
	cfl::tformat(pArg->szCmdLine, _T("\"%s\" \"%s\" -frames 0 -vo null -ao null -identify"), 
		(LPCTSTR)szBinFile, lpszFileName);

	cfl::tstring szDumpFile;

	//out file
	cfl::tformat(szDumpFile, _T("%s\\out_mplayer.dump"), (LPCTSTR)(SYS_PREF()->szTempFolder));
	pArg->pOutParser = new SaveFileParser(szDumpFile.c_str());
	pArg->pOutParser = new MPlayerMetaOutParser(m_pMap, pArg->pOutParser);

	//err file
	cfl::tformat(szDumpFile, _T("%s\\err_mplayer.dump"), (LPCTSTR)(SYS_PREF()->szTempFolder));
	pArg->pErrParser = new SaveFileParser(szDumpFile.c_str());

	//m_map.clear();
	m_pMap->Clear();
	SyncProcessExecutor spExecutor;
	int rc = spExecutor.Execute(pArg);

	return rc;
}
bool MPlayerMetaProbe::GetMeta(int nMetaID, std::string& val)
{
	return m_pMap->Get(nMetaID, val);
}