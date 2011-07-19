// LangGrammarMap.cpp: implementation of the CLangGrammarMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LangGrammarMap.h"
#include <memory>
#include "ListCtrlDemo.h"
#include "tinyxml.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME("LangGrammarMap.cpp")

CLangGrammarInfo::CLangGrammarInfo() : m_nLangType(-1), m_szLangName(""), m_pLangGrammar(NULL)
{
}
CLangGrammarInfo::~CLangGrammarInfo()
{
	if(m_pLangGrammar != NULL)
	{
		delete m_pLangGrammar;
	}
}

BOOL CLangGrammarInfo::IsUserDefinedLangGrammar(int nLangRuleType)
{
	return nLangRuleType > USER_DEFINED_LANG_ID_START;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLangGrammarMap* CLangGrammarMap::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<CLangGrammarMap> ptr(new CLangGrammarMap);	
 	return ptr.get();
}
CLangGrammarMap::CLangGrammarMap()
{
	Init(CommonUtils::GetConfFilePath(_T("lang_grammar.xml")));
}

CLangGrammarMap::~CLangGrammarMap()
{
	Save(CommonUtils::GetConfFilePath(_T("lang_grammar.xml"), GCFP_USER));

	MapInt2LangGrammarInfoPtr::iterator it;

	CLangGrammarInfo* pLangGrammarInfo;
	for(it = m_mapLangGrammar.begin() ; it != m_mapLangGrammar.end(); it++)
	{
		pLangGrammarInfo = it->second;
		ASSERT(pLangGrammarInfo);
		delete pLangGrammarInfo;
	}

	LOG4CPLUS_INFO(THE_LOGGER, "CLangGrammarMap::~CLangGrammarMap() called.")
}

void CLangGrammarMap::Init(LPCTSTR lpXmlConfigFile)
{
	TiXmlDocument doc( lpXmlConfigFile );
	bool loadOkay = doc.LoadFile();	
	if ( !loadOkay )
	{
		CString sLogInfo;
		sLogInfo.Format("Failed to load file %s. Error=%s.", lpXmlConfigFile, doc.ErrorDesc());
		LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)sLogInfo)
		return;
	}
	
	TiXmlNode *pNode = NULL;
	TiXmlElement *pElement = NULL;
	
	pNode = doc.FirstChild( "lang_grammars" );
	ASSERT( pNode );
	pElement = pNode->ToElement();
	ASSERT(pElement);

	TiXmlNode *pSubNode = NULL;
	TiXmlElement *pSubElement = NULL;
	
	const char* pBuffer = NULL;
	int iIntValue = 0;

	const char* pBuffer1 = NULL;
	const char* pBuffer2 = NULL;
	for(pNode = pElement->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
	{
		pElement = pNode->ToElement();
		ASSERT(pElement);

		CLangGrammarInfo* pLangGrammarInfo = new CLangGrammarInfo();

		pBuffer = pElement->Attribute("name");
		pLangGrammarInfo->m_szLangName = pBuffer;

		pBuffer = pElement->Attribute("type");
		if(pBuffer != NULL)
		{
			pLangGrammarInfo->m_nLangType = atoi(pBuffer);
		}

		CLangGrammarBuilder builder;
		for(pSubNode = pElement->FirstChild("line_comment"); pSubNode != NULL; pSubNode = pSubNode->NextSibling("line_comment"))
		{
			pSubElement = pSubNode->ToElement();
			ASSERT(pSubElement);

			pBuffer1 = pSubElement->Attribute("content");

			pBuffer2 = pSubElement->Attribute("start_column");
			if(pBuffer2 != NULL)
			{
				iIntValue = atoi(pBuffer2);
			}
			else
			{
				iIntValue = START_COLUMN_ANY;
			}

			BOOL bCaseSensitive = TRUE;
			pBuffer2 = pSubElement->Attribute("case_sensitive");
			if(pBuffer2 != NULL && _stricmp(pBuffer2, "false") == 0)
			{
				bCaseSensitive = FALSE;
			}
			builder.AddSingleComment(pBuffer1, iIntValue, bCaseSensitive);
		}
		for(pSubNode = pElement->FirstChild("escape"); pSubNode != NULL; pSubNode = pSubNode->NextSibling("escape"))
		{
			pSubElement = pSubNode->ToElement();
			ASSERT(pSubElement);

			pBuffer1 = pSubElement->Attribute("content");
			builder.AddEscapeStr(pBuffer1);
		}
		for(pSubNode = pElement->FirstChild("block_comment"); pSubNode != NULL; pSubNode = pSubNode->NextSibling("block_comment"))
		{
			pSubElement = pSubNode->ToElement();
			ASSERT(pSubElement);
			
			pBuffer1 = pSubElement->Attribute("start");
			pBuffer2 = pSubElement->Attribute("end");
			
			builder.AddMultiComment(pBuffer1, pBuffer2);
		}
		for(pSubNode = pElement->FirstChild("string"); pSubNode != NULL; pSubNode = pSubNode->NextSibling("string"))
		{
			pSubElement = pSubNode->ToElement();
			ASSERT(pSubElement);
			
			pBuffer1 = pSubElement->Attribute("start");
			pBuffer2 = pSubElement->Attribute("end");
			
			builder.AddStringMark(pBuffer1, pBuffer2);
		}
		for(pSubNode = pElement->FirstChild("single_character"); pSubNode != NULL; pSubNode = pSubNode->NextSibling("single_character"))
		{
			pSubElement = pSubNode->ToElement();
			ASSERT(pSubElement);
			
			pBuffer1 = pSubElement->Attribute("start");
			pBuffer2 = pSubElement->Attribute("end");
			
			builder.AddCharMark(pBuffer1, pBuffer2);
		}

		pLangGrammarInfo->m_pLangGrammar = builder.GetResult();

		AddLangGrammarInfo(pLangGrammarInfo);
// 		//Add the LangGrammarInfo to the map
// 		MapInt2LangGrammarInfoPtr::iterator it = m_mapLangGrammar.find(pLangGrammarInfo->m_nLangType);
// 		//A same type key already exists
// 		if(it != m_mapLangGrammar.end())
// 		{
// 			TCHAR sLogBuffer[128];
// 			_stprintf(sLogBuffer, "duplicate lang type [%d] exists for lang [%s] and [%s].", 
// 				pLangGrammarInfo->m_nLangType, it->second->m_szLangName, pLangGrammarInfo->m_szLangName);
// 			LOG4CPLUS_ERROR_STR(THE_LOGGER, sLogBuffer)
// 
// 			//Delete the latter one
// 			delete pLangGrammarInfo;
// 		}
// 		else
// 		{
// 			m_mapLangGrammar[pLangGrammarInfo->m_nLangType] = pLangGrammarInfo;
// 		}
	}
}

