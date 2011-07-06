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
	UINT GetLineLimit() const;
public:
	virtual BOOL ReadLine(CString& rString);
protected:
	UINT m_nLineLimit;
};

#endif // _STDIO_EX_FILE_H_
