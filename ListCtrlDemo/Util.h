#ifndef _LIST_CTRL_DEMO_UTIL_H_
#define _LIST_CTRL_DEMO_UTIL_H_

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
}CountThreadParam, *LPCountThreadParam;

typedef struct tagUpdateProgressParam
{
	CString sTitle;
	CString sStatus;
	CString sFile;
	UINT nPos;
}UpdateProgressParam, *LPUpdateProgressParam;

class CTimeCost
{
public:
	CTimeCost(UINT timeInMs = 10);
	void UpdateCurrClock();
	void UpdateLastClock();
	BOOL IsTimeOut();
private:
	UINT    m_nDiff;
	clock_t m_clockCurr;
	clock_t m_clockLast;
};

class CFileInfo
{
public:
	CFileInfo();
	void SetFileName(LPCTSTR lpszFullFileName);
	UINT GetMixedLines() const;
	CString ToString(UINT n) const;
public:
	CString m_sFullFileName;
	
    CString m_sFilePath;
    CString m_sFileName;
    CString m_sFileExt;

	UINT m_nTotalLines;
	UINT m_nCodeLines;
	UINT m_nCommentLines;
	UINT m_nBlankLines;
};

int wildcmp(const char *wild, const char *string);

#endif