// RWINIFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RWINIFile.h"
#include "IniFileReadWrite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return -1;
	}

	LPCTSTR lpFileName = ".\\test.ini";
	ShowIniFileContent(lpFileName);

	return 0;
}

void ShowIniFileContent(LPCTSTR lpFileName)
{
	CString sAllSectionNames = IniFile::GetAllSectionNames(lpFileName);
	DWORD nLen = sAllSectionNames.GetLength();
	if(nLen == 0)
	{
		return;
	}

	LPTSTR lptszBuf = sAllSectionNames.GetBuffer(nLen);
	do
    {
		cout<<"["<<lptszBuf<<"]"<<endl;
		ShowIniFileKeyName(lpFileName, lptszBuf);
        lptszBuf = _tcschr(lptszBuf, '\0') + 1;
    }
    while(lptszBuf[0] != '\0');
}

void ShowIniFileKeyName(LPCTSTR lpFileName, LPCTSTR lpSectionName)
{	
	CString sAllKeyNames = IniFile::GetAllKeyNames(lpFileName, lpSectionName);
	DWORD nLen = sAllKeyNames.GetLength();
	if(nLen == 0)
	{
		return;
	}

	CString sKeyValue;
	LPTSTR lptszBuf = sAllKeyNames.GetBuffer(nLen);
	do
    {
		cout<<lptszBuf<<"=";
		sKeyValue = IniFile::GetKeyValue(lpFileName, lpSectionName, lptszBuf);
		cout<<(LPCTSTR)sKeyValue<<endl;
        lptszBuf = _tcschr(lptszBuf, '\0') + 1;
    }
    while(lptszBuf[0] != '\0');
}


