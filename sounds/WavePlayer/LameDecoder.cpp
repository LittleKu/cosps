#include "LameDecoder.h"
#include "DebugSupport.h"

#pragma warning( disable : 4786 )
#include "BlockBufferInputStream.h"

#include <assert.h>

#define TRACE_PADDING(x, y) \
	{	\
		if(x > -1 || y > -1) \
		{ \
			if(x != m_nDelay || y != m_nPadding) \
			{ \
				TRACEA("delay and padding changed, nDelay=%d, m_nDelay=%d, nPadding=%d, m_nPadding=%d\n", x, m_nDelay, y, m_nPadding); \
			} \
		} \
	}

CLameDecoder::CLameDecoder()
{
	m_pDebug = new CDebugSupportA();
}

CLameDecoder::~CLameDecoder()
{
	if(m_pDebug != NULL)
	{
		delete m_pDebug;
		m_pDebug = NULL;
	}

	DeInit();
}

int CLameDecoder::Init()
{
	m_lame = lame_init();
	if(m_lame == NULL)
	{
		m_pDebug->Format("[lame_init] failed");
		return -1;
	}
	//set lame parameters
	lame_set_decode_only(m_lame, 1);
	lame_set_num_samples(m_lame, MAX_U_32_NUM);
	
	//init decoder
	m_hip = hip_decode_init();
	if(m_hip == NULL)
	{
		m_pDebug->Format("[hip_decode_init] failed");
		return -1;
	}

	m_is = new BlockBufferInputStream();
	
	//other internal parameters
	m_nDelay = -1;
	m_nPadding = -1;
	m_nState = STATE_INIT;
	m_nTotalSamples = 0;
	
	return 0;
}
int CLameDecoder::DeInit()
{
	//other internal parameters
	m_nDelay = -1;
	m_nPadding = -1;
	m_nState = STATE_INIT;
	m_nTotalSamples = 0;

	if(m_is != NULL)
	{
		m_is->Close();
		delete m_is;
		m_is = NULL;
	}

	hip_decode_exit(m_hip);
	m_hip = NULL;

	lame_close(m_lame);
	m_lame = NULL;

	return 0;
}

int CLameDecoder::Decode1Header(unsigned char* mp3buf, size_t len, 
								 short pcm_l[], short pcm_r[], mp3data_struct* mp3data)
{
	assert(m_nState >= STATE_IN_FILE_HEADER && m_nState <= STATE_IN_MPEG_DATA_N);

	int ret = RET_ERROR;

	//file header not parsed yet
	if(m_nState == STATE_IN_FILE_HEADER)
	{
		m_is->AddBlockBuffer(mp3buf, len);
		m_is->Reset();

		ret = DecodeFileHeader();
		//error or need more data
		if(ret <= 0)
		{
			return ret;
		}

		//file header parsed done
		m_nState = STATE_IN_MPEG_HEADER;

		//continue to parse MPEG header
		ret = DecodeMpegHeader(pcm_l, pcm_r, mp3data);
		if(mp3data->header_parsed == 1)
		{
			//header parsed
			m_nState = STATE_IN_MPEG_DATA_1;
		}
	}
	else if(m_nState == STATE_IN_MPEG_HEADER)
	{
		m_is->AddBlockBuffer(mp3buf, len);

		//parse MPEG header
		ret = DecodeMpegHeader(pcm_l, pcm_r, mp3data);
		if(mp3data->header_parsed == 1)
		{
			//header parsed
			m_nState = STATE_IN_MPEG_DATA_1;
		}
	}
	else if(m_nState == STATE_IN_MPEG_DATA_1)
	{
		m_is->AddBlockBuffer(mp3buf, len);

		int nDelay = -1, nPadding = -1;
		/* first see if we still have data buffered in the decoder: */
		ret = hip_decode1_headersB(m_hip, mp3buf, 0, pcm_l, pcm_r, mp3data, &nDelay, &nPadding);
		TRACE_PADDING(nDelay, nPadding)

		//only when buffer is empty, switch to N
		if(ret == 0)
		{
			m_nState = STATE_IN_MPEG_DATA_N;
		}
	}
	else if(m_nState == STATE_IN_MPEG_DATA_N)
	{
		if(m_is->Empty())
		{
			int nDelay = -1, nPadding = -1;
			ret = hip_decode1_headersB(m_hip, mp3buf, len, pcm_l, pcm_r, mp3data, &nDelay, &nPadding);
			TRACE_PADDING(nDelay, nPadding)
		}
		else
		{
			m_is->AddBlockBuffer(mp3buf, len);
			ret = DecodeMpegData(pcm_l, pcm_r, mp3data);
		}
	}
	
	if(ret <= 0)
	{
		return ret;
	}

	//TODO: add delay and padding support
	//modify pcm_l and pcm_r content due to m_nDelay and m_nPadding

	return ret;
}

