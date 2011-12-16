// HtmlCoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HtmlCoder.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

void Unescape(CString& szURL, int nCount)
{
	char* escaped = 0;
	while(nCount-- > 0)
	{
		escaped = curl_unescape((LPCTSTR)szURL, szURL.GetLength());
		szURL = escaped;
		
		curl_free(escaped);
		escaped = 0;
	}
}

void Unescape(CString& szURL)
{
	CString szOld;
	
	do 
	{
		szOld = szURL;
		Unescape(szURL, 1);

	} while (szOld.Compare(szURL) != 0);
}

void ConcatString(const char* lpszSrcData, int nSrcLen, CString& szString)
{
	if(nSrcLen <= 0)
	{
		return;
	}
	
	int nDataLength = szString.GetLength();
	LPTSTR lpBuffer = szString.GetBufferSetLength(nDataLength + nSrcLen);
	// fast concatenation when buffer big enough
	memcpy(lpBuffer + nDataLength, lpszSrcData, nSrcLen * sizeof(char));
	
	ASSERT(szString.GetLength() == nDataLength + nSrcLen);
}

void UnescapeFile(LPCTSTR lpszFileName, LPCTSTR lpszFileResult)
{
	static const int BUFFER_SIZE = 4096;

	FILE* fp = fopen(lpszFileName, "rb");
	if(fp == NULL)
	{
		fprintf(stderr, "Failed to open file %s\n", lpszFileName);
		return;
	}

	CString szConent;

	char buffer[BUFFER_SIZE];

	int nByteRead;
	while( (nByteRead = fread(buffer, 1, BUFFER_SIZE, fp)) > 0 )
	{
		ConcatString(buffer, nByteRead, szConent);
	}
	fclose(fp);

	if(szConent.IsEmpty())
	{
		fprintf(stderr, "File content is empty %s\n", lpszFileName);
		return;
	}

	Unescape(szConent);

	
	HANDLE hFile = CreateFile(lpszFileResult, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	unsigned long nByteWrite = 0;
	WriteFile(hFile, (LPCTSTR)szConent, szConent.GetLength(), &nByteWrite, NULL);
	ASSERT(nByteWrite == szConent.GetLength());

	CloseHandle(hFile);
}

void test()
{
	CString szURL = "http://hi.baidu.com/%%%2525%CD%FC%BC%C7%D0%FA%CF%F9/blog/item/84df328f672e54f1503d92cf.html";
	printf("URL[0]=%s\n", szURL);

	int i;
	for(i = 0; i < 5; i++)
	{
		Unescape(szURL, 1);
		printf("URL[%d]=%s\n", i + 1, szURL);
	}
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
	
	test();

	if(argc < 3)
	{
		printf("Usage: HtmlCoder.exe <file input> <file output>\n");
		return 2;
	}

	UnescapeFile(argv[1], argv[2]);

	return nRetCode;
}


