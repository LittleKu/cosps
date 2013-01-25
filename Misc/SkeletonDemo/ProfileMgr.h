// ProfileMgr.h: interface for the ProfileMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILEMGR_H__CF2F5ACB_6C57_4C7E_8D51_2F1D6E83FCF9__INCLUDED_)
#define AFX_PROFILEMGR_H__CF2F5ACB_6C57_4C7E_8D51_2F1D6E83FCF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBuilder.h"

typedef std::map<std::string, std::string> ssmap;

class ProfileCmdBuildInfo
{
public:
	std::string			m_BuilderName;
	ssmap*				m_pBuilderProp;

	ProfileCmdBuildInfo();
	~ProfileCmdBuildInfo();
};
typedef std::map<std::string, ProfileCmdBuildInfo*> ProfileInfoMap;

class ProfileMgr  
{
public:
	ProfileMgr();
	virtual ~ProfileMgr();
	static ProfileMgr* GetInstance();

	virtual CmdListBuilder* CreateBuilder(const std::string& szProfile, StrObjPtrContext* pContext);

private:
	void InitProfileGroupMap();
	void InitBuilderMap();

private:
	typedef std::map<std::string, CmdListBuilder*> BuilderMap;

	ProfileInfoMap m_mapProfile2Group;
	BuilderMap m_mapGroup2Builder;
};

#endif // !defined(AFX_PROFILEMGR_H__CF2F5ACB_6C57_4C7E_8D51_2F1D6E83FCF9__INCLUDED_)
