// SoundConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WaveFile.h"

void testWaveFile(LPCTSTR lpszFileName);

int main(int argc, char* argv[])
{
	testWaveFile("C:\\lgao1\\72audio\\wav\\msadpcm.wav");

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

		//create buffer
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

