// PropListMgr.h: interface for the CPropListMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPLISTMGR_H__48D36CFF_F05B_4E1C_B7B4_BB67D19942A8__INCLUDED_)
#define AFX_PROPLISTMGR_H__48D36CFF_F05B_4E1C_B7B4_BB67D19942A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include "cflbase/tstring.h"
#include "OptionContext.h"

class TiXmlElement;

class CPropListMgr  
{
public:
	CPropListMgr();
	virtual ~CPropListMgr();

	virtual BOOL CreatePropList(const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL Init(LPCSTR lpszXmlFile);
	virtual BOOL GetPropMap(OptionContext* pPropMap);

private:
	BOOL InitProp(CBCGPProp* pParentProp, TiXmlElement *pParentXml);
	void RemoveAllPropData();
	void CStr2TString(const char* str, cfl::tstring& szResult);
	void GetPropValue(CBCGPProp* pProp, OptionContext* pPropMap);
protected:
	CBCGPPropList	m_wndPropList;
	std::vector<std::string*> m_pPropDataVec;
};

#endif // !defined(AFX_PROPLISTMGR_H__48D36CFF_F05B_4E1C_B7B4_BB67D19942A8__INCLUDED_)
