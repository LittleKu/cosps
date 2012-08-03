// AutoEventQueue.cpp: implementation of the CAutoEventQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoEventQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//http://stackoverflow.com/questions/11706985/win32-thread-safe-queue-implementation-using-native-windows-api

CAutoEventQueue::CAutoEventQueue(int nCapacity)
{
	m_nCapacity = nCapacity;
    ::InitializeCriticalSection(&m_lock);
    m_queue_not_empty = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_queue_not_full = ::CreateEvent(NULL, FALSE, TRUE, NULL);
}

CAutoEventQueue::~CAutoEventQueue()
{
	m_list.RemoveAll();
    ::CloseHandle(m_queue_not_empty);
    ::CloseHandle(m_queue_not_full);
    ::DeleteCriticalSection(&m_lock);
}

void CAutoEventQueue::Put(void* ptr)
{
    if (m_nCapacity <= 0)
    {
        ::EnterCriticalSection(&m_lock);
        m_list.AddTail(ptr);
        SetEvent(m_queue_not_empty);
        ::LeaveCriticalSection(&m_lock);
        return;
    }
	
    bool done = false;
    while (!done)
    {
        // If the queue is full, we must wait until it isn't.
        if(::WaitForSingleObject(m_queue_not_full, INFINITE) != WAIT_OBJECT_0)
        {
            ASSERT(FALSE);
        }
		
        // However, under some (rare) conditions we'll get here and find
        // the queue is already full again, so be prepared to loop.
        ::EnterCriticalSection(&m_lock);
        if (m_list.GetCount() < m_nCapacity)
        {
            m_list.AddTail(ptr);
            done = true;
            SetEvent(m_queue_not_empty);
            if (m_list.GetCount() < m_nCapacity)
            {
                SetEvent(m_queue_not_full);
            }
        }
        ::LeaveCriticalSection(&m_lock);
    }
}

void* CAutoEventQueue::Get()
{
	bool done = false;
	void *result = NULL;
    while (!done)
    {
        // If the queue is empty, we must wait until it isn't.
        if(::WaitForSingleObject(m_queue_not_empty, INFINITE) != WAIT_OBJECT_0)
        {
            ASSERT(FALSE);
        }
		
        // However, under some (rare) conditions we'll get here and find
        // the queue is already empty again, so be prepared to loop.
        ::EnterCriticalSection(&m_lock);
        if (!m_list.IsEmpty())
        {
            result = m_list.RemoveHead();
			done = true;
            // The queue shouldn't be full at this point!
            if (m_nCapacity <= 0) ASSERT(m_list.GetCount() < m_nCapacity);
            SetEvent(m_queue_not_full);
            if (!m_list.IsEmpty())
            {
                SetEvent(m_queue_not_empty);
            }
        }
        ::LeaveCriticalSection(&m_lock);
    }
	return result;
}
