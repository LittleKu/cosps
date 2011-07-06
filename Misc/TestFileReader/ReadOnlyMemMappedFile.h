/***************************************************************************/
/* NOTE:                                                                   */
/* This document is copyright (c) by Oz Solomon, and is bound by the MIT   */
/* open source license (www.opensource.org/licenses/mit-license.html).     */
/* See License.txt for more information.                                   */
/***************************************************************************/

#if !defined(AFX_READONLYMEMMAPPEDFILE_H__48F017F8_94BC_438F_935F_32A5D99E2988__INCLUDED_)
#define AFX_READONLYMEMMAPPEDFILE_H__48F017F8_94BC_438F_935F_32A5D99E2988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReadOnlyMemMappedFile  
{
protected:
    HANDLE m_hFile;
    HANDLE m_hMap;
    BYTE *m_pBuf;
    DWORD m_iFileSize;

public:
	CReadOnlyMemMappedFile(LPCTSTR pszFileName);
	virtual ~CReadOnlyMemMappedFile();

    DWORD GetFileSize() const { return m_iFileSize; }
    const BYTE *GetFileContents() const { return m_pBuf; }
};

#endif // !defined(AFX_READONLYMEMMAPPEDFILE_H__48F017F8_94BC_438F_935F_32A5D99E2988__INCLUDED_)
