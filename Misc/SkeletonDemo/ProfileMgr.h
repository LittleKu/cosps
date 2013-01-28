// ProfileMgr.h: interface for the ProfileMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILEMGR_H__CF2F5ACB_6C57_4C7E_8D51_2F1D6E83FCF9__INCLUDED_)
#define AFX_PROFILEMGR_H__CF2F5ACB_6C57_4C7E_8D51_2F1D6E83FCF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cfltemplate/TreeMap.h"
#include "cflbase/TreeNode.h"
#include "CmdBuilder.h"

#define PF_ATTRIB_TAG	"_TAG"
#define PF_ATTRIB_ID	"id"

typedef cfl::TreeNode ProfileNode;
typedef cfl::TreeMap<std::string, std::string> AttribMap;

typedef cfl::TreeMap<std::string, std::string> SSTreeMap;
typedef cfl::TreeMap<std::string, SSTreeMap*> ProfilePropMap;
typedef cfl::TreeMap<std::string, CmdListBuilder*> BuilderMap;

class ProfileMgr  
{
public:
	ProfileMgr();
	virtual ~ProfileMgr();

	//singleton
	static ProfileMgr* GetInstance();

	//load profile tree
	bool LoadProfileTree(const char* lpFile);
	//get the root profile
	ProfileNode* GetRootProfile() { return m_pRootProfile; }

	//create cmd list builder
	virtual CmdListBuilder* CreateBuilder(const std::string& szProfile, StrObjPtrContext* pContext);

private:
	void InitBuilderMap();
	void InitProfilePropMap();
	void DestroyRootProfile();

	ProfileNode* CreateNode();
	void DestoryNode(ProfileNode* pNode);
private:
	ProfileNode*	m_pRootProfile;
	ProfilePropMap	m_mapProfileProp;
	BuilderMap		m_mapBuilder;
};

#endif // !defined(AFX_PROFILEMGR_H__CF2F5ACB_6C57_4C7E_8D51_2F1D6E83FCF9__INCLUDED_)
