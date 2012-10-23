#ifndef __LAME_FILE_DECODER_H__
#define __LAME_FILE_DECODER_H__

#pragma once

#include "cflbase/cflbase_config.h"
#include "SampleInterface.h"
#include "lame.h"

class LameDecoder;

class LameFileDecoder : public SampleDecoder
{
public:
	LameFileDecoder();
	virtual ~LameFileDecoder();

	//! initializes the input module
	virtual int Open(SampleContainer& samples, SampleContext& context);

	/*! decodes samples and stores them in the sample container
	returns number of samples decoded, or 0 if finished;
	a negative value indicates an error */
	virtual int Decode(SampleContainer& samples, SampleContext& context);

	//! called when done with decoding
	virtual int Close(SampleContainer& samples, SampleContext& context);

private:
	int ParseMpegHeader(unsigned char* mp3buffer, size_t len, mp3data_struct* mp3data, SampleContainer* samples);
private:
	enum
	{
		SAMPLES_PER_FRAME = 1152,
		MP3_BUFFER_SIZE = 4096,
		MAX_PCM_CHANNELS = 2
	};
	FILE*			m_pFile;	/* mp3 file header */
	LameDecoder*	m_pDecoder;	/* lame decoder for mp3 */

	byte*			m_mp3buf;	/* mp3 data buffer */
	short**			m_pcmbuf;	/* decoded pcm data buffer */

	int		m_nDecodedInOpen;	/* samples decoded in [Open] operation */

	mp3data_struct	m_mp3data;	/* mp3 struct data */
};
#endif