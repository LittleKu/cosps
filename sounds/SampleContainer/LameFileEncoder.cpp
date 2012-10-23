#include "LameFileEncoder.h"
#include "cflbase/tstring.h"
#include "SampleParams.h"

LameFileEncoder::LameFileEncoder() : m_pFile(NULL), m_lame(NULL), m_nSamplesEncoded(0), m_nID3v2Size(0), m_nErrorCode(0)
{
	m_mp3buf = new byte[LAME_MAXMP3BUFFER];

	m_pcmSilentBuf = new short[SAMPLES_PER_FRAME];
}

LameFileEncoder::~LameFileEncoder()
{
	if(m_mp3buf != NULL)
	{
		delete [] m_mp3buf;
		m_mp3buf = NULL;
	}
}

int LameFileEncoder::Open(SampleContainer& samples, SampleContext& context)
{
	m_nErrorCode = -1;

	//1. Open the output file
	m_pFile = _tfopen(SampleParams::GetOutputFile(context), _T("wb"));
	if(m_pFile == NULL)
	{
		SetErrorMsg(context, _T("[Open]: failed open file %s"), SampleParams::GetOutputFile(context));
		return -1;
	}

	//2. Init lame
	m_lame = lame_init();
	if(m_lame == NULL)
	{
		SetErrorMsg(context, _T("[Open]: lame init failed"));
		return -1;
	}

	//3. Set lame parameters
	SampleContainer::SampleTraits st;
	samples.GetSourceTraits(&st);

	/* make sure the header is sane */
	if (-1 == lame_set_num_channels(m_lame, st.nChannels)) 
	{
		SetErrorMsg(context, _T("Unsupported number of channels: %u\n"), (uint32_t)st.nChannels);
		return -1;
	}
	if(st.nChannels <= 0 || st.nChannels > 2)
	{
		SetErrorMsg(context, _T("Unsupported number of channels: %d\n"), st.nChannels);
		return -1;
	}
	//sample rate
	lame_set_in_samplerate(m_lame, st.nSampleRate);
	/* turn off automatic writing of ID3 tag data into mp3 stream 
	* we have to call it before 'lame_init_params', because that
	* function would spit out ID3v2 tag data.
	*/
	lame_set_write_id3tag_automatic(m_lame, 0);
	
	//TODO: ID3 tags
	
	id3tag_add_v2(m_lame);
	id3tag_set_title(m_lame, "title: tell me what you required");
	id3tag_set_artist(m_lame, "Colin");
	id3tag_set_year(m_lame, "2012");
	id3tag_set_comment(m_lame, "ANy comment? come on");
	
	
	/* Now that all the options are set, lame needs to analyze them and
	* set some more internal options and check for problems
	*/
	int ret = 0;
    ret = lame_init_params(m_lame);
    if (ret < 0) 
	{
        SetErrorMsg(context, _T("[lame_init_params] failed"));
		return ret;
    }

	size_t nLen = 0;

	//is there id3v2 tag?
	nLen = lame_get_id3v2_tag(m_lame, m_mp3buf, LAME_MAXMP3BUFFER);
	if(WriteTagBuffer(nLen, LAME_MAXMP3BUFFER, "id3v3 tag", context) < 0)
	{
		return -1;
	}
	//save the id3 v3 tag size
	m_nID3v2Size = nLen;


	//channelized and 16 bits data
	samples.SetTargetTraits(16, SampleContainer::FLAG_CHANNELIZED);
	m_nSamplesEncoded = 0;
	m_nErrorCode = 0;

	return 0;
}

