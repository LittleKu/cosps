// WaveConverter.h: interface for the CWaveConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVECONVERTER_H__59310803_F979_401E_A064_3A8196159B83__INCLUDED_)
#define AFX_WAVECONVERTER_H__59310803_F979_401E_A064_3A8196159B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmreg.h>
#include <msacm.h>

typedef struct
{
	HACMDRIVERID hadid;		/*[out]: ACM driver id found, NULL if not found */
	HACMDRIVER had;			/*[out]: ACM driver handle */
	WORD wFormatTag;		/*[in,out]: wave format want to find */
	LPWAVEFORMATEX pwfx;	/*[in]: detailed wave format */
	DWORD cbwfx;			/*[in]: Size, in bytes, available for pwfx to receive the format details.*/
} FIND_DRIVER_INFO, *PFIND_DRIVER_INFO;

class CWaveConverter  
{
public:
	CWaveConverter();
	virtual ~CWaveConverter();
	LPCTSTR GetLastErrorMsg() const
	{
		return this->m_szErrorMsg;
	}
public:
	static BOOL FindDriver(PFIND_DRIVER_INFO pfdi);
	static BOOL Convert(HACMDRIVER had, ACMSTREAMHEADER* pash, WAVEFORMATEX* pwfxSrc, WAVEFORMATEX* pwfxDst);
private:
	enum
	{
		ERROR_MSG_SIZE = 256
	};
	TCHAR m_szErrorMsg[ERROR_MSG_SIZE];	/* error message buffer */
};

#endif // !defined(AFX_WAVECONVERTER_H__59310803_F979_401E_A064_3A8196159B83__INCLUDED_)
