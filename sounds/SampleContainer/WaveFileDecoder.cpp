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

int WaveFileDecoder::Init(const TCHAR* pFileName, SampleContainer& sampleContainer)
{
	m_pFile = _tfopen(pFileName, _T("rb"));
	if(m_pFile == NULL)
	{
		SetErrorMsg(_T("[Init]: failed open file %s"), pFileName);
		return -1;
	}

	int ret = ParseWaveHeader();
	if(ret != 0)
	{
		return ret;
	}

	m_nBufLen = SAMPLE_COUNT * m_wfx.nChannels * (m_wfx.wBitsPerSample >> 3);
	if(m_nBufLen <= 0)
	{
		SetErrorMsg(_T("Parsed buffer length is invalid. m_nBufLen=%d"), m_nBufLen);
		return -1;
	}

	m_pBuffer = new byte[m_nBufLen];
	if(m_pBuffer == NULL)
	{
		SetErrorMsg(_T("Allocate buffer failed. m_nBufLen=%d"), m_nBufLen);
		return -1;
	}

	SampleContainer::SampleTraits st;
	st.nBitsPerSample = m_wfx.wBitsPerSample;
	st.nChannels = m_wfx.nChannels;
	st.nSampleRate = m_wfx.nSamplesPerSec;
	st.nFlags = SampleContainer::FLAG_INTERLEAVED;

	sampleContainer.SetSourceTraits(&st);

	return 0;
}

int WaveFileDecoder::ParseWaveHeader()
{
	FILE* pFileSrc = m_pFile;

	m_nDataLen = 0;
	
	char buf[32], msg[256];
	int nRead;
	
	/*Read RIFF file header*/
	nRead = fread(buf, 1, 12, pFileSrc);
	if(nRead != 12)
	{
		SetErrorMsg(_T("Read RIFF header length failed. length=%d"), nRead);
		return -1;
	}
	
	/* starts with RIFF? */
	if(memcmp(buf, "RIFF", 4) != 0)
	{
		SetErrorMsg(_T("Can't find 'RIFF' tag"));
		return -1;
	}
	
	int nRiffLen = cfl::bytes_to_int32((const byte*)(buf + 4));
	
	if(memcmp(buf + 8, "WAVE", 4) != 0)
	{
		SetErrorMsg(_T("Can't find 'WAVE' tag"));
		return -1;
	}
	
	int nChunkSize;
	while(true)
	{
		nRead = fread(buf, 1, 8, pFileSrc);
		if(nRead != 8)
		{
			SetErrorMsg(_T("Read chunk ID failed: %d"), nRead);
			return -1;
		}
		
		nChunkSize = cfl::bytes_to_int32((const byte*)(buf + 4));
		
		//format chunk
		if(memcmp(buf, "fmt ", 4) == 0)
		{
			if(nChunkSize < 16)
			{
				SetErrorMsg(_T("Wave Format length is too small: %d"), nChunkSize);
				return -1;
			}
			
			memset(&m_wfx, 0, sizeof(m_wfx));
			nRead = fread(&m_wfx, 1, 16, pFileSrc);
			if(nRead != 16)
			{
				SetErrorMsg(_T("Read Wave Format failed. nRead=%d"), nRead);
				return -1;
			}
			
			if(m_wfx.wFormatTag != WAVE_FORMAT_PCM)
			{
				SetErrorMsg(_T("Unsupported data format: 0x%04X"), m_wfx.wFormatTag);
				return -1;
			}
			
			if(nChunkSize > 16)
			{
				if(fseek(pFileSrc, nChunkSize - 16, SEEK_CUR) != 0)
				{
					SetErrorMsg(_T("fseek failed"));
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
				CFL_TRACE(_T("fseek failed\n"));
				return -1;
			}
		}
	}
	
	return (m_nDataLen > 0) ? 0 : -1;
}

int WaveFileDecoder::Decode(SampleContainer& sampleContainer)
{
	int nSampleRemain = m_nDataLen / (m_wfx.nChannels * (m_wfx.wBitsPerSample >> 3));
	int n = min(nSampleRemain, SAMPLE_COUNT);

	int nRead = fread(m_pBuffer, (m_wfx.nChannels * (m_wfx.wBitsPerSample >> 3)), n, m_pFile);
	if(nRead != n)
	{
		SetErrorMsg(_T("[Decode] fread error. n=%d, nRead=%d"), n, nRead);
		return -1;
	}

	sampleContainer.PutSamples(m_pBuffer, nRead);

	return nRead;
}

int WaveFileDecoder::Done()
{
	if(m_pFile == NULL)
	{
		return 0;
	}
	fclose(m_pFile);
	m_pFile = NULL;

	if(m_pBuffer != NULL)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}

	return 0;
}
