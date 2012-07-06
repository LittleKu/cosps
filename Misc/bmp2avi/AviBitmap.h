// AviBitmap.h: interface for the CAviBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIBITMAP_H__F5A465A1_559E_4798_A460_3CD63DFF019D__INCLUDED_)
#define AFX_AVIBITMAP_H__F5A465A1_559E_4798_A460_3CD63DFF019D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>

class CAviBitmap  
{
public:
	CAviBitmap(LPCTSTR lpszFileName);
	virtual ~CAviBitmap();

	HRESULT Init();
	HRESULT GetFrame(int nFrame, LPVOID lpBuffer, DWORD cbBuffer);
	HRESULT GetAllFrames();

	LPCTSTR GetLastErrorMessage() const {	return m_szLastErrorMsg;	}

private:
	void ReleaseMemory();

private:
	PAVIFILE			m_pAviFile;
	PAVISTREAM			m_pAviStream;
	PGETFRAME			m_pGetFrame;

	LONG				m_lFirstSample;
	LONG				m_lSampleCount;

	CString m_szFileName;
	CString m_szLastErrorMsg;

};

#endif // !defined(AFX_AVIBITMAP_H__F5A465A1_559E_4798_A460_3CD63DFF019D__INCLUDED_)
