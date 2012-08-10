//http://baike.baidu.com/view/1391603.htm

#include <iostream>
#include <stdexcept>
using namespace std;
#define TEST_SMARTPTR
class Stub
{
public:
	void print() {
		cout<<"Stub: print"<<endl;
	}
	~Stub(){
		cout<<"Stub: Destructor"<<endl;
	}
};
template <typename T>
class SmartPtr
{
public:
	SmartPtr(T *p = 0): ptr(p), pUse(new size_t(1)) { }
	SmartPtr(const SmartPtr& src): ptr(src.ptr), pUse(src.pUse) {
		++*pUse;
	}
	SmartPtr& operator= (const SmartPtr& rhs) {
		// self-assigning is also right
		++*rhs.pUse;
		decrUse();
		ptr = rhs.ptr;
		pUse = rhs.pUse;
		return *this;
	}
	T *operator->() {
		if (ptr)
			return ptr;
		throw std::runtime_error("access through NULL pointer");
	}
	const T *operator->() const {
		if (ptr)
			return ptr;
		throw std::runtime_error("access through NULL pointer");
	}
	T &operator*() {
		if (ptr)
			return *ptr;
		throw std::runtime_error("dereference of NULL pointer");
	}
	const T &operator*() const {
		if (ptr)
			return *ptr;
		throw std::runtime_error("dereference of NULL pointer");
	}
	~SmartPtr() {
		decrUse();
#ifdef TEST_SMARTPTR
		std::cout<<"SmartPtr: Destructor"<<std::endl; // for testing
#endif
	}
private:
	void decrUse() {
		if (--*pUse == 0) {
			delete ptr;
			delete pUse;
		}
	}
	T *ptr;
	size_t *pUse;
};
int main()
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
	return 0;
}