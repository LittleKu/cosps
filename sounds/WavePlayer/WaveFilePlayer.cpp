// WaveFilePlayer.cpp: implementation of the CWaveFilePlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "WaveFilePlayer.h"
#include <sndfile.h>
#include "DebugSupport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveFilePlayer::CWaveFilePlayer()
{
	m_pPlayer = new CWavePlayer();
	if(!m_pPlayer->Init())
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
}

CWaveFilePlayer::~CWaveFilePlayer()
{
	if(m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
}

BOOL CWaveFilePlayer::Play(LPCSTR lpszFileName)
{
	if(m_pPlayer == NULL)
	{
		TRACE("%s\n", "m_pPlayer is null");
		return FALSE;
	}
	SF_INFO sfinfo;
	SNDFILE *sndfile;

	sndfile = sf_open(lpszFileName, SFM_READ, &sfinfo);
	if(sndfile == NULL)
	{
		TRACE("%s\n", sf_strerror(NULL));
		return FALSE;
	}
	if(!m_pPlayer->Open(sfinfo.samplerate, 16, sfinfo.channels))
	{
		TRACE("%s: %s\n", "Player open error", m_pPlayer->GetErrorMsg());
		return FALSE;
	}

	sf_count_t nRead = 0, nBufferSize = BUFFER_SIZE / sizeof(short);
	while( (nRead = sf_read_short(sndfile, (short*)m_buffer, nBufferSize)) > 0 )
	{
		m_pPlayer->PlayData(m_buffer, (int)(nRead * sizeof(short)));
	}
	m_pPlayer->Flush();
	m_pPlayer->Close();

	sf_close(sndfile);

	return TRUE;
}
