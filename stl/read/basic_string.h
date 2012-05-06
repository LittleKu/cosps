#ifndef __BASIC_STRING_H__
#define __BASIC_STRING_H__
//! From: XSTRING: VC98
//#include <xmemory>
#include <xutility>
#include "char_traits.h"
#include "allocator.h"

void __cdecl throw_length_exception();
void __cdecl throw_range_exception();
// TEMPLATE CLASS basic_string
template<class E, class Traits = char_traits<E>, class Alloc = allocator<E> >
class basic_string 
{
public:
	typedef basic_string<E, Traits, Alloc> BasicStringType;
	typedef Alloc::size_type size_type;
	typedef Alloc::difference_type difference_type;
	typedef Alloc::pointer pointer;
	typedef Alloc::const_pointer const_pointer;
	typedef Alloc::reference reference;
	typedef Alloc::const_reference const_reference;
	typedef Alloc::value_type value_type;
	typedef Alloc::pointer iterator;
	typedef Alloc::const_pointer const_iterator;
	typedef std::reverse_iterator<const_iterator, value_type, const_reference, const_pointer, difference_type> 
		const_reverse_iterator;
	
	typedef std::reverse_iterator<iterator, value_type, reference, pointer, difference_type> 
		reverse_iterator;
	
	explicit basic_string(const Alloc& alloc = Alloc()) : allocator(alloc) 
	{
		Tidy(); 
	}
	basic_string(const BasicStringType& str) : allocator(str.allocator)
	{
		Tidy(), assign(str, 0, npos); 
	}
	basic_string(const BasicStringType& str, size_type pos, size_type n, 
		const Alloc& alloc = Alloc()) : allocator(alloc) 
	{
		Tidy(), assign(str, pos, n); 
	}
	basic_string(const E *str, size_type n, const Alloc& alloc = Alloc()) : allocator(alloc) 
	{
		Tidy(), assign(str, n); 
	}
	basic_string(const E *str, const Alloc& alloc = Alloc()) : allocator(alloc) 
	{
		Tidy(), assign(str); 
	}
	basic_string(size_type n, E c, const Alloc& alloc = Alloc())
		: allocator(alloc) 
	{
		Tidy(), assign(n, c); 
	}
	typedef const_iterator constIter;
	basic_string(constIter first, constIter last, const Alloc& alloc = Alloc())
		: allocator(alloc) 
	{
		Tidy(); assign(first, last); 
	}
	~basic_string()
	{
		Tidy(true); 
	}
	
	typedef Traits traits_type;
	typedef Alloc allocator_type;
	enum _Mref {_FROZEN = 255};
	static const size_type npos;
	
