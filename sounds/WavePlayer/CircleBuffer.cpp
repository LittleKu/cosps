// CircleBuffer.cpp: implementation of the CCircleBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "CircleBuffer.h"
#include <assert.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircleBuffer::CCircleBuffer(int nCache, int nMaxReq, int nItemSize) : m_nBegin(0), m_nEnd(0), m_nEnd0(0), m_nCache(0), 
 m_nMaxReq(0), m_nItemSize(0), m_nLockBegin(-1), m_nCap(0), m_buffer(0)
{
	if(nItemSize <= 0)
	{
		return;
	}
	assert(nCache > 0 && nMaxReq > 0);
	m_nCache = nCache;
	m_nMaxReq = nMaxReq;
	m_nItemSize = nItemSize;

	m_nCap = m_nCache + 2 * m_nMaxReq;

	m_buffer = new char[m_nCap * m_nItemSize];
}

CCircleBuffer::~CCircleBuffer()
{
	if(m_buffer != 0)
	{
		delete [] m_buffer;
		m_buffer = 0;
	}
}

int CCircleBuffer::GetData(char* buffer, int nItemSize)
{
	assert(m_nItemSize % nItemSize == 0);
	int nSize = 0, nDataCount;
	//continuous buffer space
	if(m_nEnd == m_nEnd0)
	{
		assert(m_nEnd >= m_nBegin);
		
		nSize = m_nEnd - m_nBegin;

		assert(nSize % m_nItemSize == 0);
		if(nSize <= m_nCache * m_nItemSize)
		{
			return 0;
		}

		nDataCount = nSize - m_nCache * m_nItemSize;
		memcpy(buffer, m_buffer + m_nBegin, nDataCount);
		m_nBegin += nDataCount;
	}
	else
	{
		assert(m_nEnd >= m_nBegin);
		assert(m_nEnd0 != 0);

		nSize = m_nEnd - m_nBegin;
		nSize += m_nEnd0;	

		assert(nSize % m_nItemSize == 0);
		if(nSize <= m_nCache * m_nItemSize)
		{
			return 0;
		}

		nDataCount = nSize - m_nCache * m_nItemSize;

		int nRightSize = m_nEnd - m_nBegin;
		//there is enough data in the right side
		if(nRightSize > nDataCount)
		{
			memcpy(buffer, m_buffer + m_nBegin, nDataCount);
			m_nBegin += nDataCount;
		}
		//no enough data
		else
		{
			memcpy(buffer, m_buffer + m_nBegin, nRightSize);
			memcpy(buffer + nRightSize, m_buffer, nDataCount - nRightSize);

			m_nBegin = nDataCount - nRightSize;
			m_nEnd = m_nEnd0;
		}
	}
	
	assert(nDataCount % m_nItemSize == 0);
	nDataCount /= m_nItemSize;
	
	return nDataCount;
}

int CCircleBuffer::Skip(int nItemSize, int nCount)
{
	int nBefore = GetSize(), nAfter;
	assert(m_nItemSize % nItemSize == 0);

	int nSize = 0, nSkip = nItemSize * nCount;
	//continuous buffer space
	if(m_nEnd == m_nEnd0)
	{
		assert(m_nEnd >= m_nBegin);
		
		nSize = m_nEnd - m_nBegin;
		
		assert(nSize % m_nItemSize == 0);
		//skip all
		if(nSize <= nSkip)
		{
			m_nBegin = m_nEnd = m_nEnd0 = 0;
			return nSize / nItemSize;
		}

		m_nBegin += nSkip;
	}
	else
	{
		assert(m_nEnd >= m_nBegin);
		assert(m_nEnd0 != 0);
		
		nSize = m_nEnd - m_nBegin;
		nSize += m_nEnd0;	
		
		assert(nSize % m_nItemSize == 0);
		//skip all
		if(nSize <= nSkip)
		{
			m_nBegin = m_nEnd = m_nEnd0 = 0;
			return nSize / nItemSize;
		}
		
		int nRightSize = m_nEnd - m_nBegin;
		//there is enough data in the right side
		if(nRightSize > nSkip)
		{
			m_nBegin += nSkip;
		}
		//no enough data
		else
		{		
			m_nBegin = nSkip - nRightSize;
			m_nEnd = m_nEnd0;
		}
	}

	nAfter = GetSize();
	assert( (nBefore - nAfter) * m_nItemSize == nItemSize * nCount );
	
	return nCount;
}

