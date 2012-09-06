// MP3Player.cpp: implementation of the CMP3Player class.
//
//////////////////////////////////////////////////////////////////////

#include "MP3Player.h"
#include "DebugSupport.h"
#include "LameDecoder.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMP3Player::CMP3Player() : m_pPlayer(NULL), m_pDecoder(NULL)
{
	m_pPlayer = new CWavePlayer();
	m_pDecoder = new CLameDecoder();
	BOOL bRet = FALSE;
	do 
	{
		if(!m_pPlayer->Init())
		{
			TRACEA("%s\n", "m_pPlayer Init failed");
			break;
		}

		bRet = TRUE;

	} while (FALSE);

	if(!bRet)
	{
		DeInit();
	}
}

CMP3Player::~CMP3Player()
{
	DeInit();
}

BOOL CMP3Player::DeInit()
{
	if(m_pDecoder != NULL)
	{
		m_pDecoder->DeInit();
		delete m_pDecoder;
		m_pDecoder = NULL;
	}
	if(m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	return TRUE;
}

void CMP3Player::Short2Bytes(short s, BYTE bytes[])
{
	bytes[0] = (BYTE)(s & 0xFF);
	bytes[1] = (BYTE)((s >> 8) & 0xFF);
}

int CMP3Player::ToInterleavedPCMData(short pcm_l[], short pcm_r[], int nSamples, int nChannels, BYTE* lpBuffer)
{
	int nOffset = 0;
	for(int i = 0; i < nSamples; i++)
	{
		Short2Bytes(pcm_l[i], lpBuffer + nOffset);
		nOffset += 2;

		if(nChannels == 2)
		{
			Short2Bytes(pcm_r[i], lpBuffer + nOffset);
			nOffset += 2;
		}
	}

	assert(nOffset == nSamples * nChannels * 2);

	return nOffset;
}

BOOL CMP3Player::Play(LPCSTR lpszFileName)
{
	if(m_pPlayer == NULL)
	{
		TRACEA("%s\n", "m_pPlayer is null");
		return FALSE;
	}

	FILE* pFileMP3 = fopen(lpszFileName, "rb");
	if(pFileMP3 == NULL)
	{
		TRACEA("failed to open file %s\n", lpszFileName);
		return FALSE;
	}

	if(m_pDecoder->Init() != 0)
	{
		TRACEA("m_pDecoder init failed\n");
		return FALSE;
	}

	unsigned char mp3_buffer[BUFFER_SIZE];
	short int pcm_l[MAX_FRAME_SIZE], pcm_r[MAX_FRAME_SIZE];
	BYTE pcm_buffer[MAX_FRAME_SIZE * 4];

	mp3data_struct mp3data;
	memset(&mp3data, 0, sizeof(mp3data));

	int nChannels = -1, nSampleRate = -1, nRead, nSamples, nDataLen, nBufferLen, nTotalFrames = 0;

	while( (nRead = fread(mp3_buffer, 1, BUFFER_SIZE, pFileMP3)) > 0 )
	{
		nBufferLen = nRead;
		do 
		{
			//try to decode at most 1 frame
			nSamples = m_pDecoder->Decode1Header(mp3_buffer, nBufferLen, pcm_l, pcm_r, &mp3data);
			
			//header parsed
			if(mp3data.header_parsed == 1)
			{
				//first time, try to open sound device
				if(nChannels < 0)
				{
					TRACEA("header parsed. channels=%d, samplerate=%d\n", mp3data.stereo, mp3data.samplerate);
					nChannels = mp3data.stereo;
					nSampleRate = mp3data.samplerate;

					if(!m_pPlayer->Open(nSampleRate, 16, nChannels))
					{
						TRACEA("Open device failed. %s\n", m_pPlayer->GetErrorMsg());
						goto CleanUP;
					}
				}
				else
				{
					if(nChannels != mp3data.stereo || nSampleRate != mp3data.samplerate)
					{
						TRACEA("channels or sample rate changed. channels=%d->%d, samplerate=%d->%d\n", 
							nChannels, mp3data.stereo, nSampleRate, mp3data.samplerate);
					}
				}
							
			}

			//no header, but has data
			if(nSamples > 0 && mp3data.header_parsed != 1)
			{
				TRACEA("lame decode error, nSamples=%d, but header not parsed yet\n", nSamples);
				goto CleanUP;
			}
			
			nDataLen = ToInterleavedPCMData(pcm_l, pcm_r, nSamples, nChannels, pcm_buffer);
			m_pPlayer->PlayData((const char*)pcm_buffer, nDataLen);
			
			/* future calls to decodeMP3 are just to flush buffers */
			nBufferLen = 0;

		} while (nSamples > 0);
	}

CleanUP:
	fclose(pFileMP3);
	m_pPlayer->Flush();
	m_pPlayer->Close();
	m_pDecoder->DeInit();
	
	return TRUE;
}
