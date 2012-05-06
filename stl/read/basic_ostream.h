// ostream standard header
#ifndef _BASIC_OSTREAM_H_
#define _BASIC_OSTREAM_H_
#include <ios>

		// I/O exception macros
 #define _TRY_IO_BEGIN	_TRY_BEGIN
 #define _CATCH_IO_END	_CATCH_ALL \
	setstate(ios_base::badbit, true); _CATCH_END
 #define _CATCH_IO_(x)	_CATCH_ALL \
	(x).setstate(ios_base::badbit, true); _CATCH_END

_STD_BEGIN
		// TEMPLATE CLASS basic_ostream
template<class E, class _Tr = char_traits<E> >
class basic_ostream : virtual public basic_ios<E, _Tr> 
{
public:
	typedef basic_ostream<E, _Tr> MyOstream;
	typedef basic_ios<E, _Tr> MyIOS;
	typedef basic_streambuf<E, _Tr> MyStreamBuf;
	typedef ostreambuf_iterator<E, _Tr> OStreamBufIter;
	typedef num_put<E, OStreamBufIter> _Nput;
	explicit basic_ostream(basic_streambuf<E, _Tr> *_S, bool _Isstd = false, bool _Doinit = true)
	{
		if (_Doinit)
		    init(_S, _Isstd); 
	}
	basic_ostream(_Uninitialized)
	{
		_Addstd(); 
	}
	virtual ~basic_ostream()
	{}

