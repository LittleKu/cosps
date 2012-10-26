#include "CmdLineParser.h"
#include "log4cplus_config.h"

DECLARE_THE_LOGGER_NAME("CmdLineParser")

const char* CmdLineParser::CMD_LOGGER = "CMD.CmdLineParser";

CmdLineParser::CmdLineParser(int argc, char* argv[])
{
	int i;
	for(i = 0; i < argc; i++)
	{
		sprintf(m_buf, "[RawCmdLine] - %2d: ", i);
		LOG4CPLUS_DEBUG(THE_LOGGER, m_buf<<argv[i]<<"[END]")

		m_rawArgs.push_back(argv[i]);
	}
}

CmdLineParser::~CmdLineParser()
{
	m_rawArgs.clear();
}

void CmdLineParser::GetAppFolder(std::string& str)
{
	str = m_szAppFolder;
}
void CmdLineParser::GetAppName(std::string& str)
{
	str = m_szAppName;
}
void CmdLineParser::GetArguments(std::string& str)
{
	str = m_szArguments;
}

void CmdLineParser::GetFullCommandLine(std::string& str)
{
	str.erase();

	str.append("\"").append(m_rawArgs.at(0)).append("\" ").append(m_szArguments);
}

//strategy depends
void CmdLineParser::GetChildCommandLine(std::string& str)
{
	str.erase();

	str.append("\"");
	if(!m_szAppFolder.empty())
	{
		str.append(m_szAppFolder).append("\\");
	}
	str.append("..\\elw\\").append(m_szAppName).append("\" ").append(m_szArguments);
}

void CmdLineParser::Execute()
{	
	//1. Parse .exe path
	ParseAppPath(m_rawArgs.at(0), m_szAppFolder, m_szAppName);

	//2. Build command line arguments as a string
	BuildArguments(m_szArguments);

	std::string cmdline;
	GetFullCommandLine(cmdline);

	LOG4CPLUS_INFO(GET_LOGGER(CMD_LOGGER), cmdline);
	LOG4CPLUS_INFO(THE_LOGGER, "[FullCmdLine]:"<<cmdline);

	std::string childcmd;
	GetChildCommandLine(childcmd);
	LOG4CPLUS_INFO(THE_LOGGER, "[ChildCmdLine]:"<<childcmd);
}

void CmdLineParser::ParseAppPath(std::string str, std::string& szAppFolder, std::string& szAppName)
{
	size_t i, size, pos;

	for(i = 0, size = str.size(); i < size; i++)
	{
		//normalize the path separator
		if(str.at(i) == '/')
		{
			str.at(i) = '\\';
		}
	}
	
	pos = str.find_last_of('\\');
	if(pos != std::string::npos)
	{
		szAppName = str.substr(pos + 1);
		szAppFolder = str.substr(0, pos);
	}
	else
	{
		szAppName = str;
		szAppFolder = "";
	}
}

void CmdLineParser::BuildArguments(std::string& szArguments)
{
	size_t i, size, pos;

	//clear the content first
	szArguments.erase();

	//start working
	for(i = 1, size = m_rawArgs.size(); i < size; i++)
	{
		if(i > 1)
		{
			szArguments.append(" ");
		}

		//find space char
		pos = m_rawArgs.at(i).find(' ');
		if(pos != std::string::npos)
		{
			//there's space char, surround the argument with 2 ""
			szArguments.append("\"").append(m_rawArgs.at(i)).append("\"");
		}
		else
		{
			szArguments.append(m_rawArgs.at(i));
		}
	}
}