void CLangGrammarMap::Save(LPCTSTR lpXmlConfigFile)
{
	TiXmlDocument doc(lpXmlConfigFile);
	
	//declaration
	TiXmlDeclaration declaration("1.0", "UTF-8", "yes");
	doc.InsertEndChild(declaration);
	
	//root element: lang_grammars
	TiXmlElement lang_grammars("lang_grammars");
	

	//sub element: lang_grammar
	MapInt2LangGrammarInfoPtr::iterator it;
	
	int nType;
	CLangGrammarInfo* pLangGrammarInfo;

	int i, size;
	ILangGrammar* pLangGrammar;
	for(it = m_mapLangGrammar.begin() ; it != m_mapLangGrammar.end(); it++)
	{
		nType = it->first;
		pLangGrammarInfo = it->second;
		ASSERT(pLangGrammarInfo);

		TiXmlElement lang_grammar("lang_grammar");
		lang_grammar.SetAttribute("name", pLangGrammarInfo->m_szLangName);
		lang_grammar.SetAttribute("type", pLangGrammarInfo->m_nLangType);

		pLangGrammar = pLangGrammarInfo->m_pLangGrammar;
		ASSERT(pLangGrammar);

		size = pLangGrammar->GetCountOfSingleLineComment();
		for(i = 0; i < size; i++)
		{
			CSingleLineComment& singleLineComment = pLangGrammar->GetSingleLineComment(i);

			TiXmlElement line_comment("line_comment");
			line_comment.SetAttribute("content", singleLineComment.m_szTag);
			if(singleLineComment.m_nStartCol != START_COLUMN_ANY)
			{
				line_comment.SetAttribute("start_column", singleLineComment.m_nStartCol);
			}	
			if(!singleLineComment.m_bCaseSensitive)
			{
				line_comment.SetAttribute("case_sensitive", "false");
			}
			
			lang_grammar.InsertEndChild(line_comment);
		}

		size = pLangGrammar->GetCountOfEscStr();
		for(i = 0; i < size; i++)
		{
			TiXmlElement escape("escape");
			escape.SetAttribute("content", pLangGrammar->GetEscapeStr(i));

			lang_grammar.InsertEndChild(escape);
		}

		size = pLangGrammar->GetCountOfMultiLineComment();
		for(i = 0; i < size; i++)
		{
			CMultiLineComment& multiComment = pLangGrammar->GetMultiLineComment(i);
			TiXmlElement block_comment("block_comment");
			block_comment.SetAttribute("start", multiComment.m_szStart);
			block_comment.SetAttribute("end", multiComment.m_szEnd);

			lang_grammar.InsertEndChild(block_comment);
		}

		size = pLangGrammar->GetCountOfStringMark();
		for(i = 0; i < size; i++)
		{
			CPair& stringMark = pLangGrammar->GetStringMark(i);
			TiXmlElement string_element("string");
			string_element.SetAttribute("start", stringMark.m_szStart);
			string_element.SetAttribute("end", stringMark.m_szEnd);
			
			lang_grammar.InsertEndChild(string_element);
		}

		size = pLangGrammar->GetCountOfCharMark();
		for(i = 0; i < size; i++)
		{
			CPair& charMark = pLangGrammar->GetCharMark(i);
			TiXmlElement single_character("single_character");
			single_character.SetAttribute("start", charMark.m_szStart);
			single_character.SetAttribute("end", charMark.m_szEnd);
			
			lang_grammar.InsertEndChild(single_character);
		}

		lang_grammars.InsertEndChild(lang_grammar);
	}

	doc.InsertEndChild(lang_grammars);

	//Validation
	if(doc.Error())
	{
		CString sLogInfo;
		sLogInfo.Format("Failed to Validation when save file %s. Error in %s: %s.", lpXmlConfigFile, doc.Value(), doc.ErrorDesc());
		LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)sLogInfo)
	}
	if(!doc.SaveFile())
	{
		CString sLogInfo;
		sLogInfo.Format("Failed to save file %s. Error in %s: %s.", lpXmlConfigFile, doc.Value(), doc.ErrorDesc());
		LOG4CPLUS_ERROR_STR(THE_LOGGER, (LPCTSTR)sLogInfo)
	}
}

