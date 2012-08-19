// AcmConverter.cpp: implementation of the CAcmConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcmConverter.h"

#pragma comment( lib, "msacm32.lib" ) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TCHAR CAcmConverter::m_szErrorMsg[ERROR_MSG_SIZE] = {0};	/* error message buffer */

BOOL CALLBACK FindFormatCallBack(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	if ( (pafd->dwFormatTag == (DWORD)pdi->wFormatTag)	//same format tag
		&& (pdi->cbwfx == pafd->cbwfx)	//same format size
		&& (memcmp(pdi->pwfx, pafd->pwfx, pdi->cbwfx) == 0) /* same detailed format */ )
	{
		// Get some details.
		ACMDRIVERDETAILS dd;
		dd.cbStruct = sizeof(dd);
		MMRESULT mmr = acmDriverDetails(hadid, &dd, 0);
		if (mmr) {
			printf("we have an acmDriverDetails error: %d\n", mmr);
		} 
		else {
			printf("   Short name: %s\n", dd.szShortName);
			printf("   Long name:  %s\n", dd.szLongName);
			printf("   Copyright:  %s\n", dd.szCopyright);
			printf("   Licensing:  %s\n", dd.szLicensing);
			printf("   Features:   %s\n", dd.szFeatures);
			printf("   Supports %u formats\n", dd.cFormatTags);
			printf("   Supports %u filter formats\n", dd.cFilterTags);
		}
		// found it
		pdi->hadid = hadid;
		printf("    %4.4lXH, %s\n", pafd->dwFormatTag, pafd->szFormat);
		return FALSE; // stop enumerating
	}
	
	return TRUE; // continue enumerating
}

BOOL CALLBACK FindDriverCallBack(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;

	MMRESULT mmr = 0;		/* ACM API call result*/
	HACMDRIVER had = NULL;	/* ACM Driver handle*/
	DWORD dwSize = 0;
	ACMFORMATDETAILS fd;
	WAVEFORMATEX* pwf = NULL;

	BOOL bContinue = FALSE;

	do 
	{
		//open ACM driver
		mmr = acmDriverOpen(&had, hadid, 0);
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("FindDriverCallBack - acmDriverOpen failed: %u"), mmr);
			break;
		}
		
		// enumerate the formats it supports
		mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("acmMetrics failed: %u"), mmr);
			break;
		}

		//create WaveFormat buffer
		if (dwSize < sizeof(WAVEFORMATEX)) 
		{
			dwSize = sizeof(WAVEFORMATEX);
		}
		pwf = (WAVEFORMATEX*)(new char[dwSize]);
		if(pwf == NULL)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("create waveformat buffer failed: dwSize=%u"), dwSize);
			break;
		}

		//init the ACMFORMATDETAILS
		memset(pwf, 0, dwSize);
		pwf->cbSize = LOWORD(dwSize - sizeof(WAVEFORMATEX));
		pwf->wFormatTag = pdi->wFormatTag;
		memset(&fd, 0, sizeof(fd));
		fd.cbStruct = sizeof(fd);
		fd.pwfx = pwf;
		fd.cbwfx = dwSize;
		fd.dwFormatTag = pdi->wFormatTag;

		//enumerate all the formats in the driver
		mmr = acmFormatEnum(had, &fd, FindFormatCallBack, (DWORD)pdi, 0);
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("acmFormatEnum failed: %u"), mmr);
			break;
		}

		//close the driver
		mmr = acmDriverClose(had, 0);
		had = NULL;
		if(mmr != 0)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("acmDriverClose failed: %u"), mmr);
			break;
		}

		//check if successful
		if(pdi->hadid != NULL)
		{
			//found it, stop enumerating
			break;
		}
		
		bContinue = TRUE;

	} while (FALSE);

	//Clean up
	if(pwf != NULL)
	{
		delete[] ( reinterpret_cast<char*>(pwf) );
		pwf = NULL;
	}
	if(had != NULL)
	{
		acmDriverClose(had, 0);
		had = NULL;
	}

	return bContinue;
}

CAcmConverter::CAcmConverter()
{

}

CAcmConverter::~CAcmConverter()
{

}

BOOL CAcmConverter::FindDriver(PFIND_DRIVER_INFO pfdi)
{
	pfdi->hadid = NULL;
	
	MMRESULT mmr;
	mmr = acmDriverEnum(FindDriverCallBack, (DWORD)pfdi, 0);
	if(mmr)
	{
		_stprintf(m_szErrorMsg, _T("acmDriverEnum failed: %u"), mmr);
	}
	return mmr == 0;
}

BOOL CAcmConverter::Convert(HACMDRIVERID hadid, ACMSTREAMHEADER* pash, WAVEFORMATEX* pwfxSrc, WAVEFORMATEX* pwfxDst)
{
	BOOL bRet = FALSE;

	MMRESULT mmr;
	HACMSTREAM has = NULL;
	HACMDRIVER had = NULL;

	do 
	{
		//open the ACM driver
		mmr = acmDriverOpen(&had, hadid, 0);
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("Convert - acmDriverOpen failed: %u"), mmr);
			break;
		}
		
		//open ACM stream
		mmr = acmStreamOpen(&has,
			had, // Any driver
			pwfxSrc, // Source format
			pwfxDst, // Destination format
			NULL, // No filter
			NULL, // No callback
			0, // Instance data (not used)
			ACM_STREAMOPENF_NONREALTIME); // flags
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("acmStreamOpen failed: %u"), mmr);
			break;
		}

		//prepare the header
		mmr = acmStreamPrepareHeader(has, pash, 0);
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("acmStreamPrepareHeader failed: %u"), mmr);
			break;
		}

		//debug
		printf("start converting...\n");
		mmr = acmStreamConvert(has, pash, 0);
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("acmStreamConvert failed: %u"), mmr);
			acmStreamUnprepareHeader(has, pash, 0);
			break;
		}

		//write data
		FILE* fp = fopen("test.pcm", "wb");
		if(fp != NULL)
		{
			fwrite(pash->pbDst, 1, pash->cbDstLengthUsed, fp);
			fclose(fp);
			fp = NULL;
		}

		//Unprepare
		mmr = acmStreamUnprepareHeader(has, pash, 0);
		if(mmr)
		{
			_stprintf(CAcmConverter::m_szErrorMsg, _T("acmStreamUnprepareHeader failed: %u"), mmr);
			break;
		}

		bRet = TRUE;

	} while (FALSE);

	if(has != NULL)
	{
		// Close the stream.
		acmStreamClose(has, 0);
	}
	if(had != NULL)
	{
		acmDriverClose(had, 0);
	}

	return bRet;
}