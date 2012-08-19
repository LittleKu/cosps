// AcmConverter.h: interface for the CAcmConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACMCONVERTER_H__06BBD1B6_A54A_4051_89C0_90391353F4C0__INCLUDED_)
#define AFX_ACMCONVERTER_H__06BBD1B6_A54A_4051_89C0_90391353F4C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmreg.h>
#include <msacm.h>

typedef struct
{
	HACMDRIVERID hadid;		/*[out]: ACM driver id found, NULL if not found */
	WORD wFormatTag;		/*[in,out]: wave format want to find */
	LPWAVEFORMATEX pwfx;	/*[in]: detailed wave format */
	DWORD cbwfx;			/*[in]: Size, in bytes, available for pwfx to receive the format details.*/
} FIND_DRIVER_INFO, *PFIND_DRIVER_INFO;

class CAcmConverter  
{
public:
	CAcmConverter();
	virtual ~CAcmConverter();

	static BOOL FindDriver(PFIND_DRIVER_INFO pfdi);
	static BOOL Convert(HACMDRIVERID hadid, ACMSTREAMHEADER* pash, WAVEFORMATEX* pwfxSrc, WAVEFORMATEX* pwfxDst);
public:
	enum
	{
		ERROR_MSG_SIZE = 256
	};
	static TCHAR m_szErrorMsg[ERROR_MSG_SIZE];	/* error message buffer */
};

#endif // !defined(AFX_ACMCONVERTER_H__06BBD1B6_A54A_4051_89C0_90391353F4C0__INCLUDED_)
