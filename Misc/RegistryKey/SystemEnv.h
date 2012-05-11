// SystemEnv.h: interface for the CSystemEnv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMENV_H__C24699C3_4264_4F0B_ABB1_10D9B0CF627C__INCLUDED_)
#define AFX_SYSTEMENV_H__C24699C3_4264_4F0B_ABB1_10D9B0CF627C__INCLUDED_

#include "Environment.h"

class CRegistryKey;

class CSystemEnv : public CEnvironment
{
public:
	CSystemEnv(BOOL bAutoFlush = TRUE);
	virtual ~CSystemEnv();

	virtual BOOL Delete(LPCWSTR lpName);

	virtual BOOL SetEnv(LPCWSTR lpName,	const std::wstring& strValue);
	virtual BOOL GetEnv(LPCWSTR lpName, std::wstring& strValue);

	void Flush();
private:
	CRegistryKey* m_pRegKey;
	BOOL m_bAutoFlush; // take effect immediately
	
	static LPCWSTR lpszEnvironmentSubKey;
};

#endif // !defined(AFX_SYSTEMENV_H__C24699C3_4264_4F0B_ABB1_10D9B0CF627C__INCLUDED_)
