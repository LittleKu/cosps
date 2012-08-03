// ManualEventQueue.h: interface for the CManualEventQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANUALEVENTQUEUE_H__E8636751_7F73_4D83_9BD6_1956CF9AE485__INCLUDED_)
#define AFX_MANUALEVENTQUEUE_H__E8636751_7F73_4D83_9BD6_1956CF9AE485__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SyncQueue.h"

class CManualEventQueue : public CSyncQueue  
{
public:
	CManualEventQueue(int nCapacity = -1);
	virtual ~CManualEventQueue();
    virtual void Put(void* ptr);
    virtual void* Get();
protected:
    int m_nCapacity;
    CPtrList m_list;
	
    CRITICAL_SECTION m_lock;    
    HANDLE m_queue_not_empty;
    HANDLE m_queue_not_full;
};

#endif // !defined(AFX_MANUALEVENTQUEUE_H__E8636751_7F73_4D83_9BD6_1956CF9AE485__INCLUDED_)
