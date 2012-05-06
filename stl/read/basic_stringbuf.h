// sstream standard header
#ifndef _BASIC_STRING_BUF_H_
#define _BASIC_STRING_BUF_H_
#include <string>

_STD_BEGIN
enum __Strstate {_Allocated = 1, _Constant = 2, _Noread = 4};
_BITMASK(__Strstate, _Strstate);
_BITMASK_OPS(__Strstate)
		// TEMPLATE CLASS basic_stringbuf
template<class E,
	class _Tr = char_traits<E>,
	class _A = allocator<E> >
class basic_stringbuf : public basic_streambuf<E, _Tr> 
{
public:
	typedef basic_string<E, _Tr, _A> BasicStringType;
	explicit basic_stringbuf(ios_base::openmode _W = ios_base::in | ios_base::out)
	{
		Init(0, 0, _Mode(_W)); 
	}
	explicit basic_stringbuf(const BasicStringType& _S,
		ios_base::openmode _W = ios_base::in | ios_base::out)
	{
		Init(_S.c_str(), _S.size(), _Mode(_W)); 
	}
	virtual ~basic_stringbuf()
	{
		_Tidy(); 
	}
	BasicStringType str() const
	{
		if (!(m_strMode & _Constant) && pptr() != 0)
		{
			BasicStringType _Str(pbase(), (m_pSeekHigh < pptr() ? pptr() : m_pSeekHigh) - pbase());
			return (_Str); 
		}
		else if (!(m_strMode & _Noread) && gptr() != 0)
		{
			BasicStringType _Str(eback(), egptr() - eback());
			return (_Str); 
		}
		else
		{
			BasicStringType _Nul;
			return (_Nul); 
		}
	}
	void str(const BasicStringType& _S)
	{
		_Tidy();
		Init(_S.c_str(), _S.size(), m_strMode); 
	}
protected:
	virtual int_type overflow(int_type _C = _Tr::eof())
	{
		if (_Tr::eq_int_type(_Tr::eof(), _C))
			return (_Tr::not_eof(_C));
		else if (pptr() != 0 && pptr() < epptr())
		{
			*_Pninc() = _Tr::to_char_type(_C);
			return (_C); 
		}
		else if (m_strMode & _Constant)
			return (_Tr::eof());
		else
		{
			size_t _Os = gptr() == 0 ? 0 : epptr() - eback();
			size_t _Ns = _Os + m_nAllocSize;
			E *_P = m_allocator.allocate(_Ns, (void *)0);
			if (0 < _Os)
				_Tr::copy(_P, eback(), _Os);
			else if (_ALSIZE < m_nAllocSize)
				m_nAllocSize = _ALSIZE;
			if (m_strMode & _Allocated)
				m_allocator.deallocate(eback(), _Os);
			m_strMode |= _Allocated;
			if (_Os == 0)
			{
				m_pSeekHigh = _P;
				setp(_P, _P + _Ns);
				setg(_P, _P, _P); 
			}
			else
			{
				m_pSeekHigh = m_pSeekHigh - eback() + _P;
				setp(pbase() - eback() + _P,
					pptr() - eback() + _P, _P + _Ns);
				if (m_strMode & _Noread)
					setg(_P, _P, _P);
				else
					setg(_P, gptr() - eback() + _P, pptr() + 1); 
			}
			*_Pninc() = _Tr::to_char_type(_C);
			return (_C); 
		}
	}
	virtual int_type pbackfail(int_type _C = _Tr::eof())
	{
		if (gptr() == 0 || gptr() <= eback()
			|| !_Tr::eq_int_type(_Tr::eof(), _C)
				&& !_Tr::eq(_Tr::to_char_type(_C), gptr()[-1])
				&& m_strMode & _Constant)
			return (_Tr::eof());
		else
		{
			gbump(-1);
			if (!_Tr::eq_int_type(_Tr::eof(), _C))
				*gptr() = _Tr::to_char_type(_C);
			return (_Tr::not_eof(_C)); 
		}
	}
	virtual int_type underflow()
	{
		if (gptr() == 0)
			return (_Tr::eof());
		else if (gptr() < egptr())
			return (_Tr::to_int_type(*gptr()));
		else if (m_strMode & _Noread || pptr() == 0
			|| pptr() <= gptr() && m_pSeekHigh <= gptr())
			return (_Tr::eof());
		else
		{
			if (m_pSeekHigh < pptr())
				m_pSeekHigh = pptr();
			setg(eback(), gptr(), m_pSeekHigh);
			return (_Tr::to_int_type(*gptr())); 
		}
	}
	virtual pos_type seekoff(off_type _O, ios_base::seekdir _Way,
		ios_base::openmode _Which = ios_base::in | ios_base::out)
		{if (pptr() != 0 && m_pSeekHigh < pptr())
			m_pSeekHigh = pptr();
		if (_Which & ios_base::in && gptr() != 0)
			{if (_Way == ios_base::end)
				_O += m_pSeekHigh - eback();
			else if (_Way == ios_base::cur
				&& !(_Which & ios_base::out))
				_O += gptr() - eback();
			else if (_Way != ios_base::beg)
				_O = _BADOFF;
			if (0 <= _O && _O <= m_pSeekHigh - eback())
				{gbump(eback() - gptr() + _O);
				if (_Which & ios_base::out && pptr() != 0)
					setp(pbase(), gptr(), epptr()); }
			else
				_O = _BADOFF; }
		else if (_Which & ios_base::out && pptr() != 0)
			{if (_Way == ios_base::end)
				_O += m_pSeekHigh - eback();
			else if (_Way == ios_base::cur)
				_O += pptr() - eback();
			else if (_Way != ios_base::beg)
				_O = _BADOFF;
			if (0 <= _O && _O <= m_pSeekHigh - eback())
				pbump(eback() - pptr() + _O);
			else
				_O = _BADOFF; }
		else
			_O = _BADOFF;
		return (pos_type(_O)); }
	virtual pos_type seekpos(pos_type _P, 
		ios_base::openmode _Which = ios_base::in | ios_base::out)
	{
		streamoff _O = (streamoff)_P;
		if (pptr() != 0 && m_pSeekHigh < pptr())
			m_pSeekHigh = pptr();
		if (_O == _BADOFF)
			;
		else if (_Which & ios_base::in && gptr() != 0)
		{
			if (0 <= _O && _O <= m_pSeekHigh - eback())
			{
				gbump(eback() - gptr() + _O);
				if (_Which & ios_base::out && pptr() != 0)
					setp(pbase(), gptr(), epptr()); 
			}
			else
				_O = _BADOFF; 
		}
		else if (_Which & ios_base::out && pptr() != 0)
		{
			if (0 <= _O && _O <= m_pSeekHigh - eback())
				pbump(eback() - pptr() + _O);
			else
				_O = _BADOFF; 
		}
		else
			_O = _BADOFF;
		return (streampos(_O)); 
	}
	void Init(const E *_S, size_t _N, _Strstate _M)
	{
		m_pPendSave = 0, m_pSeekHigh = 0;
		m_nAllocSize = _MINSIZE, m_strMode = _M;
		if (_N == 0 || (m_strMode & (_Noread | _Constant)) == (_Noread | _Constant))
		{
			setg(0, 0, 0);
			setp(0, 0); 
		}
		else
		{
			E *_P = m_allocator.allocate(_N, (void *)0);
			_Tr::copy(_P, _S, _N);
			m_pSeekHigh = _P + _N;
			if (!(m_strMode & _Noread))
				setg(_P, _P, _P + _N);
			if (!(m_strMode & _Constant))
			{
				setp(_P, _P + _N);
				if (gptr() == 0)
					setg(_P, _P, _P); 
			}
			m_strMode |= _Allocated; 
		}
	}
	void _Tidy()
	{
		if (m_strMode & _Allocated)
			m_allocator.deallocate(eback(),(pptr() != 0 ? epptr() : egptr()) - eback());
		m_pSeekHigh = 0;
		m_strMode &= ~_Allocated; 
	}
private:
	enum {_ALSIZE = 512, _MINSIZE = 32};
	_Strstate _Mode(ios_base::openmode _W)
	{
		_Strstate _St = (_Strstate)0;
		if (!(_W & ios_base::in))
			_St |= _Noread;
		if (!(_W & ios_base::out))
			_St |= _Constant;
		return (_St); 
	}
	E *m_pPendSave, *m_pSeekHigh;
	int m_nAllocSize;
	_Strstate m_strMode;
	allocator<E> m_allocator;
};


		// TEMPLATE CLASS basic_istringstream
