#ifndef _STDIO_EX_FILE_H_
#define _STDIO_EX_FILE_H_

#pragma once

class CStdioExFile : public CStdioFile
{
public:
	CStdioExFile();
	CStdioExFile(FILE* pOpenStream);
	CStdioExFile(LPCTSTR lpszFileName, UINT nOpenFlags);
	virtual ~CStdioExFile();
	void SetLineLimit(UINT nLimit);
	int GetLineLimit() const;
public:
	virtual BOOL ReadLine(CString& rString, UINT* pDiscardedCount = 0);
protected:
	int m_nLineLimit;
};

#endif // _STDIO_EX_FILE_H_
