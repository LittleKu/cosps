#ifndef __CHAR_TRAINTS_H__
#define __CHAR_TRAINTS_H__
//! From: IOSFWD: VC98

#include <cstdio>
#include <cstring>
#include <cwchar>
//#include <xstddef>

typedef int mbstate_t;

// STREAM POSITIONING TYPES (from <streambuf>)
typedef long streamoff;
const streamoff _BADOFF = -1;

typedef int streamsize;

typedef __int64 fpos_t;
extern const fpos_t _extFpos;

// TEMPLATE CLASS fpos (from <streambuf>)
template<class T>
class fpos 
{
	typedef fpos<T> TheType;
public:
	fpos(streamoff offset = 0) : m_offset(offset), m_fpos(_extFpos), m_state(_classState) 
	{}

	fpos(T state, fpos_t pos) : m_offset(0), m_fpos(pos), m_state(state) {}

	T state() const
	{
		return (m_state); 
	}

	void state(T _S)
	{
		m_state = _S; 
	}

	fpos_t get_fpos_t() const
	{
		return (m_fpos); 
	}

	operator streamoff() const
	{
		return (m_offset + _FPOSOFF(m_fpos)); 
	}

	streamoff operator-(const TheType& that) const
	{
		return ((streamoff)*this - (streamoff)that); 
	}

	TheType& operator+=(streamoff offset)
	{
		m_offset += offset;
		return (*this); 
	}
	TheType& operator-=(streamoff offset)
	{
		m_offset -= offset;
		return (*this); 
	}
	TheType operator+(streamoff offset) const
	{
		TheType _Tmp = *this;
		return (_Tmp += offset); 
	}
	TheType operator-(streamoff offset) const
	{
		TheType _Tmp = *this;
		return (_Tmp -= offset); 
	}
	bool operator==(const TheType& that) const
	{
		return ((streamoff)*this == (streamoff)that); 
	}
	bool operator!=(const TheType& that) const
	{
		return (!(*this == that)); 
	}
private:
	static T _classState;
	streamoff m_offset;
	fpos_t m_fpos;
	T m_state;
};

template<class T>
T fpos<T>::_classState;

typedef fpos<mbstate_t> streampos;
typedef streampos wstreampos;

// TEMPLATE STRUCT char_traits (FROM <string>)
template<class E>
struct char_traits 
{
	typedef E char_type;
	typedef E int_type;
	typedef streampos pos_type;
	typedef streamoff off_type;
	typedef mbstate_t state_type;

	static void __cdecl assign(E& x, const E& y)
	{
		x = y; 
	}
	static bool __cdecl eq(const E& x, const E& y)
	{
		return (x == y); 
	}
	static bool __cdecl lt(const E& x, const E& y)
	{
		return (x < y); 
	}
	static int __cdecl compare(const E *str1, const E *str2, size_t n)
	{
		for (size_t i = 0; i < n; ++i, ++str1, ++str2)
			if (!eq(*str1, *str2))
				return (lt(*str1, *str2) ? -1 : +1);
		return (0); 
	}
	static size_t __cdecl length(const E *str)
	{
		size_t n;
		for (n = 0; !eq(*str, E(0)); ++str)
			++n;
		return (n); 
	}
	static E * __cdecl copy(E *str1, const E *str2, size_t n)
	{
		E *str = str1;
		for (; 0 < n; --n, ++str1, ++str2)
			assign(*str1, *str2);
		return (str); 
	}
	static const E * __cdecl find(const E *str1, size_t n, const E& c)
	{
		for (; 0 < n; --n, ++str1)
			if (eq(*str1, c))
				return (str1);
		return (0); 
	}
	static E * __cdecl move(E *str1, const E *str2, size_t n)
	{
		E *result = str1;
		if (str2 < str1 && str1 < str2 + n)
			for (str1 += n, str2 += n; 0 < n; --n)
				assign(*--str1, *--str2);
		else
			for (; 0 < n; --n, ++str1, ++str2)
				assign(*str1, *str2);
		return (result); 
	}
	static E * __cdecl assign(E *str1, size_t n, const E& c)
	{
		E *result = str1;
		for (; 0 < n; --n, ++str1)
			assign(*str1, c);
		return (result); 
	}
	static E __cdecl to_char_type(const int_type& c)
	{
		return ((E)c); 
	}
	static int_type __cdecl to_int_type(const E& c)
	{
		return ((int_type)c); 
	}
	static bool __cdecl eq_int_type(const int_type& x, const int_type& y)
	{
		return (x == y); 
	}
	static int_type __cdecl eof()
	{
		return (EOF); 
	}
	static int_type __cdecl not_eof(const int_type& c)
	{
		return (c != eof() ? c : !eof()); 
	}
};