	BasicStringType& operator=(const BasicStringType& str)
	{
		return (assign(str)); 
	}
	BasicStringType& operator=(const E *str)
	{
		return (assign(str)); 
	}
	BasicStringType& operator=(E c)
	{
		return (assign(1, c)); 
	}
	BasicStringType& operator+=(const BasicStringType& str)
	{
		return (append(str)); 
	}
	BasicStringType& operator+=(const E *str)
	{
		return (append(str)); 
	}
	BasicStringType& operator+=(E c)
	{
		return (append(1, c)); 
	}
	BasicStringType& append(const BasicStringType& str)
	{
		return (append(str, 0, npos)); 
	}
	BasicStringType& append(const BasicStringType& str, size_type pos, size_type n)
	{
		if (str.size() < pos)
			throw_range_exception();
		size_type n = str.size() - pos;
		if (n < n)
			n = n;
		if (npos - nLength <= n)
			throw_length_exception();
		if (0 < n && Grow(n = nLength + n))
		{
			Traits::copy(ptr + nLength, &str.c_str()[pos], n);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& append(const E *s, size_type n)
	{
		if (npos - nLength <= n)
			throw_length_exception();
		size_type n;
		if (0 < n && Grow(n = nLength + n))
		{
			Traits::copy(ptr + nLength, s, n);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& append(const E *s)
	{
		return (append(s, Traits::length(s))); 
	}
	BasicStringType& append(size_type n, E c)
	{
		if (npos - nLength <= n)
			throw_length_exception();
		size_type n;
		if (0 < n && Grow(n = nLength + n))
		{
			Traits::assign(ptr + nLength, n, c);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& append(constIter first, constIter last)
	{
		return (replace(end(), end(), first, last)); 
	}
	BasicStringType& assign(const BasicStringType& str)
	{
		return (assign(str, 0, npos)); 
	}
	BasicStringType& assign(const BasicStringType& str, size_type pos, size_type count)
	{
		if (str.size() < pos)
			throw_range_exception();
		size_type n = str.size() - pos;
		if (count < n)
			n = count;
		if (this == &str)
			erase((size_type)(pos + n)), erase(0, pos);
		else if (0 < n && n == str.size() && RefCount(str.c_str()) < _FROZEN - 1 
			&& allocator == str.allocator)
		{
			Tidy(true);
			ptr = (E *)str.c_str();
			nLength = str.size();
			nCapacity = str.capacity();
			++RefCount(ptr); 
		}
		else if (Grow(n, true))
		{
			Traits::copy(ptr, &str.c_str()[pos], n);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& assign(const E *s, size_type n)
	{
		if (Grow(n, true))
		{
			Traits::copy(ptr, s, n);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& assign(const E *s)
	{
		return (assign(s, Traits::length(s))); 
	}
	BasicStringType& assign(size_type n, E c)
	{
		if (n == npos)
			throw_length_exception();
		if (Grow(n, true))
		{
			Traits::assign(ptr, n, c);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& assign(constIter first, constIter last)
	{
		return (replace(begin(), end(), first, last)); 
	}
	BasicStringType& insert(size_type pos1, const BasicStringType& str)
	{
		return (insert(pos1, str, 0, npos)); 
	}
	BasicStringType& insert(size_type pos1, const BasicStringType& str, size_type pos2,
		size_type count)
	{
		if (nLength < pos1 || str.size() < pos2)
			throw_range_exception();
		size_type n = str.size() - pos2;
		if (n < count)
			count = n;
		if (npos - nLength <= count)
			throw_length_exception();
		if (0 < count && Grow(n = nLength + count))
		{
			Traits::move(ptr + pos1 + count, ptr + pos1, nLength - pos1);
			Traits::copy(ptr + pos1, &str.c_str()[pos2], count);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& insert(size_type pos1, const E *str, size_type count)
	{
		if (nLength < pos1)
			throw_range_exception();
		if (npos - nLength <= count)
			throw_length_exception();
		size_type n;
		if (0 < count && Grow(n = nLength + count))
		{
			Traits::move(ptr + pos1 + count, ptr + pos1, nLength - pos1);
			Traits::copy(ptr + pos1, str, count);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& insert(size_type pos1, const E *s)
	{
		return (insert(pos1, s, Traits::length(s))); 
	}
	BasicStringType& insert(size_type pos1, size_type count, E c)
	{
		if (nLength < pos1)
			throw_range_exception();
		if (npos - nLength <= count)
			throw_length_exception();
		size_type n;
		if (0 < count && Grow(n = nLength + count))
		{
			Traits::move(ptr + pos1 + count, ptr + pos1, nLength - pos1);
			Traits::assign(ptr + pos1, count, c);
			SetEOS(n); 
		}
		return (*this); 
	}
	iterator insert(iterator pos, E c)
	{
		size_type pos0 = Pdif(pos, begin());
		insert(pos0, 1, c);
		return (begin() + pos0); 
	}
	void insert(iterator pos, size_type _M, E c)
	{
		size_type pos0 = Pdif(pos, begin());
		insert(pos0, _M, c); 
	}
	void insert(iterator pos, constIter first, constIter last)
	{
		replace(pos, pos, first, last); 
	}
	BasicStringType& erase(size_type _P0 = 0, size_type _M = npos)
	{
		if (nLength < _P0)
			throw_range_exception();
		Split();
		if (nLength - _P0 < _M)
			_M = nLength - _P0;
		if (0 < _M)
		{
			Traits::move(ptr + _P0, ptr + _P0 + _M, nLength - _P0 - _M);
			size_type n = nLength - _M;
			if (Grow(n))
				SetEOS(n); 
		}
		return (*this); 
	}
	iterator erase(iterator _P)
	{
		size_t _M = Pdif(_P, begin());
		erase(_M, 1);
		return (Psum(ptr, _M)); 
	}
	iterator erase(iterator _F, iterator _L)
	{
		size_t _M = Pdif(_F, begin());
		erase(_M, Pdif(_L, _F));
		return (Psum(ptr, _M)); 
	}
	BasicStringType& replace(size_type pos1, size_type count1, const BasicStringType& str)
	{
		return (replace(pos1, count1, str, 0, npos)); 
	}
	BasicStringType& replace(size_type pos1, size_type count1, const BasicStringType& str, 
		size_type pos2, size_type count2)
	{	
		if (nLength < pos1 || str.size() < pos2)
			throw_range_exception();
		if (nLength - pos1 < count1)
			count1 = nLength - pos1;
		size_type n = str.size() - pos2;
		if (n < count2)
			count2 = n;
		if (npos - count2 <= nLength - count1)
			throw_length_exception();
		Split();
		size_type _Nm = nLength - count1 - pos1;
		if (count2 < count1)
			Traits::move(ptr + pos1 + count2, ptr + pos1 + count1, _Nm);
		if ((0 < count2 || 0 < count1) && Grow(n = nLength + count2 - count1))
		{
			if (count1 < count2)
				Traits::move(ptr + pos1 + count2, ptr + pos1 + count1, _Nm);
			Traits::copy(ptr + pos1, &str.c_str()[pos2], count2);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& replace(size_type pos1, size_type count1, const E *str,
		size_type count2)
	{
		if (nLength < pos1)
			throw_range_exception();
		if (nLength - pos1 < count1)
			count1 = nLength - pos1;
		if (npos - count2 <= nLength - count1)
			throw_length_exception();
		Split();
		size_type _Nm = nLength - count1 - pos1;
		if (count2 < count1)
			Traits::move(ptr + pos1 + count2, ptr + pos1 + count1, _Nm);
		size_type n;
		if ((0 < count2 || 0 < count1) && Grow(n = nLength + count2 - count1))
		{
			if (count1 < count2)
				Traits::move(ptr + pos1 + count2, ptr + pos1 + count1, _Nm);
			Traits::copy(ptr + pos1, str, count2);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& replace(size_type pos1, size_type count1, const E *str)
	{
		return (replace(pos1, count1, str, Traits::length(str))); 
	}
	BasicStringType& replace(size_type pos1, size_type count1, size_type count2, E c)
	{
		if (nLength < pos1)
			throw_range_exception();
		if (nLength - pos1 < count1)
			count1 = nLength - pos1;
		if (npos - count2 <= nLength - count1)
			throw_length_exception();
		Split();
		size_type _Nm = nLength - count1 - pos1;
		if (count2 < count1)
			Traits::move(ptr + pos1 + count2, ptr + pos1 + count1, _Nm);
		size_type n;
		if ((0 < count2 || 0 < count1) && Grow(n = nLength + count2 - count1))
		{
			if (count1 < count2)
				Traits::move(ptr + pos1 + count2, ptr + pos1 + count1, _Nm);
			Traits::assign(ptr + pos1, count2, c);
			SetEOS(n); 
		}
		return (*this); 
	}
	BasicStringType& replace(iterator first, iterator last, const BasicStringType& str)
	{
		return (replace(Pdif(first, begin()), Pdif(last, first), str)); 
	}
	BasicStringType& replace(iterator first, iterator last, const E *str, size_type count)
	{
		return (replace(Pdif(first, begin()), Pdif(last, first), str, count)); 
	}
	BasicStringType& replace(iterator first, iterator last, const E *str)
	{
		return (replace(Pdif(first, begin()), Pdif(last, first), str)); 
	}
	BasicStringType& replace(iterator first, iterator last,	size_type count, E c)
	{
		return (replace(Pdif(first, begin()), Pdif(last, first), count, c)); 
	}
	BasicStringType& replace(iterator first1, iterator last1, constIter first2, constIter last2)
	{
		size_type pos = Pdif(first1, begin());
		size_type count = 0;
		_Distance(first2, last2, count);
		
		replace(pos, Pdif(last1, first1), count, E(0));
		
		for (first1 = begin() + pos; 0 < count; ++first1, ++first2, --count)
			*first1 = *first2;
		return (*this); 
	}
	iterator begin()
	{
		Freeze();
		return (ptr); 
	}
	const_iterator begin() const
	{
		return (ptr); 
	}
	iterator end()
	{
		Freeze();
		return ((iterator)Psum(ptr, nLength)); 
	}
	const_iterator end() const
	{
		return ((const_iterator)Psum(ptr, nLength)); 
	}
	reverse_iterator rbegin()
	{
		return (reverse_iterator(end())); 
	}
	const_reverse_iterator rbegin() const
	{
		return (const_reverse_iterator(end())); 
	}
	reverse_iterator rend()
	{
		return (reverse_iterator(begin())); 
	}
	const_reverse_iterator rend() const
	{
		return (const_reverse_iterator(begin())); 
	}
	reference at(size_type pos)
	{
		if (nLength <= pos)
			throw_range_exception();
		Freeze();
		return (ptr[pos]); 
	}
	const_reference at(size_type pos) const
	{
		if (nLength <= pos)
			throw_range_exception();
		return (ptr[pos]); 
	}
	reference operator[](size_type pos)
	{
		if (nLength < pos || ptr == 0)
			return ((reference)*Nullstr());
		Freeze();
		return (ptr[pos]); 
	}
	const_reference operator[](size_type _P0) const
	{
		if (ptr == 0)
			return (*Nullstr());
		else
			return (ptr[_P0]); 
	}
	const E *c_str() const
	{
		return (ptr == 0 ? Nullstr() : ptr); 
	}
	const E *data() const
	{
		return (c_str()); 
	}
	size_type length() const
	{
		return (nLength); 
	}
	size_type size() const
	{
		return (nLength); 
	}
	size_type max_size() const
	{
		size_type n = allocator.max_size();
		return (n <= 2 ? 1 : n - 2); 
	}
	void resize(size_type n, E c)
	{
		n <= nLength ? erase(n) : append(n - nLength, c); 
	}
	
	void resize(size_type n)
	{
		n <= nLength ? erase(n) : append(n - nLength, E(0)); 
	}
	size_type capacity() const
	{
		return (nCapacity); 
	}
	void reserve(size_type n = 0)
	{
		if (nCapacity < n)
			Grow(n); 
	}
	bool empty() const
	{
		return (nLength == 0); 
	}
	size_type copy(E *str, size_type count, size_type pos = 0) const
	{
		if (nLength < pos)
			throw_range_exception();
		if (nLength - pos < count)
			count = nLength - pos;
		if (0 < count)
			Traits::copy(str, ptr + pos, count);
		return (count); 
	}
	void swap(BasicStringType& str)
	{
		if (allocator == str.allocator)
		{
			std::swap(ptr, str.ptr);
			std::swap(nLength, str.nLength);
			std::swap(nCapacity, str.nCapacity); 
		}
		else
		{
			BasicStringType _Ts = *this; 
			*this = str, str = _Ts; 
		}
	}
	friend void swap(BasicStringType& str1, BasicStringType& str2)
	{
		str1.swap(str2); 
	}
	size_type find(const BasicStringType& str, size_type pos = 0) const
	{
		return (find(str.c_str(), pos, str.size())); 
	}
	size_type find(const E *str, size_type pos, size_type n) const
	{
		if (n == 0 && pos <= nLength)
			return (pos);
		size_type nNumber;
		if (pos < nLength && n <= (nNumber = nLength - pos))
		{
			const E *_U, *_V;
			for (nNumber -= n - 1, _V = ptr + pos; (_U = Traits::find(_V, nNumber, *str)) != 0; 
			nNumber -= _U - _V + 1, _V = _U + 1)
				
				if (Traits::compare(_U, str, n) == 0)
					return (_U - ptr); 
		}
		return (npos); 
	}
	size_type find(const E *str, size_type pos = 0) const
	{
		return (find(str, pos, Traits::length(str))); 
	}
	size_type find(E c, size_type pos = 0) const
	{
		return (find((const E *)&c, pos, 1)); 
	}
	size_type rfind(const BasicStringType& str, size_type pos = npos) const
	{
		return (rfind(str.c_str(), pos, str.size())); 
	}
	size_type rfind(const E *str, size_type pos, size_type n) const
	{
		if (n == 0)
			return (pos < nLength ? pos : nLength);
		if (n <= nLength)
			for (const E *_U = ptr + + (pos < nLength - n ? pos : nLength - n); ; --_U)
				if (Traits::eq(*_U, *str) && Traits::compare(_U, str, n) == 0)
					return (_U - ptr);
				else if (_U == ptr)
					break;
				return (npos); 
	}
	size_type rfind(const E *str, size_type pos = npos) const
	{
		return (rfind(str, pos, Traits::length(str))); 
	}
	size_type rfind(E c, size_type pos = npos) const
	{
		return (rfind((const E *)&c, pos, 1)); 
	}
	size_type find_first_of(const BasicStringType& str, size_type pos = 0) const
	{
		return (find_first_of(str.c_str(), pos, str.size())); 
	}
	size_type find_first_of(const E *str, size_type pos, size_type n) const
	{
		if (0 < n && pos < nLength)
		{
			const E *const _V = ptr + nLength;
			for (const E *_U = ptr + pos; _U < _V; ++_U)
				if (Traits::find(str, n, *_U) != 0)
					return (_U - ptr); 
		}
		return (npos); 
	}
	
	size_type find_first_of(const E *str, size_type pos = 0) const
	{
		return (find_first_of(str, pos, Traits::length(str))); 
	}
	size_type find_first_of(E c, size_type pos = 0) const
	{
		return (find((const E *)&c, pos, 1)); 
	}
	size_type find_last_of(const BasicStringType& str, size_type pos = npos) const
	{
		return (find_last_of(str.c_str(), pos, str.size())); 
	}
	size_type find_last_of(const E *str, size_type pos, size_type n) const
	{
		if (0 < n && 0 < nLength)
			for (const E *_U = ptr + (pos < nLength ? pos : nLength - 1); ; --_U)
				if (Traits::find(str, n, *_U) != 0)
					return (_U - ptr);
				else if (_U == ptr)
					break;
				return (npos); 
	}
	size_type find_last_of(const E *str, size_type pos = npos) const
	{
		return (find_last_of(str, pos, Traits::length(str))); 
	}
	size_type find_last_of(E c, size_type pos = npos) const
	{
		return (rfind((const E *)&c, pos, 1)); 
	}
	size_type find_first_not_of(const BasicStringType& str, size_type pos = 0) const
	{
		return (find_first_not_of(str.c_str(), pos, str.size())); 
	}
	size_type find_first_not_of(const E *str, size_type pos, size_type n) const
	{
		if (pos < nLength)
		{
			const E *const _V = ptr + nLength;
			for (const E *_U = ptr + pos; _U < _V; ++_U)
				if (Traits::find(str, n, *_U) == 0)
					return (_U - ptr); 
		}
		return (npos); 
	}
	size_type find_first_not_of(const E *str, size_type pos = 0) const
	{
		return (find_first_not_of(str, pos, Traits::length(str))); 
	}
	size_type find_first_not_of(E c, size_type pos = 0) const
	{
		return (find_first_not_of((const E *)&c, pos, 1)); 
	}
	size_type find_last_not_of(const BasicStringType& str, size_type pos = npos) const
	{
		return (find_last_not_of(str.c_str(), pos, str.size())); 
	}
	size_type find_last_not_of(const E *str, size_type pos, size_type n) const
	{
		if (0 < nLength)
			for (const E *_U = ptr + (pos < nLength ? pos : nLength - 1); ; --_U)
				if (Traits::find(str, n, *_U) == 0)
					return (_U - ptr);
				else if (_U == ptr)
					break;
				return (npos); 
	}
	size_type find_last_not_of(const E *str, size_type pos = npos) const
	{
		return (find_last_not_of(str, pos, Traits::length(str))); 
	}
	size_type find_last_not_of(E c, size_type pos = npos) const
	{
		return (find_last_not_of((const E *)&c, pos, 1)); 
	}
	BasicStringType substr(size_type pos = 0, size_type count = npos) const
	{
		return (BasicStringType(*this, pos, count)); 
	}
	int compare(const BasicStringType& str) const
	{
		return (compare(0, nLength, str.c_str(), str.size())); 
	}
	int compare(size_type pos, size_type count, const BasicStringType& str) const
	{
		return (compare(pos, count, str, 0, npos)); 
	}
	int compare(size_type pos1, size_type count1, const BasicStringType& str, size_type pos2, size_type count2) const
	{
		if (str.size() < pos2)
			throw_range_exception();
		if (str.nLength - pos2 < count2)
			count2 = str.nLength - pos2;
		return (compare(pos1, count1, str.c_str() + pos2, count2)); 
	}
	int compare(const E *str) const
	{
		return (compare(0, nLength, str, Traits::length(str))); 
	}
	int compare(size_type pos, size_type count, const E *str) const
	{
		return (compare(pos, count, str, Traits::length(str))); 
	}
	int compare(size_type pos1, size_type count1, const E *str, size_type count2) const
	{
		if (nLength < pos1)
			throw_range_exception();
		if (nLength - pos1 < count1)
			count1 = nLength - pos1;
		size_type _Ans = Traits::compare(Psum(ptr, pos1), str, count1 < count2 ? count1 : count2);
		return (_Ans != 0 ? _Ans : count1 < count2 ? -1 : count1 == count2 ? 0 : +1); 
	}
	Alloc get_allocator() const
	{
		return (allocator); 
	}
	
protected:
	Alloc allocator;
private:
	enum {_MIN_SIZE = sizeof (E) <= 32 ? 31 : 7};
	void Copy(size_type n)
	{
		//0. Determine the size to copy
		size_type count = n | _MIN_SIZE;
		if (max_size() < count)
			count = n;

		//1. allocate space
		E *str;
		/*		_TRY_BEGIN*/
		try
		{
			
			str = allocator.allocate(count + 2, (void *)0);
			/*		_CATCH_ALL*/
		} 
		catch (...) 
		{
			count = n;
			str = allocator.allocate(count + 2, (void *)0);
			/*		_CATCH_END*/
		}

		//2. Copy the original data to the new allocated space
		if (0 < nLength)
			Traits::copy(str + 1, ptr, (nLength > count) ? count : nLength);


		//3. Update the new fields of the object
		size_type oldLength = nLength;
		Tidy(true);
		ptr = str + 1;
		RefCount(ptr) = 0;
		nCapacity = count;
		SetEOS(oldLength > count ? count : oldLength); 
	}
	void SetEOS(size_type n)
	{
		Traits::assign(ptr[nLength = n], E(0)); 
	}
	void Freeze()
	{
		if (ptr != 0 && RefCount(ptr) != 0 && RefCount(ptr) != _FROZEN)
			Grow(nLength);
		if (ptr != 0)
			RefCount(ptr) = _FROZEN; 
	}
	bool Grow(size_type n, bool bTrim = false)
	{
		if (max_size() < n)
			throw_length_exception();
		if (ptr != 0 && RefCount(ptr) != 0 && RefCount(ptr) != _FROZEN)
			if (n == 0)
			{
				--RefCount(ptr), Tidy();
				return (false); 
			}
			else
			{
				Copy(n);
				return (true); 
			}
		if (n == 0)
		{
			if (bTrim)
				Tidy(true);
			else if (ptr != 0)
				SetEOS(0);
			return (false); 
		}
		else
		{
			if (bTrim && (_MIN_SIZE < nCapacity || nCapacity < n))
			{
				Tidy(true);
				Copy(n); 
			}
			else if (!bTrim && nCapacity < n)
				Copy(n);
			return (true); 
		}
	}
	static const E * __cdecl Nullstr()
	{
		static const E c = E(0);
		return (&c); 
	}
	static size_type Pdif(const_pointer _P2, const_pointer _P1)
	{
		return (_P2 == 0 ? 0 : _P2 - _P1); 
	}
	static const_pointer Psum(const_pointer _P, size_type n)
	{
		return (_P == 0 ? 0 : _P + n); 
	}
	static pointer Psum(pointer _P, size_type n)
	{
		return (_P == 0 ? 0 : _P + n); 
	}
	unsigned char& RefCount(const E *_U)
	{
		return (((unsigned char *)_U)[-1]); 
	}
	void Split()
	{
		if (ptr != 0 && RefCount(ptr) != 0 && RefCount(ptr) != _FROZEN)
		{
			E *_Temp = ptr;
			Tidy(true);
			assign(_Temp); 
		}
	}
	void Tidy(bool _Built = false)
	{
		if (!_Built || ptr == 0)
			;
		else if (RefCount(ptr) == 0 || RefCount(ptr) == _FROZEN)
			allocator.deallocate(ptr - 1, nCapacity + 2);
		else
			--RefCount(ptr);
		
		ptr = 0, nLength = 0, nCapacity = 0; 
	}
	
	E *ptr;
	size_type nLength, nCapacity;
};


template<class E, class Traits, class Alloc>
const basic_string<E, Traits, Alloc>::size_type
basic_string<E, Traits, Alloc>::npos = -1;

typedef basic_string<char, char_traits<char>, allocator<char> > string;
typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;


/*
* Copyright (c) 1995 by P.J. Plauger.  ALL RIGHTS RESERVED. 
* Consult your license regarding permissions and restrictions.
*/
#endif