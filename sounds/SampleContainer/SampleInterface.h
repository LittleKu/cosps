#ifndef __SAMPLE_INTERFACE_H__
#define __SAMPLE_INTERFACE_H__

#pragma once

#include "cflbase/tstring.h"
#include <tchar.h>
#include <map>
#include <string>
#include "SampleContainer.h"

class SampleContext
{
public:
	SampleContext() {}
	virtual ~SampleContext() {}

	virtual bool	Exist(const char* name);
	virtual void*	Remove(const char* name);

	virtual void	Set(const char* name, void* value);
	virtual	void*	Get(const char* name, void* defValue = NULL);

	virtual void	SetInt(const char* name, int value);
	virtual int		GetInt(const char* name, int defValue = 0);

	virtual void	SetBool(const char* name, bool value);
	virtual bool	GetBool(const char* name, bool defValue = false);
protected:
	std::map<std::string, void*>	m_map;
};

class SampleParams
{
public:
	static const TCHAR* GetInputFile(SampleContext& context);
	static const TCHAR* GetOutputFile(SampleContext& context);
	static NS_CFL::tstring* GetErrorMsg(SampleContext& context);

	static const char* INPUT_FILE_NAME;
	static const char* OUTPUT_FILE_NAME;
	static const char* ERROR_STR;
};

void SetErrorMsg(SampleContext& context, const TCHAR* fmt, ...);

class SampleDecoder
{
public:
	SampleDecoder() {}
	virtual ~SampleDecoder() {}

	//! initializes the input module
	virtual int Open(SampleContainer& samples, SampleContext& context) = 0;

	/*! decodes samples and stores them in the sample container
	returns number of samples decoded, or 0 if finished;
	a negative value indicates an error */
	virtual int Decode(SampleContainer& samples, SampleContext& context) = 0;

	//! called when done with decoding
	virtual int Close(SampleContainer& samples, SampleContext& context) = 0;
};

class SampleEncoder
{
public:
	SampleEncoder() {}
	virtual ~SampleEncoder() {}

	//! initializes the output module
	virtual int Open(SampleContainer& samples, SampleContext& context) = 0;

	/* encodes samples from the sample container
	it is required that all samples from the container will be used up;
	returns 0 if all was ok, or a negative value on error */
	virtual int Encode(SampleContainer& samples, SampleContext& context) = 0;

	//! cleans up the output module
	virtual int Close(SampleContainer& samples, SampleContext& context) = 0;
};

#endif