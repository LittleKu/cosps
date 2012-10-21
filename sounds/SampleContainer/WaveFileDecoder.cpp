#include "WaveFileDecoder.h"
#include "cflbase/byte_utils.h"
#include "cflbase/tstring.h"

WaveFileDecoder::WaveFileDecoder() : m_pFile(NULL), m_nDataLen(0), m_nSamplesRead(0), m_pBuffer(NULL)
{
	memset(&m_wfx, 0, sizeof(m_wfx));
}

WaveFileDecoder::~WaveFileDecoder()
{
	if(m_pBuffer != NULL)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}

int WaveFileDecoder::ParseWaveHeader(SampleContext& context)
{
	FILE* pFileSrc = m_pFile;

	m_nDataLen = 0;
	
	char buf[32], msg[256];
	int nRead;
	
	/*Read RIFF file header*/
	nRead = fread(buf, 1, 12, pFileSrc);
	if(nRead != 12)
	{
		SetErrorMsg(context, _T("Read RIFF header length failed. length=%d"), nRead);
		return -1;
	}
	
	/* starts with RIFF? */
	if(memcmp(buf, "RIFF", 4) != 0)
	{
		SetErrorMsg(context, _T("Can't find 'RIFF' tag"));
		return -1;
	}
	
	int nRiffLen = cfl::bytes_to_int32((const byte*)(buf + 4));
	
	if(memcmp(buf + 8, "WAVE", 4) != 0)
	{
		SetErrorMsg(context, _T("Can't find 'WAVE' tag"));
		return -1;
	}
	
	int nChunkSize;
	while(true)
	{
		nRead = fread(buf, 1, 8, pFileSrc);
		if(nRead != 8)
		{
			SetErrorMsg(context, _T("Read chunk ID failed: %d"), nRead);
			return -1;
		}
		
		nChunkSize = cfl::bytes_to_int32((const byte*)(buf + 4));
		
		//format chunk
		if(memcmp(buf, "fmt ", 4) == 0)
		{
			if(nChunkSize < 16)
			{
				SetErrorMsg(context, _T("Wave Format length is too small: %d"), nChunkSize);
				return -1;
			}
			
			memset(&m_wfx, 0, sizeof(m_wfx));
			nRead = fread(&m_wfx, 1, 16, pFileSrc);
			if(nRead != 16)
			{
				SetErrorMsg(context, _T("Read Wave Format failed. nRead=%d"), nRead);
				return -1;
			}
			
			if(m_wfx.wFormatTag != WAVE_FORMAT_PCM)
			{
				SetErrorMsg(context, _T("Unsupported data format: 0x%04X"), m_wfx.wFormatTag);
				return -1;
			}
			
			if(nChunkSize > 16)
			{
				if(fseek(pFileSrc, nChunkSize - 16, SEEK_CUR) != 0)
				{
					SetErrorMsg(context, _T("fseek failed"));
					return -1;
				}
			}
		}
		//data chunk
		else if(memcmp(buf, "data", 4) == 0)
		{
			m_nDataLen = nChunkSize;
			
			CFL_TRACE(_T("Found data chunk with length: %d(0x%08X)\n"), m_nDataLen, m_nDataLen);
			/* We've found the audio data. Read no further! */
            break;
		}
		//other chunks
		else
		{
			memcpy(msg, buf, 4);
			msg[4]= 0;
			CFL_TRACEA("Skipped chunk: %s\n", msg);
			
			if(fseek(pFileSrc, nChunkSize, SEEK_CUR) != 0)
			{
				SetErrorMsg(context, _T("fseek failed\n"));
				return -1;
			}
		}
	}
	
	return (m_nDataLen > 0) ? 0 : -1;
}

//! initializes the input module
int WaveFileDecoder::Open(SampleContainer& samples, SampleContext& context)
{
	m_pFile = _tfopen(SampleParams::GetInputFile(context), _T("rb"));
	if(m_pFile == NULL)
	{
		SetErrorMsg(context, _T("[Open]: failed open file %s"), SampleParams::GetInputFile(context));
		return -1;
	}
	
	int ret = ParseWaveHeader(context);
	if(ret != 0)
	{
		return ret;
	}
	
	m_nBufLen = SAMPLE_COUNT * m_wfx.nChannels * (m_wfx.wBitsPerSample >> 3);
	if(m_nBufLen <= 0)
	{
		SetErrorMsg(context, _T("Parsed buffer length is invalid. m_nBufLen=%d"), m_nBufLen);
		return -1;
	}
	
	m_pBuffer = new byte[m_nBufLen];
	if(m_pBuffer == NULL)
	{
		SetErrorMsg(context, _T("Allocate buffer failed. m_nBufLen=%d"), m_nBufLen);
		return -1;
	}
	
	SampleContainer::SampleTraits st;
	st.nBitsPerSample = m_wfx.wBitsPerSample;
	st.nChannels = m_wfx.nChannels;
	st.nSampleRate = m_wfx.nSamplesPerSec;
	st.nFlags = SampleContainer::FLAG_DEFAULT;
	
	samples.SetSourceTraits(&st);
	
	CFL_TRACEA("Input Wave Format: bps=%d, channels=%d, samplerate=%d\n", 
		st.nBitsPerSample, st.nChannels, st.nSampleRate);
	
	return 0;
}
/*! decodes samples and stores them in the sample container
returns number of samples decoded, or 0 if finished;
a negative value indicates an error */
int WaveFileDecoder::Decode(SampleContainer& samples, SampleContext& context)
{
	int nSampleRemain = m_nDataLen / (m_wfx.nChannels * (m_wfx.wBitsPerSample >> 3)) - m_nSamplesRead;
	int n = min(nSampleRemain, SAMPLE_COUNT);

	int nRead = fread(m_pBuffer, (m_wfx.nChannels * (m_wfx.wBitsPerSample >> 3)), n, m_pFile);
	if(nRead != n)
	{
		SetErrorMsg(context, _T("[Decode] fread error. n=%d, nRead=%d"), n, nRead);
		return -1;
	}
	
	samples.PutSamples(m_pBuffer, nRead);
	
	m_nSamplesRead += nRead;
	
	return nRead;
}
//! called when done with decoding
int WaveFileDecoder::Close(SampleContainer& samples, SampleContext& context)
{
	if(m_pBuffer != NULL)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}

	if(m_pFile == NULL)
	{
		return 0;
	}

	int ret = fclose(m_pFile);
	if(ret != 0)
	{
		SetErrorMsg(context, _T("[Close] fclose error. ret=%d"), ret);
	}
	
	return ret;
}
