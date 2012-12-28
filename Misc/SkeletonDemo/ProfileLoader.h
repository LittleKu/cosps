// ProfileLoader.h: interface for the CProfileLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILELOADER_H__09AFCEBD_9192_4108_8876_90D5631976A6__INCLUDED_)
#define AFX_PROFILELOADER_H__09AFCEBD_9192_4108_8876_90D5631976A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cfltemplate/TreeMap.h"

typedef cfl::TreeMap<std::string, std::string> AttribMap;

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

class CProfileLoader  
{
public:
	CProfileLoader();
	virtual ~CProfileLoader();

	bool LoadProfileTree(const char* lpFile);
	ProfileNode* GetRootProfile() { return m_pRootProfile; }

private:
	ProfileNode*	m_pRootProfile;
};

#endif // !defined(AFX_PROFILELOADER_H__09AFCEBD_9192_4108_8876_90D5631976A6__INCLUDED_)
