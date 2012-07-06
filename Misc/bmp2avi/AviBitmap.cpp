// AviBitmap.cpp: implementation of the CAviBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bmp2avi.h"
#include "AviBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAviBitmap::CAviBitmap(LPCTSTR lpszFileName)
{
	AVIFileInit();

	m_pAviFile = NULL;
	m_pAviStream = NULL;
	m_pGetFrame = NULL;

	m_lFirstSample = 0;
	m_lSampleCount = 0;

	m_szFileName = lpszFileName;
}

CAviBitmap::~CAviBitmap()
{
	ReleaseMemory();

	AVIFileExit();
}

void CAviBitmap::ReleaseMemory()
{
	if(m_pGetFrame != NULL)
	{
		AVIStreamGetFrameClose(m_pGetFrame);
		m_pGetFrame = NULL;
	}

	if(m_pAviStream != NULL)
	{
		AVIStreamRelease(m_pAviStream);
		m_pAviStream = NULL;
	}

	if(m_pAviFile != NULL)
	{
		AVIFileRelease(m_pAviFile);
		m_pAviFile = NULL;
	}

	m_lFirstSample = 0;
	m_lSampleCount = 0;
}

HRESULT CAviBitmap::Init()
{
	HRESULT hr = E_FAIL;
	
	do 
	{
		//Open file
		hr = AVIFileOpen(&m_pAviFile, m_szFileName, OF_READ, NULL);
		if(hr != S_OK)
		{
			m_szLastErrorMsg.Format(_T("Unable to Open the Movie File"));
			break;
		}

		//Get video stream
		hr = AVIFileGetStream(m_pAviFile, &m_pAviStream, streamtypeVIDEO /*video stream*/, 0 /*first stream*/);
		if(hr != S_OK)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the video stream"));
			break;
		}

		//Set the result to Fail
		hr = E_FAIL;

		//Get the GETFRAME handle
		m_pGetFrame = AVIStreamGetFrameOpen(m_pAviStream, NULL);
		if(m_pGetFrame == NULL)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the GETFRAME handle"));
			break;
		}

		//Get the 1st sample
		m_lFirstSample = AVIStreamStart(m_pAviStream);
		if(m_lFirstSample == -1)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the first sample"));
			break;
		}

		//Get the total sample count
		m_lSampleCount = AVIStreamLength(m_pAviStream);
		if(m_lSampleCount == -1)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the sample count"));
			break;
		}

		//Done
		hr = S_OK;

	} while (FALSE);
	
	if(hr != S_OK)
	{
		ReleaseMemory();
	}

	return hr;
}

HRESULT CAviBitmap::GetAllFrames()
{
	if(m_pGetFrame == NULL)
	{
		m_szLastErrorMsg.Format(_T("Not initialized yet"));
		return E_FAIL;
	}
	HRESULT hr = S_OK;

	BITMAPINFOHEADER bmpInfoHdr;
	LONG lpcbFormat = sizeof(bmpInfoHdr);

	BYTE* lpDib = NULL;
	BYTE* lpBuffer = NULL;

	int nPos = 0;
	for(nPos = 0; nPos < m_lSampleCount; nPos++)
	{
		//Get the frame format
		hr = AVIStreamReadFormat(m_pAviStream, nPos, &bmpInfoHdr,&lpcbFormat);
		if(hr != S_OK)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the sample format: %d"), nPos);
			break;
		}

		//Get the frame data
		lpBuffer = new BYTE[bmpInfoHdr.biSizeImage];
		hr = AVIStreamRead(m_pAviStream, nPos, 1, lpBuffer, bmpInfoHdr.biSizeImage, NULL, NULL);
		if(hr != S_OK)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the sample data: %d"), nPos);
			break;
		}

		//Get the frame at nPos
		lpDib = (BYTE*)AVIStreamGetFrame(m_pGetFrame, nPos);
		if(lpDib == NULL)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the sample: %d"), nPos);
			hr = E_FAIL;
			break;
		}

		//compare the data retrieved in 2 ways
		if(memcmp(lpBuffer, lpDib + sizeof(BITMAPINFOHEADER), bmpInfoHdr.biSizeImage) != 0)
		{
			m_szLastErrorMsg.Format(_T("not equals: %d"), nPos);
			hr = E_FAIL;
			break;
		}

		delete [] lpBuffer;
		lpBuffer = NULL;

		//Done
	}

	if(lpBuffer != NULL)
	{
		delete [] lpBuffer;
		lpBuffer = NULL;
	}

	ReleaseMemory();

	return hr;
}

HRESULT CAviBitmap::GetFrame(int nFrame, LPVOID lpBuffer, DWORD cbBuffer)
{
	HRESULT hr = E_FAIL;

	return hr;
}
