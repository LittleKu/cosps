// PosixCVQueue.h: interface for the CPosixCVQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSIXCVQUEUE_H__5D1DA938_955D_4ED2_A14C_1FB89F9A3D2C__INCLUDED_)
#define AFX_POSIXCVQUEUE_H__5D1DA938_955D_4ED2_A14C_1FB89F9A3D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ENABLE_PTHREAD

#include "SyncQueue.h"
#include "pthread.h"

class CPosixCVQueue : public CSyncQueue  
{
public:
	CPosixCVQueue(int nCapacity = -1);
	virtual ~CPosixCVQueue();
    virtual void Put(void* ptr);
    virtual void* Get();
protected:
    int m_nCapacity;
    CPtrList m_list;
	pthread_mutex_t m_mutex; 
	pthread_cond_t m_cond_not_full;
	pthread_cond_t m_cond_not_empty;
};

#endif //ENABLE_PTHREAD

#endif // !defined(AFX_POSIXCVQUEUE_H__5D1DA938_955D_4ED2_A14C_1FB89F9A3D2C__INCLUDED_)
