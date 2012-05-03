// xmemory internal header (from <memory>)
#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__
//! From: XMEMORY: VC98

#include <cstdlib>
#include <new>
#include <utility>

#ifndef _FARQ	/* specify standard memory model */
 #define _FARQ
 #define _PDFT	ptrdiff_t
 #define _SIZT	size_t
#endif

#define _POINTER_X(T, A)	T _FARQ *
#define _REFERENCE_X(T, A)	T _FARQ &

		// TEMPLATE FUNCTION _Allocate
template<class T> inline
T _FARQ *_Allocate(_PDFT n, T _FARQ *)
{
	if (n < 0)
		n = 0;
	return ((T _FARQ *)operator new((_SIZT)n * sizeof (T))); 
}
		// TEMPLATE FUNCTION _Construct
template<class T1, class T2> inline
void _Construct(T1 _FARQ *dst, const T2& src)
{
	new ((void _FARQ *)dst) T1(src); 
}
		// TEMPLATE FUNCTION _Destroy
template<class T> inline
void _Destroy(T _FARQ *_P)
{
	_DESTRUCTOR(T, _P); 
}
inline void _Destroy(char _FARQ *_P)
{}
inline void _Destroy(wchar_t _FARQ *_P)
{}
		// TEMPLATE CLASS allocator
template<class T>
class allocator 
{
public:
	typedef _SIZT size_type;
	typedef _PDFT difference_type;
	typedef T _FARQ *pointer;
	typedef const T _FARQ *const_pointer;
	typedef T _FARQ& reference;
	typedef const T _FARQ& const_reference;
	typedef T value_type;
	pointer address(reference _X) const
		{return (&_X); }
	const_pointer address(const_reference _X) const
		{return (&_X); }
	pointer allocate(size_type _N, const void *)
		{return (_Allocate((difference_type)_N, (pointer)0)); }
	char _FARQ *_Charalloc(size_type _N)
		{return (_Allocate((difference_type)_N,
			(char _FARQ *)0)); }
	void deallocate(void _FARQ *_P, size_type)
		{operator delete(_P); }
	void construct(pointer _P, const T& _V)
		{_Construct(_P, _V); }
	void destroy(pointer _P)
		{_Destroy(_P); }
	_SIZT max_size() const
	{
		_SIZT n = (_SIZT)(-1) / sizeof (T);
		return (0 < n ? n : 1); 
	}
};

template<class T, class _U> inline
	bool operator==(const allocator<T>&, const allocator<_U>&)
	{return (true); }
template<class T, class _U> inline
	bool operator!=(const allocator<T>&, const allocator<_U>&)
	{return (false); }
		// CLASS allocator<void>
template<> class _CRTIMP allocator<void> {
public:
	typedef void _Ty;
	typedef _Ty _FARQ *pointer;
	typedef const _Ty _FARQ *const_pointer;
	typedef _Ty value_type;
	};


#endif /* _XMEMORY_ */

/*
 * Copyright (c) 1995 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 */
