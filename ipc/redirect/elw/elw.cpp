#include "log4cplus_config.h"
#include "StreamGobbler.h"
#include "CmdLineParser.h"
#include "ProcessRedirect.h"

DECLARE_THE_LOGGER_NAME("elw")

using namespace log4cplus;

void InitLog4cplus();

int main(int argc, char* argv[])
{
	InitLog4cplus();

	CmdLineParser clp(argc, argv);
	clp.Execute();
	std::string childCmdLine;
	clp.GetChildCommandLine(childCmdLine);

	ProcessRedirect redirector(childCmdLine.c_str());
	int ret = redirector.Redirect();

	return ret;
}



void InitLog4cplus()
{
#ifdef ENABLE_LOG4CPLUS
	char log_file[256];
	
	//setting root logger
	sprintf(log_file, "%s\\%s.log", "C:\\Temp\\elw", "all");
	SharedAppenderPtr fileAppender(new RollingFileAppender(log_file, 10*1024*1024, 500));
	fileAppender->setName("file");
	
	std::auto_ptr<Layout> pPatternLayout(new PatternLayout("[%d{%Y-%m-%d %H:%M:%S:%q}][%p][%t][%c] - %m%n"));
	fileAppender->setLayout(pPatternLayout);
	
	ROOT_LOGGER.addAppender(fileAppender);
	ROOT_LOGGER.setLogLevel(INFO_LOG_LEVEL);

	//setting CMD logger
	sprintf(log_file, "%s\\%s.log", "C:\\Temp\\elw", "cmd");
	SharedAppenderPtr cmdAppender(new RollingFileAppender(log_file, 10*1024*1024, 500));
	cmdAppender->setName("CMD");
	
	std::auto_ptr<Layout> pCmdPatternLayout(new PatternLayout("%m%n"));
	cmdAppender->setLayout(pCmdPatternLayout);
	
	GET_LOGGER("CMD").addAppender(cmdAppender);
	GET_LOGGER("CMD").setLogLevel(INFO_LOG_LEVEL);
	GET_LOGGER("CMD").setAdditivity(false);

	LOG4CPLUS_INFO(THE_LOGGER, "The Log4cplus has been initialized.")
#endif
}
