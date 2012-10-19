#ifndef __WAVE_FILE_ENCODER_H__
#define __WAVE_FILE_ENCODER_H__

#pragma once

#include "SampleInterface.h"
#include "cflbase/ErrorMsg.h"

class WaveFileEncoder : public SampleEncoder, public NS_CFL::ErrorMsg
{
public:
	WaveFileEncoder();
	virtual ~WaveFileEncoder();
	//! initializes the input module
	virtual int Init(const TCHAR* pFileName, SampleContainer& sampleContainer);
	
	//! decodes samples and stores them in the sample container
	/*! returns number of samples decoded, or 0 if finished;
	a negative value indicates an error */
	virtual int Encode(SampleContainer& sampleContainer);
	
	//! called when done with encoding
	virtual int Done();

private:
	FILE*	m_pFile;
};

#endif