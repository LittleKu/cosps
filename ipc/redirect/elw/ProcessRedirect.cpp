#include "ProcessRedirect.h"
#include <string.h>
#include <stdio.h>
#include "log4cplus_config.h"
#include "StreamGobbler.h"

DECLARE_THE_LOGGER_NAME("ProcessRedirect")

ProcessRedirect::ProcessRedirect(const char* cmdline, cfl::Properties* pProp)
{
	m_prop = pProp;
	strcpy(m_szCmdLine, cmdline);
	memset(&m_pi, 0, sizeof(m_pi));
	memset(m_szErrorMsg, 0, ERR_STR_SIZE);
}

ProcessRedirect::~ProcessRedirect()
{
}

BOOL ProcessRedirect::RedirectStdHandle(DWORD nStdHandle, BOOL bOut, PHANDLE phOrig, PHANDLE phRead, PHANDLE phWrite)
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
			sprintf(m_szErrorMsg, "CreatePipe failed");
			break;
		}
		
		//Output stream
		if(bOut)
		{
			//redirect
			if(!SetStdHandle(nStdHandle, hPipeWrite))
			{
				sprintf(m_szErrorMsg, "Redirecting write failed");
				break;
			}
			
			//Create non-inheritable read handle
			if(!DuplicateHandle(GetCurrentProcess(), hPipeRead, GetCurrentProcess(), phRead, 
				0, FALSE, DUPLICATE_SAME_ACCESS))
			{
				sprintf(m_szErrorMsg, "DuplicateHandle failed");
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
				sprintf(m_szErrorMsg, "Redirecting read failed");
				break;
			}
			
			// Duplicate the write handle to the pipe so it is not inherited. 
			if(!DuplicateHandle(GetCurrentProcess(), hPipeWrite, GetCurrentProcess(), phWrite, 
				0, FALSE, DUPLICATE_SAME_ACCESS))
			{
				sprintf(m_szErrorMsg, "DuplicateHandle failed");
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

BOOL ProcessRedirect::CreateChildProcess()
{
	// Set up members of the STARTUPINFO structure. 
	STARTUPINFO siStartInfo; 
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	
	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &m_pi, sizeof(PROCESS_INFORMATION) );
	

	// Create the child process. 
	return CreateProcess(NULL, 
		m_szCmdLine,   // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&m_pi);		   // receives PROCESS_INFORMATION 
}

FILE* ProcessRedirect::CreateFile(LPCTSTR lpszKey)
{
	cfl::tstring szVal;
	if(m_prop->GetProperty(lpszKey, szVal))
	{
		return _tfopen(szVal.c_str(), _T("ab"));
	}

	return NULL;
}

int ProcessRedirect::Redirect()
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
		LOG4CPLUS_ERROR(THE_LOGGER, m_szErrorMsg)
		return -1;
	}
	
	//Create child process
	if(!CreateChildProcess())
	{
		sprintf(m_szErrorMsg, "CreateChildProcess failed");
		LOG4CPLUS_ERROR(THE_LOGGER, m_szErrorMsg)
		return -1;
	}
	
	bOK = FALSE;
	do 
	{
		// After process creation, restore the saved STDIN and STDOUT. 
		if (!SetStdHandle(STD_INPUT_HANDLE, hStdIn))
		{
			sprintf(m_szErrorMsg, "Re-redirecting Stdin failed");
			break;
		}
		if (!SetStdHandle(STD_OUTPUT_HANDLE, hStdOut))
		{
			sprintf(m_szErrorMsg, "Re-redirecting Stdout failed");
			break;
		}
		if (!SetStdHandle(STD_ERROR_HANDLE, hStdErr))
		{
			sprintf(m_szErrorMsg, "Re-redirecting Stderr failed");
			break;
		}
		
		bOK = TRUE;
		
	} while (FALSE);
	
	if(!bOK)
	{
		LOG4CPLUS_ERROR(THE_LOGGER, m_szErrorMsg)
		return -1;
	}

	FILE *inFile, *outFile, *errFile, *allFile;
	inFile = CreateFile(_T("log_file_stdin"));
	outFile = CreateFile(_T("log_file_stdout"));
	errFile = CreateFile(_T("log_file_stderr"));
	allFile = CreateFile(_T("log_file_stdall"));
	
	StreamGobbler sgOutput(hStdOutRead, hStdOut, "OUT", outFile, allFile);
	sgOutput.Start();

	StreamGobbler sgError(hStdErrRead, hStdErr, "ERR", errFile, allFile);
	sgError.Start();

	StreamGobbler sgInput(hStdIn, hStdInWrite, "IN", inFile, allFile);
	sgInput.Start();
	
	DWORD dwWaitRet = ::WaitForSingleObject(m_pi.hProcess, INFINITE);
	
	sprintf(m_szErrorMsg, "WaitForSingleObject process returned: %u", dwWaitRet);
	LOG4CPLUS_INFO(THE_LOGGER, m_szErrorMsg)

	if(inFile) fclose(inFile);
	if(outFile) fclose(outFile);
	if(errFile) fclose(errFile);
	if(allFile) fclose(allFile);
		
	return dwWaitRet;
}
