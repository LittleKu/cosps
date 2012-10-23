#include "LameFileDecoder.h"
#include "LameDecoder.h"
#include "SampleParams.h"

LameFileDecoder::LameFileDecoder() : m_pFile(NULL), m_pDecoder(NULL), m_mp3buf(NULL), m_pcmbuf(NULL), m_nDecodedInOpen(0)
{
	//decoder
	m_pDecoder = new LameDecoder();

	//mp3 buffer
	m_mp3buf = new byte[MP3_BUFFER_SIZE];
	memset(m_mp3buf, 0, sizeof(byte) * MP3_BUFFER_SIZE);

	//pcm buffer
	m_pcmbuf = new short* [MAX_PCM_CHANNELS];
	for(int i = 0; i < MAX_PCM_CHANNELS; i++)
	{
		m_pcmbuf[i] = new short[SAMPLES_PER_FRAME];
		memset(m_pcmbuf[i], 0, sizeof(short) * SAMPLES_PER_FRAME);
	}

	//mp3 struct data
	memset(&m_mp3data, 0, sizeof(m_mp3data));
}
LameFileDecoder::~LameFileDecoder()
{
	//free decoder instance
	if(m_pDecoder != NULL)
	{
		delete m_pDecoder;
		m_pDecoder = NULL;
	}

	//free mp3 buffer
	if(m_mp3buf != NULL)
	{
		delete [] m_mp3buf;
		m_mp3buf = NULL;
	}

	//free pcm buffer
	if(m_pcmbuf != NULL)
	{
		for(int i = 0; i < MAX_PCM_CHANNELS; i++)
		{
			delete [] m_pcmbuf[i];
		}
		delete [] m_pcmbuf;
		m_pcmbuf = NULL;
	}
}

int LameFileDecoder::Open(SampleContainer& samples, SampleContext& context)
{
	m_pFile = _tfopen(SampleParams::GetInputFile(context), _T("rb"));
	if(m_pFile == NULL)
	{
		SetErrorMsg(context, _T("[Open]: failed open file %s"), SampleParams::GetInputFile(context));
		return -1;
	}

	int ret = m_pDecoder->Init();
	if(ret != 0)
	{
		return ret;
	}

	int nRead;
	while( (nRead = fread(m_mp3buf, 1, MP3_BUFFER_SIZE, m_pFile)) > 0 )
	{
		//parse mp3 file header until get the struct data
		ret = ParseMpegHeader(m_mp3buf, nRead, &m_mp3data, &samples);

		//negative value indicates error
		if(ret < 0)
		{
			return ret;
		}

		//found mp3 struct data
		if(ret > 0)
		{
			break;
		}
		//0 indicates no error and mp3 struct data
	}

	return 0;
}

/*
return values: 
< 0 error
= 0 need more data
> 0 succeed
*/
int LameFileDecoder::ParseMpegHeader(unsigned char* mp3buffer, size_t len, 
									 mp3data_struct* mp3data, SampleContainer* samples)
{
	int nSamples; 
	//try to decode at most 1 frame
	nSamples = m_pDecoder->Decode1FrameRaw(mp3buffer, len, m_pcmbuf[0], m_pcmbuf[1], mp3data);
	
	//decoder error
	if(nSamples < 0)
	{
		return nSamples;
	}
	
	//not found mp3 header yet
	if(mp3data->header_parsed != 1)
	{
		return 0;
	}
	
	//mp3 struct data has been parsed
	CFL_TRACEA("header parsed. channels=%d, samplerate=%d\n", mp3data->stereo, mp3data->samplerate);
	int nChannels = mp3data->stereo;
	if(nChannels <= 0)
	{
		return -1;
	}
	if(nChannels > 2)
	{
		CFL_TRACEA("[warning]: only support 2 channels.");
		nChannels = 2;
	}
	//set source sample traits
	samples->SetSourceTraits(16, nChannels, mp3data->samplerate, SampleContainer::FLAG_CHANNELIZED);

	//if there is some sample data decoded
	m_nDecodedInOpen = nSamples;
	
	//positive number indicates we have successfully parsed header data
	return 1;
}

int LameFileDecoder::Decode(SampleContainer& samples, SampleContext& context)
{
	int ret = 0;

	//clean up buffer
	if(m_nDecodedInOpen > 0)
	{
		ret = m_nDecodedInOpen;
		samples.PutSamplesChannelized((void**)m_pcmbuf, m_nDecodedInOpen);
		m_nDecodedInOpen = 0;

		return ret;
	}


	memset(&m_mp3data, 0, sizeof(m_mp3data));

	int nSamples, nBufLen = 0;
	do 
	{
		//first time set nBufLen to 0, this will make the buffer data first to be used up.
		//try to decode at most 1 frame
		nSamples = m_pDecoder->Decode1FrameRaw(m_mp3buf, nBufLen, m_pcmbuf[0], m_pcmbuf[1], &m_mp3data);

		//decoder error
		if(nSamples < 0)
		{
			return nSamples;
		}

		//some data decoded
		if(nSamples > 0)
		{
			samples.PutSamplesChannelized((void**)m_pcmbuf, nSamples);
			return nSamples;
		}

		//nSamples is 0, decoder needs more data to feed
		nBufLen = fread(m_mp3buf, 1, MP3_BUFFER_SIZE, m_pFile);

	} while (nBufLen > 0);
	
	return 0;
}

int LameFileDecoder::Close(SampleContainer& samples, SampleContext& context)
{
	if(m_pFile == NULL)
	{
		return 0;
	}

	int ret = 0;
	ret = m_pDecoder->DeInit();

	fclose(m_pFile);

	return ret;
}
