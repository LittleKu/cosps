#include "StdAfx.h"
#include "IniFileReadWrite.h"

namespace IniFile
{
#define BUF_SIZE 256
	CString GetAllSectionNames(LPCTSTR lpFileName)
	{
		CString sRet;
		// try fixed buffer first (to avoid wasting space in the heap)
		TCHAR szTemp[BUF_SIZE];
		DWORD nLen = GetPrivateProfileSectionNames(szTemp, BUF_SIZE, lpFileName);
		if(nLen == 0)
		{
			CString sRet;
			return sRet;
		}
		if(BUF_SIZE - nLen > 2)
		{
			CString sRet(szTemp, nLen + 1);
			return sRet;
		}
		
		// try double buffer size, then larger size until entire string is retrieved
		int nSize = BUF_SIZE;
		do
		{
			nSize += BUF_SIZE;
			nLen = GetPrivateProfileSectionNames(sRet.GetBuffer(nSize), nSize, lpFileName);
		} while (nSize - nLen <= 2);
		sRet.ReleaseBuffer(nLen + 1);
		
		return sRet;
	}
	
	CString GetAllKeyNames(LPCTSTR lpFileName, LPCTSTR lpSectionName)
	{
		CString sRet;
		// try fixed buffer first (to avoid wasting space in the heap)
		TCHAR szTemp[BUF_SIZE];
		DWORD nLen = GetPrivateProfileString(lpSectionName, NULL, NULL, szTemp, BUF_SIZE, lpFileName);
		if(nLen == 0)
		{
			CString sRet;
			return sRet;
		}
		if(BUF_SIZE - nLen > 2)
		{
			CString sRet(szTemp, nLen + 1);
			return sRet;
		}
		
		// try double buffer size, then larger size until entire string is retrieved
		int nSize = BUF_SIZE;
		do
		{
			nSize += BUF_SIZE;
			nLen = GetPrivateProfileString(lpSectionName, NULL, NULL, sRet.GetBuffer(nSize), nSize, lpFileName);
		} while (nSize - nLen <= 2);
		sRet.ReleaseBuffer(nLen + 1);
		
		return sRet;
	}
	
	CString GetKeyValue(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyName)
	{
		CString sRet;
		// try fixed buffer first (to avoid wasting space in the heap)
		TCHAR szTemp[BUF_SIZE];
		DWORD nLen = GetPrivateProfileString(lpSectionName, lpKeyName, NULL, szTemp, BUF_SIZE, lpFileName);
		if(nLen == 0)
		{
			CString sRet;
			return sRet;
		}
		if(BUF_SIZE - nLen > 1)
		{
			CString sRet(szTemp, nLen + 1);
			return sRet;
		}
		
		// try double buffer size, then larger size until entire string is retrieved
		int nSize = BUF_SIZE;
		do
		{
			nSize += BUF_SIZE;
			nLen = GetPrivateProfileString(lpSectionName, lpKeyName, NULL, sRet.GetBuffer(nSize), nSize, lpFileName);
		} while (nSize - nLen <= 1);
		sRet.ReleaseBuffer(nLen + 1);
		
		return sRet;
	}
}