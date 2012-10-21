#include "WaveFileEncoder.h"
#include <assert.h>
#include <windows.h>
#include <MMSystem.h>
#include "cflbase/tstring.h"

const char* WaveFileEncoder::OUT_BITS_PER_SAMPLE = "WaveFileEncoder::OUT_BITS_PER_SAMPLE";

WaveFileEncoder::WaveFileEncoder() : m_pFile(NULL), m_nSamplesEncoded(0)
{
}

WaveFileEncoder::~WaveFileEncoder()
{
}

int WaveFileEncoder::WriteWaveHeader(FILE* fp, int nDataLength, int nSampleRate, int nChannels, int nBitsPerSample)
{
	WAVEFORMATEX wfx;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = (WORD)nChannels;
	wfx.nSamplesPerSec = nSampleRate;
	wfx.wBitsPerSample = (WORD)nBitsPerSample;
	wfx.cbSize = 0;
	wfx.nBlockAlign = (WORD)((wfx.wBitsPerSample * wfx.nChannels) >> 3);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	
	int val = nDataLength + 44 - 8;
	
	//RIFF Header
	fwrite("RIFF", 1, 4, fp);
	
	//File Length
	fwrite(&val, 4, 1, fp);
	fwrite("WAVEfmt ", 1, 8, fp); /* 2 labels */
	
	val = sizeof(wfx) - 2;
	
	//format chunk size
	fwrite(&val, 1, 4, fp);
	
	//format chunk content
	fwrite(&wfx, val, 1, fp);
	
	//data chunk ID
	fwrite("data", 1, 4, fp);
	//data chunk size
	fwrite(&nDataLength, 4, 1, fp);
	
	return ferror(fp);
}

int WaveFileEncoder::Open(SampleContainer& samples, SampleContext& context)
{
	m_pFile = _tfopen(SampleParams::GetOutputFile(context), _T("wb"));
	if(m_pFile == NULL)
	{
		SetErrorMsg(context, _T("[Open]: failed open file %s"), SampleParams::GetOutputFile(context));
		return -1;
	}
	
	SampleContainer::SampleTraits st;
	samples.GetSourceTraits(&st);
	
	int ret = WriteWaveHeader(m_pFile, 0x7FFFFFFF, st.nSampleRate, st.nChannels, st.nBitsPerSample);
	if(ret != 0)
	{
		SetErrorMsg(context, _T("[WriteWaveHeader]: failed. ret = %d"), ret);
		return ret;
	}
	
	int bps = context.GetInt(OUT_BITS_PER_SAMPLE, 16);
	bps = (bps + 7) & ~7;
	if(bps < 8 || bps > 32)
	{
		bps = 16;
	}

	samples.SetTargetTraits(bps, SampleContainer::FLAG_DEFAULT);
	CFL_TRACEA("Output Wave Format: bps=%d, channels=%d, samplerate=%d\n", bps, st.nChannels, st.nSampleRate);
	
	m_nSamplesEncoded = 0;
	
	return 0;
}

int WaveFileEncoder::Encode(SampleContainer& samples, SampleContext& context)
{
	void* pSamples = NULL;
	int nSampleCount = 0;
	
	samples.GetSamplesInterleaved(pSamples, nSampleCount);
	assert(nSampleCount >= 0);
	
	SampleContainer::SampleTraits st;
	samples.GetTargetTraits(&st);
	
	int nSize = (st.nChannels * (st.nBitsPerSample >> 3));
	int nWrite = fwrite(pSamples, nSize, nSampleCount, m_pFile);
	if(nWrite != nSampleCount)
	{
		SetErrorMsg(context, _T("[Encode]: fwrite error. nWrite=%d, nSampleCount=%d"), nWrite, nSampleCount);
		return -1;
	}
	
	m_nSamplesEncoded += nSampleCount;
	
	return 0;
}

int WaveFileEncoder::Close(SampleContainer& samples, SampleContext& context)
{
	if(m_pFile == NULL)
	{
		return 0;
	}
	
	int ret = 0;
	//no error?
	if(_tcscmp(SampleParams::GetErrorMsg(context)->c_str(), _T("")) == 0)
	{
		SampleContainer::SampleTraits st;
		samples.GetTargetTraits(&st);
		
		ret = fseek(m_pFile, 0, SEEK_SET);
		if(ret == 0)
		{
			ret = WriteWaveHeader(m_pFile, (m_nSamplesEncoded * st.nChannels * (st.nBitsPerSample >> 3)), 
				st.nSampleRate, st.nChannels, st.nBitsPerSample);
		}
		
		if(ret != 0)
		{
			SetErrorMsg(context, _T("[Close]: WriteWaveHeader failed"));
		}
	}
	
	if(fclose(m_pFile) != 0 && ret == 0)
	{
		SetErrorMsg(context, _T("[Close]: fclose failed"));
		ret = -1;
	}
	
	return ret;
}
