// SegmentInfoMap.cpp: implementation of the CSegmentInfoMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SegmentInfoMap.h"
#include <memory>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSegmentInfoMap::CSegmentInfoMap()
{

}

CSegmentInfoMap::~CSegmentInfoMap()
{
	// Remove the elements
	CString szKey;
	void* ptr = NULL;
	CSegmentInfoArray* pSegInfoArray = NULL;

	int i, size;

	POSITION pos = m_mapSegmentInfo.GetStartPosition();
	while(pos != NULL)
	{
		m_mapSegmentInfo.GetNextAssoc(pos, szKey, ptr);

		pSegInfoArray = (CSegmentInfoArray*)ptr;
		ASSERT(pSegInfoArray != NULL);

		//Free space of this array
		for(i = 0, size = pSegInfoArray->GetSize(); i < size; i++)
		{
			CSegmentInfo* pSegmentInfo = pSegInfoArray->GetAt(i);
			ASSERT(pSegmentInfo != NULL);

			delete pSegmentInfo;
		}
		//Free the space of the pointer
		pSegInfoArray->RemoveAll();

		delete pSegInfoArray;
	}
	m_mapSegmentInfo.RemoveAll();

	LOG4CPLUS_INFO(ROOT_LOGGER, "CSegmentInfoMap::~CSegmentInfoMap() called.")
}

CSegmentInfoMap* CSegmentInfoMap::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<CSegmentInfoMap> ptr(new CSegmentInfoMap);	
	return ptr.get();
}

CSegmentInfoArray* CSegmentInfoMap::GetSegmentInfoArray(LPCTSTR lpszUrl)
{
	void* ptr = NULL;
	if(m_mapSegmentInfo.Lookup(lpszUrl, ptr) == FALSE)
	{
		CString szLog;
		szLog.Format("The query URL is not exist. URL = %s", lpszUrl);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)

		return NULL;
	}
	return (CSegmentInfoArray*)ptr;
}

void CSegmentInfoMap::AddSegmentInfoArray(LPCTSTR lpszUrl, CSegmentInfoArray* pSegInfoArray)
{
	m_mapSegmentInfo.SetAt(lpszUrl, pSegInfoArray);
}

void CSegmentInfoMap::RemoveSegmentInfoArray(LPCTSTR lpszUrl)
{
	CSegmentInfoArray* pSegInfoArray = GetSegmentInfoArray(lpszUrl);
	ASSERT(pSegInfoArray != NULL);
	
	int i, size;
	//Free space of this array
	for(i = 0, size = pSegInfoArray->GetSize(); i < size; i++)
	{
		CSegmentInfo* pSegmentInfo = pSegInfoArray->GetAt(i);
		ASSERT(pSegmentInfo != NULL);
		
		delete pSegmentInfo;
	}
	//Free the space of the pointer
	pSegInfoArray->RemoveAll();
	delete pSegInfoArray;

	m_mapSegmentInfo.RemoveKey(lpszUrl);
}