// STRUCT char_traits<wchar_t>
template<> 
struct char_traits<wchar_t> 
{
	typedef wchar_t E;
	typedef E char_type;   // for overloads
	typedef wint_t int_type;
	typedef streampos pos_type;
	typedef streamoff off_type;
	typedef mbstate_t state_type;

	static void __cdecl assign(E& x, const E& y)
	{
		x = y; 
	}
	static bool __cdecl eq(const E& x, const E& y)
	{
		return (x == y); 
	}
	static bool __cdecl lt(const E& x, const E& y)
	{
		return (x < y); 
	}
	static int __cdecl compare(const E *str1, const E *str2, size_t n)
	{
		return (wmemcmp(str1, str2, n)); 
	}
	static size_t __cdecl length(const E *str1)
	{
		return (wcslen(str1)); 
	}
	static E *__cdecl copy(E *str1, const E *str2, size_t n)
	{
		return (wmemcpy(str1, str2, n)); 
	}
	static const E * __cdecl find(const E *str1, size_t n, const E& c)
	{
		return ((const E *)wmemchr(str1, c, n)); 
	}
	static E * __cdecl move(E *str1, const E *str2, size_t n)
	{
		return (wmemmove(str1, str2, n)); 
	}
	static E * __cdecl assign(E *str1, size_t n, const E& c)
	{
		return (wmemset(str1, c, n)); 
	}
	static E __cdecl to_char_type(const int_type& c)
	{
		return ((E)c); 
	}
	static int_type __cdecl to_int_type(const E& c)
	{
		return ((int_type)c); 
	}
	static bool __cdecl eq_int_type(const int_type& x, const int_type& y)
	{
		return (x == y); 
	}
	static int_type __cdecl eof()
	{
		return (WEOF); 
	}
	static int_type __cdecl not_eof(const int_type& c)
	{
		return (c != eof() ? c : !eof()); 
	}
};
// STRUCT char_traits<char> (FROM <string>)
template<> struct _CRTIMP char_traits<char> {
	typedef char E;
	typedef E char_type;
	typedef int int_type;
	typedef streampos pos_type;
	typedef streamoff off_type;
	typedef mbstate_t state_type;
	static void __cdecl assign(E& x, const E& y)
	{
		x = y; 
	}
	static bool __cdecl eq(const E& x, const E& y)
	{
		return (x == y); 
	}
	static bool __cdecl lt(const E& x, const E& y)
	{
		return (x < y); 
	}
	static int __cdecl compare(const E *str1, const E *str2, size_t n)
	{
		return (memcmp(str1, str2, n)); 
	}
	static size_t __cdecl length(const E *str1)
	{
		return (strlen(str1)); 
	}
	static E * __cdecl copy(E *str1, const E *str2, size_t n)
	{
		return ((E *)memcpy(str1, str2, n)); 
	}
	static const E * __cdecl find(const E *str1, size_t n, const E& c)
	{
		return ((const E *)memchr(str1, c, n)); 
	}
	static E * __cdecl move(E *str1, const E *str2, size_t n)
	{
		return ((E *)memmove(str1, str2, n)); 
	}
	static E * __cdecl assign(E *str1, size_t n, const E& c)
	{
		return ((E *)memset(str1, c, n)); 
	}
	static E __cdecl to_char_type(const int_type& c)
	{
		return ((E)c); 
	}
	static int_type __cdecl to_int_type(const E& c)
	{
		return ((int_type)((unsigned char)c)); 
	}
	static bool __cdecl eq_int_type(const int_type& x, const int_type& y)
	{
		return (x == y); 
	}
	static int_type __cdecl eof()
	{
		return (EOF); 
	}
	static int_type __cdecl not_eof(const int_type& c)
	{
		return (c != eof() ? c : !eof()); 
	}
};
// FORWARD REFERENCES
template<class T>
class allocator;
class ios_base;

