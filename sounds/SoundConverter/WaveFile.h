// WaveFile.h: interface for the CWaveFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEFILE_H__D73B6F5E_1656_4F04_B3E0_9703D2B3F317__INCLUDED_)
#define AFX_WAVEFILE_H__D73B6F5E_1656_4F04_B3E0_9703D2B3F317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaveFile  
{
public:
	CWaveFile();
	virtual ~CWaveFile();

	BOOL PrepareWaveFile(LPCTSTR lpszFileName);
	BOOL UnprepareWaveFile();
	LONG ReadWaveData(char* buffer, int nCount);

	LPCTSTR GetLastErrorMsg() const
	{
		return this->m_szErrorMsg;
	}
	BOOL IsWaveValid() const
	{
		return this->m_lpWaveFormat != NULL;
	}
	WAVEFORMATEX* GetWaveFormat() const
	{
		return this->m_lpWaveFormat;
	}
	const DWORD GetWaveFormatSize() const
	{
		return this->m_dwWaveFormatSize;
	}
	const MMCKINFO* GetChunkDataInfo() const
	{
		return &m_ckInfoData;
	}
private:
	enum
	{
		ERROR_MSG_SIZE = 256
	};
	TCHAR m_szErrorMsg[ERROR_MSG_SIZE];	/* error message buffer */
	TCHAR m_szFileName[MAX_PATH];		/* wave file name */

	HMMIO			m_hWaveFile;		/* wave file handle */
	WAVEFORMATEX*	m_lpWaveFormat;		/* wave format pointer */
	DWORD			m_dwWaveFormatSize;	/* total struct size of m_lpWaveFormat */
	MMCKINFO		m_ckInfoData;		/* chunk data info */
	DWORD			m_dwDataReadCount;	/* data count already read */
};

#endif // !defined(AFX_WAVEFILE_H__D73B6F5E_1656_4F04_B3E0_9703D2B3F317__INCLUDED_)
