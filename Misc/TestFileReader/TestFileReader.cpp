// TestFileReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestFileReader.h"
#include "TestCase.h"
#include "TimeCost.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

void ReadFileTimeCost(LPCTSTR lpFileName, int nMethod)
{
	CTimeCost timeCost;
	ReadFile(lpFileName, nMethod);
	timeCost.UpdateCurrClock();

	cout<<"time-cost("<<nMethod<<"): "<<timeCost.GetTimeCost()<<endl;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}

	LPCTSTR lpFileName = "C:\\Temp\\slideshow0822.txt";
// 	ReadFileTimeCost(lpFileName, RF_IN_PLC);
// 	ReadFileTimeCost(lpFileName, RF_IN_STL);	
	ReadFileTimeCost(lpFileName, RF_IN_MFC2);
	ReadFileTimeCost(lpFileName, RF_IN_MFC);

	return nRetCode;
}


