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

#if !defined(_countof)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

class FilterGroup
{
public:
	int nLangRuleType;
	CString szLangRuleName;
	CStringList includeList;
	CStringList excludeList;
public:
	CString ToString();
};

typedef FilterGroup* LPFilterGroup;
typedef CList<LPFilterGroup, LPFilterGroup> LPFilterGroupList;

class CountThreadParam
{
public:
	~CountThreadParam();
	HWND hwndMain;
	CStringArray dirList;
	LPFilterGroupList filterGroupList;
	BOOL bRecursive;
};

typedef CountThreadParam* LPCountThreadParam;

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
	virtual int VisitFile(LPCTSTR lpszFileName, LPVOID lpParam = NULL) = 0;
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
#define LEH_MESSAGE_BOX		0x0001
#define LEH_AFX_TRACE		0x0002

#define GCFP_AUTO			0x0000
#define GCFP_USER			0x0001
#define GCFP_DEFAULT		0x0002
	void Trim(CString& str, TCHAR chTarget);
	void LastErrorHandler(LPCTSTR lpszMsg, UINT uFlags = LEH_MESSAGE_BOX);
	CString ToString(UINT n);
	CString GetPercentStr(UINT nCount, UINT nTotal);
	CString GetCurrentTime();
	int GetIconIndex(LPCTSTR lpszPath, DWORD dwFileAttributes = FILE_ATTRIBUTE_NORMAL, UINT uFlags = 0);
	int GetWindowsDirIconIndex();
	int GetWindowsDirOpenIconIndex();
	BOOL IsFileExist(LPCTSTR lpFileName);
	CString GetConfFilePath(LPCTSTR lpFileName, UINT uFlags = GCFP_AUTO, LPCTSTR lpBaseDir = NULL);
	int wildcmp(const char *wild, const char *string);
	BOOL IsMatched(CStringArray& sFilterList, const char* sStr);
	BOOL IsMatched(FilterGroup& filterGroup, const char* sStr);
	POSITION IsMatched(LPFilterGroupList& filterGourpList, const char* sStr);
	int EnumDirectory(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker);
	int EnumDirectoryFileFirst(LPCTSTR lpszDirName, CStringArray& sFilterArray, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker);
	int EnumDirectoryIt(LPCTSTR lpszDirName, LPFilterGroupList& lpFilterGroupList, BOOL bRecursive, CFileVisitor* pVisitor, CCancelledChecker* pCancelledChecker);
	void LoadBitmapFromFile(LPCTSTR lpszBmpFilePath, CBitmap* pBitmap);
	void Split(const CString& szStr, TCHAR delim, CStringList& outStringList);
	CString ToString(LPFilterGroupList& filterGroupList);
END_NAMESPACE()


#endif