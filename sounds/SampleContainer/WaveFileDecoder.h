#ifndef __WAVE_FILE_DECODER_H__
#define __WAVE_FILE_DECODER_H__

#pragma once

#include "SampleInterface.h"
#include <Windows.h>
#include <MMSystem.h>

class WaveFileDecoder : public SampleDecoder
{
public:
	WaveFileDecoder();
	virtual ~WaveFileDecoder();

	//! initializes the input module
	virtual int Open(SampleContainer& samples, SampleContext& context);

	/*! decodes samples and stores them in the sample container
	returns number of samples decoded, or 0 if finished;
	a negative value indicates an error */
	virtual int Decode(SampleContainer& samples, SampleContext& context);

	//! called when done with decoding
	virtual int Close(SampleContainer& samples, SampleContext& context);
private:
	enum
	{
		SAMPLE_COUNT = 1024
	};
	int ParseWaveHeader(SampleContext& context);
private:
	FILE*	m_pFile;
	int		m_nDataLen;
	int		m_nSamplesRead;
	WAVEFORMATEX	m_wfx;
	byte*	m_pBuffer;
	int		m_nBufLen;
};
#endif