// ProfileLoader.cpp: implementation of the CProfileLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProfileLoader.h"
#include "tinyxml.h"
#include "cflbase/tstring.h"
#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("ProfileLoader"))

#define XML_NM_ROOT		"root"
#define PF_ATTRIB_TAG	"tag"

typedef struct
{
	ProfileNode*	pProfileNode;
	TiXmlElement*	pXmlNode;	
}ProfileXmlPair;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ProfileNode::ProfileNode() : parent(NULL), children(NULL)
{

}
ProfileNode::~ProfileNode()
{
	if(children != NULL)
	{
		for(int i = children->size() - 1; i >= 0; i--)
		{
			delete children->at(i);
		}

		delete children;
		children = NULL;
	}
}
int ProfileNode::GetChildCount()
{
	return ((children == NULL) ? 0 : children->size());
}

ProfileNode* ProfileNode::GetChildAt(int nChildIndex)
{
	if(nChildIndex < 0 || nChildIndex >= GetChildCount())
	{
		return NULL;
	}
	return children->at(nChildIndex);
}
void ProfileNode::AddChild(ProfileNode* pChild)
{
	if(children == NULL)
	{
		children = new std::vector<ProfileNode*>;
	}
	children->push_back(pChild);
	pChild->parent = this;
}

CProfileLoader::CProfileLoader() : m_pRootProfile(NULL)
{

}

CProfileLoader::~CProfileLoader()
{
	if(m_pRootProfile != NULL)
	{
		delete m_pRootProfile;
		m_pRootProfile = NULL;
	}	
}

bool CProfileLoader::LoadProfileTree(const char* lpszXmlFile)
{
	if(m_pRootProfile != NULL)
	{
		delete m_pRootProfile;
		m_pRootProfile = NULL;
	}

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

	m_pRootProfile = new ProfileNode();
	m_pRootProfile->attribMap.Put(PF_ATTRIB_TAG, XML_NM_ROOT);

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

			ProfileNode* pProfile = new ProfileNode();
			pProfileParent->AddChild(pProfile);
			pProfile->attribMap.Put(PF_ATTRIB_TAG, pNode->Value());
			
			pAttrib = pFirstAttrib = pElement->FirstAttribute();
			for( ; pAttrib != NULL; pAttrib = pAttrib->Next())
			{
				if(pAttrib->Next() == pFirstAttrib)
				{
					break;
				}
				pProfile->attribMap.Put(pAttrib->Name(), pAttrib->Value());
			}

			pxPair.pProfileNode = pProfile;
			pxPair.pXmlNode = pElement;

			pxPairList.push_back(pxPair);
		}
	}
	
	return true;
}
