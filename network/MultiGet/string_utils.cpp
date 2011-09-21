#include "stdafx.h"
#include "string_utils.h"


bool start_with(const char* str, const char* start)
{
	int len_of_str = strlen(str);
	int len_of_start = strlen(start);
	if(len_of_str < len_of_start)
	{
		return false;
	}
	return strncmp(str, start, len_of_start) == 0;
}
bool start_with(const std::string& str, const char* start)
{
	int len_of_start = strlen(start);
	if(str.length() < len_of_start)
	{
		return false;
	}
	return strncmp(str.c_str(), start, len_of_start) == 0;
}

int split_find(const std::string& str, const char* delim, const char* start, std::string& result)
{
	std::string substr;
	int len_of_delim = strlen(delim);

	size_t index, offset = 0;
	index = str.find(delim, offset);

	while(index != std::string::npos)
	{
		if(index > offset)
		{
			substr = str.substr(offset, index - offset);
			if(start_with(substr, start))
			{
				result = substr;
				return 1;
			}
		}
		offset = index + len_of_delim;
		index = str.find(delim, offset);
	}
	if(offset < str.length())
	{
		substr = str.substr(offset);
		if(start_with(substr, start))
		{
			result = substr;
			return 1;
		}
	}
	return 0;
}

int split(std::string& str, const char* delim, std::list<std::string>& result_list)
{
	std::string substr;
	int len_of_delim = strlen(delim);
	
	size_t index, offset = 0;
	index = str.find(delim, offset);
	
	while(index != std::string::npos)
	{
		if(index > offset)
		{
			substr = str.substr(offset, index - offset);
			result_list.push_back(substr);
		}
		offset = index + len_of_delim;
		index = str.find(delim, offset);
	}
	if(offset < str.length())
	{
		substr = str.substr(offset);
		result_list.push_back(substr);
	}
	return 1;
}

int find_between(const std::string& str, const char* start, const char* end, std::string& result)
{
	size_t start_index, end_index;
	int len_of_start = strlen(start);
	int len_of_end = strlen(end);

	start_index = str.find(start, 0);
	//can't find start
	if(start_index == std::string::npos)
	{
		return 0;
	}

	end_index = str.find(end, start_index + len_of_start);
	//can't find end
	if(end_index == std::string::npos)
	{
		return 0;
	}
	
	result = str.substr(start_index + len_of_start, end_index - (start_index + len_of_start));
	return 1;
}

int split_pair(const std::string& str, const char* delim, prop_map& result_map)
{
	size_t index = str.find(delim);
	if(index == std::string::npos)
	{
		return 0;
	}

	std::string key = str.substr(0, index);
	std::string value = str.substr(index + strlen(delim));

	result_map[key] = value;

	return 1;
}

void trim(std::string& str, const char* t, unsigned char flag)
{
	//start
	if(flag & POS_STRING_START)
	{
		str.erase(0, str.find_first_not_of(t));
	}
	//end
	if(flag & POS_STRING_END)
	{
		str.erase(str.find_last_not_of(t) + 1);
	}
}

void trim_all(std::string& str, const char* t)
{
	std::string::iterator iter;
	int c;
	for(iter = str.begin(); iter != str.end(); )
	{
		c = *iter;
		if(strchr(t, c))
		{
			iter = str.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void replace(std::string& str, const char* r, const char* p)
{
	std::string::iterator iter;
	
	std::string sub_str;
	size_t index = 0;
	bool is_in_pattern = false;
	int c;
	for(iter = str.begin(); iter != str.end(); iter++, index++)
	{
		c = *iter;
		if(strchr(p, c))
		{
			if(!is_in_pattern)
			{
				is_in_pattern = true;
			}
		}
		else
		{
			if(is_in_pattern)
			{
				sub_str += r;
				is_in_pattern = false;
			}
			sub_str += *iter;
		}
	}
	if(is_in_pattern)
	{
		sub_str += r;
		is_in_pattern = false;
	}
	
	str = sub_str;
}