int CLameDecoder::DecodeMpegData(short pcm_l[], short pcm_r[], mp3data_struct* mp3data)
{
	unsigned char buf[1024];
	int ret = 0, len, nDelay = -1, nPadding = -1;

	while( (len = m_is->Read(buf, sizeof(buf))) > 0 )
	{
		ret = hip_decode1_headersB(m_hip, buf, len, pcm_l, pcm_r, mp3data, &nDelay, &nPadding);
		TRACE_PADDING(nDelay, nPadding)
		//when ret is 0, lame need more data to feed
		if(ret != 0)
		{
			break;
		}
	}

	return ret;
}

int CLameDecoder::DecodeMpegHeader(short pcm_l[], short pcm_r[], mp3data_struct* mp3data)
{
	unsigned char buf[1024];
	int ret, len, enc_delay = -1, enc_padding = -1;
	memset(mp3data, 0, sizeof(*mp3data));

	len = m_is->Read(buf, sizeof(buf));
	if(len <= 0)
	{
		return RET_NEED_MORE_DATA;
	}
	/* now parse the current buffer looking for MP3 headers.    */
    /* (as of 11/00: mpglib modified so that for the first frame where  */
    /* headers are parsed, no data will be decoded.   */
    /* However, for freeformat, we need to decode an entire frame, */
    /* so mp3data->bitrate will be 0 until we have decoded the first */
    /* frame.  Cannot decode first frame here because we are not */
    /* yet prepared to handle the output. */
    ret = hip_decode1_headersB(m_hip, buf, len, pcm_l, pcm_r, mp3data, &enc_delay, &enc_padding);
	//error
	if(ret == -1)
	{
		return -1;
	}

	TRACEA("parsed=%d, enc_delay=%d, enc_padding=%d\n", mp3data->header_parsed, enc_delay, enc_padding);
    /* repeat until we decode a valid mp3 header.  */
    while (!mp3data->header_parsed) 
	{
        len = m_is->Read(buf, sizeof(buf));
		if(len <= 0)
		{
			return RET_NEED_MORE_DATA;
		}

        ret = hip_decode1_headersB(m_hip, buf, len, pcm_l, pcm_r, mp3data, &enc_delay, &enc_padding);
		//error
		if(ret == -1)
		{
			return -1;
		}
    }

	m_nDelay = enc_delay;
	m_nPadding = enc_padding;

	TRACE("enc_delay=%d, enc_padding=%d\n", enc_delay, enc_padding);

	TRACE("stereo=%d\n", mp3data->stereo);
	TRACE("samplerate=%d\n", mp3data->samplerate);
	TRACE("bitrate=%d\n", mp3data->bitrate);
	TRACE("framesize=%d\n", mp3data->framesize);
	TRACE("nsamp=%d\n", mp3data->nsamp);
	TRACE("totalframes=%d\n", mp3data->totalframes);
	TRACE("framenum=%d\n", mp3data->framenum);

    if (mp3data->totalframes > 0) 
	{
        /* mpglib found a Xing VBR header and computed nsamp & totalframes */
		TRACE("mpglib found a Xing VBR header and computed nsamp & totalframes...\n");
    }
    else 
	{
        /* set as unknown.  Later, we will take a guess based on file size
         * ant bitrate */
        mp3data->nsamp = 0xFFFFFFFF;
    }

    return ret;
}