int CCircleBuffer::AddData(char* buffer, int nItemSize, int nCount)
{
	int n = nItemSize * nCount / m_nItemSize;

	char* dst = GetBuffer(n);
	if(dst == 0)
	{
		return 0;
	}
	
	memcpy(dst, buffer, nItemSize * nCount);
	int ret = ReleaseBuffer(n, n);
	return ret;
}

char* CCircleBuffer::GetBuffer(int nSize)
{
	assert(nSize <= m_nMaxReq && m_nLockBegin < 0);

	if(nSize > m_nMaxReq || m_nLockBegin >= 0)
	{
		return 0; 
	}

	char* buffer = 0;
	if(m_nEnd == m_nEnd0)
	{
		int nSpaceR = m_nCap * m_nItemSize - m_nEnd;

//		assert(nSpaceR % m_nItemSize == 0);
		if( (nSpaceR / m_nItemSize) >= nSize )
		{
			buffer = m_buffer + m_nEnd;
			m_nLockBegin = m_nEnd;
		}
		else
		{
			assert((m_nBegin % m_nItemSize == 0) && (m_nBegin / m_nItemSize) >= nSize);

			buffer = m_buffer;
			m_nLockBegin = 0;
		}
	}
	else
	{
		int nSpaceL = m_nBegin - m_nEnd0 + 2;
		assert(nSpaceL % m_nItemSize == 0 && (nSpaceL / m_nItemSize) >= nSize);

		buffer = m_buffer + m_nEnd0;
		m_nLockBegin = m_nEnd0;
	}

	return buffer;
}

int CCircleBuffer::ReleaseBuffer(int nReqSize, int nActualSize)
{
	assert(nReqSize <= m_nMaxReq && m_nLockBegin >= 0 && nActualSize <= nReqSize);
	if(nReqSize > m_nMaxReq || m_nLockBegin < 0)
	{
		m_nLockBegin = -1;
		return 0;
	}

	//touch nothing
	if(nActualSize <= 0)
	{
		m_nLockBegin = -1;
		return 0;
	}

	if(m_nEnd == m_nEnd0)
	{
		int nSpaceR = m_nCap * m_nItemSize - m_nEnd;
		
		if( (nSpaceR / m_nItemSize) >= nReqSize )
		{
			assert(m_nLockBegin == m_nEnd);

			m_nEnd += nActualSize * m_nItemSize;
			m_nEnd0 = m_nEnd;
		}
		else
		{
			assert((m_nBegin % m_nItemSize == 0) && (m_nBegin / m_nItemSize) >= nReqSize);
			assert(m_nLockBegin == 0);

			m_nEnd0 = nActualSize * m_nItemSize;
		}
	}
	else
	{
		int nSpaceL = m_nBegin - m_nEnd0 + 2;
		assert(nSpaceL % m_nItemSize == 0 && (nSpaceL / m_nItemSize) >= nReqSize);
		assert(m_nLockBegin == m_nEnd0);

		m_nEnd0 += nActualSize * m_nItemSize;
	}

	m_nLockBegin = -1;

	return nActualSize;
}

int CCircleBuffer::GetSize()
{
	int nSize = 0;
	//continuous buffer space
	if(m_nEnd == m_nEnd0)
	{
		assert(m_nEnd >= m_nBegin);

		nSize = m_nEnd - m_nBegin;
	}
	else
	{
		assert(m_nEnd >= m_nBegin);
		assert(m_nEnd0 != 0);

		nSize = m_nEnd - m_nBegin;
		nSize += m_nEnd0;

		assert(nSize <= (m_nCache + m_nMaxReq) * m_nItemSize );
	}

	assert(nSize % m_nItemSize == 0);
	nSize /= m_nItemSize;

	assert(nSize <= m_nCap);

	return nSize;
}
