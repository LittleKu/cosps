// BitmapAvi.cpp: implementation of the CBitmapAvi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BitmapAvi.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifndef DWORD_ALIGNED_BITS
#define DWORD_ALIGNED_BITS(x) ((((x) + 31) & ~31) >> 3)
#endif

CBitmapAvi::CBitmapAvi(LPCTSTR lpszFileName, DWORD dwCodec, DWORD dwFrameRate)
{
	AVIFileInit();

	m_szFileName = lpszFileName;

	m_pAviFile = NULL;
	m_pAviStream = NULL;
	m_pAviCompressedStream = NULL;
	m_lSample = 0;

	ZeroMemory(m_szErrMsg, sizeof(m_szErrMsg));

	m_dwFCCHandler = dwCodec;
	m_dwFrameRate = dwFrameRate;

	ZeroMemory(&m_bmpInfoHdr, sizeof(m_bmpInfoHdr));
}

CBitmapAvi::~CBitmapAvi()
{
	ReleaseMemory();

	AVIFileExit();
}

void CBitmapAvi::ReleaseMemory()
{
	if(m_pAviCompressedStream)
	{
		AVIStreamRelease(m_pAviCompressedStream);
		m_pAviCompressedStream = NULL;
	}
	if(m_pAviStream)
	{
		AVIStreamRelease(m_pAviStream);
		m_pAviStream = NULL;
	}
	if(m_pAviFile)
	{
		AVIFileRelease(m_pAviFile);
		m_pAviFile = NULL;
	}

	m_szFileName.Empty();
	ZeroMemory(&m_bmpInfoHdr, sizeof(m_bmpInfoHdr));
}

void CBitmapAvi::SetErrorMessage(LPCTSTR lpszErrorMessage)
{
	_tcsncpy(m_szErrMsg, lpszErrorMessage, MAX_PATH - 1);
}

HRESULT CBitmapAvi::SetStreamFormat(DWORD dwWidth, DWORD dwHeight, WORD wBitsPerPixel)
{
	if(m_pAviCompressedStream == NULL)
	{
		SetErrorMessage("The avi stream not initialized yet.");
		return E_FAIL;
	}
	//4. Set stream format
	BITMAPINFOHEADER bmpInfoHdr;
	ZeroMemory(&bmpInfoHdr, sizeof(BITMAPINFOHEADER));
	bmpInfoHdr.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHdr.biWidth = dwWidth;
	bmpInfoHdr.biHeight = dwHeight;
	bmpInfoHdr.biPlanes = 1;
	bmpInfoHdr.biBitCount = wBitsPerPixel;
	bmpInfoHdr.biCompression = BI_RGB;
	bmpInfoHdr.biSizeImage = DWORD_ALIGNED_BITS(dwWidth * wBitsPerPixel) * dwHeight;
	
	HRESULT hr = AVIStreamSetFormat(m_pAviCompressedStream, m_lSample, (LPVOID)&bmpInfoHdr, bmpInfoHdr.biSize);
	if(hr != S_OK)
	{
		// One reason this error might occur is if your bitmap does not meet the Codec requirements.
		// For example, 
		//   your bitmap is 32bpp while the Codec supports only 16 or 24 bpp; Or
		//   your bitmap is 274 * 258 size, while the Codec supports only sizes that are powers of 2; etc...
		// Possible solution to avoid this is: make your bitmap suit the codec requirements,
		// or Choose a codec that is suitable for your bitmap.
		SetErrorMessage("Unable to Set Video Stream Format");
	}
	else
	{
		//copy the format information
		memcpy(&m_bmpInfoHdr, &bmpInfoHdr, sizeof(BITMAPINFOHEADER));
	}
	return hr;
}

