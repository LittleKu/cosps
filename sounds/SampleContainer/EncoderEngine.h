#ifndef __ENCODER_ENGINE_H__
#define __ENCODER_ENGINE_H__

#pragma once

#include "SampleInterface.h"

class EncoderEngine
{
public:
	EncoderEngine();
	virtual ~EncoderEngine();
	void SetDecoder(SampleDecoder* pDecoder);
	void SetEncoder(SampleEncoder* pEncoder);

	virtual int execute(SampleContext& context);
	int doExecute(SampleContext& context);

protected:
	SampleContainer m_samples;
	SampleDecoder*	m_pDecoder;
	SampleEncoder*	m_pEncoder;
};


#endif