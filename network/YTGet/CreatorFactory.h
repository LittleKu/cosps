// CreatorFactory.h: interface for the CCreatorFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATORFACTORY_H__88E93FD6_332F_47B9_A86D_52F3D881F08B__INCLUDED_)
#define AFX_CREATORFACTORY_H__88E93FD6_332F_47B9_A86D_52F3D881F08B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DownloaderCreator.h"

class CCreatorFactory  
{
public:
	virtual ~CCreatorFactory();
    CDownloaderCreator* GetCreator(int nType);
private:
	CCreatorFactory();
private:
	typedef CMap<int, int, void*, void*> CMapIntToPtr;
	CMapIntToPtr m_mapCreator;
public:
	static CCreatorFactory* GetInstance();
};

#endif // !defined(AFX_CREATORFACTORY_H__88E93FD6_332F_47B9_A86D_52F3D881F08B__INCLUDED_)
