#include "RegistryKey.h"
#include "Environment.h"
#include "SystemEnv.h"
#include "ProcessEnv.h"
#include "SetEnv.h"

#define THE_REG_SUB_KEYW	L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment" 
#define THE_VALUE_NAMEW		L"GLCT"
#define THE_DWORD_NAMEW		L"MYFLAG"

LONG testReg2()
{
	LONG lResult = 0;
	
	CRegistryKey regKey;
	
	regKey.Create(HKEY_LOCAL_MACHINE, THE_REG_SUB_KEYW);
	
	if(lResult != 0)
	{
		return lResult;
	}
	std::wstring strValue;
	
	lResult = regKey.GetString(THE_VALUE_NAMEW, strValue);
	if(lResult == 0)
	{
		wprintf(L"Original value: %s=%s\n", THE_VALUE_NAMEW, strValue.c_str());
	}
	else
	{
		wprintf(L"1: The value %s doesn't exist\n", THE_VALUE_NAMEW);
	}
	
	lResult = regKey.SetString(THE_VALUE_NAMEW, L"test%M2%_456", REG_EXPAND_SZ);
	if(lResult != 0)
	{
		return lResult;
	}
	
	
	lResult = regKey.GetString(THE_VALUE_NAMEW, strValue);
	if(lResult == 0)
	{
		wprintf(L"Setted value: %s=%s\n", THE_VALUE_NAMEW, strValue.c_str());
	}
	else
	{
		wprintf(L"2: The value %s doesn't exist\n", THE_VALUE_NAMEW);
	}
	
	regKey.Delete(THE_VALUE_NAMEW);
	
	DWORD dwValue = 0;
	lResult = regKey.GetDWORD(THE_DWORD_NAMEW, &dwValue);
	
	if(lResult == 0)
	{
		wprintf(L"Original value: %s=%08X\n", THE_DWORD_NAMEW, dwValue);
	}
	else
	{
		wprintf(L"1: The value %s doesn't exist\n", THE_DWORD_NAMEW);
	}
	
	dwValue = 0x8134569F;
	lResult = regKey.SetDWORD(THE_DWORD_NAMEW, dwValue);
	if(lResult != 0)
	{
		return lResult;
	}
	dwValue = 0;
	
	lResult = regKey.GetDWORD(THE_DWORD_NAMEW, &dwValue);
	if(lResult == 0)
	{
		wprintf(L"Setted value: %s=%08X\n", THE_DWORD_NAMEW, dwValue);
	}
	else
	{
		wprintf(L"2: The value %s doesn't exist\n", THE_DWORD_NAMEW);
	}
	
	regKey.Delete(THE_DWORD_NAMEW);
	
	
	return lResult;
}

LONG testReg3()
{
	CSystemEnv env;
	BOOL bResult = FALSE;

	LPCWSTR lpValueName = L"aaa";
	std::wstring strValue;

	bResult = env.GetEnv(lpValueName, strValue);
	if(bResult)
	{
		wprintf(L"O value: %s\n", strValue.c_str());
	}

	bResult = env.SetEnv(lpValueName, L"123");
	bResult = env.GetEnv(lpValueName, strValue);
	if(bResult)
	{
		wprintf(L"1 value: %s\n", strValue.c_str());
	}

	bResult = env.SetEnv(lpValueName, L"345%ddd%");
	bResult = env.GetEnv(lpValueName, strValue);
	if(bResult)
	{
		wprintf(L"2 value: %s\n", strValue.c_str());
	}

	bResult = env.Append(lpValueName, L"_TAIL");
	bResult = env.GetEnv(lpValueName, strValue);
	if(bResult)
	{
		wprintf(L"2 value: %s\n", strValue.c_str());
	}

	bResult = env.Prepend(lpValueName, L"HEAD_");
	bResult = env.GetEnv(lpValueName, strValue);
	if(bResult)
	{
		wprintf(L"2 value: %s\n", strValue.c_str());
	}

	bResult = env.Delete(lpValueName);

	return 0L;
}

LONG testReg4()
{
	SetEnvironmentVariableW(L"My35Version", L"v3.5");

	WCHAR wchBuffer[1024];

	ExpandEnvironmentStringsW(L"My35Version=%My35Version%", wchBuffer, 1024);

	wprintf(L"%s\n", wchBuffer);

	return 0;
}

int main(int argc, char* argv[])
{
	LONG lResult = testReg3();

	CSetEnv setEnv;
	setEnv.Set("VS10_VCVARS32.xml");

	return 0;
}