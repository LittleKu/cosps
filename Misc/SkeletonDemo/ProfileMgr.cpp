// ProfileMgr.cpp: implementation of the ProfileMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProfileMgr.h"
#include "MeCmdListBuilder.h"
#include "MP4CmdListBuilder.h"
#include "ProfileLoader.h"
#include "cflbase/TreeNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("ProfileMgr"))

#define BUILDER_BASE	"Builder.Base"
#define BUILDER_MP4		"Builder.MP4"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ProfileCmdBuildInfo::ProfileCmdBuildInfo()
{
	m_pBuilderProp = new ssmap();
}
ProfileCmdBuildInfo::~ProfileCmdBuildInfo()
{
	if(m_pBuilderProp != NULL)
	{
		delete m_pBuilderProp;
		m_pBuilderProp = NULL;
	}
}

ProfileMgr::ProfileMgr()
{
	InitProfileGroupMap();
	InitBuilderMap();
}

ProfileMgr::~ProfileMgr()
{
	//Profile Info map
	ProfileInfoMap::iterator profileIter = m_mapProfile2Group.begin();
	for( ; profileIter != m_mapProfile2Group.end(); profileIter++)
	{
		delete profileIter->second;
	}
	m_mapProfile2Group.clear();

	//Builder instance map
	BuilderMap::iterator builderIter = m_mapGroup2Builder.begin();
	for( ; builderIter != m_mapGroup2Builder.end(); builderIter++)
	{
		delete builderIter->second;
	}
	m_mapGroup2Builder.clear();

	LOG4CPLUS_DEBUG_STR(THE_LOGGER, _T("ProfileMgr::~ProfileMgr() called"))
}

ProfileMgr* ProfileMgr::GetInstance()
{
	static ProfileMgr singleton;
	return &singleton;
}

CmdListBuilder* ProfileMgr::CreateBuilder(const std::string& szProfile, StrObjPtrContext* pContext)
{
	ProfileInfoMap::iterator groupIter = m_mapProfile2Group.find(szProfile);
	if(groupIter == m_mapProfile2Group.end())
	{
		return NULL;
	}

	ProfileCmdBuildInfo* pBuildInfo = (ProfileCmdBuildInfo*)groupIter->second;

	BuilderMap::iterator builderIter = m_mapGroup2Builder.find(pBuildInfo->m_BuilderName);
	if(builderIter == m_mapGroup2Builder.end())
	{
		return NULL;
	}

	CmdListBuilder* pBuilder = (CmdListBuilder*)builderIter->second;
	if(pBuilder == NULL)
	{
		return NULL;
	}
	
	//set properties
	if(pContext)
	{
		ssmap::iterator propIter = pBuildInfo->m_pBuilderProp->begin();
		cfl::tstring* pVal;
		for( ; propIter != pBuildInfo->m_pBuilderProp->end(); propIter++)
		{
			OptionExpObj<cfl::tstring> *pObjProp = new OptionExpObj<cfl::tstring>();
			pVal = (cfl::tstring*)pObjProp->GetData();
			pVal->assign(CFL_STRING_TO_TSTRING(propIter->second));

			pContext->Put(propIter->first, pObjProp);
		}
	}
	
	return pBuilder->Clone();
}

static bool InitProfileGroupProc(cfl::TreeNode* pNode, void* lParam)
{
	cfl::MutableTreeNode* pMutableNode = (cfl::MutableTreeNode*)pNode;
	AttribMap* pNodeMap = (AttribMap*)pMutableNode->GetData();

	std::string key, val;
	cfl::MutableTreeNode *pProp = NULL;
	AttribMap *pPropMap = NULL;
	if(pNodeMap->Get(PF_ATTRIB_TAG, val) && val.compare("profile") == 0)
	{
		ProfileInfoMap* pOutMap = (ProfileInfoMap*)lParam;

		if(!pNodeMap->Get("id", val))
		{
			return true;
		}
		ProfileCmdBuildInfo* pBuildInfo = NULL;

		ProfileInfoMap::iterator iter = pOutMap->find(val);
		if(iter == pOutMap->end())
		{
			pBuildInfo = new ProfileCmdBuildInfo();
			pOutMap->insert(std::make_pair<std::string, ProfileCmdBuildInfo*>(val, pBuildInfo));
		}
		else
		{
			pBuildInfo = iter->second;
		}

		std::string name, value;
		int i, nChildCount = pMutableNode->GetChildCount();
		for(i = 0; i < nChildCount; i++)
		{
			pProp = (cfl::MutableTreeNode*)pMutableNode->GetChild(i);
			pPropMap = (AttribMap*)pProp->GetData();

			if(pPropMap->Get(PF_ATTRIB_TAG, val) && val.compare("property") == 0)
			{
				if(pPropMap->Get("name", name) && pPropMap->Get("value", value))
				{
					(*(pBuildInfo->m_pBuilderProp))[name] = value;
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
	ProfileCmdBuildInfo* pBuilderInfo = NULL;

	//Base Builder
	m_mapGroup2Builder[BUILDER_BASE] = new MeCmdListBuilder();

	//MP4 Builder
	m_mapGroup2Builder[BUILDER_MP4] = new MP4CmdListBuilder();
}
