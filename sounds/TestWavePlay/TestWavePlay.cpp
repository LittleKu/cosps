//from: http://wl3033619.blog.163.com/blog/static/2708106820112203266475/
#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#pragma comment( lib, "Winmm.lib" )

void printFlags(DWORD dwFlag, const char* prefix = 0)
{
	char buffer[256] = {0};
	if(dwFlag & WHDR_BEGINLOOP)
	{
		strcat(buffer, " BEGINLOOP");
	}
	if(dwFlag & WHDR_DONE)
	{
		strcat(buffer, " DONE");
	}
	if(dwFlag & WHDR_ENDLOOP)
	{
		strcat(buffer, " ENDLOOP");
	}
	if(dwFlag & WHDR_INQUEUE)
	{
		strcat(buffer, " INQUEUE");
	}
	if(dwFlag & WHDR_PREPARED)
	{
		strcat(buffer, " PREPARED");
	}

	if(prefix)
	{
		printf("%s: dwFlag=%08X, flag=%s\n", prefix, dwFlag, buffer);
	}
	else
	{
		printf("dwFlag=%08X, flag=%s\n", dwFlag, buffer);
	}
}

int main(int argc, char* argv[])
{
//    LPWSTR szFileName;//�����ļ���
    MMCKINFO mmckinfoParent;
    MMCKINFO mmckinfoSubChunk;
    DWORD dwFmtSize;
    HMMIO m_hmmio;//��Ƶ�ļ����
    DWORD m_WaveLong;
    HPSTR lpData;//��Ƶ����
    HANDLE m_hData=NULL;
    HANDLE m_hFormat;
    WAVEFORMATEX * lpFormat;
    DWORD m_dwDataOffset;
    DWORD m_dwDataSize;
    WAVEHDR pWaveOutHdr;
    WAVEOUTCAPS pwoc;
    HWAVEOUT hWaveOut;
    int SoundOffset=0;
    int SoundLong=0;
    int DevsNum;
    //�򿪲����ļ�
    if(!(m_hmmio=mmioOpen(argv[1],NULL,MMIO_READ|MMIO_ALLOCBUF)))
    {
    //File open Error
        printf("Failed to open the file.");//����������
        return false;
    }
    //�����ļ��Ƿ��������ļ�
    mmckinfoParent.fccType =mmioFOURCC('W','A','V','E');
    if(mmioDescend(m_hmmio,(LPMMCKINFO)&mmckinfoParent,NULL,MMIO_FINDRIFF))
    {
        printf("NOT WAVE FILE AND QUIT");
        return 0;
    }
    //Ѱ�� 'fmt' ��
    mmckinfoSubChunk.ckid =mmioFOURCC('f','m','t',' ');
    if(mmioDescend(m_hmmio,&mmckinfoSubChunk,&mmckinfoParent,MMIO_FINDCHUNK))
    {
        printf("Can't find 'fmt' chunk");
        return 0;
    }
    //��� 'fmt '��Ĵ�С�������ڴ�
    dwFmtSize=mmckinfoSubChunk.cksize ;
    m_hFormat=LocalAlloc(LMEM_MOVEABLE,LOWORD(dwFmtSize));
    if(!m_hFormat)
    {
        printf("failed alloc memory");
        return 0;
    }
    lpFormat=(WAVEFORMATEX*)LocalLock(m_hFormat);
    if(!lpFormat)
    {
        printf("failed to lock the memory");
        return 0;
    }
    if((unsigned long)mmioRead(m_hmmio,(HPSTR)lpFormat,dwFmtSize)!=dwFmtSize)
    {
        printf("failed to read format chunk");
        return 0;
    }
    //�뿪 fmt ��
    mmioAscend(m_hmmio,&mmckinfoSubChunk,0);
    //Ѱ�� 'data' ��
    mmckinfoSubChunk.ckid=mmioFOURCC('d','a','t','a');
    if(mmioDescend(m_hmmio,&mmckinfoSubChunk,&mmckinfoParent,MMIO_FINDCHUNK))
    {
        printf("Can't find 'data' chunk");
        return 0;
    }
    //��� 'data'��Ĵ�С
    m_dwDataSize=mmckinfoSubChunk.cksize ;
    m_dwDataOffset =mmckinfoSubChunk.dwDataOffset ;
    if(m_dwDataSize==0L)
    {
        printf("no data in the 'data' chunk");
        return 0;
    }
    //Ϊ��Ƶ���ݷ����ڴ�
    lpData=new char[m_dwDataSize];
    if(!lpData)
    {
        printf("\ncan not alloc mem");
        return 0;
    }
    if(mmioSeek(m_hmmio,m_dwDataOffset,SEEK_SET)<0)
    {
        printf("Failed to read the data chunk");
        return 0;
    }
//    m_WaveLong=mmioRead(m_hmmio,lpData,SoundLong);
    m_WaveLong=mmioRead(m_hmmio,lpData,m_dwDataSize);
   
    if(m_WaveLong<0)
    {
        printf("Failed to read the data chunk");
        return 0;
    }
    //�����Ƶ�豸��������Ƶ����豸������
    if(waveOutGetDevCaps(WAVE_MAPPER,&pwoc,sizeof(WAVEOUTCAPS))!=0)
    {
        printf("Unable to allocate or lock memory");
        return 0;
    }

    //�����Ƶ����豸�Ƿ��ܲ���ָ������Ƶ�ļ�
    DevsNum = WAVE_MAPPER;
    if(waveOutOpen(&hWaveOut,DevsNum,lpFormat,NULL,NULL,CALLBACK_NULL)!=0)
    {
        printf("Failed to OPEN the wave out devices");
//        return 0;
    }
    //׼�������ŵ�����
    pWaveOutHdr.lpData =(HPSTR)lpData;
    pWaveOutHdr.dwBufferLength =m_WaveLong;
    pWaveOutHdr.dwFlags =0;
	pWaveOutHdr.dwLoops = 10;
    if(waveOutPrepareHeader(hWaveOut,&pWaveOutHdr,sizeof(WAVEHDR))!=0)
    {
        printf("Failed to prepare the wave data buffer");
        return 0;
    }
	printFlags(pWaveOutHdr.dwFlags, "after [waveOutPrepareHeader]");

	pWaveOutHdr.dwFlags |= (WHDR_BEGINLOOP | WHDR_ENDLOOP);
    //������Ƶ�����ļ�
    if(waveOutWrite(hWaveOut,&pWaveOutHdr,sizeof(WAVEHDR))!=0)
    {
        printf("Failed to write the wave data buffer");
        return 0;
    }
	printFlags(pWaveOutHdr.dwFlags, "after [waveOutWrite]");
    //�ر���Ƶ����豸,�ͷ��ڴ�
//     printf("\npress any key");
//     getchar();
	Sleep(20000);
	printFlags(pWaveOutHdr.dwFlags, "after [Sleep]");

	if(waveOutUnprepareHeader(hWaveOut, &pWaveOutHdr, sizeof(pWaveOutHdr)) != 0)
	{
		printf("Failed to unPrepare the wave data buffer");
        return 0;
	}
	printFlags(pWaveOutHdr.dwFlags, "after [waveOutUnprepareHeader]");

    waveOutReset(hWaveOut);
	printFlags(pWaveOutHdr.dwFlags, "after [waveOutReset]");

    waveOutClose(hWaveOut);
	printFlags(pWaveOutHdr.dwFlags, "after [waveOutClose]");

    LocalUnlock(m_hFormat);
    LocalFree(m_hFormat);
    delete [] lpData;

    return 0;
}