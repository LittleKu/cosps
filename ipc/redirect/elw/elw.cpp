#include "log4cplus_config.h"
#include "StreamGobbler.h"
#include <stdio.h>

static PROCESS_INFORMATION piProcInfo;
static char szErrorMsg[256] = {0};
static char szCmdLine[4096] = {0};

DECLARE_THE_LOGGER_NAME("elw.cpp")

using namespace log4cplus;

static std::string szFullPath;
static std::string szProgName;
static std::string szChildName;
static std::string szArguments;
static std::string szChildCommandLine;

BOOL RedirectStdHandle(DWORD nStdHandle, BOOL bOutHandle, PHANDLE phOrig, PHANDLE phRead, PHANDLE phWrite);
BOOL CreateChildProcess();
int  HookChildProcess();
void InitLog4cplus();
void ParseAppCommandLine(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	size_t pos;
	int i;
	szFullPath.assign(argv[0]);
	for(i = 0; i < szFullPath.size(); i++)
	{
		if(szFullPath.at(i) == '/')
		{
			szFullPath.at(i) = '\\';
		}
	}
	
	pos = szFullPath.find_last_of('\\');
	if(pos != std::string::npos)
	{
		szProgName = szFullPath.substr(pos + 1);
		szFullPath = szFullPath.substr(0, pos);
	}
	else
	{
		szProgName = szFullPath;
		szFullPath = "";
	}

	InitLog4cplus();
	ParseAppCommandLine(argc, argv);
	int ret = HookChildProcess();
	return ret;
}

BOOL RedirectStdHandle(DWORD nStdHandle, BOOL bOut, PHANDLE phOrig, PHANDLE phRead, PHANDLE phWrite)
{
	BOOL fSuccess = FALSE;

	SECURITY_ATTRIBUTES saAttr; 
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL;
	
	*phOrig = GetStdHandle(nStdHandle);
	
	HANDLE hPipeRead = NULL, hPipeWrite = NULL;

	do 
	{
		// Create a pipe for the child process's STDOUT. 
		if ( !CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0) )
		{
			sprintf(szErrorMsg, "CreatePipe failed");
			break;
		}

		//Output stream
		if(bOut)
		{
			//redirect
			if(!SetStdHandle(nStdHandle, hPipeWrite))
			{
				sprintf(szErrorMsg, "Redirecting write failed");
				break;
			}
			
			//Create non-inheritable read handle
			if(!DuplicateHandle(GetCurrentProcess(), hPipeRead, GetCurrentProcess(), phRead, 
				0, FALSE, DUPLICATE_SAME_ACCESS))
			{
				sprintf(szErrorMsg, "DuplicateHandle failed");
				break;
			}

			//Close the inheritable read handle
			CloseHandle(hPipeRead);
			hPipeRead = NULL;

			//Set the result write handle
			*phWrite = hPipeWrite;
		}
		//Input stream
		else
		{
			//redirect
			if (!SetStdHandle(nStdHandle, hPipeRead))
			{
				sprintf(szErrorMsg, "Redirecting read failed");
				break;
			}

			// Duplicate the write handle to the pipe so it is not inherited. 
			if(!DuplicateHandle(GetCurrentProcess(), hPipeWrite, GetCurrentProcess(), phWrite, 
				0, FALSE, DUPLICATE_SAME_ACCESS))
			{
				sprintf(szErrorMsg, "DuplicateHandle failed");
				break;
			}
			
			//Close the inheritable write handle
			CloseHandle(hPipeWrite); 
			hPipeWrite = NULL;
			
			//Set the result read handle
			*phRead = hPipeRead;
		}

		fSuccess = TRUE;
	} while (FALSE);

	//clean up
	if(!fSuccess)
	{
		if(hPipeWrite != NULL)
		{
			CloseHandle(hPipeWrite);
			hPipeWrite = NULL;
		}
		if(hPipeRead != NULL)
		{
			CloseHandle(hPipeRead);
			hPipeRead = NULL;
		}
	}

	return fSuccess;
}

