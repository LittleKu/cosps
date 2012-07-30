// Command line compile with CL
#pragma comment( lib, "msacm32.lib" ) 
#include <windows.h>
#include <math.h>
#include <mmsystem.h>
#include <mmreg.h>  
#include <msacm.h>    
#include <stdio.h>

typedef struct
{
	HACMDRIVERID hadid;
	WORD wFormatTag;
} FIND_DRIVER_INFO;

// callback function for format enumeration
BOOL CALLBACK find_format_enum(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	if (pafd->dwFormatTag == (DWORD)pdi->wFormatTag)
	{
		// found it
		pdi->hadid = hadid;
		return FALSE; // stop enumerating
	}
	return TRUE; // continue enumerating
}

BOOL CALLBACK find_format_enum2(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	if (pafd->dwFormatTag == (DWORD)pdi->wFormatTag &&
		pafd->pwfx->nChannels == 1 &&
		pafd->pwfx->nSamplesPerSec == 11025)
	{
		pdi->hadid = hadid;
		return FALSE; // stop enumerating
	}
	return TRUE; // continue enumerating
}

BOOL CALLBACK find_driver_enum(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	DWORD dwSize = 0;
	ACMFORMATDETAILS fd;

	// open the driver
	HACMDRIVER had = NULL;
	MMRESULT mmr = acmDriverOpen(&had, hadid, 0);
	WAVEFORMATEX* pwf = NULL;
	if (mmr) {
		// some error
		return FALSE; // stop enumerating
	}
	// enumerate the formats it supports
	mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
	if (dwSize < sizeof(WAVEFORMATEX)) dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
	pwf = (WAVEFORMATEX*) malloc(dwSize);
	memset(pwf, 0, dwSize);
	pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
	pwf->wFormatTag = pdi->wFormatTag;
	memset(&fd, 0, sizeof(fd));
	fd.cbStruct = sizeof(fd);
	fd.pwfx = pwf;
	fd.cbwfx = dwSize;
	fd.dwFormatTag = pdi->wFormatTag;
	mmr = acmFormatEnum(had, &fd, find_format_enum, (DWORD)(VOID*)pdi, 0);  
	free(pwf);
	acmDriverClose(had, 0);
	if (pdi->hadid || mmr) {
		// found it or some error
		return FALSE; // stop enumerating
	}
	return TRUE; // continue enumeration
}

HACMDRIVERID find_driver(WORD wFormatTag)
{
	FIND_DRIVER_INFO fdi;
	MMRESULT mmr;
	fdi.hadid = NULL;
	fdi.wFormatTag = wFormatTag;
	mmr = acmDriverEnum(find_driver_enum, (DWORD)(VOID*)&fdi, 0); 
	if (mmr) return NULL;
	return fdi.hadid;
}

WAVEFORMATEX* get_driver_format(HACMDRIVERID hadid, WORD wFormatTag)
{
	// open the driver
	DWORD dwSize = 0;
	HACMDRIVER had = NULL;
	WAVEFORMATEX* pwf = NULL;
	ACMFORMATDETAILS fd;
	MMRESULT mmr = acmDriverOpen(&had, hadid, 0);
	FIND_DRIVER_INFO fdi;

	if (mmr)
	{
		printf("acmDriverOpen failed\n");
		return NULL;
	}
	// allocate a structure for the info
	mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
	if (dwSize < sizeof(WAVEFORMATEX)) dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
	pwf = (WAVEFORMATEX*) malloc(dwSize);
	memset(pwf, 0, dwSize);
	pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
	pwf->wFormatTag = wFormatTag;
	memset(&fd, 0, sizeof(fd));
	fd.cbStruct = sizeof(fd);
	fd.pwfx = pwf;
	fd.cbwfx = dwSize;
	fd.dwFormatTag = wFormatTag;
	// set up a struct to control the enumeration
	fdi.hadid = NULL;
	fdi.wFormatTag = wFormatTag;
	mmr = acmFormatEnum(had, &fd, find_format_enum2, (DWORD)(VOID*)&fdi, 0);  
	acmDriverClose(had, 0);
	if ((fdi.hadid == NULL) || mmr) 
	{
		printf("acmFormatEnum failed\n");
		free(pwf);
		return NULL;
	}
	return pwf;
}

