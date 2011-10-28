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
	static void FormatErrorMsg(DWORD dwCode, CString& szMsg);
	static void Split(CArray<CRange, CRange&>& sizeArray, UINT nFileSize, int nMinSegmentSize, int nMaxSegmentCount);
	static BOOL MergeFiles(const CStringArray& srcFileNames, LPCTSTR lpszDstFileName);
	static BOOL MergeFiles(LPCTSTR lpSrcFileName, int nSize);
	static BOOL DeleteFiles(const CStringArray& srcFileNames);
	static BOOL DeleteFiles(LPCTSTR lpSrcFileName, int nSize);

	static BOOL GetProxyInfo(CString& szProxyInfo, int nServerType);
private:
	typedef CMap<UINT, UINT, CString, LPCTSTR> CMapUInt2String;
	CCommonUtils();
	static void FormatInternalErrorMsg(int nCode, CString& szErrorMsg);
	static BOOL GetProxyInfo(CMapUInt2String& proxyInfoMap);
};

#endif // !defined(AFX_COMMONUTILS_H__D6DA1B18_3212_438C_B5AE_2B05B09C002D__INCLUDED_)
