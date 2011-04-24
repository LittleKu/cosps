#ifndef _COUNTER_H_
#define _COUNTER_H_

class CFileVisitor
{
public:
	virtual int VisitFile(LPCTSTR lpszFileName) = 0;
	virtual UINT GetResult() = 0;
};

class CCounter
{
public:
	static UINT CountThreadProc(LPVOID lpvData);
	static int EnumDirectory(LPCTSTR lpszDirName, BOOL bRecursive, CFileVisitor* pVisitor);
};

class CFileCountVisitor : public CFileVisitor
{
public:
	CFileCountVisitor();
	virtual int VisitFile(LPCTSTR lpszFileName);
	virtual UINT GetResult() { return m_nCount; }
private:
	UINT m_nCount;
};

class CFilePrintVisitor : public CFileVisitor
{
public:
	CFilePrintVisitor(HWND hWnd);
	virtual int VisitFile(LPCTSTR lpszFileName);
	virtual UINT GetResult() { return m_nCount; }
private:
	HWND m_hProgWnd;
	UINT m_nCount;
};

#endif