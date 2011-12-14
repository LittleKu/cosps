// CreatorFactory.cpp: implementation of the CCreatorFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreatorFactory.h"
#include <memory>
#include "YTDownloaderCreator.h"
#include "CNETDownloaderCreator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME("CTFT")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef CDownloaderCreator* (*CreatRoutine)();

#define REGISTER(TYPE, T)	m_mapCreator.SetAt(TYPE, (void*)T::new##T)

CCreatorFactory::CCreatorFactory()
{
	LOG4CPLUS_INFO_STR(THE_LOGGER, "CCreatorFactory::CCreatorFactory() constructed.")

	REGISTER(DLTE_CNET, CCNETDownloaderCreator);
	REGISTER(DLTE_YTB, CYTDownloaderCreator);
}

CCreatorFactory::~CCreatorFactory()
{
	m_mapCreator.RemoveAll();
}

CCreatorFactory* CCreatorFactory::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<CCreatorFactory> ptr(new CCreatorFactory);
	ASSERT(ptr.get());
	return ptr.get();
}

CDownloaderCreator* CCreatorFactory::GetCreator(int nType)
{
	void* ptr = NULL;
	if(m_mapCreator.Lookup(nType, ptr) == FALSE)
	{
		return new CDownloaderCreator();
	}

	CreatRoutine lpRoutine = (CreatRoutine)ptr;
	return lpRoutine();
}

