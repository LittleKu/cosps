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

DECLARE_THE_LOGGER_NAME("SEGM")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSegmentInfoMap::CSegmentInfoMap()
{

}

CSegmentInfoMap::~CSegmentInfoMap()
{
	// Remove the elements
	int nTaskID;
	void* ptr = NULL;
	CSegmentInfoArray* pSegInfoArray = NULL;

	int i, size;

	m_criticalSection.Lock();

	POSITION pos = m_mapSegmentInfo.GetStartPosition();
	while(pos != NULL)
	{
		m_mapSegmentInfo.GetNextAssoc(pos, nTaskID, ptr);

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

	m_criticalSection.Unlock();

	LOG4CPLUS_INFO_STR(THE_LOGGER, "CSegmentInfoMap::~CSegmentInfoMap() called.")
}

CSegmentInfoMap* CSegmentInfoMap::GetInstance()
{
	//This is the std way to make singleton
	static std::auto_ptr<CSegmentInfoMap> ptr(new CSegmentInfoMap);	
	return ptr.get();
}

CSegmentInfoArray* CSegmentInfoMap::GetSegmentInfoArray(int nTaskID)
{
	CSegmentInfoArray* pResult = NULL;
	void* ptr = NULL;

	m_criticalSection.Lock();
	if(m_mapSegmentInfo.Lookup(nTaskID, ptr) == FALSE)
	{
		CString szLog;
		szLog.Format("[GetSegmentInfoArray]: The task ID [%02d] doesn't exist", nTaskID);
		LOG4CPLUS_FATAL_STR(THE_LOGGER, (LPCTSTR)szLog)

		ASSERT(FALSE);
			
		pResult = NULL;
	}
	else
	{
		pResult = (CSegmentInfoArray*)ptr;
	}
	m_criticalSection.Unlock();

	return pResult;
}

BOOL CSegmentInfoMap::AddSegmentInfoArray(int nTaskID, CSegmentInfoArray* pSegInfoArray)
{
	m_criticalSection.Lock();

	//make sure the task doesn't exit in the map
	void* ptr;
	if(m_mapSegmentInfo.Lookup(nTaskID, ptr))
	{
		CString szLog;
		szLog.Format("[AddSegmentInfoArray]: The task ID [%02d] already exists", nTaskID);
		LOG4CPLUS_FATAL_STR(THE_LOGGER, (LPCTSTR)szLog)

		ASSERT(FALSE);
		
		m_criticalSection.Unlock();

		return FALSE;
	}

	m_mapSegmentInfo.SetAt(nTaskID, pSegInfoArray);

	m_criticalSection.Unlock();

	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		CString szLog;
		szLog.Format("[AddSegmentInfoArray]: The task ID [%02d] is added successfully. pSegInfoArray = 0x%08X", 
			nTaskID, (DWORD)pSegInfoArray);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
	}
	return TRUE;
}

BOOL CSegmentInfoMap::RemoveSegmentInfoArray(int nTaskID, CSegmentInfoArray* pTarget)
{
	void* ptr = NULL;
	CSegmentInfoArray* pSegInfoArray = NULL;

	m_criticalSection.Lock();

	if(!m_mapSegmentInfo.Lookup(nTaskID, ptr))
	{
		CString szLog;
		szLog.Format("[RemoveSegmentInfoArray]: The task ID [%02d] doesn't exist", nTaskID);
		LOG4CPLUS_FATAL_STR(THE_LOGGER, (LPCTSTR)szLog)
			
		ASSERT(FALSE);

		m_criticalSection.Unlock();

		return FALSE;
	}

	pSegInfoArray = (CSegmentInfoArray*)ptr;
	ASSERT(pSegInfoArray != NULL);
	ASSERT(pSegInfoArray == pTarget);
	
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
	
	m_mapSegmentInfo.RemoveKey(nTaskID);

	m_criticalSection.Unlock();

	if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		CString szLog;
		szLog.Format("[RemoveSegmentInfoArray]: The task ID [%02d] is removed successfully. pTarget=0x%08X", 
			nTaskID, (DWORD)pTarget);
		LOG4CPLUS_DEBUG_STR(THE_LOGGER, (LPCTSTR)szLog)
	}
	return TRUE;
}

