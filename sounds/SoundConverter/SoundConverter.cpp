// SoundConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WaveFile.h"
#include "AcmConverter.h"

void testWaveFile(LPCTSTR lpszFileName);

int main(int argc, char* argv[])
{
//	testWaveFile("C:\\lgao1\\72audio\\wav\\msadpcm.wav");
	testWaveFile("F:\\data\\sound\\wav\\output\\MS_ADPCM.wav");

	printf("Hello World!\n");
	return 0;
}

void printWaveFormat(const WAVEFORMATEX* lpWaveFormat)
{
	printf("format=\t\t\t0x%04X\n", (int)lpWaveFormat->wFormatTag);
	printf("channels=\t\t%d\n", (int)lpWaveFormat->nChannels);
	printf("sample rate=\t\t%u\n", lpWaveFormat->nSamplesPerSec);
	printf("bytes rate=\t\t%u\n", lpWaveFormat->nAvgBytesPerSec);
	printf("nBlockAlign=\t\t%d\n", (int)lpWaveFormat->nBlockAlign);
	printf("wBitsPerSample=\t\t%d\n", (int)lpWaveFormat->wBitsPerSample);
	printf("cbSize=\t\t%d\n", (int)lpWaveFormat->cbSize);
}

void printWaveFormat(const MMCKINFO* lpDataInfo)
{
	printf("size=%d(0x%08X)\n", lpDataInfo->cksize, lpDataInfo->cksize);
	printf("offset=%d(0x%08X)\n", lpDataInfo->dwDataOffset, lpDataInfo->dwDataOffset);
}

int fp2(int nAlign, int nMin)
{
	int x = nAlign;
	while(x < nMin)
	{
		x <<= 1;
	}
	return x;
}

void prepareWaveFormatSrc(WAVEFORMATEX* pwfx, WAVEFORMATEX* pwfxSrc)
{
	pwfx->wFormatTag = WAVE_FORMAT_PCM;
	pwfx->nChannels = pwfxSrc->nChannels; // stereo
	pwfx->nSamplesPerSec = pwfxSrc->nSamplesPerSec; // 44.1kHz
// 	pwfx->nChannels = 2; // stereo
// 	pwfx->nSamplesPerSec = 44100; // 44.1kHz
	pwfx->wBitsPerSample = 16; // 16 bits
	pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8; 
	pwfx->nAvgBytesPerSec = pwfx->nSamplesPerSec * pwfx->nBlockAlign;
	pwfx->cbSize = 0; // no more data to follow

	printf("\tchannels=%d, samples=%d\n", pwfx->nChannels, pwfx->nSamplesPerSec);
}
void testWaveFile(LPCTSTR lpszFileName)
{
	CWaveFile waveFile;
	BOOL bRet = FALSE;

	char* buffer = NULL;
	int nCount = 0;
	LONG lVar;
	do 
	{
		bRet = waveFile.PrepareWaveFile(lpszFileName);
		if(!bRet)
		{
			break;
		}

		printWaveFormat(waveFile.GetWaveFormat());

		printWaveFormat(waveFile.GetChunkDataInfo());

		FIND_DRIVER_INFO fdi;
		memset(&fdi, 0, sizeof(fdi));
		fdi.wFormatTag = waveFile.GetWaveFormat()->wFormatTag;
		fdi.pwfx = (LPWAVEFORMATEX)malloc(waveFile.GetWaveFormatSize());
		memcpy(fdi.pwfx, waveFile.GetWaveFormat(), waveFile.GetWaveFormatSize());
		fdi.cbwfx = waveFile.GetWaveFormatSize();

		bRet = CAcmConverter::FindDriver(&fdi);
		if(!bRet || fdi.hadid == 0)
		{
			printf("FindDriver Failed. fdi.hadid=%u\n", (UINT)fdi.hadid);
			break;
		}
		printf("FindDriver ok. driver id=%u\n", fdi.hadid);
//		free(fdi.pwfx);

		WAVEFORMATEX wfxDst;
		prepareWaveFormatSrc(&wfxDst, waveFile.GetWaveFormat());

		int nSeconds = (waveFile.GetChunkDataInfo()->cksize / waveFile.GetWaveFormat()->nAvgBytesPerSec) + 1;
		int nBytesPerSample = (wfxDst.nChannels * wfxDst.wBitsPerSample) / 8;
		int nDstSize = (nSeconds * wfxDst.nSamplesPerSec * nBytesPerSample) * 2;

		char* dstBuffer = new char[nDstSize];
		if(dstBuffer == NULL)
		{
			printf("dstBuffer allocate error\n");
			break;
		}
		memset(dstBuffer, 0, nDstSize);

		nCount = waveFile.GetChunkDataInfo()->cksize;
		buffer = new char[nCount];
		if(buffer == NULL)
		{
			printf("buffer allocate error\n");
			break;
		}
		printf("ideal buffer size: %d\n", nCount);
		lVar = waveFile.ReadWaveData(buffer, nCount);
		if(lVar != nCount)
		{
			printf("ReadWaveData error: lVar=%d, nCount=%d\n", lVar, nCount);
			break;
		}
		
//		delete[] buffer;

		ACMSTREAMHEADER ash;
		ACMSTREAMHEADER* pash = &ash;
		memset(pash, 0, sizeof(ACMSTREAMHEADER));
		pash->cbStruct = sizeof(ACMSTREAMHEADER);
		pash->pbSrc = (LPBYTE)buffer; // The source data to convert
		pash->cbSrcLength = nCount;
		pash->pbDst = (LPBYTE)dstBuffer;
		pash->cbDstLength = nDstSize;
		bRet = CAcmConverter::Convert(fdi.hadid, pash, waveFile.GetWaveFormat(), &wfxDst);
		if(!bRet)
		{
			printf("%s\n", CAcmConverter::m_szErrorMsg);
			printf("Convert error\n");
			break;
		}
		//create buffer
		/*
		nCount = fp2(waveFile.GetWaveFormat()->nBlockAlign, 4096);
		buffer = new char[nCount];
		printf("ideal buffer size: %d\n", nCount);

		int i = 0, nTotalSize = 0;
		while( (lVar = waveFile.ReadWaveData(buffer, nCount)) > 0 )
		{
			i++;
			nTotalSize += lVar;
			if(lVar != nCount)
			{
				printf("last block(%d): %d\n", i, lVar);
			}
		}
		printf("Read wave data with %d blocks, total size=%d(0x%08X)\n", i, nTotalSize, nTotalSize);

		delete[] buffer;
		*/

		bRet = waveFile.UnprepareWaveFile();
		
		if(!bRet)
		{
			break;
		}

		bRet = TRUE;

	} while (FALSE);
	
	if(!bRet)
	{
		printf("[N]: %s\n", waveFile.GetLastErrorMsg());
	}
	else
	{
		printf("[Y]: Done\n");
	}
}

