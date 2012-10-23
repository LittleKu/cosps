#include "SampleInterface.h"

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
