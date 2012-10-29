// BitmapAvi.cpp: implementation of the CBitmapAvi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BitmapAvi.h"
#include <Shlwapi.h>
#include "cflbase/byte_utils.h"

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

CBitmapAvi::CBitmapAvi(LPCTSTR lpszFileName, DWORD dwCodec, DWORD dwFrameRate, LPCTSTR lpszWaveFile)
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

	if(lpszWaveFile == NULL)
	{
		ZeroMemory(m_szWaveFile, sizeof(m_szWaveFile));
	}
	else
	{
		_tcscpy(m_szWaveFile, lpszWaveFile);
	}
	m_pAudioStream = NULL;
	m_nDataLen = 0;
	ZeroMemory(&m_wfx, sizeof(m_wfx));
	m_waveFile = NULL;
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

	if(m_pAudioStream)
	{
		AVIStreamRelease(m_pAudioStream);
		m_pAudioStream = NULL;
	}

	if(m_waveFile)
	{
		fclose(m_waveFile);
		m_waveFile = NULL;
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

		//5. Open Audio Stream if needed
		LPCTSTR lpszWaveFile = ((_tcslen(m_szWaveFile) > 0) ? m_szWaveFile : NULL);
		if(lpszWaveFile == NULL)
		{
			break;
		}
		m_waveFile = _tfopen(lpszWaveFile, _T("rb"));
		if(m_waveFile == NULL)
		{
			_stprintf(m_szErrMsg, _T("Failed to open wave file %s"), lpszWaveFile);
			break;
		}

		if(ParseWaveHeader() != 0)
		{
			fclose(m_waveFile);
			m_waveFile = NULL;

			break;
		}

		//6. Create Audio Stream
		ZeroMemory(&aviStreamInfo, sizeof(AVISTREAMINFO));
		aviStreamInfo.fccType		= streamtypeAUDIO;
		aviStreamInfo.fccHandler	= m_dwFCCHandler;
		aviStreamInfo.dwScale		= m_wfx.nBlockAlign;
		aviStreamInfo.dwRate		= m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;
		aviStreamInfo.dwQuality		= -1;				// Default Quality
		aviStreamInfo.dwSampleSize  = m_wfx.nBlockAlign;
		aviStreamInfo.dwSuggestedBufferSize = m_wfx.nAvgBytesPerSec;
		
		hr = AVIFileCreateStream(m_pAviFile, &m_pAudioStream, &aviStreamInfo);
		if(hr != S_OK)
		{
			SetErrorMessage(_T("Unable to Create the Audio Stream"));
			break;
		}

		//7. Set Audio Stream format
		hr = AVIStreamSetFormat(m_pAudioStream, 0, &m_wfx, 16);
		if(hr != S_OK)
		{
			SetErrorMessage(_T("Unable to Set Audio Stream format"));
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

HRESULT CBitmapAvi::Done()
{
	if(m_waveFile == NULL || m_nDataLen <= 0)
	{
		return S_OK;
	}
	int i, nRead, nSamples, nTotalSamples = 0, nTotalSeconds = m_lSample / m_dwFrameRate + 1;
	char* pcmbuf = new char[m_wfx.nAvgBytesPerSec];
	if(pcmbuf == NULL)
	{
		SetErrorMessage(_T("Done: Unable to allocate audio buffer"));
		return S_FALSE;
	}

	HRESULT hr = S_OK;
	for(i = 0; i < nTotalSeconds; i++)
	{
		//read audio data in one second
		nRead = fread(pcmbuf, 1, m_wfx.nAvgBytesPerSec, m_waveFile);
		//EOF
		if(nRead <= 0)
		{
			break;
		}

		nSamples = nRead / m_wfx.nBlockAlign;

		nRead = nSamples * m_wfx.nBlockAlign;

		
		hr = AVIStreamWrite(m_pAudioStream, nTotalSamples, nSamples, pcmbuf, nRead, 0, NULL, NULL);
		if(hr != S_OK)
		{
			_stprintf(m_szErrMsg, _T("AVIStreamWrite: Unable to Write Audio Stream: %u"), hr);
			break;
		}

		nTotalSamples += nSamples;
	}

	delete [] pcmbuf;

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

int CBitmapAvi::ParseWaveHeader()
{
	FILE* pFileSrc = m_waveFile;

	m_nDataLen = 0;
	
	char buf[32], msg[256];
	int nRead;
	
	/*Read RIFF file header*/
	nRead = fread(buf, 1, 12, pFileSrc);
	if(nRead != 12)
	{
		_stprintf(m_szErrMsg, _T("Read RIFF header length failed. length=%d"), nRead);
		return -1;
	}
	
	/* starts with RIFF? */
	if(memcmp(buf, "RIFF", 4) != 0)
	{
		_stprintf(m_szErrMsg, _T("Can't find 'RIFF' tag"));
		return -1;
	}
	
	int nRiffLen = cfl::bytes_to_int32((const byte*)(buf + 4));
	
	if(memcmp(buf + 8, "WAVE", 4) != 0)
	{
		_stprintf(m_szErrMsg, _T("Can't find 'WAVE' tag"));
		return -1;
	}
	
	int nChunkSize;
	while(true)
	{
		nRead = fread(buf, 1, 8, pFileSrc);
		if(nRead != 8)
		{
			_stprintf(m_szErrMsg, _T("Read chunk ID failed: %d"), nRead);
			return -1;
		}
		
		nChunkSize = cfl::bytes_to_int32((const byte*)(buf + 4));
		
		//format chunk
		if(memcmp(buf, "fmt ", 4) == 0)
		{
			if(nChunkSize < 16)
			{
				_stprintf(m_szErrMsg, _T("Wave Format length is too small: %d"), nChunkSize);
				return -1;
			}
			
			memset(&m_wfx, 0, sizeof(m_wfx));
			nRead = fread(&m_wfx, 1, 16, pFileSrc);
			if(nRead != 16)
			{
				_stprintf(m_szErrMsg, _T("Read Wave Format failed. nRead=%d"), nRead);
				return -1;
			}
			
			if(m_wfx.wFormatTag != WAVE_FORMAT_PCM)
			{
				_stprintf(m_szErrMsg, _T("Unsupported data format: 0x%04X"), m_wfx.wFormatTag);
				return -1;
			}
			
			if(nChunkSize > 16)
			{
				if(fseek(pFileSrc, nChunkSize - 16, SEEK_CUR) != 0)
				{
					_stprintf(m_szErrMsg, _T("fseek failed"));
					return -1;
				}
			}
		}
		//data chunk
		else if(memcmp(buf, "data", 4) == 0)
		{
			m_nDataLen = nChunkSize;
			
			AfxTrace(_T("Found data chunk with length: %d(0x%08X)\n"), m_nDataLen, m_nDataLen);
			/* We've found the audio data. Read no further! */
            break;
		}
		//other chunks
		else
		{
			memcpy(msg, buf, 4);
			msg[4]= 0;
			AfxTrace("Skipped chunk: %s\n", msg);
			
			if(fseek(pFileSrc, nChunkSize, SEEK_CUR) != 0)
			{
				_stprintf(m_szErrMsg, _T("fseek failed\n"));
				return -1;
			}
		}
	}
	
	return (m_nDataLen > 0) ? 0 : -1;
}
