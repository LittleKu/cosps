// SetEnv.cpp: implementation of the CSetEnv class.
//
//////////////////////////////////////////////////////////////////////

#include "SetEnv.h"
#include "tinyxml.h"

#include "ProcessEnv.h"
#include "SystemEnv.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CSetEnv::ToWide(const std::string& src, std::wstring& dst, UINT nCodePage)
{
	int nSize = ::MultiByteToWideChar(nCodePage, 0, src.c_str(), src.size() + 1, NULL, 0);
	if(nSize == 0)
	{
		return false;
	}

	wchar_t* wchBuffer = new wchar_t[nSize];
	::MultiByteToWideChar(nCodePage, 0, src.c_str(), src.size() + 1, wchBuffer, nSize);
	dst = wchBuffer;

	delete []wchBuffer;

	return true;
}

bool CSetEnv::ExpandVariable(const std::wstring& src, std::wstring& dst)
{
	DWORD dwSize = ExpandEnvironmentStringsW(src.c_str(), NULL, 0);

	if(dwSize == 0)
	{
		return false;
	}

	wchar_t* wchBuffer = new wchar_t[dwSize];
	ExpandEnvironmentStringsW(src.c_str(), wchBuffer, dwSize);
	dst = wchBuffer;

	delete []wchBuffer;

	return true;
}

CSetEnv::CSetEnv()
{
	m_pProcessEnv = new CProcessEnv();
	m_pSystemEnv = new CSystemEnv(TRUE);
}

CSetEnv::~CSetEnv()
{
	delete m_pProcessEnv;
	m_pProcessEnv = NULL;

	delete m_pSystemEnv;
	m_pSystemEnv = NULL;
}

bool CSetEnv::Set(const char * lpXMLFile)
{
	std::vector<VariableEntry> entries;
	if(!LoadFile(lpXMLFile, entries))
	{
		return false;	
	}

	std::vector<VariableEntry>::iterator entryIter;
	std::vector<VariableValue>::iterator valueIter;

	for(entryIter = entries.begin(); entryIter != entries.end(); entryIter++)
	{
		//Make the separator to Unicode
		std::wstring wsep;
		if(!entryIter->separator.empty())
		{
			ToWide(entryIter->separator, wsep, CP_UTF8);
		}

		//Concat all the values
		std::wstring targetValue;
		for(valueIter = entryIter->values.begin(); valueIter != entryIter->values.end(); valueIter++)
		{
			std::wstring wstr;
			ToWide(valueIter->value, wstr, CP_UTF8);

			if(valueIter->expand)
			{
				std::wstring expanded;
				if(ExpandVariable(wstr, expanded))
				{
					wstr = expanded;
				}
			}

			if(!wsep.empty() && valueIter != entryIter->values.begin())
			{
				targetValue.append(wsep);
			}
			targetValue.append(wstr);
		}

		//Determine which env to be used
		CEnvironment* pEnvironment;
		if(entryIter->scope == "process")
		{
			pEnvironment = m_pProcessEnv;
		}
		else
		{
			pEnvironment = m_pSystemEnv;	
		}

		std::wstring varName;
		ToWide(entryIter->name, varName, CP_UTF8);

		if(entryIter->type == "set")
		{
			pEnvironment->SetEnv(varName.c_str(), targetValue);
		}
		else if(entryIter->type == "prepend")
		{
			targetValue.append(wsep);
			pEnvironment->Prepend(varName.c_str(), targetValue);
		}
		else if(entryIter->type == "append")
		{
			targetValue.insert(0, wsep);
			pEnvironment->Append(varName.c_str(), targetValue);
		}
	}

	return true;
}

bool CSetEnv::Unset(const char * lpXMLFile)
{
	return false;
}

bool CSetEnv::LoadFile(const char* lpXMLFile, std::vector<VariableEntry>& entries)
{
	TiXmlDocument doc( lpXMLFile );
	bool loadOkay = doc.LoadFile();	
	if ( !loadOkay )
	{
		return false;
	}

	TiXmlNode *pNode = NULL;
	TiXmlElement *pElement = NULL;

	TiXmlNode *pSubNode = NULL;
	TiXmlElement *pSubElement = NULL;
	const char* pStr = NULL;

	pNode = doc.FirstChild( "environment" );
	pElement = pNode->ToElement();

	for(pNode = pElement->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
	{
		pElement = pNode->ToElement();
		//This is not an element node
		if(pElement == NULL)
		{
			continue;
		}
		pStr = pElement->Value();

		//This is not a variable element
		if(pStr == NULL || strcmp(pStr, "variable") != 0)
		{
			continue;
		}

		VariableEntry entry;
		entry.name = pElement->Attribute("name");
		entry.type = pElement->Attribute("type");
		entry.scope = pElement->Attribute("scope");

		pStr = pElement->Attribute("separator");
		if(pStr != NULL)
		{
			entry.separator = pStr;
		}

		for(pSubNode = pElement->FirstChild(); pSubNode != NULL; pSubNode = pSubNode->NextSibling())
		{
			pSubElement = pSubNode->ToElement();
			pStr = pSubElement->Value();

			//This is not a value element
			if(pStr == NULL || strcmp(pStr, "value") != 0)
			{
				continue;
			}

			VariableValue varValue;

			pStr = pSubElement->Attribute("expand");
			if(pStr != NULL && _stricmp(pStr, "true") == 0)
			{
				varValue.expand = true;
			}
			else
			{
				varValue.expand = false;
			}

			varValue.value = pSubElement->GetText();

			entry.values.push_back(varValue);
		}

		entries.push_back(entry);
	}

	return true;
}
