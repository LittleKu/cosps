// CircleBuffer.h: interface for the CCircleBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCLEBUFFER_H__B73329A9_6FBD_4968_9E5F_6F6D5B21EAC1__INCLUDED_)
#define AFX_CIRCLEBUFFER_H__B73329A9_6FBD_4968_9E5F_6F6D5B21EAC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCircleBuffer  
{
public:
	CCircleBuffer(int nCache, int nMaxReq, int nItemSize = 1);
	virtual ~CCircleBuffer();

	/* return value should be equal to or less than m_nMaxReq*/
	int GetData(char* buffer, int nItemSize);
	int Skip(int nItemSize, int nCount);
	int AddData(char* buffer, int nItemSize, int nCount);
	char* GetBuffer(int nSize);
	int ReleaseBuffer(int nReqSize, int nActualSize);
	int GetSize();
private:
	/*
	Init status: m_nBegin = m_nEnd = m_nEnd0 = 0;
	1. m_nEnd >= m_nBegin;
	2. m_nEnd == m_nEnd0 || (m_nBegin - m_nEnd0) >= m_nMaxReq;
	*/
	int m_nBegin;
	int m_nEnd;
	int m_nEnd0;		/* the last valid byte address */
	
	int m_nCache;		/* how many bytes should keep */
	int m_nMaxReq;		/* max value user will require*/
	int m_nItemSize;	/* Item size in bytes */

	int m_nLockBegin;	/* the locked buffer begin point */

	int m_nCap;			/* the buffer capacity */
	char* m_buffer;		/* the buffer */
};

#endif // !defined(AFX_CIRCLEBUFFER_H__B73329A9_6FBD_4968_9E5F_6F6D5B21EAC1__INCLUDED_)
