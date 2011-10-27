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
	CCommonUtils();
	virtual ~CCommonUtils();
public:
	static void FormatErrorMsg(DWORD dwCode, CString& szMsg);
private:
	static void FormatInternalErrorMsg(int nCode, CString& szErrorMsg);
};

#endif // !defined(AFX_COMMONUTILS_H__D6DA1B18_3212_438C_B5AE_2B05B09C002D__INCLUDED_)
