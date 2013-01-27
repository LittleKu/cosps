// ProfileLoader.h: interface for the CProfileLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILELOADER_H__09AFCEBD_9192_4108_8876_90D5631976A6__INCLUDED_)
#define AFX_PROFILELOADER_H__09AFCEBD_9192_4108_8876_90D5631976A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cfltemplate/TreeMap.h"
#include "cflbase/TreeNode.h"

#define PF_ATTRIB_TAG	"tag"

typedef cfl::TreeMap<std::string, std::string> AttribMap;

/*
class ProfileNode
{
public:
	AttribMap		attribMap;
	ProfileNode*	parent;
	std::vector<ProfileNode*>*	children;

public:
	ProfileNode();
	virtual ~ProfileNode();
	int GetChildCount();
	ProfileNode* GetChildAt(int nChildIndex);
	void AddChild(ProfileNode* pChild);
};
*/

typedef cfl::TreeNode ProfileNode;

class CProfileLoader  
{
public:
	CProfileLoader();
	virtual ~CProfileLoader();
	static CProfileLoader* GetInstance();

	bool LoadProfileTree(const char* lpFile);
	ProfileNode* GetRootProfile() { return m_pRootProfile; }

private:
	ProfileNode* CreateNode();
	void DestoryNode(ProfileNode* pNode);
private:
	ProfileNode*	m_pRootProfile;
};

#endif // !defined(AFX_PROFILELOADER_H__09AFCEBD_9192_4108_8876_90D5631976A6__INCLUDED_)
