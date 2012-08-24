// WaveFilePlayer.h: interface for the CWaveFilePlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEFILEPLAYER_H__71A85565_0D00_4D96_ABE2_AC488A891F4B__INCLUDED_)
#define AFX_WAVEFILEPLAYER_H__71A85565_0D00_4D96_ABE2_AC488A891F4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WavePlayer.h"

class CWaveFilePlayer  
{
public:
	CWaveFilePlayer();
	virtual ~CWaveFilePlayer();

	BOOL Play(LPCSTR lpszFileName);

private:
	enum
	{
		BUFFER_SIZE = 4096
	};
	CWavePlayer* m_pPlayer;		/* WAVE data player */
	char m_buffer[BUFFER_SIZE];	/* WAVE data buffer */
};

#endif // !defined(AFX_WAVEFILEPLAYER_H__71A85565_0D00_4D96_ABE2_AC488A891F4B__INCLUDED_)
