#include <iostream>
#include <stdexcept>
#include <memory>
using namespace std;
#define TEST_SMARTPTR
#define DEBUG_PRINT

#include "BaikeSmartPtr.h"
#include "MySmartPtr.h"

//using namespace baike;
using namespace me;

static int STUB_COUNT = 0;

class Stub
{
public:
	Stub()
	{
		m_id = ++STUB_COUNT;
		cout<<"[Stub::Stub] - id="<<m_id<<endl;
	}
	void print() {
		cout<<"[Stub::print] - id="<<m_id<<endl;
	}
	~Stub(){
		cout<<"[Stub::~Stub] - id="<<m_id<<endl;
	}
private:
	int m_id;
};

void test1()
{
	try {
		SmartPtr<Stub> t;
		t->print();
	} catch (const exception& err) {
		cout<<err.what()<<endl;
	}
	SmartPtr<Stub> t1(new Stub);
	SmartPtr<Stub> t2(t1);
	SmartPtr<Stub> t3(new Stub);
	t3 = t2;
	t1->print();
	(*t3).print();
}

void start_new_case(int i)
{
	STUB_COUNT = 0;
	SmartPtr<Stub>::m_count = 0;
	printf("\n[%2d]: ===========================\n", i);
}

void test2()
{
	int i = 0;
	//test normal case
	{
		start_new_case(++i);
		SmartPtr<Stub> t1(new Stub);
		t1->print();
		(*t1).print();
	}
	
	{
		start_new_case(++i);
		SmartPtr<Stub> t1(new Stub);
		SmartPtr<Stub> t2(t1);
		t2->print();
		t1 = t1;
		SmartPtr<Stub> t3(new Stub);
		t3->print();
		t3 = t1;
		t3->print();
	}
	
	{
		start_new_case(++i);
		SmartPtr<Stub> t1(new Stub);
		SmartPtr<Stub> t2(t1);
		SmartPtr<Stub> t3(t1);
		t1 = t2;
		SmartPtr<Stub> t4(new Stub);
		t1 = t4;
	}

	{
		start_new_case(++i);
		auto_ptr<Stub> t1(new Stub);
		auto_ptr<Stub> t2(t1);
		auto_ptr<Stub> t3(new Stub);
		t3 = t2;
	}
}
int main(int argc, char* argv[])
{
	test2();
	return 0;
}