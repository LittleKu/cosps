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

		hr = AVIStreamInfo(m_pAviStream, &m_aviInfo, sizeof(AVISTREAMINFO));
		if(hr != S_OK)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the video stream info"));
			break;
		}
		CString szFourCC;
		FourCC2Str(m_aviInfo.fccHandler, szFourCC);
		AfxTrace(_T("fccHandler=%s, 0x%08X\n"), szFourCC, m_aviInfo.fccHandler);

		ZeroMemory(&m_biWanted, sizeof(m_biWanted));
		LONG lFormat = sizeof(m_biWanted);

		hr = AVIStreamReadFormat(m_pAviStream, 0, &m_biWanted, &lFormat);
		if(hr != S_OK)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the foramt of the 1st frame"));
			break;
		}
		m_biWanted.biCompression = BI_RGB;
		m_biWanted.biBitCount = 32;
		m_biWanted.biSizeImage = m_biWanted.biWidth * 4 * m_biWanted.biHeight;

		//Set the result to Fail
		hr = E_FAIL;

		//Get the GETFRAME handle
		m_pGetFrame = AVIStreamGetFrameOpen(m_pAviStream, &m_biWanted);
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

HRESULT CAviBitmap::GetAllFrames(LPCTSTR lpszFolderName)
{
	if(m_pGetFrame == NULL)
	{
		m_szLastErrorMsg.Format(_T("Not initialized yet"));
		return E_FAIL;
	}
	HRESULT hr = S_OK;

	int nBmpInfoHdrSize = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 256;
	BITMAPINFOHEADER* lpBmpInfoHdr = (BITMAPINFOHEADER*)(new BYTE[nBmpInfoHdrSize]);
	LONG lpcbFormat = nBmpInfoHdrSize;

	BYTE* lpDib = NULL;
	BYTE* lpBuffer = NULL;

	LONG lBytes = 0, lSamples = 0;
	BOOL bReadRaw = FALSE;

	int nPos = 0;
	int nSampleCount = min(m_lSampleCount, 101);
	for(nPos = 0; nPos < nSampleCount; nPos++)
	{
		//Get the frame format
		hr = AVIStreamReadFormat(m_pAviStream, nPos, lpBmpInfoHdr, &lpcbFormat);
		if(hr != S_OK)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the sample format: %d"), nPos);
			break;
		}

		lpBuffer = NULL;
		//Try to read raw data when the bitmap is BI_RGB
		if(lpBmpInfoHdr->biCompression == BI_RGB && (lpBmpInfoHdr->biBitCount == 24 || lpBmpInfoHdr->biBitCount == 32))
		{
			//Get the frame data
			lpBuffer = new BYTE[m_biWanted.biSizeImage];
			hr = AVIStreamRead(m_pAviStream, nPos, 1, lpBuffer, m_biWanted.biSizeImage, &lBytes, &lSamples);
			if(hr != S_OK)
			{
				m_szLastErrorMsg.Format(_T("Unable to Get the sample data: %d"), nPos);
				break;
			}
		}
		else
		{
			CString szFourCC;
			FourCC2Str(m_aviInfo.fccHandler, szFourCC);
			AfxTrace(_T("Non-RGB format at frame(%03d)=%s, 0x%08X\n"), nPos, szFourCC, lpBmpInfoHdr->biCompression);
		}


		//Get the frame at nPos
		lpDib = (BYTE*)AVIStreamGetFrame(m_pGetFrame, nPos);
		if(lpDib == NULL)
		{
			m_szLastErrorMsg.Format(_T("Unable to Get the sample: %d"), nPos);
			hr = E_FAIL;
			break;
		}

		//compare the data retrieved in 2 ways if needed
		if(lpBuffer != NULL)
		{
			if(memcmp(lpBuffer, lpDib + sizeof(BITMAPINFOHEADER), lpBmpInfoHdr->biSizeImage) != 0)
			{
				m_szLastErrorMsg.Format(_T("not equals: %d"), nPos);
				hr = E_FAIL;
				break;
			}
		}

		CString szFileName;
		if(lpszFolderName == NULL)
		{
			szFileName.Format(_T(".\\Frame%03d.bmp"), nPos);
		}
		else
		{
			szFileName.Format(_T("%s\\Frame%03d.bmp"), lpszFolderName, nPos);
		}
		BITMAPINFOHEADER* pTemp = (BITMAPINFOHEADER*)lpDib;

//		hr = SaveBitmap(lpBmpInfoHdr, lpBuffer, lpBmpInfoHdr->biSizeImage, szFileName);
		hr = SaveBitmap(&m_biWanted, lpDib + sizeof(BITMAPINFOHEADER), m_biWanted.biSizeImage, szFileName);

		if(lpBuffer != NULL)
		{
			delete [] lpBuffer;
			lpBuffer = NULL;
		}
		//Done
	}

	if(lpBuffer != NULL)
	{
		delete [] lpBuffer;
		lpBuffer = NULL;
	}

	if(lpBmpInfoHdr != NULL)
	{
		delete [] lpBmpInfoHdr;
		lpBmpInfoHdr = NULL;
	}

	ReleaseMemory();

	return hr;
}

HRESULT CAviBitmap::SaveBitmap(BITMAPINFOHEADER *lpBMIH, BYTE *lpBuffer, DWORD cbBuffer, LPCTSTR lpszFileName)
{
	BITMAPFILEHEADER bmpFileHdr;
	bmpFileHdr.bfType = 0x4D42;
	bmpFileHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpFileHdr.bfReserved1 = bmpFileHdr.bfReserved2 = 0;
	bmpFileHdr.bfSize = bmpFileHdr.bfOffBits + cbBuffer;


	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
    {
		m_szLastErrorMsg.Format(_T("Unable to create file"));
		return E_FAIL;
    }
	
	HRESULT hr = E_FAIL;
	DWORD dwWrite = 0;
	do 
	{
		if(!WriteFile(hFile, &bmpFileHdr, sizeof(BITMAPFILEHEADER), &dwWrite, NULL))
		{
			m_szLastErrorMsg.Format(_T("Unable to write file header"));
			break;
		}
		
		if(!WriteFile(hFile, lpBMIH, sizeof(BITMAPINFOHEADER), &dwWrite, NULL))
		{
			m_szLastErrorMsg.Format(_T("Unable to write info header"));
			break;
		}
		if(!WriteFile(hFile, lpBuffer, cbBuffer, &dwWrite, NULL))
		{
			m_szLastErrorMsg.Format(_T("Unable to write image data"));
			break;
		}

		hr = S_OK;

	} while (FALSE);
	
	CloseHandle(hFile);

	return hr;
}

HRESULT CAviBitmap::GetFrame(int nFrame, LPVOID lpBuffer, DWORD cbBuffer)
{
	HRESULT hr = E_FAIL;

	return hr;
}


void CAviBitmap::FourCC2Str(DWORD dwFourCC, CString &szStr)
{
	szStr.Format(_T("%c%c%c%c"), dwFourCC%256, (dwFourCC>>8)%256, (dwFourCC>>16)%256, (dwFourCC>>24)%256);
}
