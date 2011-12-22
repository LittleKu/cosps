#ifndef __COMMON_H__
#define __COMMON_H__

#define WM_DOWNLOAD_PROGRESS	(WM_USER + 1099)
#define WM_DOWNLOAD_COMPLETE	(WM_USER + 1100)
#define WM_DOWNLOAD_STATUS		(WM_USER + 1101)

#define USER_AGENT_IE8	"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; MS-RTC LM 8; FDM)"
#define THE_APP_NAME	"XGet77"
#define TIME_COST_NAME	"TIME"
#define LENGTH_OF(A)	(sizeof(A) / sizeof(A[0]))

#define DL_OPER_FLAG_NONE		(0)
#define DL_OPER_FLAG_START		(0x0001)
#define DL_OPER_FLAG_PAUSE		(0x0002)
#define DL_OPER_FLAG_REMOVE		(0x0004)
#define DL_OPER_FLAG_REDOWNLOAD	(0x0008)
#define DL_OPER_FLAG_ALL		(0xFFFFFFFF)

typedef enum
{
	TSE_READY	= 0,	//Task has been added, not started yet
	TSE_TRANSFERRING,	//Task is transferring, no error detected
	TSE_COMPLETE,		//Task has been completed successfully
	TSE_END_WITH_ERROR,	//Task ended with error
	TSE_PAUSED,			//Task has been paused by user
	TSE_DESTROYED,   	//Task doesn't exist, or has been destroyed
	
	TSE_PAUSING,		//Task is in pausing progress
	TSE_DESTROYING		//Task is in destroying progress
} TaskStatusEnum;

typedef enum
{
	DLTE_NORMAL = 0,
	DLTE_CNET = 1,
	DLTE_YTB,
	DLTE_GOOGLE_VIDEO,
	DLTE_YAHOO
} DlTypeEnum;

typedef enum
{
	SME_AUTO,
	SME_MANUAL
} StartModeEnum;

typedef CSize CRange;

class CDownloadState
{
public:
	DWORD	m_nState;
	CString m_szDetail;
	CDownloadState(DWORD nState = TSE_READY) : m_nState (nState) {}

	void  SetState(DWORD nState, LPCTSTR lpszDetail = NULL);
	DWORD GetState();
	DWORD GetAccess(DWORD nOperFlags = DL_OPER_FLAG_ALL);
	CString ToString(BOOL bWithValue = FALSE);
	void ToString(CString& szStr, BOOL bWithValue = FALSE);

	static DWORD GetAccessPermission(DWORD nState, DWORD nOperFlags = DL_OPER_FLAG_ALL);
};

class CDownloaderMgr;
class CTaskInfo
{
public:
	enum
	{
		COL_STATUS,
		COL_FILE_NAME,
		COL_FILE_SIZE,
		COL_PROGRESS,
		COL_SPEED,
		COL_LEFT_TIME,
		COL_COST_TIME,
		COL_DETAIL,
		COL_URL,
		
		COL_COUNT
	};

	enum
	{
		TIF_STATE		= 0x0001,
		TIF_FILE_NAME	= 0x0002,
		TIF_FILE_SIZE	= 0x0004,
		TIF_DLNOW		= 0x0008,
		TIF_CURR_SPEED	= 0x0010,
		TIF_LEFT_TIME	= 0x0020,
		TIF_COST_TIME	= 0x0040,
		TIF_DETAIL		= 0x0080,
		TIF_URL			= 0x0100,

		TIF_PROGRESS	= (TIF_FILE_SIZE | TIF_DLNOW | TIF_CURR_SPEED | TIF_LEFT_TIME | TIF_COST_TIME)
	};
	
	UINT	mask;
	//internal data
	int		m_nTaskID;
	CDownloaderMgr* m_lpDownloaderMgr;
	
	//visible data
	CDownloadState m_dlState;	//State and Detail
	
	CString m_szUrl;			//original URL
	CString m_szFileName;		//File Name
	
	DWORD	m_nFileSize;		//Total File Size
	DWORD	m_nDlNow;			//Progress
	DWORD	m_nCurrSpeed;		//Speed: bytes/s
	DWORD	m_nLeftTime;		//Left time in ms
	DWORD	m_nCostTime;		//Cost time in ms
	
	CTaskInfo() : mask(0), m_nTaskID(-1), m_lpDownloaderMgr(0), m_szUrl(""), m_szFileName(""), m_dlState(TSE_READY),
		m_nFileSize(0), m_nDlNow(0), m_nCurrSpeed(0), m_nLeftTime(0), m_nCostTime(0)
	{
	}
};

class CProgressMetric
{
public:
	CProgressMetric() {}
	virtual ~CProgressMetric() {}
	virtual DWORD	GetCurrentSpeed() = 0;
	virtual DWORD	GetAverageSpeed() = 0;
	virtual DWORD	GetLeftTime() = 0;
	virtual	DWORD	GetCostTime() = 0;
};

class CProgressInfo
{
public:
	int m_nTaskID;
	DWORD64 dltotal;
	DWORD64 dlnow;
	DWORD64 ultotal;
	DWORD64 ulnow;
	CProgressMetric* m_pProgressMetric;
	CProgressInfo() : m_nTaskID(-1), m_pProgressMetric(0), dltotal(0), dlnow(0), ultotal(0), ulnow(0)
	{
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
	int		m_nTaskID;
	int		m_nDlType;
	HWND	m_hWnd;
	UINT	m_nFileSize;
	CString m_szFileName;
	CDownloadParam(LPCTSTR lpszUrl = NULL, int index = -1, HWND hwnd = NULL, UINT nFileSize = 0)
		: m_szUrl(lpszUrl), m_hWnd(hwnd), m_nTaskID(index), m_nDlType(0), m_nFileSize(nFileSize) {}
};


class CSegmentInfo
{
public:
	int		m_nIndex;		//index of the segment
	DWORD64	m_nDlBefore;	//how many bytes this segment has been download before
	DWORD64	m_nDlNow;		//how many bytes this segment is downloading now
	UINT	m_nRetry;		//how many times this connection retried
	int		m_nRemotePos;	//Remote file position
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
		m_nRemotePos = 0;
	}
	virtual ~CSegmentInfo() {}
};

size_t ThrowAwayCallback(void *ptr, size_t size, size_t nmemb, void *data);
size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, void *data);

#endif