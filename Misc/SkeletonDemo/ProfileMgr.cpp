// ProfileMgr.cpp: implementation of the ProfileMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProfileMgr.h"
#include "MeCmdListBuilder.h"
#include "MP4CmdListBuilder.h"
#include "cflbase/TreeNode.h"
#include "cflbase/TreeNodeFactory.h"
#include "tinyxml.h"
#include <list>
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

#define XML_NM_ROOT		"root"

typedef struct
{
	ProfileNode*	pProfileNode;
	TiXmlElement*	pXmlNode;	
}ProfileXmlPair;

ProfileMgr::ProfileMgr() : m_pRootProfile(NULL)
{
	InitBuilderMap();
}

ProfileMgr::~ProfileMgr()
{
	CFLPOS pos = NULL;
	std::string key;
	
	//Profile Info map
	SSTreeMap* pValue = NULL;
	pos = m_mapProfileProp.GetFirstPos();
	while(pos != NULL)
	{
		m_mapProfileProp.GetNextEntry(pos, key, pValue);
		assert(pValue);
		delete pValue;
	}
	m_mapProfileProp.ReleasePos(pos);
	m_mapProfileProp.Clear();

	//Builder instance map
	CmdListBuilder* pBuilder = NULL;
	pos = m_mapBuilder.GetFirstPos();
	while(pos != NULL)
	{
		m_mapBuilder.GetNextEntry(pos, key, pBuilder);
		assert(pBuilder);
		delete pBuilder;
	}
	m_mapBuilder.ReleasePos(pos);
	m_mapBuilder.Clear();

	//Root Profile
	DestroyRootProfile();

	LOG4CPLUS_DEBUG_STR(THE_LOGGER, _T("ProfileMgr::~ProfileMgr() called"))
}

ProfileMgr* ProfileMgr::GetInstance()
{
	static ProfileMgr singleton;
	return &singleton;
}

void ProfileMgr::InitBuilderMap()
{
	//MEncoder Builder
	m_mapBuilder.Put(BUILDER_BASE, new MeCmdListBuilder());
	
	//MP4 Builder
	m_mapBuilder.Put(BUILDER_MP4, new MP4CmdListBuilder());
}

void ProfileMgr::DestroyRootProfile()
{
	if(m_pRootProfile != NULL)
	{
		DestoryNode(m_pRootProfile);
		m_pRootProfile = NULL;
	}
}

bool ProfileMgr::LoadProfileTree(const char* lpszXmlFile)
{
	DestroyRootProfile();
	
	TiXmlDocument doc( lpszXmlFile );
	bool loadOkay = doc.LoadFile();	
	if ( !loadOkay )
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("Failed to load file %s. Error=%s."), CFL_A2T(lpszXmlFile), CFL_A2T(doc.ErrorDesc()));
		LOG4CPLUS_ERROR_STR(THE_LOGGER, szLog)
		return false;
	}
	
	TiXmlNode *pNode = NULL;
	TiXmlElement *pElement = NULL;
	
	pNode = doc.FirstChild(XML_NM_ROOT);
	if(pNode == NULL || (pElement = pNode->ToElement()) == NULL)
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("Failed to find first child node [%s]."), CFL_A2T(XML_NM_ROOT));
		LOG4CPLUS_ERROR_STR(THE_LOGGER, szLog)
		return false;
	}
	
	AttribMap* pMap = NULL;
	
	m_pRootProfile = CreateNode();
	pMap = (AttribMap*)m_pRootProfile->GetData();
	pMap->Put(PF_ATTRIB_TAG, XML_NM_ROOT);
	
	ProfileXmlPair pxPair;
	pxPair.pProfileNode = m_pRootProfile;
	pxPair.pXmlNode = pElement;
	
	std::list<ProfileXmlPair> pxPairList;
	pxPairList.push_back(pxPair);
	
	ProfileNode* pProfileParent = NULL;
	TiXmlElement* pXmlParent = NULL;
	TiXmlAttribute	*pAttrib = NULL, *pFirstAttrib = NULL;
	while(!pxPairList.empty())
	{
		pxPair = pxPairList.front();
		pxPairList.pop_front();
		
		pXmlParent = pxPair.pXmlNode;
		pProfileParent = pxPair.pProfileNode;
		
		for(pNode = pXmlParent->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
		{
			pElement = pNode->ToElement();
			if(pElement == NULL)
			{
				continue;
			}
			
			ProfileNode* pProfile = CreateNode();
			
			pProfileParent->Add(pProfile);
			pMap = (AttribMap*)pProfile->GetData();
			pMap->Put(PF_ATTRIB_TAG, pNode->Value());
			
			pAttrib = pFirstAttrib = pElement->FirstAttribute();
			for( ; pAttrib != NULL; pAttrib = pAttrib->Next())
			{
				if(pAttrib->Next() == pFirstAttrib)
				{
					break;
				}
				pMap->Put(pAttrib->Name(), pAttrib->Value());
			}
			
			pxPair.pProfileNode = pProfile;
			pxPair.pXmlNode = pElement;
			
			pxPairList.push_back(pxPair);
		}
	}

	//Init profile property map when load done
	InitProfilePropMap();
	
	return true;
}

ProfileNode* ProfileMgr::CreateNode()
{
	ProfileNode* pProfile = cfl::TreeNodeFactory::CreateTreeNode();
	pProfile->SetData(new AttribMap());
	return pProfile;
}

static void DestroyProc(cfl::TreeNode* pNode, void* pData)
{
	if(pData)
	{
		AttribMap* pAttribMap = (AttribMap*)pData;
		delete pAttribMap;
	}
}
void ProfileMgr::DestoryNode(ProfileNode* pNode)
{
	cfl::TreeNodeFactory::DestoryTreeNode(pNode, DestroyProc, true);
}

CmdListBuilder* ProfileMgr::CreateBuilder(const std::string& szProfile, StrObjPtrContext* pContext)
{
	SSTreeMap* pPropMap = NULL;
	if(!m_mapProfileProp.Get(szProfile, pPropMap))
	{
		return NULL;
	}

	std::string szBuilderName;
	if(!pPropMap->Get(BUILDER_NAME, szBuilderName))
	{
		return NULL;
	}

	CmdListBuilder* pBuilder = NULL;
	if(!m_mapBuilder.Get(szBuilderName, pBuilder))
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

	std::string val;
	AttribMap *pPropMap = NULL;
	if(pNodeMap->Get(PF_ATTRIB_TAG, val) && val.compare("profile") == 0)
	{
		ProfilePropMap* pOutMap = (ProfilePropMap*)lParam;

		if(!pNodeMap->Get(PF_ATTRIB_ID, val))
		{
			return true;
		}
		SSTreeMap* pBuildInfo = NULL;
		if(!pOutMap->Get(val, pBuildInfo))
		{
			pBuildInfo = new SSTreeMap();
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

void ProfileMgr::InitProfilePropMap()
{
	//deprecated hardcode.
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
	if(m_pRootProfile == NULL)
	{
		LOG4CPLUS_ERROR_STR(THE_LOGGER, _T("InitProfilePropMap failed. Root Profile is null"))
		return;
	}

	m_pRootProfile->BFSEnum(InitProfileGroupProc, &m_mapProfileProp);
}