	class sentry 
	{
	public:
		explicit sentry(_Myt& _Os) : _Ok(_Os.opfx()), _Ostr(_Os) {}
		~sentry()
		{
			if (!uncaught_exception())
				_Ostr.osfx(); 
		}
		operator bool() const
		{
			return (_Ok); 
		}
	private:
		bool _Ok;
		MyOstream& _Ostr;
	};
	bool opfx()
		{if (good() && tie() != 0)
			tie()->flush();
		return (good()); }
	void osfx()
		{if (flags() & unitbuf)
			flush(); }
	MyOstream& operator<<(MyOstream& (__cdecl *_F)(_Myt&))
		{return ((*_F)(*this)); }
	MyOstream& operator<<(MyIOS& (__cdecl *_F)(MyIOS&))
		{(*_F)(*(MyIOS *)this);
		return (*this); }
	MyOstream& operator<<(ios_base& (__cdecl *_F)(ios_base&))
		{(*_F)(*(ios_base *)this);
		return (*this); }
	MyOstream& operator<<(_Bool _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(short _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			fmtflags _Bfl = flags() & basefield;
			long _Y = (_Bfl == oct || _Bfl == hex)
				? (long)(unsigned short)_X : (long)_X;
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _Y).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(unsigned short _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), (unsigned long)_X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(int _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			fmtflags _Bfl = flags() & basefield;
			long _Y = (_Bfl == oct || _Bfl == hex)
				? (long)(unsigned int)_X : (long)_X;
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _Y).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(unsigned int _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), (unsigned long)_X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(long _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(unsigned long _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(float _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), (double)_X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(double _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(long double _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(const void *_X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (_Ok)
			{const _Nput& _Fac = _USE(getloc(), _Nput);
			_TRY_IO_BEGIN
			if (_Fac.put(OStreamBufIter(rdbuf()), *this,
				fill(), _X).failed())
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& operator<<(MyStreamBuf *_Pb)
		{iostate _St = goodbit;
		bool _Copied = false;
		const sentry _Ok(*this);
		if (_Ok && _Pb != 0)
			for (int_type _C = _Tr::eof(); ; _Copied = true)
				{_TRY_BEGIN
				_C = _Tr::eq_int_type(_Tr::eof(), _C)
					? _Pb->sgetc() : _Pb->snextc();
				_CATCH_ALL
					setstate(failbit);
					_RERAISE;
				_CATCH_END
				if (_Tr::eq_int_type(_Tr::eof(),_C))
					break;
				_TRY_IO_BEGIN
					if (_Tr::eq_int_type(_Tr::eof(),
						rdbuf()->sputc(_Tr::to_char_type(_C))))
						{_St |= badbit;
						break; }
				_CATCH_IO_END }
		width(0);
		setstate(!_Copied ? _St | failbit : _St);
		return (*this); }
	MyOstream& put(E _X)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (!_Ok)
			_St |= badbit;
		else
			{_TRY_IO_BEGIN
			 if (_Tr::eq_int_type(_Tr::eof(),
				rdbuf()->sputc(_X)))
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& write(const E *_S, streamsize _N)
		{iostate _St = goodbit;
		const sentry _Ok(*this);
		if (!_Ok)
			_St |= badbit;
		else
			{_TRY_IO_BEGIN
			if (rdbuf()->sputn(_S, _N) != _N)
				_St |= badbit;
			_CATCH_IO_END }
		setstate(_St);
		return (*this); }
	MyOstream& flush()
		{iostate _St = goodbit;
		if (!fail() && rdbuf()->pubsync() == -1)
			_St |= badbit;
		setstate(_St);
		return (*this); }
	MyOstream& seekp(pos_type _P)
		{if (!fail())
			rdbuf()->pubseekpos(_P, out);
		return (*this); }
	MyOstream& seekp(off_type _O, ios_base::seekdir _W)
		{if (!fail())
			rdbuf()->pubseekoff(_O, _W, out);
		return (*this); }
	pos_type tellp()
		{if (!fail())
			return (rdbuf()->pubseekoff(0, cur, out));
		else
			return (streampos(_BADOFF)); }
	};


#ifdef _DLL
#pragma warning(disable:4231) /* the extern before template is a non-standard extension */
extern template class _CRTIMP basic_ostream<char, char_traits<char> >;
extern template class _CRTIMP basic_ostream<wchar_t, char_traits<wchar_t> >;
#pragma warning(default:4231) /* restore previous warning */
#endif		// _DLL


		// INSERTERS
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>& __cdecl operator<<(
		basic_ostream<E, _Tr>& _O, const E *_X)
	{typedef basic_ostream<E, _Tr> _Myos;
	ios_base::iostate _St = ios_base::goodbit;
	size_t _N = _Tr::length(_X);
	size_t _M = _O.width() <= 0 || _O.width() <= _N
		? 0 : _O.width() - _N;
	const _Myos::sentry _Ok(_O);
	if (!_Ok)
		_St |= ios_base::badbit;
	else
		{_TRY_IO_BEGIN
		if ((_O.flags() & ios_base::adjustfield)
			!= ios_base::left)
			for (; 0 < _M; --_M)
				if (_Tr::eq_int_type(_Tr::eof(),
					_O.rdbuf()->sputc(_O.fill())))
					{_St |= ios_base::badbit;
					break; }
		if (_St == ios_base::goodbit
			&& _O.rdbuf()->sputn(_X, _N) != _N)
			_St |= ios_base::badbit;
		if (_St == ios_base::goodbit)
			for (; 0 < _M; --_M)
				if (_Tr::eq_int_type(_Tr::eof(),
					_O.rdbuf()->sputc(_O.fill())))
					{_St |= ios_base::badbit;
					break; }
		_O.width(0);
		_CATCH_IO_(_O) }
	_O.setstate(_St);
	return (_O); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>& __cdecl operator<<(
		basic_ostream<E, _Tr>& _O, E _C)
	{typedef basic_ostream<E, _Tr> _Myos;
	ios_base::iostate _St = ios_base::goodbit;
	const _Myos::sentry _Ok(_O);
	if (_Ok)
		{size_t _M = _O.width() <= 1 ? 0 : _O.width() - 1;
		_TRY_IO_BEGIN
		if ((_O.flags() & ios_base::adjustfield)
			!= ios_base::left)
			for (; _St == ios_base::goodbit && 0 < _M; --_M)
				if (_Tr::eq_int_type(_Tr::eof(),
					_O.rdbuf()->sputc(_O.fill())))
					_St |= ios_base::badbit;
		if (_St == ios_base::goodbit
			&& _Tr::eq_int_type(_Tr::eof(),
				_O.rdbuf()->sputc(_C)))
			_St |= ios_base::badbit;
		for (; _St == ios_base::goodbit && 0 < _M; --_M)
			if (_Tr::eq_int_type(_Tr::eof(),
				_O.rdbuf()->sputc(_O.fill())))
				_St |= ios_base::badbit;
		_CATCH_IO_(_O) }
	_O.width(0);
	_O.setstate(_St);
	return (_O); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>& __cdecl operator<<(
		basic_ostream<E, _Tr>& _O, const signed char *_X)
	{return (_O << (const char *)_X); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>& __cdecl operator<<(
		basic_ostream<E, _Tr>& _O, const signed char _C)
	{return (_O << (char)_C); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>& __cdecl operator<<(
		basic_ostream<E, _Tr>& _O, const unsigned char *_X)
	{return (_O << (const char *)_X); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>& __cdecl operator<<(
		basic_ostream<E, _Tr>& _O, const unsigned char _C)
	{return (_O << (char)_C); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>& __cdecl operator<<(
		basic_ostream<E, _Tr>& _O, const signed short *_X)
	{return (_O << (const wchar_t *)_X); }
		// MANIPULATORS
template<class E, class _Tr> inline
basic_ostream<E, _Tr>& __cdecl endl(basic_ostream<E, _Tr>& _O)
{
	_O.put(_O.widen('\n'));
	_O.flush();
	return (_O); 
}
_CRTIMP inline basic_ostream<char, char_traits<char> >&
	__cdecl endl(basic_ostream<char, char_traits<char> >& _O)
	{_O.put('\n');
	_O.flush();
	return (_O); }
_CRTIMP inline basic_ostream<wchar_t, char_traits<wchar_t> >&
	__cdecl endl(basic_ostream<wchar_t,
		char_traits<wchar_t> >& _O)
	{_O.put('\n');
	_O.flush();
	return (_O); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>&
		__cdecl ends(basic_ostream<E, _Tr>& _O)
	{_O.put(E('\0'));
	return (_O); }
_CRTIMP inline basic_ostream<char, char_traits<char> >&
	__cdecl ends(basic_ostream<char, char_traits<char> >& _O)
	{_O.put('\0');
	return (_O); }
_CRTIMP inline basic_ostream<wchar_t, char_traits<wchar_t> >&
	__cdecl ends(basic_ostream<wchar_t,
		char_traits<wchar_t> >& _O)
	{_O.put('\0');
	return (_O); }
template<class E, class _Tr> inline
	basic_ostream<E, _Tr>&
		__cdecl flush(basic_ostream<E, _Tr>& _O)
	{_O.flush();
	return (_O); }
_CRTIMP inline basic_ostream<char, char_traits<char> >&
	__cdecl flush(basic_ostream<char, char_traits<char> >& _O)
	{_O.flush();
	return (_O); }
_CRTIMP inline basic_ostream<wchar_t, char_traits<wchar_t> >&
	__cdecl flush(basic_ostream<wchar_t,
		char_traits<wchar_t> >& _O)
	{_O.flush();
	return (_O); }

#ifdef _DLL
#pragma warning(disable:4231) /* the extern before template is a non-standard extension */

extern template _CRTIMP basic_ostream<char, char_traits<char> >& __cdecl operator<<(
       basic_ostream<char, char_traits<char> >&, const char *);
extern template _CRTIMP basic_ostream<char, char_traits<char> >& __cdecl operator<<(
       basic_ostream<char, char_traits<char> >&, char);
extern template _CRTIMP basic_ostream<char, char_traits<char> >& __cdecl operator<<(
       basic_ostream<char, char_traits<char> >&, const signed char *);
extern template _CRTIMP basic_ostream<char, char_traits<char> >& __cdecl operator<<(
       basic_ostream<char, char_traits<char> >&, const signed char);
extern template _CRTIMP basic_ostream<char, char_traits<char> >& __cdecl operator<<(
       basic_ostream<char, char_traits<char> >&, const unsigned char *);
extern template _CRTIMP basic_ostream<char, char_traits<char> >& __cdecl operator<<(
       basic_ostream<char, char_traits<char> >&, const unsigned char);
extern template _CRTIMP basic_ostream<wchar_t, char_traits<wchar_t> >& __cdecl operator<<(
       basic_ostream<wchar_t, char_traits<wchar_t> >&, const wchar_t *);
extern template _CRTIMP basic_ostream<wchar_t, char_traits<wchar_t> >& __cdecl operator<<(
       basic_ostream<wchar_t, char_traits<wchar_t> >&, wchar_t);
extern template _CRTIMP basic_ostream<wchar_t, char_traits<wchar_t> >& __cdecl operator<<(
       basic_ostream<wchar_t, char_traits<wchar_t> >&, const signed short *);

#pragma warning(default:4231) /* restore previous warning */
#endif		// _DLL

_STD_END
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* _OSTREAM_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