int CLameDecoder::DecodeFileHeader()
{
	unsigned char buf[100];
	int len, aid_header, freeformat = 0;
	
    len = 4;
    if(m_is->Read(buf, len) != len)
	{
		/* failed, not enough data */
		return RET_NEED_MORE_DATA;
	}  
	
	//try to skip ID3v2 tag header
    if (buf[0] == 'I' && buf[1] == 'D' && buf[2] == '3') 
	{
		TRACEA("ID3v2 found. " "Be aware that the ID3 tag is currently lost when transcoding.\n");
		
        len = 6;
        if (m_is->Read(buf, len) != len)
		{
			return RET_NEED_MORE_DATA;
		}
		
        buf[2] &= 127;
        buf[3] &= 127;
        buf[4] &= 127;
        buf[5] &= 127;
        len = (((((buf[2] << 7) + buf[3]) << 7) + buf[4]) << 7) + buf[5];
		
        if(m_is->Skip(len) != (int)len)
		{
			return RET_NEED_MORE_DATA;
		}
		
        len = 4;
        if (m_is->Read(buf, len) != len)
		{
			return RET_NEED_MORE_DATA;
		}
    }
	
	//Check Album ID
    aid_header = (0 == memcmp(buf, "AiD\1", 4));
    if (aid_header) 
	{
        if (m_is->Read(buf, 2) != 2)
		{
			return RET_NEED_MORE_DATA;
		}
        aid_header = (unsigned char) buf[0] + 256 * (unsigned char) buf[1];
		
        TRACEA("Album ID found.  length=%i \n", aid_header);

        /* skip rest of AID, except for 6 bytes we have already read */
		if(m_is->Skip(aid_header - 6) != (int)(aid_header - 6))
		{
			return RET_NEED_MORE_DATA;
		}
		
        /* read 4 more bytes to set up buffer for MP3 header check */
        if (m_is->Read(buf, len) != len)
		{
			return RET_NEED_MORE_DATA;
		}
    }

	//MP3 Frame header start, stop until find the sync word
    len = 4;
	int i = 0, nSkip = 0;
    while (!is_syncword_mp123(buf)) 
	{
		nSkip++;
		//Skip at most 2048 bytes, set this limit to make sure we are not going to parse a corrupted file
		if(nSkip >= 2048)
		{
			TRACEA("Can't find sync word by skipping (%d) bytes, this stream seems corrupted\n", nSkip);
			break;
		}
		for(i = 0; i < len - 1; i++)
		{
			buf[i] = buf[i + 1];
		}
		if( m_is->Read(buf + len - 1, 1) != 1 )
		{
			return RET_NEED_MORE_DATA;
		}
    }

	int nCurPos = m_is->GetPos();
	assert(nCurPos >= 4);

	m_is->Reset(nCurPos - 4);
	
	return RET_OK;
}


/*
* header analysis for a MPEG-1/2/2.5 Layer I, II or III data stream
*/
int CLameDecoder::is_syncword_mp123(const void * headerptr)
{
    const unsigned char * p = (const unsigned char *)headerptr;
    static const char abl2[16] = { 0, 7, 7, 7, 0, 7, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8 };
	
    if ((p[0] & 0xFF) != 0xFF)
        return 0;       /* first 8 bits must be '1' */
    if ((p[1] & 0xE0) != 0xE0)
        return 0;       /* next 3 bits are also */
    if ((p[1] & 0x18) == 0x08)
        return 0;       /* no MPEG-1, -2 or -2.5 */
    if ((p[1] & 0x06) == 0x00)
        return 0;       /* no Layer I, II and III */
    if ((p[2] & 0xF0) == 0xF0)
        return 0;       /* bad bitrate */
    if ((p[2] & 0x0C) == 0x0C)
        return 0;       /* no sample frequency with (32,44.1,48)/(1,2,4)     */
    if ((p[1] & 0x18) == 0x18 && (p[1] & 0x06) == 0x04 && abl2[p[2] >> 4] & (1 << (p[3] >> 6)))
        return 0;
    if ((p[3] & 3) == 2)
        return 0;       /* reserved emphasis mode */
    return 1;
}
