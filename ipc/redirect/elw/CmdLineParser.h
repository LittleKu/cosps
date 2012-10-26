#ifndef __CMD_LINE_PARSER_H__
#define __CMD_LINE_PARSER_H__

#pragma once
#pragma warning( disable : 4786 )

#include <vector>
#include <string>

class CmdLineParser
{
public:
	CmdLineParser(int argc, char* argv[]);
	virtual ~CmdLineParser();

	virtual void Execute();

	void GetAppFolder(std::string& str);
	void GetAppName(std::string& str);
	void GetArguments(std::string& str);
	
	void GetFullCommandLine(std::string& str);
	//strategy depends
	void GetChildCommandLine(std::string& str);
private:
	enum
	{
		BUF_SIZE = 512
	};

	/* Parse the app's folder and name */
	void ParseAppPath(std::string str, std::string& szAppFolder, std::string& szAppName);

	/* Build the app's arguments as a string */
	void BuildArguments(std::string& szArguments);
private:
	std::vector<std::string>	m_rawArgs;

	std::string	m_szAppFolder;	/* raw app's folder */
	std::string m_szAppName;	/* raw app's full name */
	std::string m_szArguments;	/* raw app's argument string */

	char m_buf[BUF_SIZE];
	static const char* CMD_LOGGER;
};


#endif