int LameFileEncoder::Encode(SampleContainer& samples, SampleContext& context)
{
	m_nErrorCode = -1;

	int nSampleCount = 0;
	void** pcmbuf = NULL;

	samples.GetSamplesChannelized(pcmbuf, nSampleCount);

	if(nSampleCount < 0)
	{
		SetErrorMsg(context, _T("Input sample count is less than 0, nSampleCount=%d"), nSampleCount);
		return -1;
	}

	int nEncoded = lame_encode_buffer(m_lame, (short*)pcmbuf[0], GetSampleR(samples, pcmbuf), nSampleCount, 
		m_mp3buf, LAME_MAXMP3BUFFER);

	//Write the encoded mp3 buffer into file
	if(WriteMP3Buffer(nEncoded, context) < 0)
	{
		return -1;
	}

	m_nSamplesEncoded += nSampleCount;

	m_nErrorCode = 0;
	return 0;
}

int LameFileEncoder::Close(SampleContainer& samples, SampleContext& context)
{
	int ret = 0;

	//no error
	if(m_nErrorCode == 0)
	{
		ret = EncodeFlush(context);
	}

	if(m_lame != NULL)
	{
		lame_close(m_lame);
		m_lame = NULL;
	}

	if(m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	return ret;
}

short* LameFileEncoder::GetSampleR(SampleContainer& samples, void** pcmbuf)
{
	if(samples.GetTargetChannels() == 2)
	{
		return (short*)pcmbuf[1];
	}
	memset(m_pcmSilentBuf, 0, sizeof(short) * SAMPLES_PER_FRAME);
	return m_pcmSilentBuf;
}

int LameFileEncoder::EncodeFlush(SampleContext& context)
{
	int nLen = 0;	
	/* Encode the last frame */
	nLen = lame_encode_flush(m_lame, m_mp3buf, LAME_MAXMP3BUFFER);
	if(WriteMP3Buffer(nLen, context) < 0)
	{
		return -1;
	}

	m_nSamplesEncoded += nLen;
	
	//ID3V1 Tag
	nLen = lame_get_id3v1_tag(m_lame, m_mp3buf, LAME_MAXMP3BUFFER);
	if(WriteTagBuffer(nLen, LAME_MAXMP3BUFFER, "id3v1 tag", context) < 0)
	{
		return -1;
	}
	
	//Write xing frame
	
	//seek to the first mp3 frame, skip id3v2 tag
	if(fseek(m_pFile, m_nID3v2Size, SEEK_SET) != 0)
	{
		SetErrorMsg(context, _T("error: can't skip id3v2 tag to write xing frame"));
		return -1;
	}
	
	nLen = lame_get_lametag_frame(m_lame, m_mp3buf, LAME_MAXMP3BUFFER);
	if(WriteTagBuffer(nLen, LAME_MAXMP3BUFFER, "lame xing tag", context) < 0)
	{
		return -1;
	}
	
	return 0;
}

int LameFileEncoder::WriteMP3Buffer(int nDataLen, SampleContext& context)
{
	/* was our output buffer big enough? */
	if (nDataLen < 0) 
	{
		if (nDataLen == -1)
			SetErrorMsg(context, _T("mp3 buffer is not big enough..."));
		else
			SetErrorMsg(context, _T("mp3 internal error:  error code=%i"), nDataLen);
		return nDataLen;
	}
	int nWrite = 0;
	if(nDataLen > 0)
	{
		nWrite = (int)fwrite(m_mp3buf, 1, nDataLen, m_pFile);
		if(nWrite != nDataLen)
		{
			SetErrorMsg(context, _T("Error writing mp3 output"));
			return -1;
		}
	}

	return nWrite;
}

int LameFileEncoder::WriteTagBuffer(int nDataLen, int nMaxLen, const char* msg, SampleContext& context)
{
	if(nDataLen > nMaxLen)
	{
		SetErrorMsg(context, _T("buffer is too small when writing [%s]. nDataLen=%d"), CFL_A2T(msg), nDataLen);
		return -1;
	}
	int nWrite = 0;
	if(nDataLen > 0)
	{
		nWrite = (int)fwrite(m_mp3buf, 1, nDataLen, m_pFile);
		if(nWrite != nDataLen)
		{
			SetErrorMsg(context, _T("writing [%s] failed. nDataLen=%d, nWrite=%d"), nDataLen, nWrite);
			return -1;
		}
	}

	return nWrite;
}