template<class E,
	class _Tr = char_traits<E>,
	class _A = allocator<E> >
	class basic_istringstream : public basic_istream<E, _Tr> {
public:
	typedef basic_stringbuf<E, _Tr, _A> MyStringBuf;
	typedef basic_string<E, _Tr, _A> MyString;
	explicit basic_istringstream(openmode _M = in) : basic_istream<E, _Tr>(&m_strbuf), m_strbuf(_M | in) 
	{}
	explicit basic_istringstream(const MyString& _S, openmode _M = in)
		: basic_istream<E, _Tr>(&m_strbuf), m_strbuf(_S, _M | in) 
	{}
	virtual ~basic_istringstream()
	{}
	MyStringBuf *rdbuf() const
	{
		return ((MyStringBuf *)&m_strbuf); 
	}
	MyString str() const
	{
		return (m_strbuf.str()); 
	}
	void str(const MyString& _S)
	{
		m_strbuf.str(_S); 
	}
private:
	MyStringBuf m_strbuf;
};
		// TEMPLATE CLASS basic_ostringstream
template<class E, class _Tr = char_traits<E>,class _A = allocator<E> >
class basic_ostringstream : public basic_ostream<E, _Tr> 
{
public:
	typedef basic_stringbuf<E, _Tr, _A> MyStringBuf;
	typedef basic_string<E, _Tr, _A> MyString;
	explicit basic_ostringstream(openmode _M = out)
		: basic_ostream<E, _Tr>(&m_strbuf), m_strbuf(_M | out) {}
	explicit basic_ostringstream(const MyString& _S,
		openmode _M = out)
		: basic_ostream<E, _Tr>(&m_strbuf), m_strbuf(_S, _M | out) {}
	virtual ~basic_ostringstream()
		{}
	MyStringBuf *rdbuf() const
		{return ((MyStringBuf *)&m_strbuf); }
	MyString str() const
		{return (m_strbuf.str()); }
	void str(const MyString& _S)
		{m_strbuf.str(_S); }
private:
	MyStringBuf m_strbuf;
};
		// TEMPLATE CLASS basic_stringstream
