// BlowFishTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BlowFishTest.h"
#include "Blowfish.h"
#include "Base32.h"
#include <string>
#include <Wincrypt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

string CalcMD5(const char* s)
{
    HCRYPTPROV hCryptProv;
    HCRYPTHASH hHash = 0;
    BYTE pbData[0x7f];
    DWORD dwDataLen= 16; // The MD5 algorithm always returns 16 bytes.
    DWORD cbContent= strlen(s)*sizeof(char);
    BYTE* pbContent= (BYTE*)s;
    string retHash;
	
    if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
    {
        if (CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
        {
            if (CryptHashData(hHash, pbContent, cbContent, 0))
            {
                if (CryptGetHashParam(hHash, HP_HASHVAL, pbData, &dwDataLen, 0))
                {
                    // Make a string version of the numeric digest value
                    char tmpBuf[3];
                    for (int i = 0; i<16; i++)
                    {
                        sprintf(tmpBuf, "%02X", pbData[i]);
                        retHash += tmpBuf;
                    }
                }
            }
        }
    }
	
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
	
    return retHash;
}

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
	printf("\n\n\n");

	int tempBufSize = 20;
	int nBase32Len = Base32::GetEncode32Length(tempBufSize);
	char* out = new char[nBase32Len + 1];
	memset(out, 0x00, nBase32Len + 1);
	if(Base32::Encode32((unsigned char*)pOut, tempBufSize, (unsigned char*)out))
	{
		const char alphabet[] = "123456789ABCDEFGHJKMNPQRSTUVWXYZ";
		Base32::Map32((unsigned char*)out, nBase32Len, (unsigned char*)alphabet);
		printf("%s\n\n", out);
		szLog.Empty();
// 		for (i = 0; i < nBase32Len; i++)
// 		{
// 			szTemp.Format("%02X ", (pBase32[i] & 0xFF));
// 			szLog += szTemp;
// 			if((i > 0) && ( (i + 1) % 8) == 0)
// 			{
// 				printf("%s\n", szLog);
// 				szLog.Empty();
// 			}
// 		}
// 		if(!szLog.IsEmpty())
// 			printf("%s\n", szLog);
	}
	else
	{
		printf("Failed to encode\n");
	}
	
	//Decrypt
	blower.Decrypt( (unsigned char*)pOut, sizeof(char) * bufSize );
	printf("\nRestored Text = %s\n\n", pOut);
	printf("%s\n", pOut);

	delete [] pOut;
	delete [] out;
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

	const char* pStr = "Blow Fish Testcccccc";
	encrypt(pStr);

	string s = CalcMD5(pStr);
	cout<<s<<endl;

	return nRetCode;
}


