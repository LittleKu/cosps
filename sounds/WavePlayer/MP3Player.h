// MP3Player.h: interface for the CMP3Player class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MP3PLAYER_H__84B13D29_0102_4A43_AFC8_E4F6FF44221D__INCLUDED_)
#define AFX_MP3PLAYER_H__84B13D29_0102_4A43_AFC8_E4F6FF44221D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WavePlayer.h"

class CLameDecoder;

class CMP3Player  
{
public:
	CMP3Player();
	virtual ~CMP3Player();
	BOOL DeInit();
public:
	BOOL Play(LPCSTR lpszFileName);
private:
	int ToInterleavedPCMData(short pcm_l[], short pcm_r[], int nSamples, int nChannels, BYTE* lpBuffer);
	void Short2Bytes(short s, BYTE bytes[]);
private:
	enum
	{
		BUFFER_SIZE = 4096,
		MAX_FRAME_SIZE = 1152
	};
	CWavePlayer* m_pPlayer;		/* WAVE data player */
	CLameDecoder* m_pDecoder;	/* lame decoder */
};

#endif // !defined(AFX_MP3PLAYER_H__84B13D29_0102_4A43_AFC8_E4F6FF44221D__INCLUDED_)
