#ifndef __SAMPLE_CONTAINER_H__
#define __SAMPLE_CONTAINER_H__

#pragma once

class SampleContainer
{
public:
	enum SampleFlag
	{
		FLAG_DEFAULT		= 0,	/* default SampleTraits flag */
		FLAG_CHANNELIZED	= 0x01,	/* sample data is in channelized format */
		FLAG_ENDIAN_BIG		= 0x02,	/* sample data is in endian-big format */	
		FLAG_PCM_S8			= 0x04	/* sample is signed 8 bits (when nBitsPerSample = 8) */
	};

	typedef struct  
	{
		int nBitsPerSample;
		int nChannels;
		int nSampleRate;
		int nFlags;
	} SampleTraits;

	//ctor
	SampleContainer();

	//dtor
	virtual ~SampleContainer();

public:
	void SetSourceTraits(const SampleTraits* st);
	void GetSourceTraits(SampleTraits* st);

	void SetSourceTraits(int nBitsPerSample, int nChannels, int nSampleRate, int nFlags);
	int GetSourceChannels() {return source.nChannels;}
	int GetSourceBitsPerSample() {return source.nBitsPerSample;}
	int GetSourceSampleRate() {return source.nSampleRate;}
	int GetSourceFlags() {return source.nFlags;}

	void SetTargetTraits(const SampleTraits* st);
	void GetTargetTraits(SampleTraits* st);

	void SetTargetTraits(int nBitsPerSample, int nFlags);
	int GetTargetChannels() {return target.nChannels;}
	int GetTargetBitsPerSample() {return target.nBitsPerSample;}
	int GetTargetSampleRate() {return target.nSampleRate;}
	int GetTargetFlags() {return target.nFlags;}

	// functions to put samples in or get samples out

	//! stores samples in interleaved format in the sample container
	void PutSamplesInterleaved(void* pSamples, int nSampleCount);
	//! retrieves samples in interleaved format
	void GetSamplesInterleaved(void*& pSamples, int& nSampleCount);
	
	//! stores samples in channelized format in the sample container
	void PutSamplesChannelized(void** pSamples, int nSampleCount);
	//! retrieves samples in channelized format
	void GetSamplesChannelized(void**& pSamples, int& nSampleCount);

	//stores the source samples into the container, and convert it into the target format
	void PutSamples(void* pSamples, int nSampleCount);
	//retrieves samples
	void GetSamples(void*& pSamples, int& nSampleCount);
protected:
	typedef unsigned char byte;

	//free the space of the sample buffer
	void FreeSampleBuffer();

	//(re)allocate the sample buffer
	void EnsureCapacity(int capacity);

	
	//calculate the step in bytes of the sample buffer
	inline int GetStep(const SampleTraits& st);

	//get the sample buffer starting address
	inline void* GetSampleBuffer(const SampleTraits& st, void* samples, int channel);

	//unpack the sample in bytes into integer format
	int Unpack(const unsigned char* ptr, int bytes, int flags);

	//pack the integer format sample into bytes format
	void Pack(unsigned char* ptr, int bytes, int flags, int sample);

	//transform the sample buffer in source format into target format
	void Transform(byte* pSrcBuf, int nSrcStep, int nSampleCount, byte* pDstBuf, int nDstStep);

protected:
	//! source traits
	SampleTraits source;

	//! target traits
	SampleTraits target;

	//! sample buffer
	void*	m_pSamples;

	//! sample buffer capacity (in samples)
	int		m_nCapacity;

	//! sample count (in samples)
	int		m_nSampleCount;
};

#endif