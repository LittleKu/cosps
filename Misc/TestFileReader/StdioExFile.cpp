#include "stdafx.h"
#include "StdioExFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CStdioExFile::CStdioExFile() : CStdioFile(), m_nLineLimit(1024)
{
}
CStdioExFile::CStdioExFile(FILE* pOpenStream) : CStdioFile(pOpenStream), m_nLineLimit(1024)
{
}
CStdioExFile::CStdioExFile(LPCTSTR lpszFileName, UINT nOpenFlags)
 : CStdioFile(lpszFileName, nOpenFlags), m_nLineLimit(1024)
{
}
CStdioExFile::~CStdioExFile()
{
}

void CStdioExFile::SetLineLimit(UINT nLimit)
{
	m_nLineLimit = nLimit;
}
UINT CStdioExFile::GetLineLimit() const
{
	return m_nLineLimit;
}

BOOL CStdioExFile::ReadLine(CString& rString)
{	
	rString = &afxChNil;    // empty string without deallocating

	int nMaxSize = 128;
	LPTSTR lpsz = rString.GetBuffer(nMaxSize);
	LPTSTR lpszResult;
	int nLen = 0;

	//1. Read data in the limit of m_nLineLimit
	while(true)
	{
		lpszResult = CStdioFile::ReadString(lpsz, nMaxSize + 1);
		rString.ReleaseBuffer();
		
		// if string is read completely or EOF
		if(lpszResult == NULL || (nLen = lstrlen(lpsz)) < nMaxSize || lpsz[nLen-1] == '\n')
			break;

		// if string is read beyond the line limit length
		// [CAUTION]: use rString.GetLength() other than nLen
		if(rString.GetLength() >= m_nLineLimit)
			break;

		nLen = rString.GetLength();

		// x2 to speed up to reach the required data.
 		nMaxSize <<= 1;
		if(nLen + nMaxSize > m_nLineLimit)
		{
			nMaxSize = m_nLineLimit - nLen;
		}
		lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
	}
	
	//1.1 NULL is returned to indicate an error or an end-of-file condition.
	if(lpszResult == NULL)
	{
		return FALSE;
	}
	//1.2 if string is read completely or EOF
	//[CAUTION]: use nLen other than rString.GetLength()
	if( nLen < nMaxSize || lpsz[nLen-1] == '\n' )
	{
		// remove '\n' from end of string if present
		if (nLen != 0 && lpsz[nLen-1] == '\n')
			rString.GetBufferSetLength(rString.GetLength()-1);
		return TRUE;
	}

	//1.3. Check if read enough data to finish
	lpsz = rString.GetBuffer(0);
	nLen = rString.GetLength();
	if(nLen >= m_nLineLimit && lpsz[nLen-1] == '\n')
	{
		// remove '\n' from end of string if present
		rString.GetBufferSetLength(nLen-1);
		return TRUE;
	}

	
	//2. This line doesn't end yet, so we still need to read the left over but drop it away.
	CString szTempStr = &afxChNil;
	nMaxSize = 1024;
	lpsz = szTempStr.GetBuffer(nMaxSize);
	while(true)
	{
		lpszResult = CStdioFile::ReadString(lpsz, nMaxSize + 1);
		// if string is read completely or EOF
		if(lpszResult == NULL || (nLen = lstrlen(lpsz)) < nMaxSize || lpsz[nLen-1] == '\n')
		{
			break;
		}
	}
	szTempStr.ReleaseBuffer();
	
	return TRUE;
}