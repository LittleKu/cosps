#ifndef __BAIKE_SMART_PTR_H__
#define __BAIKE_SMART_PTR_H__

//http://baike.baidu.com/view/1391603.htm
namespace baike {
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
}

#endif