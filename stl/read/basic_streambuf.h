// streambuf standard header

#ifndef _STREAM_BUF_H_
#define _STREAM_BUF_H_
#include <xlocnum>

_STD_BEGIN
		// TEMPLATE CLASS basic_streambuf
template<class E, class CharTraits = char_traits<E> >
class basic_streambuf 
{
protected:
	basic_streambuf(): m_locale() {_Init(); }
	basic_streambuf(_Uninitialized) : m_locale(_Noinit) {}
public:
	typedef basic_streambuf<E, CharTraits> MyType;
	typedef E char_type;
	typedef CharTraits traits_type;
	virtual ~basic_streambuf() {}
	typedef CharTraits::int_type int_type;
	typedef CharTraits::pos_type pos_type;
	typedef CharTraits::off_type off_type;
	pos_type pubseekoff(off_type off, ios_base::seekdir way, ios_base::openmode mode = ios_base::in | ios_base::out)
	{
		return (seekoff(off, way, mode)); 
	}
	pos_type pubseekoff(off_type _O, ios_base::seek_dir _W, ios_base::open_mode _M)
	{
		return (pubseekoff(_O, (ios_base::seekdir)_W,(ios_base::openmode)_M)); 
	}
	pos_type pubseekpos(pos_type _P, ios_base::openmode _M = ios_base::in | ios_base::out)
	{
		return (seekpos(_P, _M)); 
	}
	pos_type pubseekpos(pos_type _P, ios_base::open_mode _M)
	{
		return (seekpos(_P, (ios_base::openmode)_M)); 
	}
	MyType *pubsetbuf(E *_S, streamsize _N)
	{
		return (setbuf(_S, _N)); 
	}
	locale pubimbue(const locale &_Ln)
	{
		locale _Lo = m_locale;
		imbue(_Ln);
		m_locale = _Ln;
		return (_Lo); 
	}
	locale getloc()
	{
		return (m_locale); 
	}
	streamsize in_avail()
	{
		return (gptr() != 0 && gptr() < egptr() ? egptr() - gptr() : showmanyc()); 
	}
	int pubsync()
	{
		return (sync()); 
	}
	int_type sbumpc()
	{
		return (gptr() != 0 && gptr() < egptr() ? CharTraits::to_int_type(*_Gninc()) : uflow()); 
	}
	int_type sgetc()
	{
		return (gptr() != 0 && gptr() < egptr() ? CharTraits::to_int_type(*gptr()) : underflow()); 
	}
	streamsize sgetn(E *str, streamsize count)
	{
		return (xsgetn(str, count)); 
	}
	int_type snextc()
	{
		return (CharTraits::eq_int_type(CharTraits::eof(), sbumpc())
			? CharTraits::eof() : sgetc()); 
	}
	int_type sputbackc(E _C)
	{
		return (gptr() != 0 && eback() < gptr()
			&& CharTraits::eq(_C, gptr()[-1])
			? CharTraits::to_int_type(*_Gndec())
			: pbackfail(CharTraits::to_int_type(_C))); 
	}
	void stossc()
	{
		if (gptr() != 0 && gptr() < egptr())
			_Gninc();
		else
			uflow(); 
	}
	//Moves the get pointer one character backwards, making the last character gotten by an input operation available once again for the next input operation.
	int_type sungetc()
	{
		return (gptr() != 0 && eback() < gptr()
			? CharTraits::to_int_type(*_Gndec()) : pbackfail()); 
	}
	int_type sputc(E ch)
	{
		return (pptr() != 0 && pptr() < epptr()
			? CharTraits::to_int_type(*_Pninc() = ch) : overflow(CharTraits::to_int_type(ch))); 
	}
	streamsize sputn(const E *str, streamsize count)
	{
		return (xsputn(str, count)); 
	}
protected:
	//Returns a reference to the first element of the accessible part of the controlled input sequence.
	E *eback() const
	{
		return (*m_ppGetBegin); 
	}

	//Current position in input sequence ("get pointer")
	E *gptr() const
	{
		return (*m_ppGetNext); 
	}

	E *pbase() const
	{
		return (*m_ppPutBegin); 
	}
	E *pptr() const
	{
		return (*m_ppPutNext); 
	}

	//End of accessible input sequence
	E *egptr() const
	{
		return (*m_ppGetNext + *m_pGetCount); 
	}

