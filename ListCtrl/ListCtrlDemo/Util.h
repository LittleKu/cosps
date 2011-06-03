#ifndef _LIST_CTRL_DEMO_UTIL_H_
#define _LIST_CTRL_DEMO_UTIL_H_

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }

#define WM_START_COUNT            (WM_USER + 100)
#define WM_END_COUNT              (WM_USER + 101)
#define WM_PROGRESS_SET_RANGE     (WM_USER + 102)
#define WM_PROGRESS_UPDATE        (WM_USER + 103)
#define WM_PROGRESS_IS_CANCELLED  (WM_USER + 104)
#define WM_SUMMARY_UPDATE         (WM_USER + 105)

typedef struct tagCountThreadParam
{
	HWND hwndMain;
	CStringArray dirList;
	CStringArray filterList;
	BOOL bRecursive;
}CountThreadParam, *LPCountThreadParam;

typedef struct tagUpdateProgressParam
{
	CString sTitle;
	CString sStatus;
	CString sFile;
	UINT nPos;
}UpdateProgressParam, *LPUpdateProgressParam;

class CFileVisitor
{
public:
	virtual ~CFileVisitor(){}
	virtual int VisitFile(LPCTSTR lpszFileName) = 0;
	virtual UINT GetResult() = 0;
};

class CCancelledChecker
{
public:
	virtual ~CCancelledChecker(){}
	virtual BOOL IsCancelled() = 0;
	virtual void Reset() = 0;
};

class CTimeCost
{
public:
	CTimeCost(UINT timeInMs = 10);
	void UpdateCurrClock();
	void UpdateLastClock();
	BOOL IsTimeOut();
	UINT GetTimeCost();
	void Reset();
private:
	UINT    m_nDiff;
	clock_t m_clockCurr;
	clock_t m_clockLast;
};

BEGIN_NAMESPACE(CommonUtils)
	CString ToString(UINT n);
	CString GetPercentStr(UINT nCount, UINT nTotal);
	CString GetCurrentTime();
	int wildcmp(const char *wild, const char *string);
	BOOL IsMatched(CStringArray& sFilterList, const char* sStr);
	int EnumDirectory(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker);
	int EnumDirectoryFileFirst(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker);
	int EnumDirectoryIt(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker);
	void LoadBitmapFromFile(LPCTSTR lpszBmpFilePath, CBitmap* pBitmap);
END_NAMESPACE()


#endif