template<class E, class _Tr = char_traits<E>, class _A = allocator<E> >
class basic_stringstream : public basic_iostream<E, _Tr> 
{
public:
	typedef basic_string<E, _Tr, _A> MyString;

	explicit basic_stringstream(openmode _W = in | out) : basic_iostream<E, _Tr>(&m_strbuf), m_strbuf(_W) {}

	explicit basic_stringstream(const MyString& _S, openmode _W = in | out)
		: basic_iostream<E, _Tr>(&m_strbuf), m_strbuf(_S, _W) 
	{}

	virtual ~basic_stringstream() {}

	basic_stringbuf<E, _Tr, _A> *rdbuf() const
	{
		return ((basic_stringbuf<E, _Tr, _A> *)&m_strbuf); 
	}
	MyString str() const
	{
		return (m_strbuf.str()); 
	}
	void str(const MyString& _S)
	{
		m_strbuf.str(_S); 
	}
private:
	basic_stringbuf<E, _Tr, _A> m_strbuf;
};


#ifdef _DLL
#pragma warning(disable:4231) /* the extern before template is a non-standard extension */
extern template class _CRTIMP basic_stringbuf<char, char_traits<char> >;
extern template class _CRTIMP basic_stringbuf<wchar_t, char_traits<wchar_t> >;
extern template class _CRTIMP basic_istringstream<char, char_traits<char> >;
extern template class _CRTIMP basic_istringstream<wchar_t, char_traits<wchar_t> >;
extern template class _CRTIMP basic_ostringstream<char, char_traits<char> >;
extern template class _CRTIMP basic_ostringstream<wchar_t, char_traits<wchar_t> >;
extern template class _CRTIMP basic_stringstream<char, char_traits<char> >;
extern template class _CRTIMP basic_stringstream<wchar_t, char_traits<wchar_t> >;
#pragma warning(default:4231) /* restore previous warning */
#endif		// _DLL


_STD_END

#endif /* _SSTREAM_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
