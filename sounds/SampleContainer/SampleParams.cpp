// SampleParams.cpp: implementation of the SampleParams class.
//
//////////////////////////////////////////////////////////////////////

#include "SampleParams.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char* SampleParams::INPUT_FILE_NAME = "sc.input_file_name";
const char* SampleParams::OUTPUT_FILE_NAME = "sc.output_file_name";
const char* SampleParams::ERROR_STR = "sc.error_str";

const TCHAR* SampleParams::GetInputFile(SampleContext& context)
{
	NS_CFL::tstring* pFileName = (NS_CFL::tstring*)context.Get(INPUT_FILE_NAME);
	assert(pFileName != NULL);
	return pFileName->c_str();
}
const TCHAR* SampleParams::GetOutputFile(SampleContext& context)
{
	NS_CFL::tstring* pFileName = (NS_CFL::tstring*)context.Get(OUTPUT_FILE_NAME);
	assert(pFileName != NULL);
	return pFileName->c_str();
}
NS_CFL::tstring* SampleParams::GetErrorMsg(SampleContext& context)
{
	NS_CFL::tstring* pErrorStr = (NS_CFL::tstring*)context.Get(ERROR_STR);
	assert(pErrorStr != NULL);
	return pErrorStr;
}


void SetErrorMsg(SampleContext& context, const TCHAR* fmt, ...)
{
	NS_CFL::tstring* pErrorStr = SampleParams::GetErrorMsg(context);
	
	va_list args;
	va_start(args, fmt);
	
	int nWrite = NS_CFL::tvformat(*pErrorStr, fmt, args);
	
	va_end(args);
}
