// PosixCVQueue.cpp: implementation of the CPosixCVQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PosixCVQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef ENABLE_PTHREAD

#pragma comment(lib, "pthreadVC2.lib")

CPosixCVQueue::CPosixCVQueue(int nCapacity) : m_nCapacity(nCapacity)
{
	pthread_mutex_init(&m_mutex, NULL); 
	pthread_cond_init(&m_cond_not_full, NULL);
	pthread_cond_init(&m_cond_not_empty, NULL);
}

CPosixCVQueue::~CPosixCVQueue()
{
	m_list.RemoveAll();
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond_not_full);
	pthread_cond_destroy(&m_cond_not_empty);
}

void CPosixCVQueue::Put(void* ptr)
{
	pthread_mutex_lock(&m_mutex);
	while(m_nCapacity > 0 && m_list.GetCount() >= m_nCapacity)
	{
		pthread_cond_wait(&m_cond_not_full, &m_mutex);
	}
	m_list.AddTail(ptr);
	//notify consumers
	pthread_cond_broadcast(&m_cond_not_empty);

	pthread_mutex_unlock(&m_mutex);
}
void* CPosixCVQueue::Get()
{
	pthread_mutex_lock(&m_mutex);
	while(m_list.IsEmpty())
	{
		pthread_cond_wait(&m_cond_not_empty, &m_mutex);
	}
	void *result = m_list.RemoveHead();
	//notify producers
	pthread_cond_broadcast(&m_cond_not_full);
	
	pthread_mutex_unlock(&m_mutex);

	return result;
}
#endif