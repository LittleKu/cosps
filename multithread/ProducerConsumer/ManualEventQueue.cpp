// ManualEventQueue.cpp: implementation of the CManualEventQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ManualEventQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//http://stackoverflow.com/questions/11706985/win32-thread-safe-queue-implementation-using-native-windows-api

CManualEventQueue::CManualEventQueue(int nCapacity)
{
    m_nCapacity = nCapacity;
    ::InitializeCriticalSection(&m_lock);
    m_queue_not_empty = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    m_queue_not_full = ::CreateEvent(NULL, TRUE, TRUE, NULL);
}

CManualEventQueue::~CManualEventQueue()
{
    m_list.RemoveAll();
    ::CloseHandle(m_queue_not_empty);
    ::CloseHandle(m_queue_not_full);
    ::DeleteCriticalSection(&m_lock);
}

void CManualEventQueue::Put(void* ptr)
{
    bool done = false;
    while (!done)
    {
        // If the queue is full, we must wait until it isn't.
        if(::WaitForSingleObject(m_queue_not_full, INFINITE) != WAIT_OBJECT_0)
        {
            ASSERT(FALSE);
        }
		
        // However, we might not be the first to respond to the event,
        // so we still need to check whether the queue is full and loop
        // if it is.
        ::EnterCriticalSection(&m_lock);
        if (m_nCapacity <= 0 || m_list.GetCount() < m_nCapacity)
        {
            m_list.AddTail(ptr);
            done = true;
            // The queue is definitely not empty.
            SetEvent(m_queue_not_empty);
            // Check whether the queue is now full.
            if (m_nCapacity > 0 && m_list.GetCount() >= m_nCapacity)
            {
                ResetEvent(m_queue_not_full);
            }
        }
        ::LeaveCriticalSection(&m_lock);
    }
}

void* CManualEventQueue::Get()
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
		
        // However, we might not be the first to respond to the event,
        // so we still need to check whether the queue is empty and loop
        // if it is.
        ::EnterCriticalSection(&m_lock);
        if (!m_list.IsEmpty())
        {
            result = m_list.RemoveHead();
			done = true;
            // The queue shouldn't be full at this point!
            ASSERT(m_nCapacity <= 0 || m_list.GetCount() < m_nCapacity);
            SetEvent(m_queue_not_full);
            // Check whether the queue is now empty.
            if (m_list.IsEmpty())
            {
                ResetEvent(m_queue_not_empty);
            }
        }
        ::LeaveCriticalSection(&m_lock);
    }

	return result;
}
