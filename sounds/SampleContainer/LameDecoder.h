#ifndef __LAME_DECODER_H__
#define __LAME_DECODER_H__

#pragma once

#include "cflbase/ByteBuffer.h"
#include "cflbase/ErrorMsg.h"
#include "lame.h"

class LameDecoder : public NS_CFL::ErrorMsg
{
public:
	LameDecoder();
	virtual ~LameDecoder();

	/* Init the decoder, return 0 if succeed, else non-zero returned*/
	int Init();
	int DeInit();
	
	/* decode at most 1 frame without skip padding support */
	int Decode1FrameRaw(unsigned char* mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct* mp3data);
private:
	int DecodeFileHeader();
	int DecodeMpegHeader(short pcm_l[], short pcm_r[], mp3data_struct* mp3data);
	int DecodeMpegData(short pcm_l[], short pcm_r[], mp3data_struct* mp3data);
	
	static int is_syncword_mp123(const void * headerptr);
private:
	enum
	{
		MAX_U_32_NUM = 0xFFFFFFFF,
		MAX_SAMPLES_PER_FRAME = 1152,
		
		RET_ERROR = -1,
		RET_NEED_MORE_DATA = 0,
		RET_OK = 1,
		
		STATE_IN_FILE_HEADER = 0,
		STATE_IN_MPEG_HEADER = 1,
		STATE_IN_MPEG_DATA_1 = 2,	/* first time in mpeg data*/
		STATE_IN_MPEG_DATA_N = 3,   /* non-1st time in mpeg data*/
		
		STATE_INIT = STATE_IN_FILE_HEADER
	};
	
	lame_t				m_lame;
	hip_t				m_hip;
	bool				m_bSkipPadding;
	
	int					m_nDelay;
	int					m_nPadding;
	int					m_nState;
	int					m_nTotalSamples;	/* total samples decoded until now */
	
	NS_CFL::ByteBuffer* m_is;
};

#endif