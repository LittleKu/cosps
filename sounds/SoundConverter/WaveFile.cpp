// WaveFile.cpp: implementation of the CWaveFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveFile::CWaveFile()
{
	ZeroMemory(m_szErrorMsg, ERROR_MSG_SIZE);
	ZeroMemory(m_szFileName, MAX_PATH);

	m_hWaveFile = NULL;
	m_lpWaveFormat = NULL;
	ZeroMemory(&m_ckInfoData, sizeof(m_ckInfoData));
	m_dwDataReadCount = 0;
}

CWaveFile::~CWaveFile()
{
	UnprepareWaveFile();
}

BOOL CWaveFile::PrepareWaveFile(LPCTSTR lpszFileName)
{
	if(lpszFileName == NULL || *lpszFileName == _T('\0'))
	{
		_stprintf(m_szErrorMsg, _T("Empty file name"));
		return FALSE;
	}

	//The previous opened file is not closed yet
	if(m_hWaveFile != NULL)
	{
		_stprintf(m_szErrorMsg, _T("The previous open file [%s] is not closed yet"), m_szFileName);
		return FALSE;
	}

	_tcscpy(m_szFileName, lpszFileName);

	//Open wave file
	m_hWaveFile = mmioOpen(m_szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF);
	if(m_hWaveFile == NULL)
	{
		_stprintf(m_szErrorMsg, _T("mmioOpen file %s failed"), m_szFileName);
		return FALSE;
	}

	//Chunk info structures
	MMCKINFO ckInfoParent, ckInfo;
	MMRESULT mmResult = 0;

	char* fmtBuffer = NULL;
	LONG lVar;
	BOOL bRet = FALSE;

	do
	{
		//Find riff wave
		ckInfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmResult = mmioDescend(m_hWaveFile, &ckInfoParent, NULL, MMIO_FINDRIFF);
		if(mmResult != MMSYSERR_NOERROR)
		{
			_stprintf(m_szErrorMsg, _T("mmioDescend find riff wave failed"));
			break;
		}

		//Find chunk "fmt "
		ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmResult = mmioDescend(m_hWaveFile, &ckInfo, &ckInfoParent, MMIO_FINDCHUNK);
		if(mmResult != MMSYSERR_NOERROR)
		{
			_stprintf(m_szErrorMsg, _T("mmioDescend find chunk 'fmt ' failed"));
			break;
		}

		//create 'fmt ' buffer
		fmtBuffer = new char[ckInfo.cksize];

		//Read 'fmt ' content
		lVar = mmioRead(m_hWaveFile, (HPSTR)fmtBuffer, ckInfo.cksize);
		if( (DWORD)lVar != ckInfo.cksize )
		{
			_stprintf(m_szErrorMsg, _T("mmioRead read 'fmt ' content failed. fmtSize=%d, read=%d"), 
				ckInfo.cksize, lVar);
			break;
		}
		//remember the fmt
		m_lpWaveFormat = (WAVEFORMATEX*)fmtBuffer;
		m_dwWaveFormatSize = ckInfo.cksize;
		
		//leave chunk 'fmt '
		mmResult = mmioAscend(m_hWaveFile, &ckInfo, 0);
		if(mmResult != MMSYSERR_NOERROR)
		{
			_stprintf(m_szErrorMsg, _T("mmioAscend leave 'fmt ' chunk failed. mmResult=%u"), mmResult);
			break;
		}
		
		//Find chunk 'data'
		ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmResult = mmioDescend(m_hWaveFile, &ckInfo, &ckInfoParent, MMIO_FINDCHUNK);
		if(mmResult != MMSYSERR_NOERROR)
		{
			_stprintf(m_szErrorMsg, _T("mmioDescend find 'data' chunk failed. mmResult=%u"), mmResult);
			break;
		}
		//remember the data chunk info
		m_ckInfoData = ckInfo;
		m_dwDataReadCount = 0;
		
		//Seek the file position to the chunk 'data' start
		lVar = mmioSeek(m_hWaveFile, m_ckInfoData.dwDataOffset, SEEK_SET);
		if((DWORD)lVar != m_ckInfoData.dwDataOffset)
		{
			_stprintf(m_szErrorMsg, _T("mmioSeek to chunk 'data' failed. position=%d"), lVar);
			break;
		}
		
		//everything is ok
		bRet = TRUE;

	} while (FALSE);
	
	if(!bRet)
	{
		if(fmtBuffer != NULL)
		{
			delete[] fmtBuffer;
			fmtBuffer = NULL;
		}
		m_lpWaveFormat = NULL;
	}

	return bRet;
}

BOOL CWaveFile::UnprepareWaveFile()
{
	if(m_lpWaveFormat != NULL)
	{
		delete[] ( reinterpret_cast<char*>(m_lpWaveFormat) );
		m_lpWaveFormat = NULL;
	}
	MMRESULT mmResult = 0;

	if(m_hWaveFile != NULL)
	{
		mmResult = ::mmioClose(m_hWaveFile, 0);
		m_hWaveFile = NULL;

		if(mmResult != 0)
		{
			_stprintf(m_szErrorMsg, _T("mmioClose failed. %d"), mmResult);
			return FALSE;
		}
	}

	m_dwDataReadCount = 0;

	return TRUE;
}

LONG CWaveFile::ReadWaveData(char* buffer, int nCount)
{
	if(!IsWaveValid())
	{
		_stprintf(m_szErrorMsg, _T("ReadWaveData failed. Invalid wave file"));
		return -1;
	}

	//There may be extra data exising after chunk 'data' ending
	int nDataLeft = (int)(m_ckInfoData.cksize - m_dwDataReadCount);
	if(nDataLeft < nCount)
	{
		nCount = nDataLeft;
	}
	if(nCount <= 0)
	{
		return nCount;
	}

	LONG lVar = mmioRead(m_hWaveFile, buffer, nCount);
	if(lVar < 0)
	{
		_stprintf(m_szErrorMsg, _T("ReadWaveData error"));
		return lVar;
	}

	m_dwDataReadCount += (DWORD)lVar;

	return lVar;
}