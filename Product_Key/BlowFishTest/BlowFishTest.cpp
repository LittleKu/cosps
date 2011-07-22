// BlowFishTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BlowFishTest.h"
#include "Blowfish.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

void encrypt(const char* str)
{
	const char* pKey = "Hello, BlowFish!";
	int nKeyLen = strlen(pKey);
	CBlowFish blower((const unsigned char*)pKey, nKeyLen);
	
	int nLen = strlen(str);
	int bufSize = ( (nLen / 8) + 1 ) * 8;
	char* pOut = new char[bufSize + 1];
	memset(pOut, 0, sizeof(char) * (bufSize + 1));
	strcpy(pOut, str);
	
	printf("Plaint Text = %s, len = (%d)\n", str, nLen);
	//Encrypt
	blower.Encrypt( (unsigned char*)pOut, sizeof(char) * bufSize );
	
	printf("\nEncrypted:\n");
	CString szLog, szTemp;
	int i;
	for (i = 0; i < bufSize; i++)
	{
		szTemp.Format("%02X ", (pOut[i] & 0xFF));
		szLog += szTemp;
		if((i > 0) && ( (i + 1) % 8) == 0)
		{
			printf("%s\n", szLog);
			szLog.Empty();
		}
    }
	if(!szLog.IsEmpty())
		printf("%s\n", szLog);
	
	//Decrypt
	blower.Decrypt( (unsigned char*)pOut, sizeof(char) * bufSize );
	printf("\nRestored Text = %s\n\n", pOut);
	printf("%s\n", pOut);

	delete [] pOut;
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

	encrypt("Blow Fish Testcccccc");

	return nRetCode;
}


