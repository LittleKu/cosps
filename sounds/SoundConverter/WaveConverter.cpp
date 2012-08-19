// WaveConverter.cpp: implementation of the CWaveConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#pragma comment( lib, "msacm32.lib" ) 
#include "WaveConverter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CALLBACK FindFormatCallBack(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	if ( (pafd->dwFormatTag == (DWORD)pdi->wFormatTag)	//same format tag
		&& (pdi->cbwfx == pafd->cbwfx)	//same format size
		&& (memcmp(pdi->pwfx, pafd->pwfx, pdi->cbwfx) == 0) /* same detailed format */ )
	{
		// found it
		pdi->hadid = hadid;
		printf("    %4.4lXH, %s\n", pafd->dwFormatTag, pafd->szFormat);
		return FALSE; // stop enumerating
	}
	
	return TRUE; // continue enumerating
}

BOOL CALLBACK FindDriverCallBack(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
	MMRESULT mmr = 0;
	// open the driver
	HACMDRIVER had = NULL;
	mmr = acmDriverOpen(&had, hadid, 0);
	if(mmr != 0)
	{
		// error: stop enumerating
		return FALSE;
	}

	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	DWORD dwSize = 0;
	ACMFORMATDETAILS fd;
	WAVEFORMATEX* pwf = NULL;

	// enumerate the formats it supports
	mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
	if(mmr != 0)
	{
		// error: stop enumerating
		return FALSE;
	}
	if (dwSize < sizeof(WAVEFORMATEX)) 
		dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
	pwf = (WAVEFORMATEX*) malloc(dwSize);
	if(pwf == NULL)
	{
		// error: stop enumerating
		return FALSE;
	}
	memset(pwf, 0, dwSize);
	pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
	pwf->wFormatTag = pdi->wFormatTag;
	memset(&fd, 0, sizeof(fd));
	fd.cbStruct = sizeof(fd);
	fd.pwfx = pwf;
	fd.cbwfx = dwSize;
	fd.dwFormatTag = pdi->wFormatTag;

// 	mmr = acmFormatEnum(had, &fd, FindFormatCallBack, (DWORD)pdi, ACM_FORMATENUMF_WFORMATTAG
// 		| ACM_FORMATENUMF_WBITSPERSAMPLE | ACM_FORMATENUMF_NSAMPLESPERSEC | ACM_FORMATENUMF_NCHANNELS);
	mmr = acmFormatEnum(had, &fd, FindFormatCallBack, (DWORD)pdi, 0);
	free(pwf);
	if(mmr != 0)
	{
		// error: stop enumerating
		return FALSE;
	}
	if(pdi->hadid) 
	{
		// found it, don't close driver
		pdi->had = had;
		return FALSE; // stop enumerating
	}

	mmr = acmDriverClose(had, 0);
	if(mmr != 0)
	{
		// error: stop enumerating
		return FALSE;
	}

	return TRUE; // continue enumeration
}


CWaveConverter::CWaveConverter()
{

}

CWaveConverter::~CWaveConverter()
{

}

BOOL CWaveConverter::FindDriver(PFIND_DRIVER_INFO pfdi)
{
	pfdi->hadid = NULL;

	MMRESULT mmr;
	mmr = acmDriverEnum(FindDriverCallBack, (DWORD)pfdi, 0);
	return mmr == 0;
}

BOOL CWaveConverter::Convert(HACMDRIVER had, ACMSTREAMHEADER* pash, WAVEFORMATEX* pwfxSrc, WAVEFORMATEX* pwfxDst)
{
	MMRESULT mmr;
	HACMSTREAM hastr = NULL;

	// Convert the source wave to the PCM format supported by the CODEC.
	// We use any driver that can do the PCM to PCM conversion.

	mmr = acmStreamOpen(&hastr,
		had, // Any driver
		pwfxSrc, // Source format
		pwfxDst, // Destination format
		NULL, // No filter
		NULL, // No callback
		0, // Instance data (not used)
		ACM_STREAMOPENF_NONREALTIME); // flags
	if(mmr) 
	{
		printf("Failed to open a stream to do PCM to PCM conversion: %d\n", mmr);
		return FALSE;
	}

	// Prep the header.
	mmr = acmStreamPrepareHeader(hastr, pash, 0);

	if(mmr)
	{
		printf("acmStreamPrepareHeader failed\n");
		acmStreamClose(hastr, 0);
		return FALSE;
	}

	printf("Converting to intermediate PCM format...\n");
	mmr = acmStreamConvert(hastr, pash, 0);
	if(mmr) 
	{
		printf("Failed to do PCM to PCM conversion\n");
		acmStreamUnprepareHeader(hastr, pash, 0);
		acmStreamClose(hastr, 0);
		return FALSE;
	}

	FILE* fp = fopen("test.pcm", "wb");
	if(fp != NULL)
	{
		fwrite(pash->pbDst, 1, pash->cbDstLengthUsed, fp);
		fclose(fp);
		fp = NULL;
	}

	mmr = acmStreamUnprepareHeader(hastr, pash, 0);
	if(mmr)
	{
		printf("acmStreamUnprepareHeader failed\n");
		acmStreamClose(hastr, 0);
		return FALSE;
	}

	// Close the stream.
	mmr = acmStreamClose(hastr, 0);
	if(mmr)
	{
		printf("Failed to close acm stream\n");
		return FALSE;
	}

	acmDriverClose(had, 0);

	return TRUE;
}
