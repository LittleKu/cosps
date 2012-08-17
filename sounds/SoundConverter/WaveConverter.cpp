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
		&& (memcmp(&pdi->wfx, pafd->pwfx, pdi->cbwfx) == 0) /* same detailed format */ )
	{
		// found it
		pdi->hadid = hadid;
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

	mmr = acmFormatEnum(had, &fd, FindFormatCallBack, (DWORD)pdi, 0);
	free(pwf);
	if(mmr != 0)
	{
		// error: stop enumerating
		return FALSE;
	}

	mmr = acmDriverClose(had, 0);
	if(mmr != 0)
	{
		// error: stop enumerating
		return FALSE;
	}

	if(pdi->hadid) 
	{
		// found it
		return FALSE; // stop enumerating
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
