#include "cflbase/Properties.h"
#include "log4cplus_config.h"
#include "StreamGobbler.h"
#include "CmdLineParser.h"
#include "ProcessRedirect.h"

DECLARE_THE_LOGGER_NAME("elw")

using namespace log4cplus;

void InitProperties(cfl::Properties& prop);
void InitLog4cplus(cfl::Properties& prop);

int main(int argc, char* argv[])
{
	cfl::Properties prop;
	InitProperties(prop);
	InitLog4cplus(prop);

	CmdLineParser clp(argc, argv);
	clp.Execute();
	std::string childCmdLine;
	clp.GetChildCommandLine(childCmdLine);

	ProcessRedirect redirector(childCmdLine.c_str(), &prop);
	int ret = redirector.Redirect();

	return ret;
}

void InitProperties(cfl::Properties& prop)
{
	const char* log_dir = "C:\\Temp\\elw";
	prop.SetProperty("log_dir", log_dir);

	prop.SetProperty(_T("log_file_all"), "C:\\Temp\\elw" "\\all.log");
	prop.SetProperty("log_file_cmd", "C:\\Temp\\elw" "\\cmd.log");

	prop.SetProperty("log_file_stdin", "C:\\Temp\\elw" "\\stdin.log");
	prop.SetProperty("log_file_stdout", "C:\\Temp\\elw" "\\stdout.log");
	prop.SetProperty("log_file_stderr", "C:\\Temp\\elw" "\\stderr.log");

	prop.SetProperty("log_file_stdall", "C:\\Temp\\elw" "\\stdall.log");

	TCHAR* szPropFile = _tgetenv(_T("elw_prop"));
	if(szPropFile != NULL)
	{
		prop.Load(szPropFile);
	}
}

void InitLog4cplus(cfl::Properties& prop)
{
#ifdef ENABLE_LOG4CPLUS
	cfl::tstring szVal;
	
	prop.GetProperty(_T("log_file_all"), szVal);
	//setting root logger
	SharedAppenderPtr fileAppender(new RollingFileAppender(szVal.c_str(), 10*1024*1024, 500));
	fileAppender->setName("file");
	
	std::auto_ptr<Layout> pPatternLayout(new PatternLayout("[%d{%Y-%m-%d %H:%M:%S:%q}][%p][%t][%c] - %m%n"));
	fileAppender->setLayout(pPatternLayout);
	
	ROOT_LOGGER.addAppender(fileAppender);
	ROOT_LOGGER.setLogLevel(INFO_LOG_LEVEL);

	//setting CMD logger
	prop.GetProperty(_T("log_file_cmd"), szVal);
	SharedAppenderPtr cmdAppender(new RollingFileAppender(szVal.c_str(), 10*1024*1024, 500));
	cmdAppender->setName("CMD");
	
	std::auto_ptr<Layout> pCmdPatternLayout(new PatternLayout("%m%n"));
	cmdAppender->setLayout(pCmdPatternLayout);
	
	GET_LOGGER("CMD").addAppender(cmdAppender);
	GET_LOGGER("CMD").setLogLevel(INFO_LOG_LEVEL);
	GET_LOGGER("CMD").setAdditivity(false);

	TCHAR* szPropFile = _tgetenv(_T("elw_prop"));
	cfl::format(szVal, _T("${elw_prop}=%s"), (szPropFile == NULL ? _T("NULL") : szPropFile));
	LOG4CPLUS_INFO(THE_LOGGER, "The Log4cplus has been initialized. " << szVal)
#endif
}
