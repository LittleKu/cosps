#include "SampleInterface.h"
#include "cflbase/tstring.h"
#include <assert.h>

bool SampleContext::Exist(const char* name)
{
	// search property
	std::map<std::string, void*>::iterator iter = m_map.find(name);
	return iter != m_map.end();
}

void* SampleContext::Remove(const char* name)
{
	// search property
	std::map<std::string, void*>::iterator iter = m_map.find(name);

	void* oldValue = NULL;
	if(iter != m_map.end())
	{
		m_map.erase(iter);
	}

	return oldValue;
}

void SampleContext::Set(const char* name, void* value)
{
	// search property
	std::map<std::string, void*>::iterator iter = m_map.find(name);
	if (iter == m_map.end())
	{
		// new property
		m_map.insert(std::make_pair<std::string, void*>(name, value));
	}
	else
	{
		// update property
		(*iter).second = value;
   }
}

void* SampleContext::Get(const char* name, void* defValue)
{
	// search property
	std::map<std::string, void*>::iterator iter = m_map.find(name);
	if (iter == m_map.end())
	{
		return defValue;
	}
	else
	{
		return (*iter).second;
	}
}

void SampleContext::SetInt(const char* name, int value)
{
	Set(name, (void*)value);
}

int	SampleContext::GetInt(const char* name, int defValue)
{
	return (int)Get(name, (void*)defValue);
}

void SampleContext::SetBool(const char* name, bool value)
{
	Set(name, (void*)value);
}

bool SampleContext::GetBool(const char* name, bool defValue)
{
	int var = GetInt(name, (int)defValue);
	return var != 0;
}

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
