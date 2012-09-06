// LameDecoder.h: interface for the CLameDecoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAME2DECODER_H__6211E54A_E1DA_46FE_B115_0E42844AB1C1__INCLUDED_)
#define AFX_LAME2DECODER_H__6211E54A_E1DA_46FE_B115_0E42844AB1C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "lame.h"

class CDebugSupportA;
class BlockBufferInputStream;

class CLameDecoder  
{
public:
	CLameDecoder();
	virtual ~CLameDecoder();

public:
	int Init();
	int DeInit();
	int Decode1Header(unsigned char* mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct* mp3data);

private:
	int DecodeFileHeader();
	int DecodeMpegHeader(short pcm_l[], short pcm_r[], mp3data_struct* mp3data);
	int DecodeMpegData(short pcm_l[], short pcm_r[], mp3data_struct* mp3data);

	static int is_syncword_mp123(const void * headerptr);
private:
	enum
	{
		MAX_U_32_NUM = 0xFFFFFFFF,

		RET_ERROR = -1,
		RET_NEED_MORE_DATA = 0,
		RET_OK = 1,

		STATE_IN_FILE_HEADER = 0,
		STATE_IN_MPEG_HEADER = 1,
		STATE_IN_MPEG_DATA_1 = 2,	/* first time in mpeg data*/
		STATE_IN_MPEG_DATA_N = 3,   /* non-1st time in mpeg data*/

		STATE_INIT = STATE_IN_FILE_HEADER
	};
	CDebugSupportA*		m_pDebug;
	lame_t				m_lame;
	hip_t				m_hip;

	int					m_nDelay;
	int					m_nPadding;
	int					m_nState;
	int					m_nTotalSamples;	/* total samples decoded until now */

	BlockBufferInputStream* m_is;
};

#endif // !defined(AFX_LAME2DECODER_H__6211E54A_E1DA_46FE_B115_0E42844AB1C1__INCLUDED_)
