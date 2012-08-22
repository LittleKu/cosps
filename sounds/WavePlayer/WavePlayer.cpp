// WavePlayer.cpp: implementation of the CWavePlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "WavePlayer.h"
#include <stdio.h>
#include "DebugSupport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CALLBACK CWavePlayer::waveOutCallBack(HWAVEOUT hwave, UINT msg, DWORD_PTR data, DWORD param1, DWORD param2)
{
	CWavePlayer* player = (CWavePlayer*)data;
	player->WaveOutCallBack(hwave, msg, param1, param2);
}

CWavePlayer::CWavePlayer()
{
	m_pWaveBlocks = NULL;
	m_nBlockSize = 0;
	m_nFreeBlockCount = 0;
	m_nCurrentBlock = 0;

	memset(&m_szErrorMsg, 0, sizeof(m_szErrorMsg));
	memset(&m_wfx, 0, sizeof(m_wfx));
	m_hWaveOut = NULL;

	m_hHasFreeBlocks = NULL;
}

CWavePlayer::~CWavePlayer()
{
	DeInit();
}

BOOL CWavePlayer::Init()
{
	memset(&m_szErrorMsg, 0, sizeof(m_szErrorMsg));
	memset(&m_wfx, 0, sizeof(m_wfx));
	m_hWaveOut = NULL;

	InitializeCriticalSection(&m_cs);
	//Auto-reset event, no signal initially
	m_hHasFreeBlocks = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_pWaveBlocks = AllocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
	if(m_pWaveBlocks == NULL)
	{
		sprintf(m_szErrorMsg, "[Init]: allocateBlocks failed");
		return FALSE;
	}
	m_nBlockSize = BLOCK_SIZE;
	m_nFreeBlockCount = BLOCK_COUNT;
	m_nCurrentBlock = 0;

	return TRUE;
}

BOOL CWavePlayer::DeInit()
{
	if(m_pWaveBlocks != NULL)
	{
		FreeBlocks(m_pWaveBlocks);
		m_pWaveBlocks = NULL;
	}
	
	memset(&m_szErrorMsg, 0, sizeof(m_szErrorMsg));
	memset(&m_wfx, 0, sizeof(m_wfx));
	
	if(m_hHasFreeBlocks != NULL)
	{
		::CloseHandle(m_hHasFreeBlocks);
		m_hHasFreeBlocks = NULL;
	}
	DeleteCriticalSection(&m_cs);

	return TRUE;
}

BOOL CWavePlayer::Open(DWORD nSamplesPerSec, WORD wBitsPerSample, WORD nChannels)
{
	WAVEFORMATEX wfx;
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	wfx.nSamplesPerSec = nSamplesPerSec; /* sample rate */
	wfx.wBitsPerSample = wBitsPerSample; /* sample size */
	wfx.nChannels= nChannels; /* channels*/

	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	wfx.cbSize = 0; /* size of _extra_ info */

	return SetWaveFormatEx(&wfx);
}

BOOL CWavePlayer::SetWaveFormatEx(WAVEFORMATEX* pwfx)
{
	if(m_pWaveBlocks == NULL)
	{
		sprintf(m_szErrorMsg, "[SetWaveFormatEx]: not initialized yet");
		return FALSE;
	}

	if(m_hWaveOut != NULL)
	{
		sprintf(m_szErrorMsg, "[SetWaveFormatEx]: last wave-form audio doesn't stop playing");
		return FALSE;
	}

	if(pwfx == NULL)
	{
		sprintf(m_szErrorMsg, "[SetWaveFormatEx]: input parameter is null");
		return FALSE;
	}

	memcpy(&m_wfx, pwfx, sizeof(m_wfx));
	/*
	* try to open the default wave device. WAVE_MAPPER is
	* a constant defined in mmsystem.h, it always points to the
	* default wave device on the system (some people have 2 or
	* more sound cards).
	*/
	MMRESULT mmr = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_wfx, 
		(DWORD_PTR)(CWavePlayer::waveOutCallBack), (DWORD_PTR)this, CALLBACK_FUNCTION);
	if(mmr != MMSYSERR_NOERROR)
	{
		sprintf(m_szErrorMsg, "[waveOutOpen]: failed - 0x%08XH", mmr);
		return FALSE;
	}

	//calculate the block size
	m_nBlockSize = BLOCK_SIZE - (BLOCK_SIZE % m_wfx.nBlockAlign);
	TRACE("block size: %d, equals to BLOCK_SIZE=[%d]\n", m_nBlockSize, m_nBlockSize == BLOCK_SIZE);

	//Currently there are free blocks
	SetEvent(m_hHasFreeBlocks);

	return TRUE;
}

