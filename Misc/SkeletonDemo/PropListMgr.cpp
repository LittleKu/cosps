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
		//FIXME! remove to the last call
		m_wndPropList.Invalidate();
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
	
	cfl::tstring szVal;
	for(pNode = pElement->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
	{
		pElement = pNode->ToElement();
		ASSERT(pElement);
		
		CStr2TString(pElement->Value(), szVal);
		CBCGPProp* pGroup = new CBCGPProp (szVal.c_str());
		
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
	cfl::tstring tname, tdesc, tvalue;
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
		CStr2TString(szName, tname);
		CStr2TString(szDesc, tdesc);
		
		CBCGPProp* pProp = new CBCGPProp(tname.c_str(), _T(""), tdesc.c_str(), (DWORD_PTR)pDataPtr);
		bDefSet = false;
		for(pChildNode = pElement->FirstChild(); pChildNode != NULL; pChildNode = pChildNode->NextSibling())
		{
			pChildElement = pChildNode->ToElement();
			ASSERT(pChildElement);
			
			szValue = pChildElement->Attribute("value");
			if(szValue)
			{
				CStr2TString(szValue, tvalue);
				pProp->AddOption(tvalue.c_str());
				
				szDefault = pChildElement->Attribute("default");
				if(szDefault != NULL && stricmp(szDefault, "true") == 0)
				{
					pProp->SetOriginalValue(tvalue.c_str());
					pProp->SetValue(tvalue.c_str());
					bDefSet = true;
				}
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

void CPropListMgr::CStr2TString(const char* str, cfl::tstring& szResult)
{
#if defined(_UNICODE) || defined(UNICODE)
	cfl::char2wstring(szResult, str, -1, CP_UTF8);
#else
	std::wstring wstr;
	if(cfl::char2wstring(wstr, str, -1, CP_UTF8))
	{
		cfl::wstring2string(szResult, wstr, CP_ACP);
	}
#endif
}

BOOL CPropListMgr::GetPropMap(OptionContext* pPropMap)
{
	int n = m_wndPropList.GetPropertyCount();
	
	CBCGPProp* pProp = NULL;
	for(int i = 0; i < n; i++)
	{
		pProp = m_wndPropList.GetProperty(i);
		GetPropValue(pProp, pPropMap);
	}
	
	return TRUE;
}

void CPropListMgr::GetPropValue(CBCGPProp* pProp, OptionContext* pPropMap)
{
	if(pProp == NULL)
	{
		return;
	}
	
	//use id as key
	std::string* pData = (std::string*)pProp->GetData();
	if(pData != NULL)
	{
		//value
		std::string szVal;
		SysUtils::Val2Str(pProp->GetValue(), szVal);
		
		
		pPropMap->Put(pData->c_str(), szVal.c_str());
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("%s=%s"), CFL_A2T(pData->c_str()), CFL_A2T(szVal.c_str()));
			LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
		}
	}
	
	for(int i = 0; i < pProp->GetSubItemsCount(); i++)
	{
		GetPropValue(pProp->GetSubItem(i), pPropMap);
	}
}