template<class _E, class _Tr = char_traits<_E> >
class basic_ios;

template<class _E, class _Tr = char_traits<_E> >
class istreambuf_iterator;

template<class _E, class _Tr = char_traits<_E> >
class ostreambuf_iterator;

template<class _E, class _Tr = char_traits<_E> >
class basic_streambuf;

template<class _E, class _Tr = char_traits<_E> >
class basic_istream;

template<class _E, class _Tr = char_traits<_E> >
class basic_ostream;

template<class _E, class _Tr = char_traits<_E> >
class basic_iostream;

template<class _E, class _Tr = char_traits<_E>,
class _A = allocator<_E> >
class basic_stringbuf;

template<class _E, class _Tr = char_traits<_E>,
class _A = allocator<_E> >
class basic_istringstream;

template<class _E, class _Tr = char_traits<_E>,
class _A = allocator<_E> >
class basic_ostringstream;

template<class _E, class _Tr = char_traits<_E>,
class _A = allocator<_E> >
class basic_stringstream;

template<class _E, class _Tr = char_traits<_E> >
class basic_filebuf;

template<class _E, class _Tr = char_traits<_E> >
class basic_ifstream;

template<class _E, class _Tr = char_traits<_E> >
class basic_ofstream;

template<class _E, class _Tr = char_traits<_E> >
class basic_fstream;

// char TYPEDEFS
typedef basic_ios<char, char_traits<char> > ios;
typedef basic_streambuf<char, char_traits<char> > streambuf;
typedef basic_istream<char, char_traits<char> > istream;
typedef basic_ostream<char, char_traits<char> > ostream;
typedef basic_iostream<char, char_traits<char> > iostream;

typedef basic_stringbuf<char, char_traits<char>,
allocator<char> > stringbuf;

typedef basic_istringstream<char, char_traits<char>,
allocator<char> > istringstream;

typedef basic_ostringstream<char, char_traits<char>,
allocator<char> > ostringstream;

typedef basic_stringstream<char, char_traits<char>,
allocator<char> > stringstream;

typedef basic_filebuf<char, char_traits<char> > filebuf;
typedef basic_ifstream<char, char_traits<char> > ifstream;
typedef basic_ofstream<char, char_traits<char> > ofstream;
typedef basic_fstream<char, char_traits<char> > fstream;
// wchar_t TYPEDEFS
typedef basic_ios<wchar_t, char_traits<wchar_t> > wios;
typedef basic_streambuf<wchar_t, char_traits<wchar_t> >
wstreambuf;

typedef basic_istream<wchar_t, char_traits<wchar_t> > wistream;
typedef basic_ostream<wchar_t, char_traits<wchar_t> > wostream;
typedef basic_iostream<wchar_t, char_traits<wchar_t> > wiostream;
typedef basic_stringbuf<wchar_t, char_traits<wchar_t>,
allocator<wchar_t> > wstringbuf;

typedef basic_istringstream<wchar_t, char_traits<wchar_t>,
allocator<wchar_t> > wistringstream;

typedef basic_ostringstream<wchar_t, char_traits<wchar_t>,
allocator<wchar_t> > wostringstream;

typedef basic_stringstream<wchar_t, char_traits<wchar_t>,
allocator<wchar_t> > wstringstream;

typedef basic_filebuf<wchar_t, char_traits<wchar_t> > wfilebuf;
typedef basic_ifstream<wchar_t, char_traits<wchar_t> > wifstream;
typedef basic_ofstream<wchar_t, char_traits<wchar_t> > wofstream;
typedef basic_fstream<wchar_t, char_traits<wchar_t> > wfstream;

#endif
