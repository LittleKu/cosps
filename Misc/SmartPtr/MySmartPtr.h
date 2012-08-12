#ifndef __MY_SMART_PTR_H__
#define __MY_SMART_PTR_H__

#ifdef DEBUG_PRINT
#include <stdio.h>
#endif
namespace me {
template<typename T>
class SmartPtr
{
public:
#ifdef DEBUG_PRINT
	char buffer1[256];
	char buffer2[256];
#define PTR_VALUE(ptr) (ptr ? *ptr : 0)
#define FORMAT(x, buf) \
	sprintf(buf, "id=%d, ptr=0x%08X, used=%d", (x)->m_id, (x)->m_ptr, PTR_VALUE((x)->m_pUsed))
#define FORMAT1(x) FORMAT(x, buffer1)
#define FORMAT2(x) FORMAT(x, buffer2)
#define PRINT1(prefix, x) \
	FORMAT1(x); \
	printf("%s: %s\n", prefix, buffer1)
#define PRINT2(prefix, x, y) \
	FORMAT1(x); \
	FORMAT2(y); \
	printf("%s: %s ; %s\n", prefix, buffer1, buffer2)
#define SHOW1(prefix) \
	PRINT1(prefix, this)
#define SHOW2(prefix, rhs) \
	PRINT2(prefix, this, &rhs)

#else
#define SHOW1(prefix) 
#define SHOW2(prefix, rhs) 
#endif

public:
	SmartPtr(T* ptr = NULL): m_ptr(ptr), m_pUsed(new int(1)), m_id(++m_count)
	{
		SHOW1("[Constructor] - SmartPtr");
	}
	SmartPtr(const SmartPtr& rhs) : m_ptr(rhs.m_ptr), m_pUsed(rhs.m_pUsed), m_id(++m_count)
	{
		(*m_pUsed)++;
		SHOW2("[Copy Constructor] - SmartPtr", rhs);
	}
	SmartPtr& operator=(const SmartPtr& rhs)
	{
		if(this == &rhs)
		{
			SHOW1("[operator=] - SmartPtr: (this == &rhs)");
			return *this;
		}

		decreaseUse();
		
		m_ptr = rhs.m_ptr;
		m_pUsed = rhs.m_pUsed;
		
		(*m_pUsed)++;
		
		SHOW2("[operator=] - SmartPtr", rhs);
		return *this;
	}
	virtual ~SmartPtr()
	{
		SHOW1("[A - Destructor] - SmartPtr");
		decreaseUse();
		SHOW1("[Z - Destructor] - SmartPtr");
	}
	T* operator-> ()
	{
		return m_ptr;
	}
	const T* operator->() const
	{
		return m_ptr;
	}
	const T& operator*() const
	{
		return *m_ptr;
	}
	T& operator*()
	{
		return *m_ptr;
	}
private:
	void decreaseUse()
	{
		SHOW1("[A - decreaseUse] - SmartPtr");
		(*m_pUsed)--;
		if(*m_pUsed <= 0)
		{
			delete m_ptr;
			m_ptr = NULL;
			
			delete m_pUsed;
			m_pUsed = NULL;
		}
		SHOW1("[Z - decreaseUse] - SmartPtr");
	}
	T* m_ptr;
	int* m_pUsed;
	int m_id;

public:
	static int m_count;
};

template<typename T>
int SmartPtr<T>::m_count = 0;
}

#endif