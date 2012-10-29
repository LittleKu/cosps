// BitmapAvi.h: interface for the CBitmapAvi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPAVI_H__FF83AB2A_9868_48BF_A80E_A56AE6B0D781__INCLUDED_)
#define AFX_BITMAPAVI_H__FF83AB2A_9868_48BF_A80E_A56AE6B0D781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>

class CBitmapAvi  
{
public:
	CBitmapAvi(LPCTSTR lpszFileName, DWORD dwCodec = 0, DWORD dwFrameRate = 1, LPCTSTR lpszWaveFile = NULL);
	virtual ~CBitmapAvi();
private:
	void ReleaseMemory();
	void SetErrorMessage(LPCTSTR lpszErrorMessage);
	HRESULT SetStreamFormat(DWORD dwWidth, DWORD dwHeight, WORD wBitsPerPixel);
	int ParseWaveHeader();
public:
	HRESULT Init(DWORD dwWidth, DWORD dwHeight, WORD wBitsPerPixel);
	HRESULT AddFrame(LPVOID lpBuffer, DWORD cbBuffer);
	HRESULT AddFrame(LPCTSTR lpszBitmapFile);
	HRESULT Done();

	LPCTSTR GetLastErrorMessage() const {	return m_szErrMsg;	}
private:
	PAVIFILE			m_pAviFile;
	PAVISTREAM			m_pAviStream;
	PAVISTREAM			m_pAviCompressedStream;
	LONG				m_lSample;	// Keeps track of the current Frame Index

	CString				m_szFileName;
	DWORD				m_dwFrameRate;				// Frames Per Second Rate (FPS)
	DWORD				m_dwFCCHandler;				// Video Codec FourCC

	BITMAPINFOHEADER	m_bmpInfoHdr;	//First frame format

	TCHAR m_szErrMsg[MAX_PATH];		// Holds the Last Error Message, if any

	TCHAR				m_szWaveFile[MAX_PATH];
	WAVEFORMATEX		m_wfx;
	FILE*				m_waveFile;
	int					m_nDataLen;
	PAVISTREAM			m_pAudioStream;
};

#endif // !defined(AFX_BITMAPAVI_H__FF83AB2A_9868_48BF_A80E_A56AE6B0D781__INCLUDED_)