HRESULT CBitmapAvi::Init(DWORD dwWidth, DWORD dwHeight, WORD wBitsPerPixel)
{
	HRESULT hr = S_OK;

	do 
	{
		if(::PathFileExists(m_szFileName))
		{
			::DeleteFile(m_szFileName);
		}
		//1. Create AVI File
		hr = AVIFileOpen(&m_pAviFile, m_szFileName, OF_CREATE|OF_WRITE, NULL);
		if(hr != S_OK)
		{
			SetErrorMessage(_T("Unable to Create the Movie File"));
			break;
		}

		//2. Create stream
		AVISTREAMINFO	aviStreamInfo;
		ZeroMemory(&aviStreamInfo, sizeof(AVISTREAMINFO));
		aviStreamInfo.fccType		= streamtypeVIDEO;
		aviStreamInfo.fccHandler	= m_dwFCCHandler;
		aviStreamInfo.dwScale		= 1;
		aviStreamInfo.dwRate		= m_dwFrameRate;	// Frames Per Second;
		aviStreamInfo.dwQuality		= -1;				// Default Quality
		aviStreamInfo.dwSuggestedBufferSize = DWORD_ALIGNED_BITS(dwWidth * wBitsPerPixel) * dwHeight;
		SetRect(&aviStreamInfo.rcFrame, 0, 0, dwWidth, dwHeight);
		_tcscpy(aviStreamInfo.szName, _T("Video Stream"));

		hr = AVIFileCreateStream(m_pAviFile, &m_pAviStream, &aviStreamInfo);
		if(hr != S_OK)
		{
			SetErrorMessage(_T("Unable to Create the Movie File"));
			break;
		}

		//3. Compress stream
		AVICOMPRESSOPTIONS m_AviCompressOptions;
		ZeroMemory(&m_AviCompressOptions, sizeof(AVICOMPRESSOPTIONS));
		m_AviCompressOptions.fccType = streamtypeVIDEO;
		m_AviCompressOptions.fccHandler = aviStreamInfo.fccHandler;
		m_AviCompressOptions.dwFlags = AVICOMPRESSF_KEYFRAMES|AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
		m_AviCompressOptions.dwKeyFrameEvery = 1;
		//m_AviCompressOptions.dwBytesPerSecond=1000/8;
		//m_AviCompressOptions.dwQuality=100;
		hr = AVIMakeCompressedStream(&m_pAviCompressedStream, m_pAviStream, &m_AviCompressOptions, NULL);
		if(hr != S_OK)
		{
			// One reason this error might occur is if you are using a Codec that is not 
			// available on your system. Check the mmioFOURCC() code you are using and make
			// sure you have that codec installed properly on your machine.
			SetErrorMessage("Unable to Create Compressed Stream: Check your CODEC options");
			break;
		}

		//4. Set stream format only once
		hr = SetStreamFormat(dwWidth, dwHeight, wBitsPerPixel);
		if(hr != S_OK)
		{
			break;
		}

		//Done

	} while (FALSE);

	//Clean up
	if(hr != S_OK)
	{
		//TODO some cleaning
		ReleaseMemory();
	}
	return hr;
}

HRESULT CBitmapAvi::AddFrame(LPVOID lpBuffer, DWORD cbBuffer)
{
	HRESULT hr = AVIStreamWrite(m_pAviCompressedStream, m_lSample++, 1, lpBuffer, cbBuffer, 0, NULL, NULL);
	if(hr != S_OK)
	{
		SetErrorMessage(_T("AddFrame: Unable to Write Video Stream to the output Movie File"));
		return hr;
	}
	return hr;
}

HRESULT CBitmapAvi::AddFrame(LPCTSTR lpszBitmapFile)
{
	//Read bitmap file
	FILE *fp = fopen(lpszBitmapFile, "rb");

	BYTE* lpBuffer = NULL;
	HRESULT hr = E_FAIL;
	do 
	{
		//1. check if the file opened
		if(fp == NULL)
		{
			TCHAR szBuffer[MAX_PATH];
			_sntprintf(szBuffer, MAX_PATH, _T("AddFrame: Unable to Open bitmap file: %s"), lpszBitmapFile);
			
			SetErrorMessage(szBuffer);
			break;
		}

		//2. Read file header
		BITMAPFILEHEADER bmpFileHdr;
		if(fread(&bmpFileHdr, sizeof(BITMAPFILEHEADER), 1, fp) != 1)
		{
			SetErrorMessage(_T("Read bitmap file header failed."));
			break;
		}

		//3. Read info header
		BITMAPINFOHEADER bmpInfoHdr;
		if(fread(&bmpInfoHdr, sizeof(BITMAPINFOHEADER), 1, fp) != 1)
		{
			SetErrorMessage(_T("Read bitmap info header failed."));
			break;
		}

		//Init if needed
		if(m_pAviCompressedStream == NULL && Init(bmpInfoHdr.biWidth, bmpInfoHdr.biHeight, bmpInfoHdr.biBitCount) != S_OK)
		{
			break;
		}

		//Check if the format is same to the first frame
		if(m_bmpInfoHdr.biWidth != bmpInfoHdr.biWidth || m_bmpInfoHdr.biHeight != bmpInfoHdr.biHeight
			|| m_bmpInfoHdr.biBitCount != bmpInfoHdr.biBitCount)
		{
			TCHAR szBuffer[MAX_PATH];
			_sntprintf(szBuffer, MAX_PATH, _T("Check frame format failed. file=%s,w=%u,h=%u,b=%u."), 
				lpszBitmapFile, bmpInfoHdr.biWidth, bmpInfoHdr.biHeight, (DWORD)bmpInfoHdr.biBitCount);

			SetErrorMessage(szBuffer);
			break;
		}

		//4. Read bitmap data
		DWORD dwBiSizeImage = DWORD_ALIGNED_BITS(bmpInfoHdr.biWidth * bmpInfoHdr.biBitCount) * bmpInfoHdr.biHeight;
		ASSERT(dwBiSizeImage == bmpInfoHdr.biSizeImage);

		lpBuffer = new BYTE[dwBiSizeImage];
		if(fread(lpBuffer, 1, dwBiSizeImage, fp) != dwBiSizeImage)
		{
			SetErrorMessage(_T("Read bitmap data failed."));
			break;
		}

		if(AddFrame(lpBuffer, dwBiSizeImage) != S_OK)
		{
			break;
		}

		hr = S_OK;

	} while (FALSE);

	if(fp != NULL)
	{
		fclose(fp);
	}
	if(lpBuffer != NULL)
	{
		delete [] lpBuffer;
		lpBuffer = NULL;
	}

	//Clean up
	if(hr != S_OK)
	{
		ReleaseMemory();
	}

	return hr;
}
