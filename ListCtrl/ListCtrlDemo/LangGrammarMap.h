// LangGrammarMap.h: interface for the CLangGrammarMap class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_LANGGRAMMARMAP_H__E5DEA38A_3B11_44BA_B323_FB1DC7C1DEA9__INCLUDED_)
#define AFX_LANGGRAMMARMAP_H__E5DEA38A_3B11_44BA_B323_FB1DC7C1DEA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LangGrammar.h"
#include <map>

#define USER_DEFINED_LANG_ID_START		10000

class CLangGrammarInfo
{
public:
	CLangGrammarInfo();
	~CLangGrammarInfo();
	BOOL IsUserDefined() const { return m_nLangType > USER_DEFINED_LANG_ID_START; }
	static BOOL IsUserDefinedLangGrammar(int nLangRuleType);
public:
	int m_nLangType;
	CString m_szLangName;
	ILangGrammar* m_pLangGrammar;
};

class CLangGrammarMap  
{
public:
	typedef std::map<int, CLangGrammarInfo*> MapInt2LangGrammarInfoPtr;
	virtual ~CLangGrammarMap();
	void Init(LPCTSTR lpXmlConfigFile);
	void Save(LPCTSTR lpXmlConfigFile);
	void AddLangGrammarInfo(CLangGrammarInfo* pLangGrammarInfo);
	void ModifyLangGrammarInfo(CLangGrammarInfo* pLangGrammarInfo);
	void DeleteLangGrammarInfo(int nLangType);
	CLangGrammarInfo* GetLangGrammarInfo(int nLangType);
	std::map<int, CLangGrammarInfo*>& GetLangGrammaInfoMap();
private:
	CLangGrammarMap();	
	MapInt2LangGrammarInfoPtr m_mapLangGrammar;
public:
	static CLangGrammarMap* GetInstance();
};

#endif // !defined(AFX_LANGGRAMMARMAP_H__E5DEA38A_3B11_44BA_B323_FB1DC7C1DEA9__INCLUDED_)
