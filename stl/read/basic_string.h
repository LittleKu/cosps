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

	explicit basic_string(const Alloc& _Al = Alloc()) : allocator(_Al) 
	{
		tidy(); 
	}
	basic_string(const BasicStringType& str) : allocator(str.allocator)
	{
		tidy(), assign(str, 0, npos); 
	}
	basic_string(const BasicStringType& str, size_type _P, size_type _M, 
		const Alloc& _Al = Alloc()) : allocator(_Al) 
	{
		tidy(), assign(str, _P, _M); 
	}
	basic_string(const E *_S, size_type _N, const Alloc& _Al = Alloc()) : allocator(_Al) 
	{
		tidy(), assign(_S, _N); 
	}
	basic_string(const E *_S, const Alloc& _Al = Alloc()) : allocator(_Al) 
	{
		tidy(), assign(_S); 
	}
	basic_string(size_type _N, E _C, const Alloc& _Al = Alloc())
		: allocator(_Al) 
	{
		tidy(), assign(_N, _C); 
	}
	typedef const_iterator _It;
	basic_string(_It _F, _It _L, const Alloc& _Al = Alloc())
		: allocator(_Al) 
	{
		tidy(); assign(_F, _L); 
	}
	~basic_string()
	{
		tidy(true); 
	}
	typedef Traits traits_type;
	typedef Alloc allocator_type;
	enum _Mref {_FROZEN = 255};
	static const size_type npos;
	BasicStringType& operator=(const BasicStringType& str)
	{
		return (assign(str)); 
	}
	BasicStringType& operator=(const E *_S)
	{
		return (assign(_S)); 
	}
	BasicStringType& operator=(E _C)
	{
		return (assign(1, _C)); 
	}
	BasicStringType& operator+=(const BasicStringType& str)
	{
		return (append(str)); 
	}
	BasicStringType& operator+=(const E *_S)
	{
		return (append(_S)); 
	}
	BasicStringType& operator+=(E _C)
	{
		return (append(1, _C)); 
	}
	BasicStringType& append(const BasicStringType& str)
	{
		return (append(str, 0, npos)); 
	}
	BasicStringType& append(const BasicStringType& str, size_type _P, size_type _M)
	{
		if (str.size() < _P)
			throw_range_exception();
		size_type _N = str.size() - _P;
		if (_N < _M)
			_M = _N;
		if (npos - nLength <= _M)
			throw_length_exception();
		if (0 < _M && _Grow(_N = nLength + _M))
		{
			Traits::copy(ptr + nLength, &str.c_str()[_P], _M);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& append(const E *_S, size_type _M)
	{
		if (npos - nLength <= _M)
			throw_length_exception();
		size_type _N;
		if (0 < _M && _Grow(_N = nLength + _M))
		{
			Traits::copy(ptr + nLength, _S, _M);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& append(const E *_S)
	{
		return (append(_S, Traits::length(_S))); 
	}
	BasicStringType& append(size_type _M, E _C)
	{
		if (npos - nLength <= _M)
			throw_length_exception();
		size_type _N;
		if (0 < _M && _Grow(_N = nLength + _M))
		{
			Traits::assign(ptr + nLength, _M, _C);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& append(_It _F, _It _L)
	{
		return (replace(end(), end(), _F, _L)); 
	}
	BasicStringType& assign(const BasicStringType& str)
	{return (assign(str, 0, npos)); }
	BasicStringType& assign(const BasicStringType& str, size_type _P, size_type _M)
	{
		if (str.size() < _P)
			throw_range_exception();
		size_type _N = str.size() - _P;
		if (_M < _N)
			_N = _M;
		if (this == &str)
			erase((size_type)(_P + _N)), erase(0, _P);
		else if (0 < _N && _N == str.size() && _Refcnt(str.c_str()) < _FROZEN - 1 
			&& allocator == str.allocator)
		{
			tidy(true);
			ptr = (E *)str.c_str();
			nLength = str.size();
			nCapacity = str.capacity();
			++_Refcnt(ptr); 
		}
		else if (_Grow(_N, true))
		{
			Traits::copy(ptr, &str.c_str()[_P], _N);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& assign(const E *_S, size_type _N)
	{
		if (_Grow(_N, true))
		{
			Traits::copy(ptr, _S, _N);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& assign(const E *_S)
	{
		return (assign(_S, Traits::length(_S))); 
	}
	BasicStringType& assign(size_type _N, E _C)
	{
		if (_N == npos)
			throw_length_exception();
		if (_Grow(_N, true))
		{
			Traits::assign(ptr, _N, _C);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& assign(_It _F, _It _L)
	{return (replace(begin(), end(), _F, _L)); }
	BasicStringType& insert(size_type _P0, const BasicStringType& str)
	{return (insert(_P0, str, 0, npos)); }
	BasicStringType& insert(size_type _P0, const BasicStringType& str, size_type _P,
		size_type _M)
	{
		if (nLength < _P0 || str.size() < _P)
			throw_range_exception();
		size_type _N = str.size() - _P;
		if (_N < _M)
			_M = _N;
		if (npos - nLength <= _M)
			throw_length_exception();
		if (0 < _M && _Grow(_N = nLength + _M))
		{
			Traits::move(ptr + _P0 + _M, ptr + _P0, nLength - _P0);
			Traits::copy(ptr + _P0, &str.c_str()[_P], _M);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& insert(size_type _P0, const E *_S, size_type _M)
	{
		if (nLength < _P0)
			throw_range_exception();
		if (npos - nLength <= _M)
			throw_length_exception();
		size_type _N;
		if (0 < _M && _Grow(_N = nLength + _M))
		{
			Traits::move(ptr + _P0 + _M, ptr + _P0, nLength - _P0);
			Traits::copy(ptr + _P0, _S, _M);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& insert(size_type _P0, const E *_S)
	{return (insert(_P0, _S, Traits::length(_S))); }
	BasicStringType& insert(size_type _P0, size_type _M, E _C)
	{
		if (nLength < _P0)
			throw_range_exception();
		if (npos - nLength <= _M)
			throw_length_exception();
		size_type _N;
		if (0 < _M && _Grow(_N = nLength + _M))
		{
			Traits::move(ptr + _P0 + _M, ptr + _P0, nLength - _P0);
			Traits::assign(ptr + _P0, _M, _C);
			_Eos(_N); 
		}
		return (*this); 
	}
	iterator insert(iterator _P, E _C)
	{
		size_type _P0 = _Pdif(_P, begin());
		insert(_P0, 1, _C);
		return (begin() + _P0); 
	}
	void insert(iterator _P, size_type _M, E _C)
	{
		size_type _P0 = _Pdif(_P, begin());
		insert(_P0, _M, _C); 
	}
	void insert(iterator _P, _It _F, _It _L)
	{
		replace(_P, _P, _F, _L); 
	}
	BasicStringType& erase(size_type _P0 = 0, size_type _M = npos)
	{
		if (nLength < _P0)
			throw_range_exception();
		_Split();
		if (nLength - _P0 < _M)
			_M = nLength - _P0;
		if (0 < _M)
		{
			Traits::move(ptr + _P0, ptr + _P0 + _M, nLength - _P0 - _M);
			size_type _N = nLength - _M;
			if (_Grow(_N))
				_Eos(_N); 
		}
		return (*this); 
	}
	iterator erase(iterator _P)
	{
		size_t _M = _Pdif(_P, begin());
		erase(_M, 1);
		return (_Psum(ptr, _M)); 
	}
	iterator erase(iterator _F, iterator _L)
	{
		size_t _M = _Pdif(_F, begin());
		erase(_M, _Pdif(_L, _F));
		return (_Psum(ptr, _M)); 
	}
	BasicStringType& replace(size_type _P0, size_type _N0, const BasicStringType& str)
	{
		return (replace(_P0, _N0, str, 0, npos)); 
	}
	BasicStringType& replace(size_type _P0, size_type _N0, const BasicStringType& str,
		size_type _P, size_type _M)
	{	if (nLength < _P0 || str.size() < _P)
			throw_range_exception();
		if (nLength - _P0 < _N0)
			_N0 = nLength - _P0;
		size_type _N = str.size() - _P;
		if (_N < _M)
			_M = _N;
		if (npos - _M <= nLength - _N0)
			throw_length_exception();
		_Split();
		size_type _Nm = nLength - _N0 - _P0;
		if (_M < _N0)
			Traits::move(ptr + _P0 + _M, ptr + _P0 + _N0, _Nm);
		if ((0 < _M || 0 < _N0) && _Grow(_N = nLength + _M - _N0))
		{
			if (_N0 < _M)
			Traits::move(ptr + _P0 + _M, ptr + _P0 + _N0, _Nm);
			Traits::copy(ptr + _P0, &str.c_str()[_P], _M);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& replace(size_type _P0, size_type _N0, const E *_S,
		size_type _M)
	{
		if (nLength < _P0)
		throw_range_exception();
		if (nLength - _P0 < _N0)
			_N0 = nLength - _P0;
		if (npos - _M <= nLength - _N0)
			throw_length_exception();
		_Split();
		size_type _Nm = nLength - _N0 - _P0;
		if (_M < _N0)
			Traits::move(ptr + _P0 + _M, ptr + _P0 + _N0, _Nm);
		size_type _N;
		if ((0 < _M || 0 < _N0) && _Grow(_N = nLength + _M - _N0))
		{
			if (_N0 < _M)
			Traits::move(ptr + _P0 + _M, ptr + _P0 + _N0, _Nm);
			Traits::copy(ptr + _P0, _S, _M);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& replace(size_type _P0, size_type _N0, const E *_S)
	{
		return (replace(_P0, _N0, _S, Traits::length(_S))); 
	}
	BasicStringType& replace(size_type _P0, size_type _N0,
		size_type _M, E _C)
	{
		if (nLength < _P0)
		throw_range_exception();
		if (nLength - _P0 < _N0)
			_N0 = nLength - _P0;
		if (npos - _M <= nLength - _N0)
			throw_length_exception();
		_Split();
		size_type _Nm = nLength - _N0 - _P0;
		if (_M < _N0)
			Traits::move(ptr + _P0 + _M, ptr + _P0 + _N0, _Nm);
		size_type _N;
		if ((0 < _M || 0 < _N0) && _Grow(_N = nLength + _M - _N0))
		{
			if (_N0 < _M)
				Traits::move(ptr + _P0 + _M, ptr + _P0 + _N0, _Nm);
			Traits::assign(ptr + _P0, _M, _C);
			_Eos(_N); 
		}
		return (*this); 
	}
	BasicStringType& replace(iterator _F, iterator _L, const BasicStringType& str)
	{
		return (replace(_Pdif(_F, begin()), _Pdif(_L, _F), str)); 
	}
	BasicStringType& replace(iterator _F, iterator _L, const E *_S,
		size_type _M)
	{
		return (replace(_Pdif(_F, begin()), _Pdif(_L, _F), _S, _M)); 
	}
	BasicStringType& replace(iterator _F, iterator _L, const E *_S)
	{
		return (replace(
	_Pdif(_F, begin()), _Pdif(_L, _F), _S)); 
	}
	BasicStringType& replace(iterator _F, iterator _L,	size_type _M, E _C)
	{
		return (replace(
	_Pdif(_F, begin()), _Pdif(_L, _F), _M, _C)); 
	}
	BasicStringType& replace(iterator _F1, iterator _L1,
		_It _F2, _It _L2)
	{
		size_type _P0 = _Pdif(_F1, begin());
		size_type _M = 0;
		_Distance(_F2, _L2, _M);
		replace(_P0, _Pdif(_L1, _F1), _M, E(0));
		for (_F1 = begin() + _P0; 0 < _M; ++_F1, ++_F2, --_M)
			*_F1 = *_F2;
		return (*this); 
	}
	iterator begin()
	{
		_Freeze();
		return (ptr); 
	}
	const_iterator begin() const
	{
		return (ptr); 
	}
	iterator end()
	{
		_Freeze();
		return ((iterator)_Psum(ptr, nLength)); 
	}
	const_iterator end() const
	{return ((const_iterator)_Psum(ptr, nLength)); }
	reverse_iterator rbegin()
	{return (reverse_iterator(end())); }
	const_reverse_iterator rbegin() const
	{return (const_reverse_iterator(end())); }
	reverse_iterator rend()
	{return (reverse_iterator(begin())); }
	const_reverse_iterator rend() const
	{return (const_reverse_iterator(begin())); }
	reference at(size_type _P0)
	{
		if (nLength <= _P0)
			throw_range_exception();
		_Freeze();
		return (ptr[_P0]); 
	}
	const_reference at(size_type _P0) const
	{
		if (nLength <= _P0)
			throw_range_exception();
		return (ptr[_P0]); 
	}
	reference operator[](size_type _P0)
	{
		if (nLength < _P0 || ptr == 0)
			return ((reference)*_Nullstr());
		_Freeze();
		return (ptr[_P0]); 
	}
	const_reference operator[](size_type _P0) const
	{
		if (ptr == 0)
			return (*_Nullstr());
		else
			return (ptr[_P0]); 
	}
	const E *c_str() const
	{
		return (ptr == 0 ? _Nullstr() : ptr); 
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
	{return (nLength); }
	size_type max_size() const
	{size_type _N = allocator.max_size();
	return (_N <= 2 ? 1 : _N - 2); }
	void resize(size_type _N, E _C)
	{
		_N <= nLength ? erase(_N) : append(_N - nLength, _C); }

	void resize(size_type _N)
	{_N <= nLength ? erase(_N) : append(_N - nLength, E(0)); }
	size_type capacity() const
	{return (nCapacity); }
	void reserve(size_type _N = 0)
	{
		if (nCapacity < _N)
			_Grow(_N); 
	}
	bool empty() const
	{return (nLength == 0); }
	size_type copy(E *_S, size_type _N, size_type _P0 = 0) const
	{
		if (nLength < _P0)
			throw_range_exception();
		if (nLength - _P0 < _N)
			_N = nLength - _P0;
		if (0 < _N)
			Traits::copy(_S, ptr + _P0, _N);
		return (_N); 
	}
	void swap(BasicStringType& str)
	{
		if (allocator == str.allocator)
		{
			std::swap(ptr, str._Ptr);
			std::swap(nLength, str._Len);
			std::swap(nCapacity, str._Res); 
		}
		else
		{
			BasicStringType _Ts = *this; 
			*this = str, str = _Ts; 
		}
	}
	friend void swap(BasicStringType& str, BasicStringType& _Y)
	{
		str.swap(_Y); 
	}
	size_type find(const BasicStringType& str, size_type _P = 0) const
	{return (find(str.c_str(), _P, str.size())); }
	size_type find(const E *_S, size_type _P,
		size_type _N) const
	{
		if (_N == 0 && _P <= nLength)
			return (_P);
		size_type _Nm;
		if (_P < nLength && _N <= (_Nm = nLength - _P))
		{
			const E *_U, *_V;
			for (_Nm -= _N - 1, _V = ptr + _P; (_U = Traits::find(_V, _Nm, *_S)) != 0; 
				_Nm -= _U - _V + 1, _V = _U + 1)

				if (Traits::compare(_U, _S, _N) == 0)
					return (_U - ptr); 
		}
		return (npos); 
	}
	size_type find(const E *_S, size_type _P = 0) const
	{return (find(_S, _P, Traits::length(_S))); }
	size_type find(E _C, size_type _P = 0) const
	{return (find((const E *)&_C, _P, 1)); }
	size_type rfind(const BasicStringType& str, size_type _P = npos) const
	{return (rfind(str.c_str(), _P, str.size())); }
	size_type rfind(const E *_S, size_type _P,
		size_type _N) const
	{
		if (_N == 0)
			return (_P < nLength ? _P : nLength);
		if (_N <= nLength)
		for (const E *_U = ptr + + (_P < nLength - _N ? _P : nLength - _N); ; --_U)
			if (Traits::eq(*_U, *_S) && Traits::compare(_U, _S, _N) == 0)
				return (_U - ptr);
			else if (_U == ptr)
				break;
		return (npos); 
	}
	size_type rfind(const E *_S, size_type _P = npos) const
	{
		return (rfind(_S, _P, Traits::length(_S))); 
	}
	size_type rfind(E _C, size_type _P = npos) const
	{return (rfind((const E *)&_C, _P, 1)); }
	size_type find_first_of(const BasicStringType& str,
		size_type _P = 0) const
	{return (find_first_of(str.c_str(), _P, str.size())); }
	size_type find_first_of(const E *_S, size_type _P,
		size_type _N) const
	{
		if (0 < _N && _P < nLength)
		{
			const E *const _V = ptr + nLength;
			for (const E *_U = ptr + _P; _U < _V; ++_U)
				if (Traits::find(_S, _N, *_U) != 0)
					return (_U - ptr); 
		}
		return (npos); 
	}

	size_type find_first_of(const E *_S, size_type _P = 0) const
	{return (find_first_of(_S, _P, Traits::length(_S))); }
	size_type find_first_of(E _C, size_type _P = 0) const
	{return (find((const E *)&_C, _P, 1)); }
	size_type find_last_of(const BasicStringType& str,
		size_type _P = npos) const
	{return (find_last_of(str.c_str(), _P, str.size())); }
	size_type find_last_of(const E *_S, size_type _P,
		size_type _N) const
	{
		if (0 < _N && 0 < nLength)
			for (const E *_U = ptr + (_P < nLength ? _P : nLength - 1); ; --_U)
				if (Traits::find(_S, _N, *_U) != 0)
					return (_U - ptr);
				else if (_U == ptr)
					break;
			return (npos); 
	}
	size_type find_last_of(const E *_S,
		size_type _P = npos) const
	{return (find_last_of(_S, _P, Traits::length(_S))); }
	size_type find_last_of(E _C, size_type _P = npos) const
	{return (rfind((const E *)&_C, _P, 1)); }
	size_type find_first_not_of(const BasicStringType& str,
		size_type _P = 0) const
	{return (find_first_not_of(str.c_str(), _P,
	str.size())); }
	size_type find_first_not_of(const E *_S, size_type _P,
		size_type _N) const
	{
		if (_P < nLength)
		{
			const E *const _V = ptr + nLength;
			for (const E *_U = ptr + _P; _U < _V; ++_U)
				if (Traits::find(_S, _N, *_U) == 0)
					return (_U - ptr); 
		}
		return (npos); 
	}
	size_type find_first_not_of(const E *_S,
		size_type _P = 0) const
	{return (find_first_not_of(_S, _P, Traits::length(_S))); }
	size_type find_first_not_of(E _C, size_type _P = 0) const
	{return (find_first_not_of((const E *)&_C, _P, 1)); }
	size_type find_last_not_of(const BasicStringType& str,
		size_type _P = npos) const
	{return (find_last_not_of(str.c_str(), _P, str.size())); }
	size_type find_last_not_of(const E *_S, size_type _P,
		size_type _N) const
	{
		if (0 < nLength)
		for (const E *_U = ptr + (_P < nLength ? _P : nLength - 1); ; --_U)
		if (Traits::find(_S, _N, *_U) == 0)
			return (_U - ptr);
		else if (_U == ptr)
			break;
		return (npos); 
	}
	size_type find_last_not_of(const E *_S,
		size_type _P = npos) const
	{return (find_last_not_of(_S, _P, Traits::length(_S))); }
	size_type find_last_not_of(E _C, size_type _P = npos) const
	{return (find_last_not_of((const E *)&_C, _P, 1)); }
	BasicStringType substr(size_type _P = 0, size_type _M = npos) const
	{return (BasicStringType(*this, _P, _M)); }
	int compare(const BasicStringType& str) const
	{return (compare(0, nLength, str.c_str(), str.size())); }
	int compare(size_type _P0, size_type _N0,
		const BasicStringType& str) const
	{return (compare(_P0, _N0, str, 0, npos)); }
	int compare(size_type _P0, size_type _N0, const BasicStringType& str,
		size_type _P, size_type _M) const
	{
		if (str.size() < _P)
			throw_range_exception();
		if (str._Len - _P < _M)
			_M = str._Len - _P;
	return (compare(_P0, _N0, str.c_str() + _P, _M)); }
	int compare(const E *_S) const
	{return (compare(0, nLength, _S, Traits::length(_S))); }
	int compare(size_type _P0, size_type _N0, const E *_S) const
	{return (compare(_P0, _N0, _S, Traits::length(_S))); }
	int compare(size_type _P0, size_type _N0, const E *_S,
		size_type _M) const
	{
		if (nLength < _P0)
			throw_range_exception();
		if (nLength - _P0 < _N0)
			_N0 = nLength - _P0;
		size_type _Ans = Traits::compare(_Psum(ptr, _P0), _S, _N0 < _M ? _N0 : _M);
		return (_Ans != 0 ? _Ans : _N0 < _M ? -1 : _N0 == _M ? 0 : +1); 
	}
	Alloc get_allocator() const
	{return (allocator); }
protected:
	Alloc allocator;
private:
	enum {_MIN_SIZE = sizeof (E) <= 32 ? 31 : 7};
	void _Copy(size_type _N)
	{
		size_type _Ns = _N | _MIN_SIZE;
		if (max_size() < _Ns)
			_Ns = _N;
		E *_S;
		_TRY_BEGIN
			_S = allocator.allocate(_Ns + 2, (void *)0);
		_CATCH_ALL
			_Ns = _N;
		_S = allocator.allocate(_Ns + 2, (void *)0);
		_CATCH_END
		if (0 < nLength)
			Traits::copy(_S + 1, ptr,nLength>_Ns?_Ns:nLength);
		size_type _Olen = nLength;
		tidy(true);
		ptr = _S + 1;
		_Refcnt(ptr) = 0;
		nCapacity = _Ns;
		_Eos(_Olen>_Ns?_Ns:_Olen); 
	}
	void _Eos(size_type _N)
	{
		Traits::assign(ptr[nLength = _N], E(0)); 
	}
	void _Freeze()
	{
		if (ptr != 0 && _Refcnt(ptr) != 0 && _Refcnt(ptr) != _FROZEN)
			_Grow(nLength);
		if (ptr != 0)
			_Refcnt(ptr) = _FROZEN; 
	}
	bool _Grow(size_type _N, bool _Trim = false)
	{
		if (max_size() < _N)
			throw_length_exception();
		if (ptr != 0 && _Refcnt(ptr) != 0 && _Refcnt(ptr) != _FROZEN)
			if (_N == 0)
			{
				--_Refcnt(ptr), tidy();
				return (false); 
			}
			else
			{
				_Copy(_N);
				return (true); 
			}
		if (_N == 0)
		{
			if (_Trim)
				tidy(true);
			else if (ptr != 0)
				_Eos(0);
			return (false); 
		}
		else
		{
			if (_Trim && (_MIN_SIZE < nCapacity || nCapacity < _N))
			{
				tidy(true);
				_Copy(_N); 
			}
			else if (!_Trim && nCapacity < _N)
				_Copy(_N);
			return (true); 
		}
	}
	static const E * __cdecl _Nullstr()
	{
		static const E _C = E(0);
		return (&_C); 
	}
	static size_type _Pdif(const_pointer _P2, const_pointer _P1)
	{
		return (_P2 == 0 ? 0 : _P2 - _P1); 
	}
	static const_pointer _Psum(const_pointer _P, size_type _N)
	{
		return (_P == 0 ? 0 : _P + _N); 
	}
	static pointer _Psum(pointer _P, size_type _N)
	{
		return (_P == 0 ? 0 : _P + _N); 
	}
	unsigned char& _Refcnt(const E *_U)
	{
		return (((unsigned char *)_U)[-1]); 
	}
	void _Split()
	{
		if (ptr != 0 && _Refcnt(ptr) != 0 && _Refcnt(ptr) != _FROZEN)
		{
			E *_Temp = ptr;
			tidy(true);
			assign(_Temp); 
		}
	}
	void tidy(bool _Built = false)
	{
		if (!_Built || ptr == 0)
			;
		else if (_Refcnt(ptr) == 0 || _Refcnt(ptr) == _FROZEN)
			allocator.deallocate(ptr - 1, nCapacity + 2);
		else
			--_Refcnt(ptr);

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