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

class CLangGrammarInfo
{
public:
	CLangGrammarInfo();
	~CLangGrammarInfo();
public:
	int m_nLangType;
	CString m_szLangName;
	ILangGrammar* m_pLangGrammar;
};

class CLangGrammarMap  
{
private:
	CLangGrammarMap();
	typedef std::map<int, CLangGrammarInfo*> MapInt2LangGrammarInfoPtr;
	MapInt2LangGrammarInfoPtr m_mapLangGrammar;
public:
	void Init(LPCTSTR lpXmlConfigFile);
	void Save(LPCTSTR lpXmlConfigFile);
	void AddLangGrammarInfo(CLangGrammarInfo* pLangGrammarInfo);
	virtual ~CLangGrammarMap();
public:
	static CLangGrammarMap* GetInstance();
};

#endif // !defined(AFX_LANGGRAMMARMAP_H__E5DEA38A_3B11_44BA_B323_FB1DC7C1DEA9__INCLUDED_)
