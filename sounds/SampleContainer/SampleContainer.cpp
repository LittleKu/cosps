#include "SampleContainer.h"
#include <stddef.h>
#include <assert.h>
#include <memory.h>

SampleContainer::SampleContainer() : m_pSamples(NULL), m_nCapacity(0), m_nSampleCount(0)
{
	memset(&source, 0, sizeof(source));
	memset(&target, 0, sizeof(target));
}

SampleContainer::~SampleContainer()
{
	FreeSampleBuffer();
}

void SampleContainer::SetSourceTraits(const SampleTraits* st)
{
	assert(st != NULL);
	assert(st->nBitsPerSample >= 8 && st->nBitsPerSample <= 32);
	source = *st;
	target = *st;
}
void SampleContainer::SetSourceTraits(int nBitsPerSample, int nChannels, int nSampleRate, int nFlags)
{
	SampleTraits st;
	st.nBitsPerSample = nBitsPerSample;
	st.nChannels = nChannels;
	st.nSampleRate = nSampleRate;
	st.nFlags = nFlags;

	SetSourceTraits(&st);
}
void SampleContainer::GetSourceTraits(SampleTraits* st)
{
	assert(st != NULL);
	*st = source;
}

void SampleContainer::SetTargetTraits(const SampleTraits* st)
{
	assert(st != NULL);
	assert(source.nBitsPerSample > 0);
	assert(source.nChannels == st->nChannels);
	target = *st;
}

void SampleContainer::SetTargetTraits(int nBitsPerSample, int nFlags)
{
	assert(source.nChannels > 0);
	assert(source.nBitsPerSample > 0 && nBitsPerSample > 0);
	target.nChannels = source.nChannels;
	target.nSampleRate = source.nSampleRate;
	target.nBitsPerSample = nBitsPerSample;
	target.nFlags = nFlags;
}

void SampleContainer::GetTargetTraits(SampleTraits* st)
{
	assert(st != NULL);
	*st = target;
}

void SampleContainer::PutSamplesInterleaved(void* pSamples, int nSampleCount)
{
	assert((source.nFlags & FLAG_CHANNELIZED) != FLAG_CHANNELIZED);
	PutSamples(pSamples, nSampleCount);
}
void SampleContainer::GetSamplesInterleaved(void*& pSamples, int& nSampleCount)
{
	assert((target.nFlags & FLAG_CHANNELIZED) != FLAG_CHANNELIZED);
	pSamples = m_pSamples;
	nSampleCount = m_nSampleCount;
}

void SampleContainer::PutSamplesChannelized(void** pSamples, int nSampleCount)
{
	assert(source.nFlags & FLAG_CHANNELIZED);
	PutSamples((void*)pSamples, nSampleCount);
}

void SampleContainer::GetSamplesChannelized(void**& pSamples, int& nSampleCount)
{
	assert(target.nFlags & FLAG_CHANNELIZED);
	pSamples = (void**)m_pSamples;
	nSampleCount = m_nSampleCount;
}

void SampleContainer::PutSamples(void* pSamples, int nSampleCount)
{
	// check if there is enough space in the buffer
	if(nSampleCount > m_nCapacity)
	{
		EnsureCapacity(nSampleCount);
	}
	
	int nSrcStep = GetStep(source);
	int nDstStep = GetStep(target);
	
	byte *pSrcBuf = NULL, *pDstBuf = NULL;
	for(int i = 0; i < source.nChannels; i++)
	{
		pSrcBuf = (byte*)GetSampleBuffer(source, pSamples, i);
		pDstBuf = (byte*)GetSampleBuffer(target, m_pSamples, i);
		
		Transform(pSrcBuf, nSrcStep, nSampleCount, pDstBuf, nDstStep);
	}
	
	m_nSampleCount = nSampleCount;
}

void SampleContainer::GetSamples(void*& pSamples, int& nSampleCount)
{
	pSamples = m_pSamples;
	nSampleCount = m_nSampleCount;
}

void SampleContainer::FreeSampleBuffer()
{
	if(m_pSamples == NULL)
	{
		return;
	}
	
	//channelized
	if(target.nFlags & FLAG_CHANNELIZED)
	{
		byte** buf = (byte**)m_pSamples;
		
		for(int i = 0; i < target.nChannels; i++)
		{
			delete [] buf[i];
		}
		delete [] buf;
	}
	//interleaved
	else
	{
		delete [] m_pSamples;
	}
	
	m_pSamples = NULL;
}