	//Advances the get pointer by n positions.
	void gbump(int n)
	{
		*m_pGetCount -= n;
		*m_ppGetNext += n; 
	}
	void setg(E *begin, E *next, E *last)
	{
		*m_ppGetBegin = begin, *m_ppGetNext = next, *m_pGetCount = last - next; 
	}
	E *epptr() const
	{
		return (*m_ppPutNext + *m_pPutCount); 
	}
	E *_Gndec()
	{
		++*m_pGetCount;
		return (--*m_ppGetNext); 
	}
	E *_Gninc()
	{
		--*m_pGetCount;
		return ((*m_ppGetNext)++); 
	}
	void pbump(int n)
	{
		*m_pPutCount -= n;
		*m_ppPutNext += n; 
	}
	void setp(E *begin, E *last)
	{
		*m_ppPutBegin = begin, *m_ppPutNext = begin, *m_pPutCount = last - begin; 
	}
	void setp(E *begin, E *next, E *last)
	{
		*m_ppPutBegin = begin, *m_ppPutNext = next, *m_pPutCount = last - next; 
	}
	E *_Pninc()
	{
		--*m_pPutCount;
		return ((*m_ppPutNext)++); 
	}
	void _Init()
	{
		m_ppGetBegin = &m_pGetBegin, m_ppPutBegin = &m_pPutBegin;
		m_ppGetNext = &m_pGetNext, m_ppPutNext = &m_pPutNext;
		m_pGetCount = &m_nGetCount, m_pPutCount = &m_nPutCount;
		setp(0, 0), setg(0, 0, 0); 
	}
	void _Init(E **ppGetBegin, E **ppGetNext, int *pGetCount, E **ppPutBegin, E **ppPutNext, int *pPutCount)
	{
		m_ppGetBegin = ppGetBegin, m_ppPutBegin = ppPutBegin;
		m_ppGetNext = ppGetNext, m_ppPutNext = ppPutNext;
		m_pGetCount = pGetCount, m_pPutCount = pPutCount; 
	}
	virtual int_type overflow(int_type = CharTraits::eof())
	{
		return (CharTraits::eof()); 
	}
	virtual int_type pbackfail(int_type = CharTraits::eof())
	{
		return (CharTraits::eof()); 
	}
	virtual int showmanyc()
	{
		return (0); 
	}

	virtual int_type underflow()
	{
		return (CharTraits::eof()); 
	}

	virtual int_type uflow()
	{
		return (CharTraits::eq_int_type(CharTraits::eof(), underflow()) 
			? CharTraits::eof() : CharTraits::to_int_type(*_Gninc())); 
	}

	//Gets up to n characters from the input sequence and stores them in the array pointed by s.
	virtual streamsize xsgetn(E * str, streamsize count)
	{
		int_type ch;
		streamsize nGetCount, nSize;
		for (nSize = 0; 0 < count; )
			if (gptr() != 0 && 0 < (nGetCount = egptr() - gptr()))
			{
				if (count < nGetCount)
					nGetCount = count;
				CharTraits::copy(str, gptr(), nGetCount);
				str += nGetCount, nSize += nGetCount, count -= nGetCount, gbump(nGetCount); 
			}
			else if (CharTraits::eq_int_type(CharTraits::eof(), ch = uflow()))
				break;
			else
				*str++ = CharTraits::to_char_type(ch), ++nSize, --count;
		return (nSize); 
	}

	//Writes up to n characters from the array pointed by s to the output sequence controlled by the stream buffer.
	virtual streamsize xsputn(const E *str, streamsize count)
	{
		streamsize nPutCount, nSize;
		for (nSize = 0; 0 < count; )
			if (pptr() != 0 && 0 < (nPutCount = epptr() - pptr()))
			{
				if (count < nPutCount)
					nPutCount = count;
				CharTraits::copy(pptr(), str, nPutCount);
				str += nPutCount, nSize += nPutCount, count -= nPutCount, pbump(nPutCount); 
			}
			else if (CharTraits::eq_int_type(CharTraits::eof(), overflow(CharTraits::to_int_type(*str))))
				break;
			else
				++str, ++nSize, --count;
		return (nSize); 
	}

	virtual pos_type seekoff(off_type, ios_base::seekdir,
		ios_base::openmode = ios_base::in | ios_base::out)
	{
		return (streampos(_BADOFF)); 
	}

	virtual pos_type seekpos(pos_type,
		ios_base::openmode = ios_base::in | ios_base::out)
	{
		return (streampos(_BADOFF)); 
	}
	virtual MyType *setbuf(E *, streamsize)
	{
		return (this); 
	}
	virtual int sync()
	{
		return (0); 
	}
	virtual void imbue(const locale& _Loc)
	{}
private:
	E *m_pGetBegin, *m_pPutBegin;
	E **m_ppGetBegin, **m_ppPutBegin;
	E *m_pGetNext, *m_pPutNext;
	E **m_ppGetNext, **m_ppPutNext;
	int m_nGetCount, m_nPutCount;
	int *m_pGetCount, *m_pPutCount;
	locale m_locale;
};

#ifdef _DLL
#pragma warning(disable:4231) /* the extern before template is a non-standard extension */
extern template class _CRTIMP basic_streambuf<char, char_traits<char> >;
extern template class _CRTIMP basic_streambuf<wchar_t, char_traits<wchar_t> >;
#pragma warning(default:4231) /* restore previous warning */
#endif		// _DLL

_STD_END

#endif /* _STREAMBUF_ */

/*
 * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
