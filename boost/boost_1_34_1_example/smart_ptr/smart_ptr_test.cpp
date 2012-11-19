#include "smart_ptr_test.h"
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include <iostream>
#include <cassert>

class A 
{
public:
	A()
	{
		std::cout<< "A::A" << std::endl;
	}
	~A()
	{
		std::cout<< "A::~A" << std::endl;
	}
};

int test_smart_ptr() 
{
	
	std::cout<<"Enter main"<<std::endl;
	boost::weak_ptr<A> w;
	assert(w.expired());
	{
		std::cout<<"Enter block"<<std::endl;
		boost::shared_ptr<A> p(new A());
		assert(p.use_count()==1);
		w=p;
		assert(p.use_count()==w.use_count());
		assert(p.use_count()==1);
		
		// ?weak_ptr??shared_ptr 
		boost::shared_ptr<A> p2(w);
		assert(p2==p);
		std::cout<< p.use_count() << std::endl;
		std::cout<< p2.use_count() << std::endl;
		std::cout<< w.use_count() << std::endl;
		std::cout<<"Exit block start"<<std::endl;
	}
	std::cout<<"Exit block done"<<std::endl;
	assert(w.expired());
	boost::shared_ptr<A> p3=w.lock();
	assert(!p3);
	assert(p3.use_count() == 0);
	std::cout<<"Exit main"<<std::endl;
	
	return 0;
}