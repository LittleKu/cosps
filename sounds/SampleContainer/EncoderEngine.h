#ifndef __ENCODER_ENGINE_H__
#define __ENCODER_ENGINE_H__

#pragma once

#include "SampleInterface.h"
#include "cflbase/tstring.h"
#include "cflbase/ErrorMsg.h"

class EncoderEngine : public NS_CFL::ErrorMsg
{
public:
	EncoderEngine();
	virtual ~EncoderEngine();
	
	void SetDecoder(SampleDecoder* pDecoder);
	void SetEncoder(SampleEncoder* pEncoder);
	
	void SetSrcFile(const TCHAR* pFileName);
	void SetDstFile(const TCHAR* pFileName);

	virtual int execute();
	int doExecute();

protected:
	SampleDecoder*	m_pDecoder;
	SampleEncoder*	m_pEncoder;
	NS_CFL::tstring	m_srcFile;
	NS_CFL::tstring	m_dstFile;
	SampleContainer m_samples;
};


#endif