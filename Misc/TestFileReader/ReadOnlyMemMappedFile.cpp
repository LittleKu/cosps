// ReadOnlyMemMappedFile.cpp: implementation of the CReadOnlyMemMappedFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReadOnlyMemMappedFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReadOnlyMemMappedFile::CReadOnlyMemMappedFile(LPCTSTR pszFileName) : 
    m_pBuf(NULL)
{
    // define a threshold beyond which a file is mapped rather than read
    // into memory
    const int threshold = 64 * 1024 - 1;

    m_hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, 0, NULL);
    if (m_hFile == INVALID_HANDLE_VALUE) 
        throw "Could not open file";

    DWORD dwSizeLow, dwSizeHigh;
    dwSizeLow = ::GetFileSize(m_hFile, &dwSizeHigh);
    ASSERT(dwSizeHigh == 0); // not actually built for use with huge files
    m_iFileSize = dwSizeLow;
    if (dwSizeLow > threshold)
    {
        // use memory map
        m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READONLY,
            0, 0, NULL);
        ASSERT(m_hMap != INVALID_HANDLE_VALUE);

        // map the file and store the memory pointer
        m_pBuf = (BYTE *)MapViewOfFileEx(m_hMap, FILE_MAP_READ, 0, 0, 0, 0);
    }
    else
    {
        // <= threshold, read the entire file into memory
        m_pBuf = new BYTE[m_iFileSize];
        DWORD dwRead;
        ReadFile(m_hFile, m_pBuf, m_iFileSize, &dwRead, NULL);
        ASSERT(dwRead == m_iFileSize);
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;  // not using the file any more
        m_hMap  = INVALID_HANDLE_VALUE;  // not using a memory map
    }
}

CReadOnlyMemMappedFile::~CReadOnlyMemMappedFile()
{
    if (m_hMap != INVALID_HANDLE_VALUE)
    {
		if (m_pBuf != NULL)
		{
			BOOL bRet = UnmapViewOfFile(m_pBuf);
			ASSERT(bRet);
		}
        CloseHandle(m_hMap);
    }
    else
    {
        // if it wasn't a memory map, it means we have a regular buffer
        delete [] m_pBuf;
    }
    if (m_hFile)
    {
        CloseHandle(m_hFile);
    }
}
