// PropListMgr.cpp: implementation of the CPropListMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PropListMgr.h"
#include "tinyxml.h"
#include "cflbase/tstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(_T("PropListMgr"))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define XML_NM_PROFILE		"profile"
#define XML_NM_PARAMS		"params"

CPropListMgr::CPropListMgr()
{

}

CPropListMgr::~CPropListMgr()
{
	RemoveAllPropData();
}

void CPropListMgr::RemoveAllPropData()
{
	std::vector<std::string*>::iterator iter = m_pPropDataVec.begin();
	for( ; iter != m_pPropDataVec.end(); iter++)
	{
		delete (*iter);
	}
	m_pPropDataVec.clear();
}

BOOL CPropListMgr::CreatePropList(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if(!m_wndPropList.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, pParentWnd, nID))
	{
		return FALSE;
	}
	m_wndPropList.EnableHeaderCtrl ();
	m_wndPropList.EnableDesciptionArea ();
	
	m_wndPropList.SetCustomColors (
		RGB (250, 250, 250),
		RGB (0, 0, 0),
		RGB (240, 240, 240),
		RGB (40, 76, 165),
		RGB (250, 250, 250),
		RGB (0, 85, 136),
		RGB (168, 189, 216));

	return TRUE;
}

BOOL CPropListMgr::Init(LPCSTR lpszXmlFile)
{
	m_wndPropList.RemoveAll();
	RemoveAllPropData();

	TiXmlDocument doc( lpszXmlFile );
	bool loadOkay = doc.LoadFile();	
	if ( !loadOkay )
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("Failed to load file %s. Error=%s."), CFL_A2T(lpszXmlFile), CFL_A2T(doc.ErrorDesc()));
		LOG4CPLUS_ERROR_STR(THE_LOGGER, szLog)
		return FALSE;
	}
	
	TiXmlNode *pNode = NULL;
	TiXmlElement *pElement = NULL;
	
	pNode = doc.FirstChild( XML_NM_PROFILE );
	ASSERT( pNode );
	pElement = pNode->ToElement();
	ASSERT(pElement);
	
	pNode = pElement->FirstChild(XML_NM_PARAMS);
	if(pNode == NULL)
	{
		return FALSE;
	}
	pElement = pNode->ToElement();
	ASSERT(pElement);
	
	
	for(pNode = pElement->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
	{
		pElement = pNode->ToElement();
		ASSERT(pElement);
		
		CBCGPProp* pGroup = new CBCGPProp (pElement->Value());
		
		if(!InitProp(pGroup, pElement))
		{
			return FALSE;
		}
		m_wndPropList.AddProperty(pGroup);
	}
	
	return TRUE;
}

BOOL CPropListMgr::InitProp(CBCGPProp* pParentProp, TiXmlElement *pParentXml)
{
	TiXmlNode *pNode, *pChildNode = NULL;
	TiXmlElement *pElement, *pChildElement = NULL;
	
	std::string* pDataPtr = NULL;
	const char* szID = NULL;
	const char* szName = NULL;
	const char* szDesc = NULL;
	const char* szValue = NULL;
	const char* szDefault = NULL;
	bool bDefSet = false;
	for(pNode = pParentXml->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
	{
		pElement = pNode->ToElement();
		ASSERT(pElement);

		szID = pElement->Attribute("id");
		if(szID == NULL || *szID == '\0')
		{
			continue;
		}
		pDataPtr = new std::string(szID);
		m_pPropDataVec.push_back(pDataPtr);
		
		szName = pElement->Attribute("name");
		if(szName == NULL)
		{
			szName = "Dummy";
		}
		szDesc = pElement->Attribute("desc");
		if(szDesc == NULL)
		{
			szDesc = szName;
		}
		
		CBCGPProp* pProp = new CBCGPProp(CFL_A2T(szName), _T(""), CFL_A2T(szDesc), (DWORD_PTR)pDataPtr);
		bDefSet = false;
		for(pChildNode = pElement->FirstChild(); pChildNode != NULL; pChildNode = pChildNode->NextSibling())
		{
			pChildElement = pChildNode->ToElement();
			ASSERT(pChildElement);
			
			szValue = pChildElement->Attribute("value");
			if(szValue)
			{
				pProp->AddOption(CFL_A2T(szValue));
			}
			szDefault = pChildElement->Attribute("default");
			if(szDefault != NULL && stricmp(szDefault, "true") == 0)
			{
				pProp->SetOriginalValue(szValue);
				pProp->SetValue(szValue);
				bDefSet = true;
			}
		}
		if(!bDefSet && pProp->GetOptionCount() > 0)
		{
			LPCTSTR lpFirstOpt = pProp->GetOption(0);
			if(lpFirstOpt != NULL)
			{
				pProp->SetOriginalValue(lpFirstOpt);
				pProp->SetValue(lpFirstOpt);
			}
		}
		pProp->AllowEdit (FALSE);
		
		pParentProp->AddSubItem (pProp);
	}
	
	return TRUE;
}