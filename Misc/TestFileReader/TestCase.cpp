#include "StdAfx.h"
#include "TestCase.h"
#include "LineViewOfFile.h"
#include "StdioExFile.h"
#include <fstream>

CString GetOutputFileName(LPCTSTR lpFileName, int nMethod);
BOOL ReadFileInPLC(LPCTSTR lpFileName);
BOOL ReadFileInSTL(LPCTSTR lpFileName);
BOOL ReadFileInMFC(LPCTSTR lpFileName);
BOOL ReadFileInMFC2(LPCTSTR lpFileName);

BOOL ReadFile(LPCTSTR lpFileName, int nMethod)
{
	BOOL bResult = FALSE;
	if(nMethod == RF_IN_PLC)
	{
		bResult = ReadFileInPLC(lpFileName);
	}
	else if(nMethod == RF_IN_MFC)
	{
		bResult = ReadFileInMFC(lpFileName);
	}
	else if(nMethod == RF_IN_STL)
	{
		bResult = ReadFileInSTL(lpFileName);
	}
	else if(nMethod == RF_IN_MFC2)
	{
		bResult = ReadFileInMFC2(lpFileName);
	}
	return bResult;
}

BOOL ReadFileInPLC(LPCTSTR lpFileName)
{
	CLineViewOfFile file(lpFileName);

	std::ofstream of;
	of.open(GetOutputFileName(lpFileName, RF_IN_PLC));

	CString* sLine = NULL;
	while(file.GetNextLine(sLine))
	{
		of<<(LPCSTR)(*sLine)<<std::endl;
	}

	of.close();

	return TRUE;
}

const int buf_size = 128;
char buf[buf_size];
BOOL ReadFileInSTL(LPCTSTR lpFileName)
{
	std::ifstream finput;
	finput.open(lpFileName);

	std::ofstream of;
	of.open(GetOutputFileName(lpFileName, RF_IN_STL));

	while(finput.getline(buf, buf_size))
	{
		of<<buf<<std::endl;
	}
	finput.clear();

	of.close();
	finput.close();

	return TRUE;
}

BOOL ReadFileInMFC(LPCTSTR lpFileName)
{
	CStdioFile file;
	if(!file.Open(lpFileName, CFile::modeRead | CFile::shareDenyWrite))
	{
		return FALSE;
	}

	std::ofstream of;
	of.open(GetOutputFileName(lpFileName, RF_IN_MFC));

	CString sLine;
	while(file.ReadString(sLine))
	{
		of<<(LPCTSTR)sLine<<std::endl;
	}
	of.close();
	file.Close();

	return TRUE;
}

BOOL ReadFileInMFC2(LPCTSTR lpFileName)
{
	CStdioExFile file;
	if(!file.Open(lpFileName, CFile::modeRead | CFile::shareDenyWrite))
	{
		return FALSE;
	}
	
	std::ofstream of;
	of.open(GetOutputFileName(lpFileName, RF_IN_MFC2));
	
	CString sLine;
	while(file.ReadLine(sLine))
	{
		of<<(LPCTSTR)sLine<<std::endl;
	}
	of.close();
	file.Close();
	
	return TRUE;
}

CString GetOutputFileName(LPCTSTR lpFileName, int nMethod)
{
	CString str;
	str.Format("%s-%d.txt", lpFileName, nMethod);
	return str;
}