#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#pragma warning( disable : 4786 )  // Disable warning messages

#include <string>
#include <list>
#include <map>

typedef std::map<std::string, std::string> prop_map;

#define POS_STRING_START		0x02
#define POS_STRING_END			0x01
#define POS_STRING_BOTH			0x03

bool start_with(const char* str, const char* start);
bool start_with(const std::string& str, const char* start);
int split_find(const std::string& str, const char* delim, const char* start, std::string& result);
int split(std::string& str, const char* delim, std::list<std::string>& result_list);
int find_between(const std::string& str, const char* start, const char* end, std::string& result);
int split_pair(const std::string& str, const char* delim, prop_map& result_map);
void trim(std::string& str, const char* t = " \t\r\n", unsigned char flag = POS_STRING_BOTH);
void trim_all(std::string& str, const char* t = " \t\r\n");
void replace(std::string& str, const char* r, const char* p = " \t\r\n");

#endif
