#ifndef __COMMON_H__
#define __COMMON_H__

#define WM_DOWNLOAD_PROGRESS	(WM_USER + 1099)
#define WM_DOWNLOAD_COMPLETE	(WM_USER + 1100)
#define WM_DOWNLOAD_STATUS		(WM_USER + 1101)

#define USER_AGENT_IE8	"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; MS-RTC LM 8; FDM)"
#define THE_APP_NAME	"MultiGet"

typedef enum
{
	TSE_READY	= 0,	//Task has been added, not started yet
	TSE_TRANSFERRING,	//Task is transferring, no error detected
	TSE_PAUSED,			//Task has been paused by user
	TSE_STOPPED,		//Task has been stopped by user
	TSE_COMPLETE,		//Task has been completed successfully
	TSE_END_WITH_ERROR	//Task ended with error
} TaskStatusEnum;

typedef enum
{
	RC_MAJOR_OK = 0,
	RC_MAJOR_PAUSED,
	RC_MAJOR_STOPPED,
	RC_MAJOR_TERMINATED_BY_INTERNAL_ERROR,
	RC_MAJOR_TERMINATED_BY_CURL_CODE
} ResultCodeMajor;

typedef enum
{
	RC_MINOR_OK = 0,
	RC_MINOR_MULTI_FDSET_ERROR,
	RC_MINOR_MULTI_TIMEOUT_ERROR,
	RC_MINOR_SELECT_ERROR,
	RC_MINOR_RETRY_OVER_ERROR
} ResultCodeMinor;

typedef CSize CRange;

class CProgressInfo
{
public:
	DWORD64 dltotal;
	DWORD64 dlnow;
	DWORD64 ultotal;
	DWORD64 ulnow;
	int retCode;
	CString szReason;
	int index;
	CProgressInfo() : dltotal(0), dlnow(0), ultotal(0), ulnow(0), index(-1)
	{
	}
};

class CControlInfo
{
public:
	BOOL isModified;
	BOOL isPaused;
	BOOL isStopped;

	CControlInfo() : isPaused(FALSE), isStopped(FALSE), isModified(FALSE)
	{
	}

	BOOL IsStopped()
	{
		return isStopped;
	}

	BOOL IsPaused()
	{
		return isPaused;
	}

	BOOL IsModified()
	{
		return isModified;
	}
};

class CHeaderInfo
{
public:
	int		m_nCurlResult;
	int		m_nHTTPCode;
	long	m_nContentLength;
	BOOL	m_bRangeBytes;

	CString m_szContentType;
	CString m_szStatusLine;

	int		m_nContentRangeX;
	int		m_nContentRangeY;
	int		m_nContentRangeTotal;

	CHeaderInfo() : m_nCurlResult(0), m_nHTTPCode(0), m_nContentLength(0), m_bRangeBytes(FALSE), 
		m_nContentRangeX(0), m_nContentRangeY(0), m_nContentRangeTotal(0)
	{}
	void Reset()
	{
		m_nCurlResult = 0;
		m_nHTTPCode = 0;
		m_nContentLength = 0;
		m_bRangeBytes = FALSE;
		m_szContentType.Empty();
		m_szStatusLine.Empty();

		m_nContentRangeX = 0;
		m_nContentRangeY = 0;
		m_nContentRangeTotal = 0;
	}
};

class CDownloadParam
{
public:
	CString m_szUrl;
	HWND	m_hWnd;
	int		m_nTaskID;
	UINT	m_nFileSize;
	CString m_szSaveToFileName;
	CDownloadParam(LPCTSTR lpszUrl = NULL, HWND hwnd = NULL, int index = -1, UINT nFileSize = 0)
		: m_szUrl(lpszUrl), m_hWnd(hwnd), m_nTaskID(index), m_nFileSize(nFileSize) {}
};


class CSegmentInfo
{
public:
	int		m_nIndex;		//index of the segment
	DWORD64	m_nDlBefore;	//how many bytes this segment has been download before
	DWORD64	m_nDlNow;		//how many bytes this segment is downloading now
	int		m_nRetry;		//how many times this connection retried
	CSize	m_range;		//range
	CString m_szFileHeader;	//header file name
	CString m_szFileData;	//data file name

	CSegmentInfo()
	{
		m_nIndex = -1;
		m_nDlBefore = 0;
		m_nDlNow = 0;
		m_nRetry = 0;
		m_range.cx = 0;
		m_range.cy = 0;
	}
};


#endif