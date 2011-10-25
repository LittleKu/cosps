#ifndef __COMMON_H__
#define __COMMON_H__

#define WM_DOWNLOAD_PROGRESS	(WM_USER + 1099)
#define WM_DOWNLOAD_COMPLETE	(WM_USER + 1100)


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
	int		httpcode;
	long	header_size;
	bool	is_range_bytes;
	CHeaderInfo() : httpcode(0), header_size(0), is_range_bytes(false) {}
	void Reset()
	{
		httpcode = 0;
		header_size = 0;
		is_range_bytes = false;
	}
};

class CDownloadParam
{
public:
	HWND	m_hWnd;
	int		m_nIndex;
	UINT	m_nFileSize;
	CDownloadParam(HWND hwnd = NULL, int index = -1, UINT nFileSize = 0)
		: m_hWnd(hwnd), m_nIndex(index), m_nFileSize(nFileSize) {}
};

#endif