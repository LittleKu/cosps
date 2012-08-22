// WavePlayer.h: interface for the CWavePlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEPLAYER_H__B7E625EA_DB08_4A53_900C_D09F4C641629__INCLUDED_)
#define AFX_WAVEPLAYER_H__B7E625EA_DB08_4A53_900C_D09F4C641629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

class CWavePlayer  
{
public:
	CWavePlayer();
	virtual ~CWavePlayer();
	BOOL Init();
	BOOL Open(DWORD nSamplesPerSec, WORD wBitsPerSample, WORD nChannels);
	BOOL PlayData(const char* lpData, int nDataLength);
	BOOL Flush();
	BOOL Close();
	const char* GetErrorMsg() const
	{
		return this->m_szErrorMsg;
	}

private:
	BOOL SetWaveFormatEx(WAVEFORMATEX* pwfx);
	BOOL DeInit();
	WAVEHDR* AllocateBlocks(int size, int count);
	void FreeBlocks(LPVOID lpMem);
	void QueueWaveBlock(WAVEHDR* pWaveHdr);
	
	void WaveOutCallBack(HWAVEOUT hwave, UINT msg, DWORD param1, DWORD param2);
	static void CALLBACK waveOutCallBack(HWAVEOUT hwave, UINT msg, DWORD_PTR data, DWORD param1, DWORD param2);
private:
	enum
	{
		ERROR_MSG_BUF_SIZE = 256,
		BLOCK_SIZE = (1 << 14),
		BLOCK_COUNT = 4
	};

	char m_szErrorMsg[ERROR_MSG_BUF_SIZE];	/* error message buffer*/

	WAVEFORMATEX m_wfx;						/* Wave Format for playing*/
	HWAVEOUT m_hWaveOut;					/* device handle for writing wave data */
	CRITICAL_SECTION m_cs;					/* to control access to shared memory */
	HANDLE m_hHasFreeBlocks;				/* event that there is at least one buffer is free */
	WAVEHDR* m_pWaveBlocks;					/* wave block buffer */
	int m_nBlockSize;						/* block size that align with nBlockAlign, equals to BLOCK_SIZE in most case */
	int m_nFreeBlockCount;					/* free block count */
	int m_nCurrentBlock;					/* current block index */
};

#endif // !defined(AFX_WAVEPLAYER_H__B7E625EA_DB08_4A53_900C_D09F4C641629__INCLUDED_)
