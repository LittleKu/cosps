#ifndef __COMMON_H__
#define __COMMON_H__

#define WM_DOWNLOAD_PROGRESS	(WM_USER + 1099)
#define WM_DOWNLOAD_COMPLETE	(WM_USER + 1100)

class CTaskInfo
{
public:
	CString m_url;
	CString m_fileName;
	CString m_fileSize;
	CString m_progress;
};

class CProgressInfo
{
public:
	DWORD64 dltotal;
	DWORD64 dlnow;
	DWORD64 ultotal;
	DWORD64 ulnow;
	int retCode;
	CString szReason;
	CProgressInfo() : dltotal(0), dlnow(0), ultotal(0), ulnow(0)
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

#endif