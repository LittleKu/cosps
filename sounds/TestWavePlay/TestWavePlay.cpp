//from: http://wl3033619.blog.163.com/blog/static/2708106820112203266475/
#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#pragma comment( lib, "Winmm.lib" )

int main(int argc, char* argv[])
{
//    LPWSTR szFileName;//声音文件名
    MMCKINFO mmckinfoParent;
    MMCKINFO mmckinfoSubChunk;
    DWORD dwFmtSize;
    HMMIO m_hmmio;//音频文件句柄
    DWORD m_WaveLong;
    HPSTR lpData;//音频数据
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
    //打开波形文件
    if(!(m_hmmio=mmioOpen(argv[1],NULL,MMIO_READ|MMIO_ALLOCBUF)))
    {
    //File open Error
        printf("Failed to open the file.");//错误处理函数
        return false;
    }
    //检查打开文件是否是声音文件
    mmckinfoParent.fccType =mmioFOURCC('W','A','V','E');
    if(mmioDescend(m_hmmio,(LPMMCKINFO)&mmckinfoParent,NULL,MMIO_FINDRIFF))
    {
        printf("NOT WAVE FILE AND QUIT");
        return 0;
    }
    //寻找 'fmt' 块
    mmckinfoSubChunk.ckid =mmioFOURCC('f','m','t',' ');
    if(mmioDescend(m_hmmio,&mmckinfoSubChunk,&mmckinfoParent,MMIO_FINDCHUNK))
    {
        printf("Can't find 'fmt' chunk");
        return 0;
    }
    //获得 'fmt '块的大小，申请内存
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
    //离开 fmt 块
    mmioAscend(m_hmmio,&mmckinfoSubChunk,0);
    //寻找 'data' 块
    mmckinfoSubChunk.ckid=mmioFOURCC('d','a','t','a');
    if(mmioDescend(m_hmmio,&mmckinfoSubChunk,&mmckinfoParent,MMIO_FINDCHUNK))
    {
        printf("Can't find 'data' chunk");
        return 0;
    }
    //获得 'data'块的大小
    m_dwDataSize=mmckinfoSubChunk.cksize ;
    m_dwDataOffset =mmckinfoSubChunk.dwDataOffset ;
    if(m_dwDataSize==0L)
    {
        printf("no data in the 'data' chunk");
        return 0;
    }
    //为音频数据分配内存
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
    //检查音频设备，返回音频输出设备的性能
    if(waveOutGetDevCaps(WAVE_MAPPER,&pwoc,sizeof(WAVEOUTCAPS))!=0)
    {
        printf("Unable to allocate or lock memory");
        return 0;
    }

    //检查音频输出设备是否能播放指定的音频文件
    DevsNum = WAVE_MAPPER;
    if(waveOutOpen(&hWaveOut,DevsNum,lpFormat,NULL,NULL,CALLBACK_NULL)!=0)
    {
        printf("Failed to OPEN the wave out devices");
//        return 0;
    }
    //准备待播放的数据
    pWaveOutHdr.lpData =(HPSTR)lpData;
    pWaveOutHdr.dwBufferLength =m_WaveLong;
    pWaveOutHdr.dwFlags =0;
    if(waveOutPrepareHeader(hWaveOut,&pWaveOutHdr,sizeof(WAVEHDR))!=0)
    {
        printf("Failed to prepare the wave data buffer");
        return 0;
    }
    //播放音频数据文件
    if(waveOutWrite(hWaveOut,&pWaveOutHdr,sizeof(WAVEHDR))!=0)
    {
        printf("Failed to write the wave data buffer");
        return 0;
    }
    //关闭音频输出设备,释放内存
//     printf("\npress any key");
//     getchar();
	Sleep(50000);
    waveOutReset(hWaveOut);
    waveOutClose(hWaveOut);
    LocalUnlock(m_hFormat);
    LocalFree(m_hFormat);
    delete [] lpData;

    return 0;
}