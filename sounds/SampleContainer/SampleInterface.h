#ifndef __SAMPLE_INTERFACE_H__
#define __SAMPLE_INTERFACE_H__

#pragma once

#include <tchar.h>
#include "SampleContainer.h"

class SampleDecoder
{
public:
	SampleDecoder() {}
	virtual ~SampleDecoder() {}
	
	//! initializes the input module
	virtual int Init(const TCHAR* pFileName, SampleContainer& sampleContainer) = 0;
	
	//! decodes samples and stores them in the sample container
	/*! returns number of samples decoded, or 0 if finished;
	a negative value indicates an error */
	virtual int Decode(SampleContainer& sampleContainer) = 0;
	
	//! called when done with decoding
	virtual int Done() = 0;
};

class SampleEncoder
{
public:
	SampleEncoder() {}
	virtual ~SampleEncoder() {}
	
	//! initializes the input module
	virtual int Init(const TCHAR* pFileName, SampleContainer& sampleContainer) = 0;
	
	//! decodes samples and stores them in the sample container
	/*! returns number of samples decoded, or 0 if finished;
	a negative value indicates an error */
	virtual int Encode(SampleContainer& sampleContainer) = 0;
	
	//! called when done with encoding
	virtual int Done() = 0;
};

#endif