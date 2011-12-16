// HexView.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HexView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

const int BUFFER_SIZE = 4096;
static char const HEX_CHAR_TABLE[] = "0123456789ABCDEF";

void ViewHex(LPCTSTR lpszFileName);

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

	if(argc <= 1)
	{
		printf("Usage: HexView.exe <file name>\n");
		return 1;
	}

	
	ViewHex(argv[1]);

	return nRetCode;
}


void ViewHex(LPCTSTR lpszFileName)
{
	FILE* fp = fopen(lpszFileName, "rb");
	if(fp == NULL)
	{
		printf("Failed to open file %s\n", lpszFileName);
		return;
	}
	int i, nCount = 0, nByteRead;
	char hi, lo;
	unsigned char c;

	char buffer[BUFFER_SIZE];
	
	//10 spaces
	printf("\n           ");
	for(i = 0; i < 16; i++)
	{
		printf("%02X ", i);
	}
	printf("\n%08XH: ", 0);
	while( (nByteRead = fread(buffer, 1, BUFFER_SIZE, fp)) > 0 )
	{
		for(i = 0; i < nByteRead; i++)
		{
			if(nCount > 0 && nCount % 16 == 0)
			{
				printf("\n%08XH: ", nCount);
			}

			c = (unsigned char)buffer[i];
			
			hi = HEX_CHAR_TABLE[(c >> 4) & 0x0F];
			lo = HEX_CHAR_TABLE[(c     ) & 0x0F];

			printf("%c%c ", hi, lo);

			nCount++;
		}
	}

	printf("\n\n[Size] = %d\n", nCount);

	fclose(fp);
}