int HookChildProcess()
{
	BOOL bOK = FALSE;

	HANDLE hStdIn, hStdInRead, hStdInWrite;
	HANDLE hStdOut, hStdOutRead, hStdOutWrite;
	HANDLE hStdErr, hStdErrRead, hStdErrWrite;

	//Redirect std handles
	do 
	{
		//STDIN
		if(!RedirectStdHandle(STD_INPUT_HANDLE, FALSE, &hStdIn, &hStdInRead, &hStdInWrite))
		{
			break;
		}
		
		//STDOUT
		if(!RedirectStdHandle(STD_OUTPUT_HANDLE, TRUE, &hStdOut, &hStdOutRead, &hStdOutWrite))
		{
			break;
		}

		//STDERR
		if(!RedirectStdHandle(STD_ERROR_HANDLE, TRUE, &hStdErr, &hStdErrRead, &hStdErrWrite))
		{
			break;
		}

		bOK = TRUE;

	} while (FALSE);

	if(!bOK)
	{
		LOG4CPLUS_ERROR(THE_LOGGER, szErrorMsg)
		return -1;
	}

	//Create child process
	if(!CreateChildProcess())
	{
		sprintf(szErrorMsg, "CreateChildProcess failed");
		LOG4CPLUS_ERROR(THE_LOGGER, szErrorMsg)
		return -1;
	}

	bOK = FALSE;
	do 
	{
		// After process creation, restore the saved STDIN and STDOUT. 
		if (!SetStdHandle(STD_INPUT_HANDLE, hStdIn))
		{
			sprintf(szErrorMsg, "Re-redirecting Stdin failed");
			break;
		}
		if (!SetStdHandle(STD_OUTPUT_HANDLE, hStdOut))
		{
			sprintf(szErrorMsg, "Re-redirecting Stdout failed");
			break;
		}
		if (!SetStdHandle(STD_ERROR_HANDLE, hStdErr))
		{
			sprintf(szErrorMsg, "Re-redirecting Stderr failed");
			break;
		}

		bOK = TRUE;

	} while (FALSE);

	if(!bOK)
	{
		LOG4CPLUS_ERROR(THE_LOGGER, szErrorMsg)
		return -1;
	}

	StreamGobbler sgInput(hStdIn, hStdInWrite, "IN");
	StreamGobbler sgOutput(hStdOutRead, hStdOut, "OUT");
	StreamGobbler sgError(hStdErrRead, hStdErr, "ERR");

	sgInput.Start();
	sgOutput.Start();
	sgError.Start();

	DWORD dwWaitRet = ::WaitForSingleObject(piProcInfo.hProcess, INFINITE);

	sprintf(szErrorMsg, "WaitForSingleObject process returned: %u", dwWaitRet);
	LOG4CPLUS_INFO(THE_LOGGER, szErrorMsg)

	return dwWaitRet;
}

BOOL CreateChildProcess() 
{ 
	// Set up members of the STARTUPINFO structure. 
	STARTUPINFO siStartInfo; 
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	
	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
	
	strcpy(szCmdLine, szChildCommandLine.c_str());
	// Create the child process. 
	return CreateProcess(NULL, 
		szCmdLine,       // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 
}

void InitLog4cplus()
{
#ifdef ENABLE_LOG4CPLUS
	char log_file[256];
	sprintf(log_file, "%s\\%s.log", "C:\\Temp\\elw", "all");
	SharedAppenderPtr fileAppender(new RollingFileAppender(log_file, 10*1024*1024, 500));
	fileAppender->setName("file");
	
	std::auto_ptr<Layout> pPatternLayout(new PatternLayout("[%d{%H:%M:%S}][%p][%t][%c] - %m%n"));
	fileAppender->setLayout(pPatternLayout);
	
	ROOT_LOGGER.addAppender(fileAppender);
	
	ROOT_LOGGER.setLogLevel(INFO_LOG_LEVEL);
	LOG4CPLUS_INFO(THE_LOGGER, "The Log4cplus has been initialized.")
#endif
}

void ParseAppCommandLine(int argc, char* argv[])
{
	std::string szFull, szArgs, szArg;
	int i;
	size_t pos;
	for(i = 1; i < argc; i++)
	{
		if(i > 1)
		{
			szArgs.append(" ");
		}

		szArg.assign(argv[i]);

		//check if space char exist
		pos = szArg.find(' ');
		if(pos != std::string::npos)
		{
			szArg.erase();
			szArg.append("\"");
			szArg.append(argv[i]);
			szArg.append("\"");
		}
		szArgs.append(szArg);
	}

	szFull = argv[0];
	szFull.append(" ");
	szFull.append(szArgs);

	szArguments = szArgs;

	LOG4CPLUS_INFO(THE_LOGGER, "raw complete command line: " << szFull <<"[END]")

	/*
	szFullPath.assign(argv[0]);
	for(i = 0; i < szFullPath.size(); i++)
	{
		if(szFullPath.at(i) == '/')
		{
			szFullPath.at(i) = '\\';
		}
	}

	pos = szFullPath.find_last_of('\\');
	if(pos != std::string::npos)
	{
		szProgName = szFullPath.substr(pos + 1);
		szFullPath = szFullPath.substr(0, pos);
	}
	else
	{
		szProgName = szFullPath;
		szFullPath = "";
	}
	*/

	//TODO
	szChildName.assign("\"");
	szChildName.append(szFullPath);
	szChildName.append("\\..\\elw\\");
	szChildName.append(szProgName);
	szChildName.append("\"");

	szChildCommandLine.assign(szChildName);
	szChildCommandLine.append(" ");
	szChildCommandLine.append(szArguments);

	LOG4CPLUS_INFO(THE_LOGGER, "szFullPath= " << szFullPath <<"[END]")
	LOG4CPLUS_INFO(THE_LOGGER, "szProgName= " << szProgName <<"[END]")
	LOG4CPLUS_INFO(THE_LOGGER, "szChildName= " << szChildName <<"[END]")
	LOG4CPLUS_INFO(THE_LOGGER, "szChildCommandLine= " << szChildCommandLine <<"[END]")
}