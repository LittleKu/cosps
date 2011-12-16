// CommonUtils.h: interface for the CCommonUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONUTILS_H__D6DA1B18_3212_438C_B5AE_2B05B09C002D__INCLUDED_)
#define AFX_COMMONUTILS_H__D6DA1B18_3212_438C_B5AE_2B05B09C002D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCommonUtils  
{
public:
	enum
	{
		PROXY_SERVER_ALL = -1,
		PROXY_SERVER_HTTP = 0,
		PROXY_SERVER_HTTPS,
		PROXY_SERVER_FTP,
		PROXY_SERVER_SOCKS
	};
	enum
	{
		INT_OPER_RESET = 0,
		INT_OPER_KEEP,
		INT_OPER_INCREASE
	};
	enum
	{
		MAX_INTEGER = 0x7FFFFFFF,
		BUFFER_SIZE = 4096
	};

	enum
	{
		ONE_KB = 1024,
		ONE_MB = 1024 * 1024,
		ONE_GB = 1024 * 1024 * 1024
	};
	~CCommonUtils();
public:
	//convert error code to error message
	static void State2Str(CString& szMsg, DWORD dwState, LPCTSTR lpszDetail = NULL, BOOL bWithValue = FALSE);
	static CString State2Str(DWORD dwState);
	static CString CurlCode2Str(DWORD dwCurlCode); 

	//Replace the CR(\r) char with lpCR and the LF(\n) char with lpNL
	static void ReplaceCRLF(CString& szStr, LPCTSTR lpCR = _T("[0x0D]"), LPCTSTR lpLF = _T("[0x0A]"));

	//split the file size into several parts
	static void Split(CArray<CRange, CRange&>& sizeArray, UINT nFileSize, int nMinSegmentSize, int nMaxSegmentCount);

	//merge the files in the srcFileNames into lpszDstFileName
	static BOOL MergeFiles(const CStringArray& srcFileNames, LPCTSTR lpszDstFileName);

	//merge the files whose name begins with lpSrcFileName into one file
	static BOOL MergeFiles(LPCTSTR lpSrcFileName, int nSize);

	//Delete the files
	static BOOL DeleteFiles(const CStringArray& srcFileNames);
	static BOOL DeleteFiles(LPCTSTR lpSrcFileName, int nSize);

	//Get the proxy server information
	static BOOL GetProxyInfo(CString& szProxyInfo, int nServerType);

	//Get the HTTP response code by the response status line
	static int  GetHTTPStatusCode(const char* buffer);

	//Extract the file name from the raw URL address
	static BOOL ExtractFileName(LPCTSTR lpszUrl, CString& szFileName);

	static BOOL RemoveDirectory(LPCTSTR lpPathName);
	
	static BOOL VerifyDirectoryExist(LPCTSTR lpPathName);

	static CString StripInvalidFilenameChars(const CString& strText);

	static int Intersection(const CRange& r1, const CRange& r2, CRange& rResult);

	static BOOL IntegerOper(UINT& nData, int nOperType = INT_OPER_RESET);

	static int GetUniqueID();

	static LRESULT SendMessage(HWND hWnd, UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	static void FormatSpeed(DWORD dwBytesPerSeconds, CString& szOut);

	static void FormatFileSize(DWORD dwBytes, CString& szOut);

	static void FormatTime(DWORD dwTimeInMs, CString& szOut);

	static void FormatPercent(int nCurrent, int nMax, CString& szOut);

	static int GetIconIndex(LPCTSTR lpszPath, DWORD dwFileAttributes = FILE_ATTRIBUTE_NORMAL, UINT uFlags = 0);

	static void ConcatString(const char* lpszSrcData, int nSrcLen, CString& szString);

	static BOOL GetFileContent(LPCTSTR lpFileName, CString& szString);

	static int GetDownloadType(const CString& szURL);

	static void Unescape(CString& szURL, int nCount = -1);

	static void DecodeSpecialChars(CString& str);

	static void GetTempFolder(CString& szTempFolder, const CDownloadParam& dlParam);
private:
	typedef CMap<UINT, UINT, CString, LPCTSTR> CMapUInt2String;
	CCommonUtils();
	static BOOL GetProxyInfo(CMapUInt2String& proxyInfoMap);

	static long m_nUniqueID;
};

#endif // !defined(AFX_COMMONUTILS_H__D6DA1B18_3212_438C_B5AE_2B05B09C002D__INCLUDED_)
