// AutoEventQueue.h: interface for the CAutoEventQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOEVENTQUEUE_H__2969B3E8_94F8_43B7_BBA4_253214956BE5__INCLUDED_)
#define AFX_AUTOEVENTQUEUE_H__2969B3E8_94F8_43B7_BBA4_253214956BE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SyncQueue.h"

class CAutoEventQueue : public CSyncQueue  
{
public:
	CAutoEventQueue(int nCapacity = -1);
	virtual ~CAutoEventQueue();
	virtual void Put(void* ptr);
    virtual void* Get();
protected:
    int m_nCapacity;
    CPtrList m_list;
	
    CRITICAL_SECTION m_lock;    
    HANDLE m_queue_not_empty;
    HANDLE m_queue_not_full;
};

#endif // !defined(AFX_AUTOEVENTQUEUE_H__2969B3E8_94F8_43B7_BBA4_253214956BE5__INCLUDED_)