void SampleContainer::EnsureCapacity(int capacity)
{
	if(m_pSamples != NULL)
	{
		FreeSampleBuffer();
	}
	
	//capacity by channel in bytes
	int nCapByChannel = capacity * (target.nBitsPerSample>>3);

	//channelized
	if(target.nFlags & FLAG_CHANNELIZED)
	{
		byte** buf = new byte* [target.nChannels];
		for(int i = 0; i < target.nChannels; i++)
		{
			buf[i] = new byte[nCapByChannel];
		}
		m_pSamples = (byte*)buf;
	}
	//interleaved
	else
	{
		m_pSamples = new byte[target.nChannels * nCapByChannel];
	}

	m_nCapacity = capacity;
}

int SampleContainer::GetStep(const SampleTraits& st)
{
	int step = st.nBitsPerSample >> 3;
	//interleaved
	if((st.nFlags & FLAG_CHANNELIZED) != FLAG_CHANNELIZED)
	{
		step *= st.nChannels;
	}
	return step;
}

void* SampleContainer::GetSampleBuffer(const SampleTraits& st, void* samples, int channel)
{
	void* buf = NULL;
	
	//channelized
	if(st.nFlags & FLAG_CHANNELIZED)
	{
		buf = ((byte**)samples)[channel];
	}
	//interleaved	
	else
	{
		buf =  (byte*)(samples) + channel * (st.nBitsPerSample >> 3);
	}
	
	return buf;
}

int SampleContainer::Unpack(const unsigned char* ptr, int bytes, int flags)
{
	int x = 0, i, shift;
	
	//PCM U8
	if(bytes == 1 && ((flags & FLAG_PCM_S8) != FLAG_PCM_S8))
	{
		/* convert from unsigned */
		x = (ptr[0] ^ 0x80) << 24 | 0x7F << 16;
	}
	//Endian little
	else if((flags & FLAG_ENDIAN_BIG) != FLAG_ENDIAN_BIG)
	{
		for(i = bytes - 1, shift = 24; i >= 0; i--, shift -= 8)
		{
			x |= ((ptr[i] & 0xFF) << shift);
		}
	}
	//Endian big
	else
	{
		for(i = 0, shift = 24; i < bytes; i++, shift -= 8)
		{
			x |= ((ptr[i] & 0xFF) << shift);
		}
	}
	
	return x;
}

void SampleContainer::Pack(unsigned char* ptr, int bytes, int flags, int sample)
{
	int i, shift;
	//PCM U8
	if(bytes == 1 && ((flags & FLAG_PCM_S8) != FLAG_PCM_S8))
	{
		ptr[0] = (unsigned char)(((sample >> 24) ^ 0x80) & 0xFF);
	}
	//Endian little
	else if((flags & FLAG_ENDIAN_BIG) != FLAG_ENDIAN_BIG)
	{
		for(i = 0, shift = 32 - (bytes << 3); i < bytes; i++, shift += 8)
		{
			ptr[i] = (unsigned char)((sample >> shift) & 0xFF);
		}
	}
	//Endian big
	else
	{
		for(i = 0, shift = 24; i < bytes; i++, shift -= 8)
		{
			ptr[i] = (unsigned char)((sample >> shift) & 0xFF);
		}
	}
}

void SampleContainer::Transform(byte* pSrcBuf, int nSrcStep, int nSampleCount, byte* pDstBuf, int nDstStep)
{
	int nTargetShift = 32 - target.nBitsPerSample;
	int nRoundVar = nTargetShift > 0 ? (1 << (nTargetShift - 1)) : 0;
	int nTargetHigh = (1 << 31) - nRoundVar;
	
	int i, sample;
	for(i = 0; i < nSampleCount; i++)
	{
		//normalize: convert the source sample to 32 bit
		sample = Unpack(pSrcBuf, source.nBitsPerSample >> 3, source.nFlags);
		
		// add rounding value
		if (sample < nTargetHigh)
		{
			sample += nRoundVar;
		}
		
		//convert the 32 bit source sample to the destination byte buffer
		Pack(pDstBuf, target.nBitsPerSample >> 3, target.nFlags, sample);
		
		pDstBuf += nDstStep;
		pSrcBuf += nSrcStep;
	}
}