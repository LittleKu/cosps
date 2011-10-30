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
	virtual ~CCommonUtils();
public:
	//convert error code to error message
	static void FormatErrorMsg(DWORD dwCode, CString& szMsg);

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

	static CString StripInvalidFilenameChars(const CString& strText);
private:
	typedef CMap<UINT, UINT, CString, LPCTSTR> CMapUInt2String;
	CCommonUtils();
	static void FormatInternalErrorMsg(int nCode, CString& szErrorMsg);
	static BOOL GetProxyInfo(CMapUInt2String& proxyInfoMap);
};

#endif // !defined(AFX_COMMONUTILS_H__D6DA1B18_3212_438C_B5AE_2B05B09C002D__INCLUDED_)
