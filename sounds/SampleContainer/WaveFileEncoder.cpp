#include "WaveFileEncoder.h"
#include <assert.h>

WaveFileEncoder::WaveFileEncoder() : m_pFile(NULL)
{
}

WaveFileEncoder::~WaveFileEncoder()
{
}

int WaveFileEncoder::Init(const TCHAR* pFileName, SampleContainer& sampleContainer)
{
	m_pFile = _tfopen(pFileName, _T("wb"));
	if(m_pFile == NULL)
	{
		SetErrorMsg(_T("[Init]: failed open file %s"), pFileName);
		return -1;
	}

	//TODO: write wave file header
	sampleContainer.SetTargetTraits(8, SampleContainer::FLAG_INTERLEAVED);
	
	return 0;
}

int WaveFileEncoder::Encode(SampleContainer& sampleContainer)
{
	void* pSamples = NULL;
	int nSampleCount = 0;

	sampleContainer.GetSamplesInterleaved(pSamples, nSampleCount);
	assert(nSampleCount >= 0);

	SampleContainer::SampleTraits st;
	sampleContainer.GetTargetTraits(&st);
	
	int nSize = (st.nChannels * (st.nBitsPerSample >> 3));
	int nWrite = fwrite(pSamples, nSize, nSampleCount, m_pFile);
	if(nWrite != nSampleCount)
	{
		SetErrorMsg(_T("[Encode]: fwrite error. nWrite=%d, nSampleCount=%d"), nWrite, nSampleCount);
		return -1;
	}
	
	return 0;
}

int WaveFileEncoder::Done()
{
	if(m_pFile == NULL)
	{
		return 0;
	}
	fclose(m_pFile);

	return 0;
}