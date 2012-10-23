#ifndef __LAME_FILE_ENCODER_H__
#define __LAME_FILE_ENCODER_H__

#pragma once

#include "cflbase/cflbase_config.h"
#include "SampleInterface.h"
#include "lame.h"

class LameFileEncoder : public SampleEncoder
{
public:
	LameFileEncoder();
	virtual ~LameFileEncoder();

	//! initializes the output module
	virtual int Open(SampleContainer& samples, SampleContext& context);

	/* encodes samples from the sample container
	it is required that all samples from the container will be used up;
	returns 0 if all was ok, or a negative value on error */
	virtual int Encode(SampleContainer& samples, SampleContext& context);

	//! cleans up the output module
	virtual int Close(SampleContainer& samples, SampleContext& context);
private:
	int WriteMP3Buffer(int nDataLen, SampleContext& context);
	int WriteTagBuffer(int nDataLen, int nMaxLen, const char* msg, SampleContext& context);
	int EncodeFlush(SampleContext& context);
	short* GetSampleR(SampleContainer& samples, void** pcmbuf);
private:
	enum
	{
		SAMPLES_PER_FRAME = 1152,
		MAX_PCM_CHANNELS = 2
	};
	FILE*	m_pFile;
	lame_t	m_lame;
	int		m_nErrorCode;
	int		m_nSamplesEncoded;

	byte*	m_mp3buf;	/* mp3 data buffer */
	short*	m_pcmSilentBuf;
	size_t	m_nID3v2Size;
};

#endif