#ifndef __WAVE_FILE_ENCODER_H__
#define __WAVE_FILE_ENCODER_H__

#pragma once

#include "SampleInterface.h"

class WaveFileEncoder : public SampleEncoder
{
public:
	WaveFileEncoder();
	virtual ~WaveFileEncoder();

	//! initializes the output module
	virtual int Open(SampleContainer& samples, SampleContext& context);

	/* encodes samples from the sample container
	it is required that all samples from the container will be used up;
	returns 0 if all was ok, or a negative value on error */
	virtual int Encode(SampleContainer& samples, SampleContext& context);

	//! cleans up the output module
	virtual int Close(SampleContainer& samples, SampleContext& context);

	static const char* OUT_BITS_PER_SAMPLE;
private:
	int WriteWaveHeader(FILE* fp, int nDataLength, int nSampleRate, int nChannels, int nBitsPerSample);
private:
	FILE*	m_pFile;
	int		m_nSamplesEncoded;
};

#endif