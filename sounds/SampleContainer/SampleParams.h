// SampleParams.h: interface for the SampleParams class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEPARAMS_H__6A5AF735_5985_4078_9A2F_1D4AF2F7E0C2__INCLUDED_)
#define AFX_SAMPLEPARAMS_H__6A5AF735_5985_4078_9A2F_1D4AF2F7E0C2__INCLUDED_

#pragma once

#include "cflbase/tstring.h"
#include "SampleInterface.h"

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

#endif // !defined(AFX_SAMPLEPARAMS_H__6A5AF735_5985_4078_9A2F_1D4AF2F7E0C2__INCLUDED_)