int convert(HACMDRIVERID hadid,WORD wFormatTag)
{
	HACMDRIVER had = NULL;
	HACMSTREAM hstr = NULL;
	MMRESULT mmr;
	// First we create a wave that might have been just recorded.
	// The format is 11.025 kHz, 8 bit mono PCM which is a recording
	// format available on all machines.
	// Our sample wave will be 1 second long and will be a sine wave 
	// of 1kHz, which is exactly 1,000 cycles.
	WAVEFORMATEX wfSrc;
	memset(&wfSrc, 0, sizeof(wfSrc));
	wfSrc.cbSize = 0;
	wfSrc.wFormatTag = WAVE_FORMAT_PCM; // PCM
	wfSrc.nChannels = 1; // Mono
	wfSrc.nSamplesPerSec = 11025; // 11.025 kHz
	wfSrc.wBitsPerSample = 8; // 8 bit
	wfSrc.nBlockAlign = wfSrc.nChannels * wfSrc.wBitsPerSample / 8;
	wfSrc.nAvgBytesPerSec = wfSrc.nSamplesPerSec * wfSrc.nBlockAlign;
	DWORD dwSrcSamples = wfSrc.nSamplesPerSec;
	BYTE* pSrcData = new BYTE [dwSrcSamples]; // 1 second duration
	BYTE* pData = pSrcData;
	double f = 1000.0;
	double pi = 4.0 * atan(1.0);
	double w = 2.0 * pi * f;
	for (DWORD dw = 0; dw < dwSrcSamples; dw++) {
		double t = (double) dw / (double) wfSrc.nSamplesPerSec; 
		*pData++ = 128 + (BYTE)(127.0 * sin(w * t));
	}
	// Get the details of the format.
	// Note: this is just the first of one or more possible formats for the given tag.
	WAVEFORMATEX* pwfDrv = get_driver_format(hadid, wFormatTag);
	if (pwfDrv == NULL) {
		printf("Error getting format info\n");
		return -1;
	}
	printf("Driver format: %u bits, %lu samples per second\n",
		pwfDrv->wBitsPerSample, pwfDrv->nSamplesPerSec);
	// Get a PCM format tag the driver supports.
	// Note: we just pick the first supported PCM format which might not really
	// be the best choice.
	WAVEFORMATEX* pwfPCM = get_driver_format(hadid, wFormatTag);
	if (pwfPCM == NULL) {
		printf("Error getting PCM format info\n");
		return -1;
	}
	printf("PCM format: %u bits, %lu samples per second\n",
		pwfPCM->wBitsPerSample, pwfPCM->nSamplesPerSec);
	// First step
	/////////////////////////////////////////////////////////////////////////////
	// Convert the source wave to the PCM format supported by the CODEC.
	// We use any driver that can do the PCM to PCM conversion.
	//HACMSTREAM hstr = NULL;
	mmr  = acmStreamOpen(&hstr,
		NULL, // Any driver
		&wfSrc, // Source format
		pwfPCM, // Destination format
		NULL, // No filter
		NULL, // No callback
		0, // Instance data (not used)
		ACM_STREAMOPENF_NONREALTIME); // flags
	if (mmr) {
		printf("Failed to open a stream to do PCM to PCM conversion\n");
		return -1;
	}
	// Allocate a buffer for the result of the conversion.
	DWORD dwSrcBytes = dwSrcSamples * wfSrc.wBitsPerSample / 8;
	DWORD dwDst1Samples = dwSrcSamples * pwfPCM->nSamplesPerSec / wfSrc.nSamplesPerSec;
	DWORD dwDst1Bytes = dwDst1Samples * pwfPCM->wBitsPerSample / 8;
	BYTE* pDst1Data = new BYTE [dwDst1Bytes];
	// Fill in the conversion info.
	ACMSTREAMHEADER strhdr;
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = pSrcData; // The source data to convert
	strhdr.cbSrcLength = dwSrcBytes;
	strhdr.pbDst = pDst1Data;
	strhdr.cbDstLength = dwDst1Bytes;
	mmr = acmStreamPrepareHeader(hstr, &strhdr, 0); 
	printf("Converting to intermediate PCM format...\n");
	mmr = acmStreamConvert(hstr, &strhdr, 0);
	if (mmr) {
		printf("Failed to do PCM to PCM conversion\n");
		return -1;
	}
	printf("Intermediate Converted OK\n");

	// Close the stream.
	acmStreamClose(hstr, 0);

	//Final step
	///////////////////////////////////////////////////////////////////////////////////
	// Convert the intermediate PCM format to the final format.

	// Open the conversion stream.
	// Note the use of the ACM_STREAMOPENF_NONREALTIME flag. Without this
	// some software compressors will report error 512 - not possible.
	hstr = NULL;
	mmr = acmStreamOpen(&hstr,
		NULL, // had, // Driver handle
		pwfPCM, // Source format
		pwfDrv, // Destination format
		NULL, // No filter
		NULL, // No callback
		0, // Instance data (not used)
		ACM_STREAMOPENF_NONREALTIME ); // Flags
	if (mmr) {
		printf("Failed to open a stream to do PCM to driver format conversion\n");
		return -1;
	}
	// Allocate a buffer for the result of the conversion.
	// Compute the output buffer size based on the average byte rate
	// and add a bit for randomness.
	// The IMA_ADPCM driver fails the conversion without this extra space.
	DWORD dwDst2Bytes = pwfDrv->nAvgBytesPerSec * dwDst1Samples /
		pwfPCM->nSamplesPerSec;
	dwDst2Bytes = dwDst2Bytes * 3 / 2; // add a little room
	BYTE* pDst2Data = new BYTE [dwDst2Bytes];
	// Fill in the conversion info.
	ACMSTREAMHEADER strhdr2;
	memset(&strhdr2, 0, sizeof(strhdr2));
	strhdr2.cbStruct = sizeof(strhdr2);
	strhdr2.pbSrc = pDst1Data; // the source data to convert
	strhdr2.cbSrcLength = dwDst1Bytes;
	strhdr2.pbDst = pDst2Data;
	strhdr2.cbDstLength = dwDst2Bytes;
	// Prep the header.
	mmr = acmStreamPrepareHeader(hstr, &strhdr2, 0); 
	// Convert the data.
	printf("Converting to final format...\n");
	mmr = acmStreamConvert(hstr, &strhdr2, 0);
	if (mmr) {
		printf("Failed to do PCM to driver format conversion\n");
		return -1;
	}
	printf("Final Step Converted OK\n");
	// Close the stream and driver.
	mmr = acmStreamClose(hstr, 0);
	mmr = acmDriverClose(had, 0);
	// Show the conversion stats.
	printf("Source wave had %lu bytes\n", dwSrcBytes);
	printf("Converted wave has %lu bytes\n", strhdr2.cbDstLengthUsed);
	printf("Compression ratio is %f\n", (double) dwSrcBytes /
		(double) strhdr2.cbDstLengthUsed);
	return 0;
}

int main3(void)
{
	WORD wFormatTag = WAVE_FORMAT_PCM;
	HACMDRIVERID hadid = find_driver(wFormatTag);
	if (hadid == NULL) {
		printf("No driver found\n");
		return -1;
	}
	else
	{
		printf("Driver found (hadid: %4.4lXH)\n", hadid);
		return( convert(hadid,wFormatTag));
	}
}