void CLangGrammarMap::AddLangGrammarInfo(CLangGrammarInfo* pLangGrammarInfo)
{
	ASSERT(pLangGrammarInfo != NULL);
	//TODO: make sure we can give an unique id for the type
	if(pLangGrammarInfo->m_nLangType < 0)
	{
		//Find the biggest type value
		MapInt2LangGrammarInfoPtr::reverse_iterator rit = m_mapLangGrammar.rbegin();
		int nMaxType = (rit != m_mapLangGrammar.rend()) ? rit->first : USER_DEFINED_LANG_ID_START;
		if(nMaxType < USER_DEFINED_LANG_ID_START)
		{
			nMaxType = USER_DEFINED_LANG_ID_START;
		}
		nMaxType++;
		pLangGrammarInfo->m_nLangType = nMaxType;
	}

	//Add the LangGrammarInfo to the map
	MapInt2LangGrammarInfoPtr::iterator it = m_mapLangGrammar.find(pLangGrammarInfo->m_nLangType);
	//A same type key already exists
	if(it != m_mapLangGrammar.end())
	{
		TCHAR sLogBuffer[128];
		_stprintf(sLogBuffer, "duplicate lang type [%d] exists for lang [%s] and [%s].", 
			pLangGrammarInfo->m_nLangType, it->second->m_szLangName, pLangGrammarInfo->m_szLangName);
		LOG4CPLUS_ERROR_STR(THE_LOGGER, sLogBuffer)
			
		//Delete the latter one
		delete pLangGrammarInfo;
	}
	else
	{
		m_mapLangGrammar[pLangGrammarInfo->m_nLangType] = pLangGrammarInfo;
	}
}

std::map<int, CLangGrammarInfo*>& CLangGrammarMap::GetLangGrammaInfoMap()
{
	return m_mapLangGrammar;
}

CLangGrammarInfo* CLangGrammarMap::GetLangGrammarInfo(int nLangType)
{
	MapInt2LangGrammarInfoPtr::const_iterator iter = m_mapLangGrammar.find(nLangType);
	if(iter == m_mapLangGrammar.end())
	{
		return NULL;
	}
	return iter->second;
}

void CLangGrammarMap::ModifyLangGrammarInfo(CLangGrammarInfo* pLangGrammarInfo)
{
	MapInt2LangGrammarInfoPtr::iterator it = m_mapLangGrammar.find(pLangGrammarInfo->m_nLangType);
	if(it != m_mapLangGrammar.end())
	{
		CLangGrammarInfo* pOldItem = it->second;
		delete pOldItem;

		m_mapLangGrammar[pLangGrammarInfo->m_nLangType] = pLangGrammarInfo;
	}
}
void CLangGrammarMap::DeleteLangGrammarInfo(int nLangType)
{
	MapInt2LangGrammarInfoPtr::iterator it = m_mapLangGrammar.find(nLangType);
	if(it != m_mapLangGrammar.end())
	{
		CLangGrammarInfo* pOldItem = it->second;
		delete pOldItem;
		
		m_mapLangGrammar.erase(it);
	}
}

