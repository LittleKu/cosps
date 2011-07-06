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
	UINT nLine = 0;
	while(file.ReadString(sLine))
	{
		nLine++;
		of<<(LPCTSTR)sLine<<std::endl;
	}

	sprintf(buf, "[INFO]: Total line = %d", nLine);
	std::cout<<buf<<std::endl;

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
	
	UINT nDiscardedCount = 0; 
	CString sLine;
	UINT nLine = 0;
	while(file.ReadLine(sLine, &nDiscardedCount))
	{
		nLine++;
		of<<(LPCTSTR)sLine<<std::endl;
		if(nDiscardedCount > 0)
		{
			sprintf(buf, "[INFO]: Line(%d): Discard %d characters", nLine, nDiscardedCount);
			std::cout<<buf<<std::endl;
		}
	}
	sprintf(buf, "[INFO]: Total line = %d", nLine);
	std::cout<<buf<<std::endl;
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