BOOL CWavePlayer::PlayData(const char* lpData, int nDataLength)
{
	int remain;
	WAVEHDR* current = &m_pWaveBlocks[m_nCurrentBlock];
	
	while(nDataLength > 0) 
	{
		//first make sure the header we're going to use is unprepared
		if(current->dwFlags & WHDR_PREPARED)
		{
			waveOutUnprepareHeader(m_hWaveOut, current, sizeof(WAVEHDR));
		}
		
		//the input data is not big enough to fill the current wave block
		if(nDataLength < (int)(m_nBlockSize - current->dwUser)) 
		{
			memcpy(current->lpData + current->dwUser, lpData, nDataLength);
			current->dwUser += nDataLength;
			break;
		}

		//calculate the remaining buffer size
		remain = m_nBlockSize - current->dwUser;
		memcpy(current->lpData + current->dwUser, lpData, remain);

		nDataLength -= remain;
		lpData += remain;

		current->dwBufferLength = m_nBlockSize;
		current->dwUser = 0;

		//add the wave block data to the queue, block if there is not more blocks
		QueueWaveBlock(current);

		//point to the next block
		m_nCurrentBlock = (m_nCurrentBlock + 1) % BLOCK_COUNT;
		current = &m_pWaveBlocks[m_nCurrentBlock];
	}
	return TRUE;
}

BOOL CWavePlayer::Flush()
{
	WAVEHDR* current = &m_pWaveBlocks[m_nCurrentBlock];
	TRACE("[Flush]: Buffer left bytes: %d\n", (int)current->dwUser);
	//there's remaining buffered data
	if(current->dwUser > 0)
	{
		//first make sure the header we're going to use is unprepared
		if(current->dwFlags & WHDR_PREPARED)
		{
			waveOutUnprepareHeader(m_hWaveOut, current, sizeof(WAVEHDR));
		}

		//calculate the buffer size, pad bytes if needed
		int nBufferLength = (int)(current->dwUser);

		//maybe the padding is not needed
		int remainder = nBufferLength % m_wfx.nBlockAlign;
		if(remainder != 0)
		{
			//not nBlockAlign align, add pad bytes
			int nPad = (int)(m_wfx.nBlockAlign) - remainder;
			memset(current->lpData + nBufferLength, 0, nPad);

			nBufferLength += nPad;

			TRACE("pad=%d, length=%d\n", nPad, nBufferLength);
		}

		//set the current buffer length
		current->dwBufferLength = nBufferLength;
		current->dwUser = 0;

		//add the wave block data to the queue, block if there is not more blocks
		QueueWaveBlock(current);

		//point to the next block
		m_nCurrentBlock = (m_nCurrentBlock + 1) % BLOCK_COUNT;
	}
	return TRUE;
}

void CWavePlayer::QueueWaveBlock(WAVEHDR* pWaveHdr)
{
	waveOutPrepareHeader(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));
	waveOutWrite(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));
	
	EnterCriticalSection(&m_cs);
	m_nFreeBlockCount--;
	//if there are free blocks, make sure the event is in signal status
	if(m_nFreeBlockCount > 0)
	{
		SetEvent(m_hHasFreeBlocks);
	}
	LeaveCriticalSection(&m_cs);
	
	//wait for a block to become free
	WaitForSingleObject(m_hHasFreeBlocks, INFINITE);
}

BOOL CWavePlayer::Close()
{
	TRACE("[Close]: call Flush\n");
	Flush();

	TRACE("[Close]: Flush done, wait to close\n");
	BOOL done = FALSE;
	while(!done)
	{
		//Wait until there are free blocks available
		WaitForSingleObject(m_hHasFreeBlocks, INFINITE);

		EnterCriticalSection(&m_cs);
		//not all the blocks are free
		if(m_nFreeBlockCount == BLOCK_COUNT)
		{
			done = TRUE;
		}
		TRACE("[Close]: m_nFreeBlockCount=%d\n", m_nFreeBlockCount);
		LeaveCriticalSection(&m_cs);
	}

	//Clean up, unprepare any blocks that are still prepared
	for(int i = 0; i < BLOCK_COUNT; i++)
	{
		if(m_pWaveBlocks[i].dwFlags & WHDR_PREPARED)
		{
			waveOutUnprepareHeader(m_hWaveOut, &m_pWaveBlocks[i], sizeof(WAVEHDR));
		}
	}

	//Close the wave out device
	waveOutClose(m_hWaveOut);
	m_hWaveOut = NULL;

	return TRUE;
}

void CWavePlayer::WaveOutCallBack(HWAVEOUT hwave, UINT msg, DWORD param1, DWORD param2)
{
	if(msg != WOM_DONE)
	{
		return;
	}

	EnterCriticalSection(&m_cs);

	//one more block is free
	m_nFreeBlockCount++;

	//there is free blocks now
	SetEvent(m_hHasFreeBlocks);

	LeaveCriticalSection(&m_cs);
}

WAVEHDR* CWavePlayer::AllocateBlocks(int size, int count)
{
	DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
	/*
	* allocate memory for the entire set in one go
	*/
	char* buffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, totalBufferSize);
	if(buffer == NULL)
	{
		sprintf(m_szErrorMsg, "[AllocateBlocks]: HeapAlloc failed");
		return NULL;
	}
	/*
	* and set up the pointers to each bit
	*/
	WAVEHDR* blocks = (WAVEHDR*)buffer;
	buffer += sizeof(WAVEHDR) * count;
	for(int i = 0; i < count; i++) 
	{
		blocks[i].dwBufferLength = size;
		blocks[i].lpData = (LPSTR)buffer;
		buffer += size;
	}
	return blocks;
}
void CWavePlayer::FreeBlocks(LPVOID lpMem)
{
	HeapFree(GetProcessHeap(), 0, lpMem);
}