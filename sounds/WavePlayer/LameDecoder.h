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
class CCircleBuffer;

class CLameDecoder  
{
public:
	CLameDecoder();
	virtual ~CLameDecoder();
	void SetSkipPadding(bool bSkipPadding);
public:
	/* Init the decoder, return 0 if succeed, else non-zero returned*/
	int Init();
	int DeInit();

	/* decode at most 1 frame with skip padding support if needed */
	int Decode1Frame(unsigned char* mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct* mp3data);

	/* decode at most 1 frame without skip padding support */
	int Decode1FrameRaw(unsigned char* mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct* mp3data);

	int GetSkipStart();
	int GetSkipEnd();

private:
	int DecodeFileHeader();
	int DecodeMpegHeader(short pcm_l[], short pcm_r[], mp3data_struct* mp3data);
	int DecodeMpegData(short pcm_l[], short pcm_r[], mp3data_struct* mp3data);

	static int is_syncword_mp123(const void * headerptr);
	static void ArrayCopy(short* src, int srcOff, short* dst, int dstOff, int length);
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

	class DelayInfo
	{
	public:
		DelayInfo(int nSkipEnd = 0);
		~DelayInfo();
	public:
// 		short* pBufferL;
// 		short* pBufferR;
// 
// 		int nBufferCap;
// 		int nBufferSize;
// 		int nBufferOff;
		CCircleBuffer* pBufferL;
		CCircleBuffer* pBufferR;

		int nStartSkipped;
	};

	friend class DelayInfo;

	CDebugSupportA*		m_pDebug;
	lame_t				m_lame;
	hip_t				m_hip;
	bool				m_bSkipPadding;

	int					m_nDelay;
	int					m_nPadding;
	int					m_nState;
	int					m_nTotalSamples;	/* total samples decoded until now */

	BlockBufferInputStream* m_is;
	DelayInfo*			m_pDelayInfo;
};

#endif // !defined(AFX_LAME2DECODER_H__6211E54A_E1DA_46FE_B115_0E42844AB1C1__INCLUDED_)
