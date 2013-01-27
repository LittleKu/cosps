// ProfileMgr.cpp: implementation of the ProfileMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProfileMgr.h"
#include "MeCmdListBuilder.h"
#include "MP4CmdListBuilder.h"
#include "ProfileLoader.h"
#include "cflbase/TreeNode.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("ProfileMgr"))

#define BUILDER_NAME	"param.builder"
#define BUILDER_BASE	"ME"
#define BUILDER_MP4		"MP4"

ProfileMgr::ProfileMgr()
{
	InitProfileGroupMap();
	InitBuilderMap();
}

ProfileMgr::~ProfileMgr()
{
	CFLPOS pos = NULL;
	std::string key;
	
	//Profile Info map
	SSTreeMap* pValue = NULL;
	pos = m_mapProfile2Group.GetFirstPos();
	while(pos != NULL)
	{
		m_mapProfile2Group.GetNextEntry(pos, key, pValue);
		assert(pValue);
		delete pValue;
	}
	m_mapProfile2Group.ReleasePos(pos);
	m_mapProfile2Group.Clear();

	//Builder instance map
	CmdListBuilder* pBuilder = NULL;
	pos = m_mapGroup2Builder.GetFirstPos();
	while(pos != NULL)
	{
		m_mapGroup2Builder.GetNextEntry(pos, key, pBuilder);
		assert(pBuilder);
		delete pBuilder;
	}
	m_mapGroup2Builder.ReleasePos(pos);
	m_mapGroup2Builder.Clear();

	LOG4CPLUS_DEBUG_STR(THE_LOGGER, _T("ProfileMgr::~ProfileMgr() called"))
}

ProfileMgr* ProfileMgr::GetInstance()
{
	static ProfileMgr singleton;
	return &singleton;
}

CmdListBuilder* ProfileMgr::CreateBuilder(const std::string& szProfile, StrObjPtrContext* pContext)
{
	SSTreeMap* pPropMap = NULL;
	if(!m_mapProfile2Group.Get(szProfile, pPropMap))
	{
		return NULL;
	}

	std::string szBuilderName;
	if(!pPropMap->Get(BUILDER_NAME, szBuilderName))
	{
		return NULL;
	}

	CmdListBuilder* pBuilder = NULL;
	if(!m_mapGroup2Builder.Get(szBuilderName, pBuilder))
	{
		return NULL;
	}
	
	//set properties
	if(pContext)
	{
		std::string key, val;

		CFLPOS pos = pPropMap->GetFirstPos();		
		while(pos != NULL)
		{
			pPropMap->GetNextEntry(pos, key, val);

			OptionExpObj<cfl::tstring> *pObjProp = new OptionExpObj<cfl::tstring>(CFL_STRING_TO_TSTRING(val));

			pContext->Put(key, pObjProp);
		}
		pPropMap->ReleasePos(pos);
	}
	
	return pBuilder->Clone();
}

static bool InitProfileGroupProc(cfl::TreeNode* pNode, void* lParam)
{
	AttribMap* pNodeMap = (AttribMap*)pNode->GetData();

	std::string key, val;
	cfl::TreeNode *pProp = NULL;
	AttribMap *pPropMap = NULL;
	if(pNodeMap->Get(PF_ATTRIB_TAG, val) && val.compare("profile") == 0)
	{
		ProfilePropMap* pOutMap = (ProfilePropMap*)lParam;

		if(!pNodeMap->Get("id", val))
		{
			return true;
		}
		ProfileCmdBuildInfo* pBuildInfo = NULL;
		if(!pOutMap->Get(val, pBuildInfo))
		{
			pBuildInfo = new ProfileCmdBuildInfo();
			pOutMap->Put(val, pBuildInfo);
		}
		assert(pBuildInfo != NULL);

		std::string name, value;
		int i, nChildCount = pNode->GetChildCount();
		for(i = 0; i < nChildCount; i++)
		{
			pPropMap = (AttribMap*)pNode->GetChild(i)->GetData();

			if(pPropMap->Get(PF_ATTRIB_TAG, val) && val.compare("property") == 0)
			{
				if(pPropMap->Get("name", name) && pPropMap->Get("value", value))
				{
					pBuildInfo->Put(name, value);
				}
			}
		}
	}

	return true;
}

void ProfileMgr::InitProfileGroupMap()
{
// 	ProfileCmdBuildInfo* pBuildInfo = NULL;
// 
// 	//Example: iPhone
// 	pBuildInfo = new ProfileCmdBuildInfo();
// 	pBuildInfo->m_BuilderName = BUILDER_MP4;
// 	(*(pBuildInfo->m_pBuilderProp))[PARAM_OUT_EXT] = ".mp4";
// 	m_mapProfile2Group["pf_iphone"] = pBuildInfo;
// 
// 	//DVD
// 	pBuildInfo = new ProfileCmdBuildInfo();
// 	pBuildInfo->m_BuilderName = BUILDER_BASE;
// 	(*(pBuildInfo->m_pBuilderProp))[PARAM_OUT_EXT] = ".mpg";
// 	(*(pBuildInfo->m_pBuilderProp))[PARAM_OF] = "dvd_ntsc";
// 	m_mapProfile2Group["pf_dvd_ntsc"] = pBuildInfo;

	ProfileNode* pRootProfile = CProfileLoader::GetInstance()->GetRootProfile();
	if(pRootProfile == NULL)
	{
		LOG4CPLUS_ERROR_STR(THE_LOGGER, _T("InitProfileGroupMap failed. Root Profile is null"))
		return;
	}

	pRootProfile->BFSEnum(InitProfileGroupProc, &m_mapProfile2Group);
}
void ProfileMgr::InitBuilderMap()
{
	//Base Builder
	m_mapGroup2Builder.Put(BUILDER_BASE, new MeCmdListBuilder());

	//MP4 Builder
	m_mapGroup2Builder.Put(BUILDER_MP4, new MP4CmdListBuilder());
}
